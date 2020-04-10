#include "Collider.hpp"
int Collider::count;
Collider::Collider(GameObject* parent, glm::vec3 center, glm::vec3 halfsize)
	: parent(parent), center(center), halfsize(halfsize)
{

}


Collider::~Collider() {
	parent = nullptr;
}

