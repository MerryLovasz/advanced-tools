#include <iostream>
#include "mge/Octree/Level/Levels/Octree.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Octree/Level/Components/Collider.hpp"
#include "mge/Octree/Level/AABB.hpp"
#include "mge/Octree/Level/OBB.hpp"
#include "mge/materials/ColorMaterial.hpp"
#include<vector>

int Octree::count;

Octree::Octree(glm::vec3 center, glm::vec3 halfSize, int level, Octree * _root)
{
	this->root = root != NULL ? root : this; // given root or be root to self
	if (_root == nullptr) {
		this->root = this;
	}
	else
		root = _root;

	this->id = Octree::count++; // for debug
	this->level = level;
	this->center = center;
	this->halfSize = halfSize;

	if (this->level > 0) {
		glm::vec3 childHalfSize = 0.5f * halfSize;
		std::vector<Octree*> childs(8);
		children = childs;
		this->children[0] = new Octree(center - childHalfSize, childHalfSize, level - 1, this->root);
		this->children[1] = new Octree(center + glm::vec3(+childHalfSize.x, -childHalfSize.y, +childHalfSize.z), childHalfSize, level - 1, this->root);
		this->children[2] = new Octree(center + glm::vec3(-childHalfSize.x, +childHalfSize.y, -childHalfSize.z), childHalfSize, level - 1, this->root);
		this->children[3] = new Octree(center + childHalfSize, childHalfSize, level - 1, this->root);
		
		this->children[4] = new Octree(center + glm::vec3(+childHalfSize.x, -childHalfSize.y, -childHalfSize.z), childHalfSize, level - 1, this->root);
		this->children[5] = new Octree(center + glm::vec3(+childHalfSize.x, +childHalfSize.y, -childHalfSize.z), childHalfSize, level - 1, this->root);
		this->children[6] = new Octree(center + glm::vec3(-childHalfSize.x, +childHalfSize.y, +childHalfSize.z), childHalfSize, level - 1, this->root);
		this->children[7] = new Octree(center + glm::vec3(-childHalfSize.x, -childHalfSize.y, +childHalfSize.z), childHalfSize, level - 1, this->root);
	}

	this->transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(center));
	transform = glm::scale(transform, glm::vec3(halfSize));
}

Octree::~Octree() {

}

//delete bounds for optimization
Octree * Octree::Add(GameObject * obj)
{
	// parent makes sure it fits
	if (level > 0)
	{
		AABB * bounds = new AABB(obj,obj->getLocalPosition() + obj->getCollider()->GetCenter(), obj->getCollider()->GetHalfSize());
		if (bounds->GetMax().x < center.x)
		{                 // - x
			if (bounds->GetMax().y < center.y)
			{             // - y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[0]->Add(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[7]->Add(obj);
				}
			}
			else if (bounds->GetMin().y > center.y)
			{     // + y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[2]->Add(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[6]->Add(obj);
				}
			}
		}
		else if (bounds->GetMin().x > center.x)
		{          // + x
			if (bounds->GetMax().y < center.y)
			{             // - y
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[4]->Add(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[1]->Add(obj);
				}
			}
			else if (bounds->GetMin().y > center.y)
			{          // + y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[5]->Add(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[3]->Add(obj);
				}
			}
		}
		delete bounds;
	}
	objects.push_back(obj); // place here if bottom or no child holds obj.
	return this;
}


std::vector<Pair> * Octree::Collisions(std::vector<Pair> * pairs)
{

	if (pairs == nullptr) {
		pairs = new std::vector<Pair>();
	}

	for (size_t a = 0; a<objects.size(); a++) {
		GameObject * one = objects[a];

		for (size_t b = a + 1; b<objects.size(); b++) {

			GameObject * other = objects[b];

			if (one->Collides(other)) {
				pairs->push_back(Pair(one, other));
			}
		}

		if (level > 0) {
			for (Octree* child : children) {
				child->Collides(pairs, one);
			}
		}
	}
	if (level > 0) {
		for (Octree* child : children) {
			child->Collisions(pairs);
		}
	}

	return pairs;
}

void Octree::Collides(std::vector<Pair>* pairs, GameObject * one)
{
	for (size_t b = 0; b<objects.size(); b++) {
		GameObject * other = objects[b];
		if (one->Collides(other)) {
			pairs->push_back(Pair(one, other));
		}
	}
	if (level > 0) {
		for(Octree * child : children){
			child->Collides(pairs, one);
		}
	}
}

//delete bounds for optimization
bool Octree::Remove(GameObject * obj)
{
	if (this->level > 0) {
		AABB * bounds = new AABB(obj, obj->getWorldPosition() + obj->getCollider()->GetCenter(), obj->getCollider()->GetHalfSize());

		if (bounds->GetMax().x < center.x)
		{                 // - x
			if (bounds->GetMax().y < center.y)
			{             // - y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[0]->Remove(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[7]->Remove(obj);
				}
			}
			else if (bounds->GetMin().y > center.y)
			{     // + y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[2]->Remove(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[6]->Remove(obj);
				}
			}
		}
		else if (bounds->GetMin().x > center.x)
		{          // + x
			if (bounds->GetMax().y < center.y)
			{             // - y
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[4]->Remove(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[1]->Remove(obj);
				}
			}
			else if (bounds->GetMin().y > center.y)
			{          // + y 
				if (bounds->GetMax().z < center.z)            // - z
				{
					delete bounds;
					return children[5]->Remove(obj);
				}
				else if (bounds->GetMax().z > center.z)       // + z
				{
					delete bounds;
					return children[3]->Remove(obj);
				}
			}
		}
		delete bounds;
	}
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i] == obj)
		{
			objects.erase(objects.begin() + i);
			return true;
		}
	}
	return false;	// Nothing was found
}

void Octree::Update(float pStep)
{
	std::vector<GameObject*> all = objects;
	objects.clear();
	objects.shrink_to_fit();
	for (GameObject * obj : all)
	{
		root->Add(obj);
	}

	if (level > 0) {
		for (Octree * child : children) {
			child->Update(pStep);
		}
	}
	
	all.clear();
	all.shrink_to_fit();
}
