#include "SeekAction.h"
#include "Agents.h"
#include "Utils.h"

#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>

SeekAction::SeekAction()
{
	m_actionType = AN_SEEK;
	controls.distance = 0.0f;
	controls.target = glm::vec3(0.0f);
	m_pTarget = nullptr;
	m_arrive = false;
	m_waiting = false;
}

SeekAction::SeekAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_SEEK;
	controls.target = a_target->movedata.position;
	controls.distance = 0.0f;
	m_arrive = false;
	m_waiting = false;
}



SeekAction::~SeekAction()
{
}


void SeekAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
		glm::vec3 targetPos = Utils::closestPosition(*m_pTarget, a_agent);

		float distance = glm::distance(targetPos, a_agent.position());
		//std::cout << distance << std::endl;
		if (distance > (a_agent.movedata.sight * 2))
		{
			m_arrive = true;
		}
		else
		{
			m_waiting = false;
			m_arrive = false;
		}

		if (!m_arrive && !m_waiting)
		{
			// vector from agent to target
			a_agent.movedata.heading = glm::normalize(targetPos - a_agent.movedata.position);
			// scale vector by maxSpeed
			// calculate the acceleration required to move agent to target (distance minus current velocity)
			glm::vec3 acceleration = (a_agent.movedata.heading * a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

			if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
			{
				acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
			}
			// set acceleration
			a_agent.movedata.acceleration = acceleration;
			// adjust velocity based on agent mass
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

			// Apply accleration to agent
			//a_agent.movedata.velocity += acceleration * a_dt;
			// adjust agent position accordingly
			a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
		}
		else
		{
			arrive(a_dt, a_agent);
		}
	}
	else
	{
		std::cout << "ERROR :: Seek Action - target agent is nullptr" << std::endl;
	}
}

void SeekAction::arrive(float a_dt, Agent & a_agent)
{
	controls.target = m_pTarget->movedata.position;
	float velocity = glm::distance(a_agent.movedata.velocity, glm::vec3(0.0f));
	if (velocity > 0.05 && !m_waiting)
	{
		// subtract agent postion from target position + targets velocity 
		a_agent.movedata.heading = glm::normalize(controls.target - a_agent.movedata.position);
		// scale resultant vector by maxSpeed
		// calculate the acceleration required to move agent away to targets estimated location
		glm::vec3 acceleration = glm::normalize(a_agent.movedata.heading) * a_agent.movedata.maxSpeed;

		// get distance to target
		float distance = glm::distance(a_agent.movedata.position, controls.target);
		// distance scalar
		float scalar = std::min(distance / controls.distance, 1.0f);

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
	else
	{
		a_agent.movedata.velocity = glm::vec3(0.0f,0.0f,0.0f);
		m_waiting = true;
	}
}




