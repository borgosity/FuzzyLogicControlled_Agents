#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;
typedef std::shared_ptr<Agent> spAgent;

class ArriveAction :
	public Action
{
public:
	ArriveAction();
	virtual ~ArriveAction();
	virtual void update(float a_dt, Agent & a_agent, glm::vec3 a_target);

	glm::vec3 target() { return m_target; }
	glm::vec3 target(glm::vec3 a_target) { m_target = a_target; return m_target; }


private:
	// agent member variables
	glm::vec3	m_target;
	bool m_arrived;

	// private member functions
	void arrive(float a_dt, Agent & a_agent, glm::vec3 a_target);
};

