#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;

struct FleeValues
{
	glm::vec3	target;			// target vector
	glm::vec3	prevLoc;		// previous location
	float		currentDistance;		// target distance from player	
	float		maxDistance;
};


class FleeAction :
	public Action
{
public:
	FleeAction();
	FleeAction(Agent * a_target);
	virtual ~FleeAction();
	virtual void update(float a_dt, Agent & a_agent);

	Agent * targetAgent() { return m_pTarget; };
	Agent * targetAgent(Agent * a_targetAgent) {
		m_pTarget = a_targetAgent; return m_pTarget;
	};
	FleeValues controls;

private:
	// agent member variables
	Agent * m_pTarget;

	void flee(float a_dt, Agent & a_agent);
	void arrive(float a_dt, Agent & a_agent, glm::vec3 a_target);
};

