#include "CollisionDetect.h"
#include <iostream>

//���κ;�����ײ���
bool CollisionDetect::CheckCollision(GameObject &one, GameObject &two)
{
	//���Ǽ���һ����������Ҳ��Ƿ���ڵڶ������������ಢ�ҵڶ�����������Ҳ��Ƿ���ڵ�һ�����������ࣻ
	//��ֱ������������ơ�
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x
		&& two.Position.x + two.Size.x >= one.Position.x;
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y
		&& two.Position.y + two.Size.y >= one.Position.y;
	return collisionX && collisionY;
}

//���κ�Բ����ײ���
bool CollisionDetect::CheckColision1(GameObject &one, GameObject &two)
{
	// ��ȡԲ������ 
	glm::vec2 center(one.Position + one.Size/2.0f);//Position�����Ͻ�
												// ����AABB����Ϣ�����ġ���߳���
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	// ��ȡ�������ĵĲ�ʸ��
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// AABB_center����clamped�����͵õ�����ײ���Ͼ���Բ����ĵ�closest
	glm::vec2 closest = aabb_center + clamped;
	// ���Բ��center�������closest��ʸ�����ж��Ƿ� length <= radius
	difference = closest - center;
	if (glm::length(difference) <= one.Size.x/2)
		return GL_TRUE;
	else
		return GL_FALSE;
}

bool CollisionDetect::CheckCollision2(GameObject &one, GameObject &two)
{
	glm::vec2 center1 = glm::vec2(one.Position + one.Size / 2.0f);
	glm::vec2 center2 = glm::vec2(two.Position + two.Size / 2.0f);
	glm::vec2 difference = center1 - center2;
	if (glm::length(difference) < one.Size.x / 2 + two.Size.x / 2)
		return GL_TRUE;
	else
		return GL_FALSE;
}

bool CollisionDetect::CheckCollisionWidthWall(GameObject &one, GLuint &width, GLuint &height)
{
	glm::vec2 pos = one.Position;
	if (pos.x < 0 || (pos.x + one.Size.x) > width) {
		//std::cout << "WALL->" << (pos.x) << std::endl;
		return GL_TRUE;
	}
	if (pos.y < 0 || (pos.y + one.Size.y) > height)return GL_TRUE;
	return GL_FALSE;
}

bool CollisionDetect::CheckCollision2(glm::vec2 &one, glm::vec2 &size1, glm::vec2 &two, glm::vec2 &size2)
{
	glm::vec2 center1 = glm::vec2(one + size1 / 2.0f);
	glm::vec2 center2 = glm::vec2(two + size2 / 2.0f);
	glm::vec2 difference = center1 - center2;
	if (glm::length(difference) < size1.x / 3 + size2.x / 3) {
		std::cout << one.x << "," << one.y << "->" << two.x << "," << two.y << std::endl;
		std::cout << "Yes" << std::endl;
		return GL_TRUE;
	}
	else
		return GL_FALSE;
}