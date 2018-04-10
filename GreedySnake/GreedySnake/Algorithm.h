#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <queue>
#include <list>

struct Object {
	glm::ivec2 Index;//�����±�
	glm::vec3 Color;//��ɫ
	Object(int r,int c) :Index(r, c) {
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

//�㷨�߼���
class Algorithm{
public:
	Algorithm(GLuint x,GLuint y);
	//��������µ�ʳ��
	glm::ivec2 NewFood();
	//����board�����
	void ResetBoard(std::list<Object> &psnake, Object &pfood,
		std::vector<std::vector<int> > &pboard);
	//��ʼ���ߺ�ʳ��
	void ResetSnakeAndFood();
	//������ȱ�������board�����
	bool RefreshBoard(std::list<Object> &psnake, Object &pfood, 
		std::vector<std::vector<int> > &pboard);
	//�ҵ�һ����ȫ��·��
	glm::ivec2 FindSafeWay();
	//�����һ��·
	glm::ivec2 AnyPossibleWay();
	//AI˼��
	glm::ivec2 AIThinking();
	void Display();
	//�ƶ�����
	bool make_move(glm::ivec2 step);
	std::list<Object> snake;//��
	std::shared_ptr<Object> food;//ʳ��
	
private:
	//����������
	GLuint row, col;
	//�������board��ÿ��λ�õ�״����0�ǿյģ�1������2��ʳ��
	std::vector<std::vector<int> >board;
	int EMPTY, SNAKE, FOOD;
	//�߽��ж�
	inline bool CouldMove(glm::ivec2 &target) {
		if (target.x < 0 || target.x >= row)return false;
		if (target.y < 0 || target.y >= col)return false;
		return true;
	}
	//��ά����Ľ�����ϡ��¡������ĸ���չ����
	const int dir[4][2] = {
		{ -1,0 },{ +1,0 },{ 0,-1 },{ 0,+1 }
	};
	//�ҵ�һ����̵�·���ķ���
	inline glm::ivec2 ShortestMove(std::list<Object> &psnake, 
		std::vector<std::vector<int> >pboard){
		int minv = SNAKE;
		glm::ivec2 move;
		for (auto x = 0; x < 4; ++x) {
			glm::ivec2 tmp = glm::ivec2(psnake.front().Index.x + dir[x][0], psnake.front().Index.y + dir[x][1]);
			if (CouldMove(tmp) && minv > pboard[tmp.x][tmp.y]) {
				minv = pboard[tmp.x][tmp.y];
				move = tmp;
			}
		}
		return move;
	}
};

