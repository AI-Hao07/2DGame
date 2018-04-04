#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "GameObject.h"
#include "ResourceManager.h"

class GameLevel
{
public:
	//��¼ÿ��ש�飬ÿ��ש�鶼��һ��GameObject
	std::vector<GameObject> Bricks;

	GameLevel() { }
	// ���ļ��м��عؿ�
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// ��Ⱦ�ؿ�
	void Draw(SpriteRenderer &renderer);
	// ���һ���ؿ��Ƿ������ (���зǼ�Ӳ�Ĵ�ש�����ݻ�)
	GLboolean IsCompleted();
private:
	// ��ש�����ݳ�ʼ���ؿ�
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};

