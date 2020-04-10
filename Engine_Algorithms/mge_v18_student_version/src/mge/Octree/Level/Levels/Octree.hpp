#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <string>
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Octree/Level/Common/Pair.hpp"

class Octree {
public:
	Octree(glm::vec3 center, glm::vec3 halfSize, int level, Octree * _root = nullptr);
	virtual ~Octree();

	static int count; // for id
	int id;

	Octree * Add(GameObject * obj);
	std::vector<Pair> * Collisions(std::vector<Pair> * pairs);
	void Collides(std::vector<Pair> * pairs, GameObject * one);
	bool Remove(GameObject * obj);
	void Update(float pStep);	
private:
	int level;
	glm::vec3 center;
	glm::vec3 halfSize;
	Octree * root;
	std::vector<Octree*> children;
	std::vector<GameObject*> objects;
	glm::mat4 transform;
};

#endif
