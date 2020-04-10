#include "mge/Octree/Level/OBB.hpp"
#include "glm.hpp"
#include "mge/Octree/Level/AABB.hpp"

OBB::OBB(GameObject * parent, glm::vec3 center, glm::vec3 halfsize)
	: Collider(parent, center, halfsize)
{
	parent = parent;
	center = center;
	halfsize = halfsize;
	id = 2;
}


bool OBB::CollidesWith(Collider * otherCollider)
{
	switch (otherCollider->id) {
	case 0:
		return Collides(static_cast<Collider*>(otherCollider));
		break;
	case 1:
		return Collides(static_cast<AABB*>(otherCollider));
		break;
	case 2:
		return Collides(static_cast<OBB*>(otherCollider));
		break;
	default:
		break;
	}
}

bool OBB::Collides(Collider * otherCollider)
{
	return otherCollider->Collides(this);
}

bool OBB::Collides(AABB * otherCollider)
{
	return otherCollider->Collides(this);
}

bool OBB::Collides(OBB * otherCollider) {
	count++;
	glm::vec3 oneCenter = this->GetCenter(); // object's pos = collider center
	glm::mat4 oneTransform = glm::scale(this->parent->getTransform(), this->GetHalfSize()); // scaling for halfsize
	glm::vec3 otherCenter = otherCollider->GetCenter();
	glm::mat4 otherTransform = glm::scale(otherCollider->GetParent()->getTransform(), otherCollider->GetHalfSize());

	for (int a = 0; a<3; a++) {
		glm::vec3 l = glm::vec3(oneTransform[a]); // one axis to project on
		float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
		float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2]))); // radius a 
		float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2]))); // radius b
		float penetration = (ra + rb) - tl;
		if (penetration <= 0) { // no overlap
			return false;
		}
	}
	for (int b = 0; b<3; b++) {
		glm::vec3 l = glm::vec3(otherTransform[b]); // other axis to project on
		float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
		float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
		float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
		float penetration = (ra + rb) - tl;
		if (penetration <= 0) { // no overlap
			return false;
		}
	}
	for (int a = 0; a<3; a++) {
		glm::vec3 aAxis = glm::vec3(oneTransform[a]);
		for (int b = 0; b<3; b++) {
			glm::vec3 bAxis = glm::vec3(otherTransform[b]);
			if (aAxis != bAxis) {
				glm::vec3 l = glm::cross(aAxis, bAxis); // has flaw when axis are same, result in (0,0,0), solved by if
				float tl = std::abs(glm::dot(l, otherCenter) - glm::dot(l, oneCenter)); // center distance
				float ra = std::abs(glm::dot(l, glm::vec3(oneTransform[0]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[1]))) + std::abs(glm::dot(l, glm::vec3(oneTransform[2])));
				float rb = std::abs(glm::dot(l, glm::vec3(otherTransform[0]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[1]))) + std::abs(glm::dot(l, glm::vec3(otherTransform[2])));
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