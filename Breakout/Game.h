#pragma once
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "PowerUp.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game
{
public:
	//����ֵ
	GLuint Lives;
	//��ǰ��Ϸ״̬
	GameState		State;
	GLboolean		Keys[1024];//������
	GLboolean KeysProcessed[1024];
	GLuint			Width, Height;//��Ϸ���
	//��Ϸ�ؿ�
	std::vector<GameLevel> Levels;
	GLuint                 Level;

	//����
	std::vector<PowerUp>  PowerUps;
	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);

	Game(GLuint width, GLuint height);
	~Game();
	//��Ϸ��ʼ��
	void Init();
	//��Ϸѭ��
	void ProcessInput(GLfloat dt);//��������
	void Update(GLfloat dt);//����
	void Render();//��Ⱦ

	//��ײ���
	void DoCollisions();
private:
	GLboolean CheckCollisionAABB(GameObject &one, GameObject &two);
	Collision CheckCollisionSphere(BallObject &one, GameObject &two);
	Direction VectorDirection(glm::vec2 target);
	void ResetLevel();
	void ResetPlayer();
};

