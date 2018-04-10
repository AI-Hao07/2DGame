#pragma once
#include <list>
#include <glm/glm.hpp>
#include "Algorithm.h"
#include "GameObject.h"
#include "CollisionDetect.h"

class SnakeObject : public GameObject{
public:

	SnakeObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

	~SnakeObject();

	void DrawSnake(std::list<Object> &target, SpriteRenderer &render, Texture2D  &headSprite);

private:
	//����ά����ת����UI���꣬ע��˳�����෴��
	inline glm::vec2 Index(const glm::ivec2 &index) {
		return glm::vec2(index.y*this->Size.x, index.x*this->Size.y);
	}
};

