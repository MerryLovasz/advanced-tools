#ifndef ROTATINGBEHAVIOUR_HPP
#define ROTATINGBEHAVIOUR_HPP

#include "glm.hpp"
#include "mge/behaviours/AbstractBehaviour.hpp"

/**
 * Simply rotates the object around its origin with a fixed speed.
 */
class MovingBehaviour : public AbstractBehaviour
{
	public:
		MovingBehaviour(glm::vec3 _velocity = glm::vec3(0.0f, 0.0f, 0.0f));
		virtual ~MovingBehaviour();

		virtual void update(float pStep);

private:
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

};

#endif // ROTATINGBEHAVIOUR_HPP
