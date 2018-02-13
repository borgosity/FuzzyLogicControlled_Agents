#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;

struct SeekValues
{
	glm::vec3	target;			// target vector
	glm::vec3	prevLoc;		// previous location
	float		distance;		// target distance from player	
};


class SeekAction :
	public Action
{
public:
	SeekAction();
	SeekAction(Agent * a_target);
	virtual ~SeekAction();
	virtual void update(float a_dt, Agent & a_agent);

	Agent * targetAgent() { return m_pTarget;};
	Agent * targetAgent(Agent * a_targetAgent) {
		m_arrive = false;
		m_pTarget = a_targetAgent; return m_pTarget;
	};
	bool arrivedToggle() { m_arrive = !m_arrive; return m_arrive; };
	SeekValues controls;

private:
	// agent member variables
	Agent * m_pTarget;
	bool m_arrive;
	bool m_waiting;
	void arrive(float a_dt, Agent & a_agent);
};

