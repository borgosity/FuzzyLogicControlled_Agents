#include "ArriveAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>


ArriveAction::ArriveAction()
{
	m_actionType = AN_ARRIVE;
	m_arrived = false;
}

ArriveAction::~ArriveAction()
{
}


void ArriveAction::update(float a_dt, Agent & a_agent, glm::vec3 a_target)
{
	m_target = a_target;
	arrive(a_dt, a_agent, a_target);
}

void ArriveAction::arrive(float a_dt, Agent & a_agent, glm::vec3 a_target)
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



