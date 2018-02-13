#include "Utils.h"
#include "Agents.h"

#include "imgui_glfw3.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#define M_MINDISTANCE       10.0f
#define M_PI       3.14159265358979323846   // pi



	glm::vec3 Utils::closestPosition(Agent & a_target, Agent & a_agent)
	{
		double radius = a_target.vitals.size + M_MINDISTANCE;
		// orient towards
		double dx = a_agent.position().x - a_target.position().x;
		double dy = a_agent.position().y - a_target.position().y;

		double angleRad = atan2(dy, dx) + 2 * M_PI;

		double x = a_target.position().x + radius * cos(angleRad);
		double y = a_target.position().y + radius * sin(angleRad);

		return glm::vec3(x, y, 0.0f);
	}

