#include "FleeAction.h"
#include "ArriveAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>

FleeAction::FleeAction()
{
	m_actionType = AN_FLEE;
	controls.currentDistance = 0.0f;
	controls.maxDistance = 50.0f;
	controls.target = glm::vec3(0.0f);
	m_pTarget = nullptr;
}

FleeAction::FleeAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_FLEE;
	controls.target = a_target->movedata.position;
	controls.currentDistance = 0.0f;
	controls.currentDistance = 50.0f;
}



FleeAction::~FleeAction()
{
}


void FleeAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
		if (controls.currentDistance < controls.maxDistance)
		{
			flee(a_dt, a_agent);
		}
		else
		{
			glm::vec3 target = (m_pTarget->position() - a_agent.position()) + a_agent.position();
			arrive(a_dt, a_agent, target);
		}
	}
	else
	{
		std::cout << "ERROR :: Flee Action - target agent is nullptr" << std::endl;
	}
}

void FleeAction::flee(float a_dt, Agent & a_agent)
{
	// subtract target position + targets velocity from agent postion
	// scale resultant vector by maxSpeed
	// calculate the acceleration required to move agent away from targets estimated location
	glm::vec3 acceleration = (glm::normalize(a_agent.movedata.position - m_pTarget->movedata.position)
		* a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

	// limit acceleration
	if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
	{
		acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
		a_agent.movedata.acceleration = acceleration;
	}

	// Apply accleration to agent
	a_agent.movedata.velocity += acceleration * a_dt;
	a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
}

void FleeAction::arrive(float a_dt, Agent & a_agent, glm::vec3 a_target)
{
	// subtract agent postion from target position + targets velocity 
	a_agent.movedata.heading = glm::normalize(a_target - a_agent.movedata.position);
	// scale resultant vector by maxSpeed
	// calculate the acceleration required to move agent away to targets estimated location
	glm::vec3 acceleration = glm::normalize(a_agent.movedata.heading) * a_agent.movedata.maxSpeed;

	// get distance to target
	float distance = glm::distance(a_agent.movedata.position, a_target);
	// distance scalar
	float scalar = std::min(distance / a_agent.movedata.sight, 1.0f);

	// if within radius
	if (scalar < 1)
	{
		acceleration *= scalar;

		float d = glm::dot(a_agent.movedata.heading, glm::normalize(a_agent.movedata.velocity));

		glm::vec3 resistance = -(glm::normalize(a_agent.movedata.velocity)) * glm::length(a_agent.movedata.velocity) * d * 2.0f;

		acceleration += resistance;
	}
	acceleration -= a_agent.movedata.velocity;
	// set acceleration
	a_agent.movedata.acceleration = acceleration;

	if (a_agent.vitals.mass > 0)
	{
		// Apply accleration to agent
		a_agent.movedata.velocity += (acceleration * a_agent.vitals.mass) * a_dt;
	}
	else
	{
		// Apply accleration to agent
		a_agent.movedata.velocity += acceleration * a_dt;
	}
	// adjust agents position
	a_agent.movedata.position += a_agent.movedata.velocity * a_dt;

}




