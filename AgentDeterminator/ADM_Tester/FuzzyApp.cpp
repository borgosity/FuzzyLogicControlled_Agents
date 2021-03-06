#include "FuzzyApp.h"
#include "Font.h"
#include "Input.h"
#include "glm\glm.hpp"
#include "WanderAction.h"

FuzzyApp::FuzzyApp()
{
}


FuzzyApp::~FuzzyApp()
{
}

bool FuzzyApp::startup()
{
	// renderer
	m_renderer = new aie::Renderer2D();
	
	m_font = new aie::Font("../res/font/consolas.ttf", 18);

	//scene controller
	m_sceneController = new SceneController();
	// agents
	m_playerAgent = new PlayerAgent("Hero", glm::vec3(400.0f, 400.0f, 0.0f),
		PLAYER_HEALTH, PLAYER_SIZE, PLAYER_MINDISTANCE,
		PLAYER_MAXSPEED, PLAYER_MAXACCEL, PLAYER_LIVELYNESS, PLAYER_SIGHT);
	// buddy
	m_buddyAgent = new CompanionAgent("Buddy", glm::vec3(400.0f, 500.0f, 0.0f),
		BUDDY_HEALTH, BUDDY_SIZE, BUDDY_MINDISTANCE,
		BUDDY_MAXSPEED, BUDDY_MAXACCEL, BUDDY_SIGHT);
	// enemies
	for (int i = 0; i < ENEMY_COUNT +1; i++)
	{
		// new spawn point
		spawnPosition(m_playerAgent->position(), m_buddyAgent->position());
		// add enemy
		m_enemies.push_back(new EnemyAgent("Enemy_" + std::to_string(i), m_tempPos,
			ENEMY_HEALTH, ENEMY_SIZE, ENEMY_MINDISTANCE,
			ENEMY_MAXSPEED, ENEMY_MAXACCEL, ENEMY_SIGHT, i));
	}
	// selected agent
	m_enemyAgent = m_enemies.front();
	// add egents to scene controller
	m_sceneController->addAgent(m_buddyAgent);
	m_sceneController->addAgent(m_playerAgent);
	for (auto enemy : m_enemies)
	{
		m_sceneController->addAgent(enemy);
	}
	// initialise agent behaviours
	m_buddyAgent->buddyAgent(m_playerAgent);
	for (auto enemy : m_enemies)
	{
		enemy->findTarget();
	}

	return true;
}

void FuzzyApp::shutdown()
{
	delete m_font;
	delete m_renderer;
	delete m_buddyAgent;
	for (auto enemy : m_enemies)
	{
		delete enemy;
	}
	//	delete m_enemyAgent;
	delete m_playerAgent;
	delete m_sceneController;
}

