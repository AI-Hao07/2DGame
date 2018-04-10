#pragma once
#include <list>
#include <glm/glm.hpp>
#include "Algorithm.h"
#include "GameObject.h"
#include "CollisionDetect.h"

struct Body {//����
	glm::vec2 pos;
	glm::vec2 velocity;//��¼ÿ������ķ���
	glm::vec3 Color;//ÿ���������ɫ
	Body(GLfloat r, GLfloat c) :pos(glm::vec2(r,c)) {
		int decision = rand() % 4;
		switch (decision) {
		case 0:Color = glm::vec3(0.2f, 0.6f, 1.0f); break;
		case 1:Color = glm::vec3(0.0f, 0.7f, 0.0f); break;
		case 2:Color = glm::vec3(0.8f, 0.8f, 0.4f); break;
		case 3:Color = glm::vec3(1.0f, 0.5f, 0.0f); break;
		default:
			Color = glm::vec3(1.0f, 0.5f, 0.0f); break;
		}
	}
};
const int totalFrame = 9;
const float perFrame = 0.125;

class SnakeObject :
	public GameObject
{
public:
	//��һ������
	glm::vec2 nextdir;
	SnakeObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);
	~SnakeObject();

	//�������
	void AddBody(glm::vec2 pos, bool way = false);
	//�ƶ��������ģʽ
	void Move(GLfloat dt, Algorithm &algorithm);
	//�ƶ�����AIģʽ
	void MoveByAi(glm::vec2 step, Algorithm &algorithm);
	//����
	void Reset(glm::vec2 position, glm::vec2 velocity, Algorithm &algorithm);
	//��Ⱦ
	//��ͷ
	void DrawHead(SpriteRenderer &render, Texture2D  &Sprite);
	virtual void Draw(SpriteRenderer &renderer);
	//��ȡ��ͷλ��
	glm::vec2 GetHeadPos();
	//��ȡβ��λ��
	glm::vec2 GetTailPos() {
		return snake.back().pos;
	}
	//��ȡ�����ڶ�����λ��
	glm::vec2 GetTailPrevPos() {
		auto it = snake.rbegin();
		++it;
		return it->pos;
	}
	//�Լ����Լ�
	bool isCollisionSelf();
private:
	std::list<Body> snake;
	int frameCounter;
};

