#include "AttackAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>

AttackAction::AttackAction()
{
	m_actionType = AN_ATTACK;
	controls.target = glm::vec3(0.0f);
	controls.distance = 0.0f;
	m_pTarget = nullptr;
}

AttackAction::AttackAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_ATTACK;
	controls.target = a_target->movedata.position;
	controls.distance = a_target->vitals.mass * 0.5f;
}



AttackAction::~AttackAction()
{
}


void AttackAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
			// do some attack stuff
			float attackStrength = a_agent.vitals.strength + a_agent.vitals.health;
			float oppenentStrength = m_pTarget->vitals.strength + m_pTarget->vitals.health;
			bool inRange = (a_agent.vitals.foeDistance <= a_agent.movedata.sight);

			// ## if opponent not attacking agent gets first strike
			// ## else - random chance at first strike?


			if (!m_pTarget->vitals.dead && inRange) {
				// if as tough of tough do full damage, else half
				if (attackStrength >= oppenentStrength){
					m_pTarget->vitals.health -= a_agent.vitals.mass / 100;
				}
				else {
					m_pTarget->vitals.health -= (a_agent.vitals.mass * 0.5f) / 100;
				}
			}
		//}
	}
	else
	{
		std::cout << "ERROR :: Attack Action - target agent is nullptr" << std::endl;
	}
}






