#include "mge/behaviours/RotatingBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <cstdlib>

RotatingBehaviour::RotatingBehaviour(glm::vec3 pVelocity) :AbstractBehaviour(), velocity(pVelocity)
{
	//ctor

	velocity = pVelocity;

	float x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	velocity = glm::vec3(x, y, z);
}

RotatingBehaviour::~RotatingBehaviour()
{
	//dtor
}

void RotatingBehaviour::update(float pStep)
{
	//Taken from Quadtree project by Eelco Jannick
	float radius = 15;
	glm::vec3 worldPosition = _owner->getWorldPosition();
	if (worldPosition.x < -radius || worldPosition.x > radius) velocity.x = -velocity.x;
	if (worldPosition.y < -radius || worldPosition.y > radius) velocity.y = -velocity.y;
	if (worldPosition.z < -radius || worldPosition.z > radius) velocity.z = -velocity.z;
	_owner->translate(velocity);
}
