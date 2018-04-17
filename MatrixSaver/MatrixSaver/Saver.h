#pragma once
#include "Shader.h"
#include "Texture2D.h"

class Saver{
public:
	GLboolean		Keys[1024];//������Ӧ
	GLboolean		KeysProcessed[1024];
	GLuint			Width, Height;//���ڿ��

	Saver(GLuint width, GLuint height);

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

};

