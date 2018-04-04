#pragma once
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "GameLevel.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	//��ǰ��Ϸ״̬
	GameState		State;
	GLboolean		Keys[1024];//������Ӧ
	GLuint			Width, Height;//��Ϸ���
	//��Ϸ�ؿ�
	std::vector<GameLevel> Levels;
	GLuint                 Level;

	Game(GLuint width, GLuint height);
	~Game();
	//��Ϸ��ʼ��
	void Init();
	//��Ϸѭ��
	void ProcessInput(GLfloat dt);//��������
	void Update(GLfloat dt);//����
	void Render();//��Ⱦ
};

