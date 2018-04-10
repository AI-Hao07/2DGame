#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <queue>
#include <list>

//����࣬����bfs����
struct Node {
	std::pair<int, int> index;
	int hn, gn;//����ʽ����
	Node(int x, int y, int h, int g) :index(std::pair<int, int>(x, y)), hn(h), gn(g) {}
	bool operator>(const Node &n)const {
		return (this->hn + this->gn) > (n.hn + n.gn);
	}
	bool operator<(const Node &n)const {
		return (this->hn + this->gn) < (n.hn + n.gn);
	}
};

//״̬���ü�¼��ά������ÿ�����״̬���Ƿ񱻷��ʹ�����¼���ǵ�ǰ��
struct State {
	std::pair<int, int> prev;//ǰ����ǰ��Ϊ-1����û��ǰ��
	int visited;//��¼�Ƿ���ʹ�
	State() :prev(std::pair<int, int>(-1, -1)), visited(0) {}
};

//��ά����Ľ�����ϡ��¡������ĸ���չ����
const int dir[][2] = {
	{-1,0},{+1,0},{0,-1},{0,+1}
};

//�㷨��
class Algorithm
{
public:
	Algorithm(GLuint x,GLuint y);
	~Algorithm();

	//�ж��ܷ��ڴ˴�����ʳ��
	bool CouldPlaceFood(GLuint x, GLuint y);
	//����
	void Reset();
	//����������(y,x)Ϊ1���������λ�ñ���ռ��
	void Place(GLuint x, GLuint y);

	//�˹����ܲ���
	//bfs+��ѡ����Ѱ��һ������·��
	bool Search(std::pair<int, int> head, std::pair<int, int> dest,int type);
	//�����һ��·
	bool JustFindOne(std::pair<int, int> head);
	//�ж�������һ��������ȫ
	bool IsSafe(std::pair<int,int> head,std::pair<int,int> tail, std::pair<int,int> tailPrev, bool isFood);
	//·���ԣ��洢���ƶ���һ����·��
	std::pair<int,int> path;

	void Display();
	
private:
	GLuint gridX, gridY;
	std::vector<std::vector<int> >board;

	//���������±�֮���ֱ�߾���
	int DirectLen(const std::pair<int, int> &n1, const std::pair<int, int> &n2) {
		return std::sqrt((n1.first - n2.first)*(n1.first - n2.first)
			+ (n1.second - n2.second)*(n1.second - n2.second));
	}
};

