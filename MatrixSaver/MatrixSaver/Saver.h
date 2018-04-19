#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include "CharList.h"
#include "SpriteRenderer.h"
#include <list>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>

class Saver{
public:
	GLboolean		Keys[1024];//������Ӧ
	GLboolean		KeysProcessed[1024];
	GLuint			Width, Height;//���ڿ��
	float znear, zfar, aspect, fovy;

	Saver(GLuint width = 800, GLuint height = 600,
		GLfloat zn = 1.0f, GLfloat zf = 70.0f, GLfloat ap = 1.0, GLfloat fy = 45.0f);

	~Saver() = default;

	//��ʼ��
	void Init();
	//����
	void ProcessInput(GLfloat dt);
	//����
	void Update(GLfloat dt);
	//��Ⱦ
	void Render();

private:
	std::vector<CharList*> matrix;
	std::shared_ptr<SpriteRenderer> sprite;
	float timeCounter;
};

