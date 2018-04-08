#pragma once
#include "GameObject.h"

//��ײ�ķ���
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
//����Բ�κ;��ε���ײ���
//��ײ�ķ���ֵ���ֱ����Ƿ���ײ����ײ����Բ�ĺ�������ʸ����
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class CollisionDetect
{
public:

	//���κ;�����ײ���
	static bool CheckCollision(GameObject &one, GameObject &two);

	//���κ�Բ����ײ���
	static bool CheckColision1(GameObject &one, GameObject &two);

	//Բ�κ�Բ����ײ���
	static bool CheckCollision2(GameObject &one, GameObject &two);

	//Բ�κ�Բ����ײ���
	static bool CheckCollision2(glm::vec2 &one, glm::vec2 &size1, glm::vec2 &two, glm::vec2 &size2);

	//��ǽ����ײ���
	static bool CheckCollisionWidthWall(GameObject &one, GLuint &width, GLuint &height);
private:
	CollisionDetect() {}
};

