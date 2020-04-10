#include "mge/Octree/Level/AABB.hpp"
#include "mge/Octree/Level/OBB.hpp"
#include "glm.hpp"

AABB::AABB(GameObject * parent, glm::vec3 center, glm::vec3 halfsize) 
	:	Collider(parent,center,halfsize)
{
	parent = parent;
	center = center;
	halfsize = halfsize;
	id = 1;
}

bool AABB::CollidesWith(Collider * otherCollider) {
	switch (otherCollider->id) {
	case 0 :
		return Collides(static_cast<Collider*>(otherCollider));
		break;
	case 1 :
		return Collides(static_cast<AABB*>(otherCollider));
		break;
	case 2:
		return Collides(static_cast<OBB*>(otherCollider));
		break;
	default:
		break;
	}

}

bool AABB::Collides(Collider * otherCollider)
{
	return otherCollider->Collides(this);
}

bool AABB::Collides(AABB * otherCollider)
{
	Collider::count++;
	//If the ''bounds'' of the collider crosses the other coliders bounds, return true;
	bool hit = this->GetMax().x > otherCollider->GetMin().x
		&& this->GetMin().x < otherCollider->GetMax().x
		&& this->GetMax().y > otherCollider->GetMin().y
		&& this->GetMin().y < otherCollider->GetMax().y
		&& this->GetMax().z > otherCollider->GetMin().z
		&& this->GetMin().z < otherCollider->GetMax().z;
//	if (hit) std::cout << "HIT AABB" << '\n';
	return hit;
}

bool AABB::Collides(OBB * otherCollider) {
	glm::vec3 oneCenter = this->GetCenter(); // object's pos = collider center
	glm::mat4 oneTransform = glm::scale(this->parent->getTransform(), this->GetHalfSize()); // scaling for halfsize
	glm::vec3 otherCenter = otherCollider->GetCenter();
	glm::mat4 otherTransform = glm::scale(otherCollider->GetParent()->getTransform(), otherCollider->GetHalfSize());

	//axis for obj a
	for (int a = 0; a<3; a++) {
		glm::vec3 L = glm::vec3(oneTransform[a]); // one axis to project on
		float TL = std::abs(glm::dot(L, otherCenter) - glm::dot(L, oneCenter)); // center distance
		float RA = std::abs(glm::dot(L, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[2]))); //radius a
		float RB = std::abs(glm::dot(L, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[2]))); //radis b
		float penetration = (RA + RB) - TL;
		if (penetration <= 0) { // no overlap
			return false;
		}
	}
	//axis for obj b
	for (int b = 0; b<3; b++) {
		glm::vec3 L = glm::vec3(otherTransform[b]); // other axis to project on
		float TL = std::abs(glm::dot(L, otherCenter) - glm::dot(L, oneCenter)); // center distance
		float RA = std::abs(glm::dot(L, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[2])));//radius a
		float RB = std::abs(glm::dot(L, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[2])));//radis b
		float penetration = (RA + RB) - TL;
		if (penetration <= 0) { // no overlap
			return false;
		}
	}

	//cross product for axis obj a + b
	for (int a = 0; a<3; a++) {
		glm::vec3 aAxis = glm::vec3(oneTransform[a]);
		for (int b = 0; b<3; b++) {
			glm::vec3 bAxis = glm::vec3(otherTransform[b]);
			if (aAxis != bAxis) {
				glm::vec3 L = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
				float tl = std::abs(glm::dot(L, otherCenter) - glm::dot(L, oneCenter)); // center distance
				float ra = std::abs(glm::dot(L, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(L, glm::vec3(oneTransform[2])));
				float rb = std::abs(glm::dot(L, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(L, glm::vec3(otherTransform[2])));
				float penetration = (ra + rb) - tl;
				if (penetration <= 0) { // no overlap
					return false;
				}
			}
		}
	}
//	std::cout << "HIT OBB" << '\n';
	return true;
}


