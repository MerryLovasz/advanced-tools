#ifndef COLLIDER_HPP
#define COLLIDER_HPP

class OBB;
class AABB;
#include "mge/core/GameObject.hpp"

class Collider {
public:
	Collider(GameObject* parent, glm::vec3 center, glm::vec3 halfsize);
	virtual ~Collider();

	virtual bool CollidesWith(Collider * otherCollider) = 0;
	virtual bool Collides(Collider * otherCollider) = 0;
	virtual bool Collides(AABB * otherCollider) = 0;
	virtual bool Collides(OBB * otherCollider) = 0;

	bool isColliding = false;
	static int count;

	glm::vec3 GetSize() { return 2.0f * halfsize; }
	glm::vec3 GetHalfSize() { return halfsize; }
	glm::vec3 GetMin() { return center - halfsize; }
	glm::vec3 GetMax() { return center + halfsize; }
	void SetCenter(glm::vec3 newCenter) { center = newCenter; }

	glm::vec3 GetCenter() { return center; }
	GameObject * GetParent() { return parent; }

	int id = 0;
protected:
	GameObject * parent;
	glm::vec3 center;
	glm::vec3 halfsize;
};

#endif
