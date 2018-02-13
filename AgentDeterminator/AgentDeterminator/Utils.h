#pragma once
#include <vector>
#include <iostream>
#include <glm\glm.hpp>

class Agent;


	class Utils
	{
	public:
		static glm::vec3 closestPosition(Agent & a_target, Agent & a_agent);

	protected:
		static Utils* m_instnce;

	private:
		Utils() {}

	};

