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
#include "SnakeObject.h"
#include "GameObject.h"
#include "Algorithm.h"

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
	~Game();
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
	std::shared_ptr<PostProcessor> effects;
	std::shared_ptr<ISoundEngine> sound;
	std::shared_ptr<SnakeObject> snake;
	std::shared_ptr<GameObject> food;
	std::shared_ptr<GameObject> firework;
	std::shared_ptr<ParticleGenerator> temptation, boom[3];
	std::shared_ptr<Algorithm> algorithm;//�㷨
	//��Ԫ���Ӵ�С
	int unitX, unitY;
	GLuint gridX, gridY;
	double firetimer[3];//�̻���ʱ
	int fireindex;//�������ĸ��̻�
	int score;//��ǰ����
	//��������ʼ״̬
	void Reset();
	//����ά����ת����UI���꣬ע��˳�����෴��
	inline glm::vec2 Index(const glm::ivec2 &index) {
		return glm::vec2(index.y*unitX, index.x*unitY);
	}
};

