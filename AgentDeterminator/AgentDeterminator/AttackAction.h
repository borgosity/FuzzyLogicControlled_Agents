#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;

struct AttackValues
{
	glm::vec3	target;			// target vector
	glm::vec3	prevLoc;		// previous location
	float		distance;		// target distance from player	
};


class AttackAction :
	public Action
{
public:
	AttackAction();
	AttackAction(Agent * a_target);
	virtual ~AttackAction();
	virtual void update(float a_dt, Agent & a_agent);

	Agent * targetAgent() { return m_pTarget; };
	Agent * targetAgent(Agent * a_targetAgent) {
		m_pTarget = a_targetAgent; 
		return m_pTarget;
	};
	AttackValues controls;

private:
	// agent member variables
	Agent * m_pTarget;

};