void FuzzyApp::update(float a_dt)
{
	// update input
	aie::Input* input = aie::Input::getInstance();

	// update agents
	m_sceneController->update(a_dt);

	// agent off screen?
	offScreen();

	// detect agent select
	if (input->wasMouseButtonReleased(0))
	{
		int x = 0;
		int y = 0;
		input->getMouseXY(&x, &y);
		m_clickPos = glm::vec3(x, y, 0.0f);
		for (auto enemy : m_enemies)
		{
			float dis = glm::distance(m_clickPos, enemy->position());
			if (dis <= enemy->vitals.size)
			{
				m_enemyAgent = enemy;
			}
		}
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FuzzyApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	// draw your stuff here!
	drawAgents(m_renderer);
	drawGUI();

	// output some text
	glm::vec4 colour = glm::vec4(0.0f);
	m_renderer->setRenderColour(colour.r, colour.g, colour.b, colour.a);
	m_fpsString = "__" + std::to_string(m_fps) + "fps__";
	// std::cout << m_fpsString << ", " << m_fps << std::endl;
	m_renderer->drawText(m_font, m_fpsString.c_str(), (float)SCREEN_W - ((m_fpsString.length() * 10) + 20), SCREEN_H - 20);
	m_renderer->drawText(m_font, m_fpsString.c_str(), HALF_SW, HALF_SH);

	// done drawing sprites
	m_renderer->end();
}

void FuzzyApp::drawAgents(aie::Renderer2D * a_renderer)
{
	glm::vec4 colour = glm::vec4(0.0f);
	glm::vec3 position = glm::vec3(0.0f);
	// draw agents from list
	for (auto agent : m_sceneController->m_agentObjects)
	{
		colour = agent->colour();
		position = agent->position();
		a_renderer->setRenderColour(colour.r, colour.g, colour.b, colour.a);
		a_renderer->drawSprite(nullptr, position.x, position.y, agent->vitals.size, agent->vitals.size);
		// write agent name
		int nameLength = agent->name().length() * 5;
		a_renderer->drawText(m_font, agent->name().c_str(), 
			agent->position().x - (nameLength), 
			agent->position().y - (agent->movedata.sight * 0.8f)
		);
		// don't draw if dead
		if (!agent->vitals.dead)
			drawAction(*agent);
	}
}

void FuzzyApp::drawAction(Agent & a_agent)
{
	switch (a_agent.vitals.action)
	{
	case AN_WANDER:
	{
		switch (a_agent.vitals.type)
		{
		case PLAYER:
			drawWander(a_agent, *m_playerAgent->wanderPtr());
		default:
			break;
		}
		break;
	}
	case AN_EVADE:
		drawSeek(a_agent);
		break;
	case AN_FOLLOW:
		drawSeek(a_agent);
		break;
	case AN_SEEK:
		drawSeek(a_agent);
		break;
	default:
		break;
	}
	// draw agent sight radius
	drawRadius(a_agent);
}

void FuzzyApp::drawWander(Agent & a_agent, WanderAction & a_wander)
{
	// line from original location to centre of circle
	//m_renderer->drawLine(a_agent.movedata.position.x, a_agent.movedata.position.y, a_wander.controls.circleCentre.x, a_wander.controls.circleCentre.y, 1.0f);
	// line to edge of circle
	//m_renderer->drawLine(a_wander.controls.circleCentre.x, a_wander.controls.circleCentre.y, a_wander.controls.target.x, a_wander.controls.target.y, 2.0f);
	// path agent should be heading along
	m_renderer->drawLine(a_agent.movedata.position.x, a_agent.movedata.position.y, a_wander.controls.target.x, a_wander.controls.target.y, 3.0f);

	// draw a cirle with lines
	double slice = 2 * M_PI / 360;
	glm::vec2 point1(0.0f, 0.0f);


	//for (int i = 0; i < 360; i++)
	//{
	//	double angle = slice * i;
	//	glm::vec2 point2(a_wander.controls.circleCentre.x + a_wander.controls.radius * cos(angle), a_wander.controls.circleCentre.y + a_wander.controls.radius * sin(angle));
	//	if (point1.x == 0 && point1.y == 0)
	//	{
	//		point1 = point2;
	//	}
	//	m_renderer->drawLine(point1.x, point1.y, point2.x, point2.y);
	//	point1 = point2;
	//}
}

void FuzzyApp::drawSeek(Agent & a_agent)
{
	// path agent should be heading along
	glm::vec3 heading = a_agent.movedata.position + (glm::normalize(a_agent.movedata.heading) * (a_agent.movedata.sight * 0.5f));
	m_renderer->drawLine(a_agent.movedata.position.x, a_agent.movedata.position.y, heading.x, heading.y, 3.0f);
}

void FuzzyApp::drawGUI()
{
	for (auto agent : m_sceneController->m_agentObjects)
	{
		if (agent->vitals.type != ENEMY)
		{
			agent->drawGUI();
		}
		else
		{
			// draw last selected enemy
			if (agent->name() == m_enemyAgent->name())
			{
				agent->drawGUI();
			}
		}
	}
}

void FuzzyApp::drawRadius(Agent & a_agent)
{
	// draw a cirle with lines
	double slice = 2 * M_PI / 360;
	glm::vec2 point1(0.0f, 0.0f);

	float radius = a_agent.movedata.sight / 2;
	glm::vec3 center = a_agent.movedata.position;

	float circumference = a_agent.vitals.health * 3.6f;

	for (int i = 0; i < circumference; i++)
	{
		double angle = slice * i;
		glm::vec2 point2(center.x + radius * cos(angle), center.y + radius * sin(angle));
		if (point1.x == 0 && point1.y == 0)
		{
			point1 = point2;
		}
		m_renderer->drawLine(point1.x, point1.y, point2.x, point2.y);
		point1 = point2;
	}
}
glm::vec3 FuzzyApp::spawnPosition(glm::vec3 a_playerPos, glm::vec3 a_buddyPos)
{
	m_tempPos = glm::vec3(rand() % SCREEN_W, rand() % SCREEN_H, 0.0f);

	if (glm::distance(m_tempPos, a_playerPos) <= (ENEMY_SIGHT * 0.5f) ||
		glm::distance(m_tempPos, a_buddyPos) <= (ENEMY_SIGHT * 0.5f))
	{
		m_tempPos = spawnPosition(a_playerPos, a_buddyPos);
	}

	return m_tempPos;
}
/// relocate player if off screen
void FuzzyApp::offScreen()
{
	for (auto agent : m_sceneController->m_agentObjects)
	{
		// gone off east side of screen
		if (agent->movedata.position.x > (SCREEN_W + agent->vitals.size))
		{
			agent->movedata.position.x = (0.0f + agent->vitals.size);
			continue;
		}
		// gone off west side of screen
		if (agent->movedata.position.x < (0.0f - agent->vitals.size))
		{
			agent->movedata.position.x = (SCREEN_W - agent->vitals.size);
			continue;
		}
		// gone off north side of screen
		if (agent->movedata.position.y > (SCREEN_H + agent->vitals.size))
		{
			agent->movedata.position.y = (0.0f + agent->vitals.size);
			continue;
		}
		// gone off south side of screen
		if (agent->movedata.position.y < (0.0f - agent->vitals.size))
		{
			agent->movedata.position.y = (SCREEN_H - agent->vitals.size);
			continue;
		}
	}
}

