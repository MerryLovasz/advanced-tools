#ifndef AABB_HPP
#define AABB_HPP

#include <string>
#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Octree/Level/Components/Collider.hpp"

class AABB : public Collider {
public:
	AABB(GameObject * parent, glm::vec3 center, glm::vec3 halfsize);
	
	bool CollidesWith(Collider * otherCollider);
	bool Collides(Collider * otherCollider);
	bool Collides(AABB * otherCollider);
	bool AABB::Collides(OBB * otherCollider);
};

#endif 