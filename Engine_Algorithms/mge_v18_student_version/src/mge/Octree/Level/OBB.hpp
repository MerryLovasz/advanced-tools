#ifndef OBB_HPP
#define OBB_HPP

#include "glm.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/Octree/Level/Components/Collider.hpp"
#include "mge/Octree/Level/AABB.hpp"

class OBB : public Collider {
public:
	OBB(GameObject * parent, glm::vec3 center, glm::vec3 halfsize);

	bool CollidesWith(Collider * otherCollider);
	bool Collides(Collider * otherCollider);
	bool Collides(AABB * otherCollider);
	bool Collides(OBB * otherCollider);
};

#endif
