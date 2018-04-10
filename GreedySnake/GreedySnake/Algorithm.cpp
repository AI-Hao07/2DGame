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
	return board[x][y] == 0;
}

void Algorithm::Reset() {
	for (auto &t : board) {
		std::fill(t.begin(), t.end(), 0);
	}
}

void Algorithm::Display() {
	for (auto &t : board) {
		for (auto &x : t) {
			std::cout << x << "-";
		}
		std::cout << std::endl;
	}
}

void Algorithm::Place(GLuint x, GLuint y) {
	board[y][x] = 1;
}

bool Algorithm::Search(std::pair<int, int> head, std::pair<int, int> dest, int type) {
	//����ʽ����f(n) = g(n) + h(n)
	//g(n)Ϊhead����ǰ�ڵ��ʵ�ʾ��룬h(n)Ϊ��ǰ�ڵ㵽Ŀ�ĵ�ŷ��ֱ�߾���
	if (type == 2)board[dest.first][dest.second] = 0;
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>>record;//����
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
			if (loc.first < 0 || loc.first >= gridX) continue;
			if (loc.second < 0 || loc.second >= gridY) continue;
			//������λ�������������ǰ�Ѿ����ʹ��ˣ�ֱ������
			if (prev[loc.first][loc.second].visited == 1 
				|| board[loc.first][loc.second] == 1)continue;
			//��¼���λ���Ѿ����ʹ��˲���¼����ǰ��
			prev[loc.first][loc.second].visited = 1;
			prev[loc.first][loc.second].prev = cur.index;
			record.push(Node(loc.first, loc.second, cur.hn + 1, DirectLen(loc, dest)));
			//����Ѿ�����Ŀ�ĵأ���ֹͣ����
			if (loc == dest) {
				flag = true;
				if(type != 2)break;//type == 2��������β�ģ�Ѱ���·��
			}
		}
	}
	if (type == 2)board[dest.first][dest.second] = 1;
	if (!flag) return false;
	if (type == 3)return true;
	//�ҵ���·����Ŀ�Ŀ�ʼ����
	std::pair<int, int> in = dest;
	while (prev[in.first][in.second].prev != head){
		in = prev[in.first][in.second].prev;
	}
	path = in;
	return true;
}

//�ж�������һ��������ȫ
bool Algorithm::IsSafe(std::pair<int, int> head, std::pair<int, int> tail,
	std::pair<int, int> tailPrev, bool isFood) {
	std::pair<int, int> realTail;
	//�������һ��
	realTail = (!isFood) ? tailPrev : tail;
	//realTail = tail;
	board[path.first][path.second] = 1;
	board[realTail.first][realTail.second] = 0;
	board[tail.first][tail.second] = 0;
	bool ret = Search(path, realTail, 3);
	//�ָ�ԭλ
	board[path.first][path.second] = 0;
	board[realTail.first][realTail.second] = 1;
	board[tail.first][tail.second] = 1;
	//�ƶ���ͷ��β������һ��
	//int ds = abs(realTail.first - path.first) + abs(realTail.second - path.second);
	//if (ds == 1)std::cout << "very unsafe\n";
	return ret;
}

bool Algorithm::JustFindOne(std::pair<int, int> head) {
	for (auto x = 0; x < 4; ++x) {
		//�߽紦��
		if (head.first + dir[x][0] < 0 || head.first + dir[x][0] >= gridX) continue;
		if (head.second + dir[x][1] < 0 || head.second + dir[x][1] >= gridY) continue;
		//���һ�������ߣ�ֻҪû����ռ�ĵط�����
		if (board[head.first + dir[x][0]][head.second + dir[x][1]] == 0) {
			path = (std::pair<int, int>(head.first + dir[x][0], head.second + dir[x][1]));
			return true;
		}
	}
	return false;//��;ĩ·
}

