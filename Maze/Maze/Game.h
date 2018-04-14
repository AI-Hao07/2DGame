#pragma once
#include <memory>
#include <irrKlang.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using namespace irrklang;
#include "Shader.h"
#include "Texture2D.h"
#include "TextRenderer.h"
#include "PostProcessor.h"
#include "ParticleGenerator.h"
#include "GameObject.h"
#include "MazeAlgorithm.h"

//��Ϸ״̬
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOST,
	GAME_PAUSE
};

class Game
{
public:
	GLboolean		Keys[1024];//������Ӧ
	GLboolean		KeysProcessed[1024];
	GLuint			Width, Height;//��Ϸ���
	GameState		State;//��Ϸ״̬
	Game(GLuint width,GLuint height);
	~Game() = default;
	//��Ϸ��ʼ��
	void Init();
	//��������
	void ProcessInput(GLfloat dt);
	//��Ϸ����
	void Update(GLfloat dt);
	//��Ⱦ
	void Render();

private:
	//��Ϸ�ؼ�
	std::shared_ptr<SpriteRenderer> sprite;
	std::shared_ptr<TextRenderer> text;
	std::shared_ptr<ISoundEngine> sound;
	std::shared_ptr<GameObject> wall;
	std::shared_ptr<GameObject> terminal;
	std::shared_ptr<GameObject> player;
	std::shared_ptr<MazeAlgorithm> maze;
	std::shared_ptr<ParticleGenerator> particle;
	//�Թ������㷨
	int type;
	//��Ԫ���Ӵ�С
	int unitX, unitY;
	GLuint gridX, gridY;
	//��������ʼ״̬
	void Reset();

};

