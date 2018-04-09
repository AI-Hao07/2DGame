#include "Algorithm.h"
#include <algorithm>
#include <iostream>
#include <functional>

Algorithm::Algorithm(GLuint x, GLuint y)
	:gridX(x),gridY(y)
{
	board.resize(gridX, std::vector<int>(gridY, 0));
}

Algorithm::~Algorithm()
{
}

bool Algorithm::CouldPlaceFood(GLuint x, GLuint y){
	//board[y][x] <= 0�������λ��û������ռλ
	return board[y][x] <= 0;
}

void Algorithm::Reset() {
	for (auto &t : board) {
		std::fill(t.begin(), t.end(), 0);
	}
}

void Algorithm::Place(GLuint x, GLuint y) {
	board[y][x] = 1;
}

void Algorithm::Display() {
	for (auto &t : board) {
		for (auto &u : t) {
			std::cout << u << "-";
		}
		std::cout << std::endl;
	}
	std::cout << "----------------------\n";
}

bool Algorithm::Brute_force(std::pair<int, int> head, std::pair<int, int> dest){
	//����ʽ����f(n) = g(n) + h(n)
	//g(n)Ϊhead����ǰ�ڵ��ʵ�ʾ��룬h(n)Ϊ��ǰ�ڵ㵽Ŀ�ĵ�ŷ��ֱ�߾���
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>>record;
	Node first(head.first,head.second,0, DirectLen(head,dest));
	record.push(first);
	//��������·������¼ÿ�����ǰ��
	std::vector<std::vector<State>> prev;
	//��ʼ������ʱÿ���ڵ��ǰ��Ϊ-1������û��ǰ��
	prev.resize(this->gridX, std::vector<State>(this->gridY));
	//����Ѿ����ӽ���������
	prev[head.first][head.second].visited = 1;
	//��¼�Ƿ��ܹ��ҵ�һ������㵽Ŀ�ĵص�·��
	bool flag = false;
	while (!record.empty()) {
		Node cur = record.top();
		record.pop();
		//������ĸ�������չ
		for (auto x = 0; x < 4; ++x) {
			std::pair<int, int> loc = cur.index;
			loc.first += dir[x][0];
			loc.second += dir[x][1];
			//�߽��ж�
			if (loc.first < 0 || loc.first >= gridX)continue;
			if (loc.second < 0 || loc.second >= gridY)continue;
			//������λ�������������ǰ�Ѿ����ʹ��ˣ�ֱ������
			if (prev[loc.first][loc.second].visited == 1 || board[loc.first][loc.second] == 1)continue;//��bug
			//��¼���λ���Ѿ����ʹ���
			prev[loc.first][loc.second].visited = 1;
			//��¼ǰ��
			prev[loc.first][loc.second].prev = cur.index;
			//���������
			record.push(Node(loc.first, loc.second, cur.hn + 1, DirectLen(loc, dest)));
			//����Ѿ�����Ŀ�ĵأ���ֹͣ����
			if (loc == dest) {
				flag = true;
				//std::cout << "Yes" << std::endl;
				//std::cout << loc.first << "," << loc.second << std::endl;
				break;
			}
		}
	}
	//û���ҵ�·,���ĸ�����Ϲ��
	if (!flag) {
		for (auto x = 0; x < 4; ++x) {
			if (board[head.first + dir[x][0]][head.second + dir[x][1]] == 0) {
				path.push_front(std::pair<int, int>(head.first + dir[x][0],
					head.second + dir[x][1]));
				return true;
			}
		}
		//���һ�����߲��ˣ�ֱ�Ӳ�����
		return false;
	}
	//�ҵ���·����Ŀ�Ŀ�ʼ����
	path.clear();
	std::pair<int, int> in = dest;
//	std::cout << "find path:\n";
	while (prev[in.first][in.second].prev != std::pair<int, int>(-1, -1)) 
	{
		path.push_front(in);
		//std::cout << in.first << "," << in.second << "->";
		in = prev[in.first][in.second].prev;
	}
	std::cout << std::endl;
	return true;
}