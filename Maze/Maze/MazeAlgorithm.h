#pragma once
#include <vector>
#include <stack>
#include <glm/glm.hpp>

enum Neighbor { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

struct Cell {//�Թ���Ԫ
	int neighbors[4];//�ĸ�������ھ�
	bool visited;//��¼�Ƿ���ʹ���
	Cell() :visited(false) {
		neighbors[LEFT] = neighbors[UP] = neighbors[RIGHT] = neighbors[DOWN] = 0;
	}
};

struct Wall {//ǽ
	glm::vec2 Pos;//UI����
	float angle;//��ת�Ƕ�
	Wall(glm::vec2 pos, float ang) :Pos(pos), angle(ang) {}
};

class MazeAlgorithm{
public:
	MazeAlgorithm(int r,int c, int ux, int uy);

	void Init();//��ʼ��

	void GetWallFromCells();//���Թ���Ԫ�������ҵ�ȫ��ǽȻ��ŵ�walls��

	//�Թ������㷨
	//�㷨һ����������㷨
	bool Generator_Dfs();

	std::vector<Wall> walls;//�Թ���ǽ

private:
	const int row, col;//�Թ���Ԫ���С�����
	std::vector<std::vector<Cell>> cells;//�Թ���Ԫ����

	//�Թ���Ԫ�Ĵ�С
	const int unitX, unitY;
	
	//dfs���������Թ���ջ
	std::stack<glm::ivec2> record;
	//dfs���������Թ��ĵ�ǰ��ǵ�λ��
	glm::ivec2 cur;
	//ÿһ֡dfs̽���Ĵ���
	int frame;

	//����ά����ת����UI���꣬ע��˳�����෴��
	inline glm::vec2 Index(const glm::ivec2 &index) {
		/*if(index.y == 0)
			return glm::vec2(index.y*unitX + unitX*0.5, index.x*unitY + unitX*0.5);
		else */
			return glm::vec2(index.y*unitX + unitX*0.5, index.x*unitY + unitX*0.5);
	}

	//�Թ���Ԫ�ĸ������UI����
	std::pair<glm::vec2, float> dir[4];
	//��ά����Ľ�����ϡ��¡������ĸ���չ����
	const int to[4][2] = {
		{ 0,-1 },{ -1,0 },{ 0,+1 },{ +1,0 }
	};

	//�߽���
	inline bool CouldMove(const glm::ivec2 &target) {
		if (target.x < 0 || target.x >= row)return false;
		if (target.y < 0 || target.y >= col)return false;
		return true;
	}
};

