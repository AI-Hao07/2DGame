#pragma once
#include <vector>
#include <stack>
#include <list>
#include <queue>
#include <glm/glm.hpp>

enum Neighbor { LEFT = 0, UP = 1, RIGHT = 2, DOWN = 3 };

struct Cell {//�Թ���Ԫ
	int neighbors[4];//�ĸ�������ھ�
	int visited;//��¼�Ƿ���ʹ���
	//��������Ѱ·�㷨
	glm::ivec2 prev;//��¼ǰ��
	//����A���㷨��open��closed��
	bool inOpen, inClosed;
	//����ʽ����fn = gn + hn
	//����gnΪ��㵽n��ʵ�ʾ��룬hnΪn���յ�Ĺ��ܶ�
	int gn, hn;
	Cell() :visited(0),inOpen(false),inClosed(false) {
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

	void Generation_Init();
	void Pathfinding_Init();

	void GetWallFromCells();//���Թ���Ԫ�������ҵ�ȫ��ǽȻ��ŵ�walls��
	void GetPathFromCells();//���Թ���Ԫ�������ҵ�����������·���ŵ�paths��

	//�Թ������㷨
	//�㷨һ����������㷨
	bool Generator_Dfs();
	//�㷨����Prim����㷨
	bool Generator_Prim();
	//�㷨�����ݹ�ָ��㷨
	bool Generator_Recursive();

	//�Թ�Ѱ·�㷨��
	//�㷨һ��������ȱ���
	bool Pathfinding_Dfs();
	//�㷨����������ȱ���
	bool Pathfinding_Bfs();
	//�㷨����A���㷨
	bool Pathfinding_Astar();

	//����ǰ������·��
	void GetSolution();
	//�ƶ�ð�ռ�һ��
	bool AdventurerMove();

	std::vector<Wall> walls;//�Թ���ǽ
	std::vector<glm::vec2> paths;//������·
	std::vector<glm::vec2> solution;//���յ�·��
	glm::vec2 start, end;//�Թ��������յ�
	glm::vec2 adventurer;//�Թ�ð�ռҵ�λ��
	int adven_index;//ð�ռ���solution�е�����

private:
	const int row, col;//�Թ���Ԫ���С�����
	static std::vector<std::vector<Cell>> cells;//�Թ���Ԫ����
	struct Node {//A���㷨�Ľ��
		glm::ivec2 index;//cells������±�����
		Node(int r, int c) :index((glm::ivec2(r, c))) {}
	};
	class Compare {//���ȶ��еıȽϷº�������
	public:
		bool operator()(const Node& n1, const Node& n2) {
			return cells[n1.index.x][n1.index.y].gn + cells[n1.index.x][n1.index.y].hn
				> cells[n2.index.x][n2.index.y].gn + cells[n2.index.x][n2.index.y].hn;
		}
	};

	//�Թ���Ԫ�Ĵ�С
	const int unitX, unitY;
	
	//�Թ������㷨һ���ݽṹ:������ȵ�ջ
	std::stack<glm::ivec2> record;
	//�Թ������㷨�����ݽṹ�����ɾ��������
	std::list<glm::ivec2> prim;
	//�Թ������㷨�����ݽṹ���Ĳ���������ȵĶ���
	std::queue<std::pair<glm::ivec2, glm::ivec2>> recursive;

	//�Թ�Ѱ·�㷨һ���ݽṹ��������ȵ�ջ
	std::stack<glm::ivec2> path_dfs;
	//�Թ�Ѱ·�㷨һ���ݽṹ��������ȵĶ���
	std::queue<glm::ivec2> path_bfs;
	//�Թ�Ѱ·�㷨һ���ݽṹ��A���㷨�����ȶ���
	std::priority_queue<Node,std::vector<Node>,Compare> path_astar;

	//�����Թ��ĵ�ǰ��ǵ�λ��
	glm::ivec2 cur;
	//ÿһ֡�Թ����ɽ�չ�Ĵ�����������ʾ����
	int frame;

	//����ά����ת����UI���꣬ע��˳�����෴��
	inline glm::vec2 Index(const glm::ivec2 &index) {
		return glm::vec2(index.y*unitX + unitX*0.8, index.x*unitY + unitX*0.8);
	}

	//�Թ���Ԫ�ĸ������UI����
	std::pair<glm::vec2, float> dir[4];
	//��ά����Ľ�������ϡ��ҡ����ĸ���չ����
	const int to[4][2] = {
		{ 0,-1 },{ -1,0 },{ 0,+1 },{ +1,0 }
	};

	//�߽���
	inline bool CouldMove(const glm::ivec2 &target) {
		if (target.x < 0 || target.x >= row)return false;
		if (target.y < 0 || target.y >= col)return false;
		return true;
	}

	//������ܶپ���
	inline int DirectLen(const glm::ivec2& n1, const glm::ivec2& n2) {
		return abs(n1.x - n2.x) + abs(n1.y - n2.y);
	}
};

