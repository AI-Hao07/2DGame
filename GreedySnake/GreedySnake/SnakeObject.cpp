#include "SnakeObject.h"
#include <iostream>

SnakeObject::SnakeObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
	: GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f),
		velocity),frameCounter(1),nextdir(this->Velocity)
{
	snake.push_back(Body(pos.x, pos.y));
	snake.front().velocity = this->Velocity;
	AddBody(pos);
	AddBody(pos);
	test = 0;
}

SnakeObject::~SnakeObject()
{
}

//�������
void SnakeObject::AddBody(glm::vec2 pos) {
	pos = snake.front().pos + this->Velocity;
	snake.push_front(Body(pos.x, pos.y));
	this->Position = pos;
	snake.front().velocity = this->Velocity;
}

//�ƶ�����
void SnakeObject::Move(GLfloat dt) {
	//�ƶ�����
	++frameCounter;
	test += perFrame;
	//�ƶ�ÿ������
	for (std::list<Body>::iterator it = snake.begin(); it != snake.end(); ++it) {
		it->pos += it->velocity*perFrame;
	}
	this->Position = snake.front().pos;
	if (frameCounter == totalFrame) {
		std::cout << test << std::endl;
		test = 0;
		frameCounter = 1;
		//����ÿ��������ٶȷ���ǰһ�������ǰһ���ٶȸ�����һ������ĵ�ǰ�ٶ�
		glm::vec2 prevVel = this->Velocity;
		std::list<Body>::iterator it = snake.begin();
		it->velocity = nextdir;
		this->Velocity = nextdir;//��¼��ͷ���ٶ�
		for (++it; it != snake.end(); ++it) {
			glm::vec2 tmp = it->velocity;
			it->velocity = prevVel;
			prevVel = tmp;
		}
	}
}

void SnakeObject::Draw(SpriteRenderer &renderer) {
	for (auto & body : snake) {
		renderer.DrawSprite(this->Sprite, body.pos, this->Size, this->Rotation, body.Color);
	}
}

//����
void SnakeObject::Reset(glm::vec2 position, glm::vec2 velocity) {
	snake.clear();
	this->Position = position;
	this->Velocity = velocity;
	this->nextdir = velocity;
	snake.push_back(Body(position.x, position.y));
	snake.front().velocity = this->Velocity;
	AddBody(position);
	AddBody(position);
}

glm::vec2 SnakeObject::GetHeadPos() {//��ȡ��ͷλ��
	return snake.front().pos;
}

bool SnakeObject::isCollisionSelf(){//�Ƿ�Ե��Լ�
	std::list<Body>::iterator it = snake.begin();
	for (++it; it != snake.end(); ++it) {
		if (CollisionDetect::CheckCollision2(snake.front().pos, this->Size, it->pos, this->Size))
		{
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}