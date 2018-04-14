#include "MazeAlgorithm.h"
#include <iterator>
#include <iostream>

std::vector<std::vector<Cell>> MazeAlgorithm::cells = std::vector<std::vector<Cell>>();

MazeAlgorithm::MazeAlgorithm(int r, int c, int ux, int uy)
	:row(r), col(c), unitX(ux), unitY(uy), frame(1), adventurer(0)
{
	//�ĸ��ڽ�ǽ������
	dir[LEFT] = { glm::vec2(-unitX*0.5,+unitY*0.4),90.0f };
	dir[UP] = { glm::vec2(0 - 0.1*unitX,0),0.0f };
	dir[RIGHT] = { glm::vec2(+0.5*unitX,+unitY*0.4),90.0f };
	dir[DOWN] = { glm::vec2(0 - 0.1*unitX,unitY),0.0f };
	cells.resize(row, std::vector<Cell>(col));
	std::cout << cells.size() << std::endl;
	GetWallFromCells();
}

void MazeAlgorithm::Generation_Init() {
	paths.clear();
	solution.clear();
	//�Թ���ʼ������ʼȫ������ǽ
	for (auto it = cells.begin(); it != cells.end(); ++it) {
		for (auto in = it->begin(); in != it->end(); ++in) {
			in->neighbors[LEFT] = in->neighbors[UP] = in->neighbors[RIGHT] = in->neighbors[DOWN] = 0;
			in->visited = false;
		}
	}
	//��ʼλ�ú��յ�λ����ǽ
	cells[0][0].neighbors[LEFT] = 1;
	cells[row - 1][col - 1].neighbors[RIGHT] = 1;
	start = Index(glm::ivec2(0, 0));
	end = Index(glm::ivec2(row - 1, col - 1));
	//ð�ռ�
	adventurer = Index(glm::ivec2(0, 0));

	//dfs�Թ����ɵĸ���stack��գ�Ȼ������һ��Ԫ��-->���Ͻ�
	while (!record.empty())record.pop();
	cur = glm::ivec2(0, 0);
	record.push(cur);

	//prim�Թ����ɵĸ���list��գ�Ȼ������һ��Ԫ��-->���Ͻ�
	prim.clear();
	prim.push_back(cur);

	//�Ĳ����ָ����ɵĸ���queue��գ�Ȼ������һ��Ԫ��
	while (!recursive.empty())recursive.pop();
	recursive.push({ glm::ivec2(0,row - 1),glm::ivec2(0,col - 1) });
}

void MazeAlgorithm::Pathfinding_Init() {
	//�Թ���visited��ʼ����Ѱ·��ʼȫ������δ���ʵ�
	for (auto it = cells.begin(); it != cells.end(); ++it) {
		for (auto in = it->begin(); in != it->end(); ++in) {
			in->visited = 0;
			in->prev = glm::ivec2(-1, -1);
			in->inOpen = in->inClosed = false;
		}
	}

	//���֮ǰ����õ���·��
	solution.clear();

	//dfs�Թ�Ѱ·���
	while (!path_dfs.empty())path_dfs.pop();
	path_dfs.push(glm::ivec2(0, 0));
	//bfs�Թ�Ѱ·���
	while (!path_bfs.empty())path_bfs.pop();
	path_bfs.push(glm::vec2(0, 0));
	//A*�Թ�Ѱ·���
	while (!path_astar.empty())path_astar.pop();
	path_astar.push(Node(0, 0));
	cells[0][0].inOpen = true;
	cells[0][0].gn = 0;
	cells[0][0].hn = DirectLen(glm::ivec2(0, 0), glm::ivec2(row - 1, col - 1));

	cells[0][0].visited = 1;
}

void MazeAlgorithm::GetWallFromCells() {
	walls.clear();
	for (auto x = 0; x < row; ++x) {
		for (auto y = 0; y < col; ++y) {
			//UI�����µ����Ͻ�����
			glm::vec2 leftTop = Index(glm::ivec2(x, y));
			//std::cout << x << "," << y << std::endl;
			//��Ϊ���ظ��ģ�����ÿ����Ԫֻ������
			if (cells[x][y].neighbors[LEFT] == 0)//0������ǽ
				walls.push_back(Wall(leftTop + dir[LEFT].first, dir[LEFT].second));
			if (cells[x][y].neighbors[UP] == 0)
				walls.push_back(Wall(leftTop + dir[UP].first, dir[UP].second));
			if (y == col - 1) {//����ߵ�
				if (cells[x][y].neighbors[RIGHT] == 0)
					walls.push_back(Wall(leftTop + dir[RIGHT].first, dir[RIGHT].second));
			}
			if (x == row - 1) {//���±ߵ�
				if (cells[x][y].neighbors[DOWN] == 0)
					walls.push_back(Wall(leftTop + dir[DOWN].first, dir[DOWN].second));
			}
		}
	}
}

void MazeAlgorithm::GetPathFromCells() {
	paths.clear();
	for (auto x = 0; x < row; ++x) {
		for (auto y = 0; y < col; ++y) {
			if (cells[x][y].visited == 1) {//��������·
				glm::vec2 leftTop = Index(glm::ivec2(x, y));
				leftTop += unitX*0.2;
				paths.push_back(leftTop);
			}
		}
	}
}

bool MazeAlgorithm::Generator_Dfs() {
	frame = 5;
	while (!record.empty() && frame--) {//�����л���frame������0ʱ
		cells[cur.x][cur.y].visited = 1;//��ǵ�ǰ��λ��Ϊ���ʹ�����
		bool hasNeigh = false;//�Ƿ����ھ�δ����
		std::vector<std::pair<glm::ivec2,int>> tmp; //��¼δ���ʵ��ھ�, tmp.second���������ĸ��ھ�
		tmp.reserve(4);
		glm::ivec2 loc;
		//Ѱ���Ƿ����δ���ʵ��ھ�
		for (auto x = 0; x < 4; ++x) {
			loc = glm::ivec2(cur.x + to[x][0], cur.y + to[x][1]);
			if (CouldMove(loc) && !cells[loc.x][loc.y].visited) {//��δ���ʵ��ھ�
				tmp.push_back({ loc,x });
				hasNeigh = true;
			}
		}
		if (hasNeigh) {//��δ���ʵ��ھ������ѡ��һ��
			int got = rand() % tmp.size();
			record.push(cur);//��ǰ�Թ���Ԫ��ջ
			//���cur��tmp[got]֮���ǽ
			cells[cur.x][cur.y].neighbors[tmp[got].second] = 1;
			cells[tmp[got].first.x][tmp[got].first.y].neighbors[(tmp[got].second + 2) % 4] = 1;
			//�ǰ��Ǳ�Ϊ���ھ�
			cur = tmp[got].first;
		}
		else {//û�ҵ�һ���ھӣ���ʱ�������
			cur = record.top();
			record.pop();
		}
	}
	//frame = 1;
	//ջβ�մ����Ѿ�������
	if (record.empty())return true;
	else return false;
}

bool MazeAlgorithm::Generator_Prim() {
	frame = 5;
	//primΪlist
	while (!prim.empty() && frame--) {
		//�����list��ѡһ��
		int choice = rand() % prim.size();
		auto it = prim.begin();
		std::advance(it, (choice == 0) ? 0 : choice);
		cur = *it;
		//���Ϊ�ѷ��ʹ���Ȼ���Listɾ��
		cells[cur.x][cur.y].visited = 1;
		prim.erase(it);
		//�鿴�ھӵ����
		std::vector<std::pair<glm::ivec2, int>> tmp; //��¼δ���ʵ��ھ�
		//�ĸ��ھ�
		for (auto x = 0; x < 4; ++x) {
			glm::ivec2 loc = glm::ivec2(cur.x + to[x][0], cur.y + to[x][1]);
			if (CouldMove(loc)) {//�߽���
				//�����ʹ�������tmp�У�������Ҫ�����ȡ��Щ
				if (cells[loc.x][loc.y].visited == 1)tmp.push_back(std::pair<glm::ivec2, int>(loc, x));
				else if (cells[loc.x][loc.y].visited == 0) {
					//δ�����ʹ�������list�У������Ϊ2������������list��
					prim.push_back(loc);
					cells[loc.x][loc.y].visited = 2;
				}
			}
		}
		//�б����ʹ����ھ�
		if (!tmp.empty()) {
			//�������ѡһ������ͨ����֮���ǽ
			int got = rand() % tmp.size();
			cells[cur.x][cur.y].neighbors[tmp[got].second] = 1;
			cells[tmp[got].first.x][tmp[got].first.y].neighbors[(tmp[got].second + 2) % 4] = 1;
		}
	}
	if (prim.empty())return true;
	else return false;
}

bool MazeAlgorithm::Generator_Recursive() {
	frame = 10;
	//recursive��queue�����е�Ԫ��Ϊpair<glm::ivec2,glm::ivec2>
	//firstΪ�з�Χ��secondΪ�з�Χ
	while (!recursive.empty() && frame--) {
		std::pair<glm::ivec2, glm::ivec2> head = recursive.front();
		recursive.pop();
		//head.first == head.second����±����һ���ߣ���Ҫ���⴦��
		if (head.first.x < head.first.y && head.second.x < head.second.y) {
			glm::ivec2 center;
			//��[head.first,head.first)֮��ѡ��һ�����꣬�������������зָ�
			center.x = head.first.x + rand() % (head.first.y - head.first.x);
			center.y = head.second.x + rand() % (head.second.y - head.second.x);
			int subRow[2], subCol[2];//�洢�ĸ������ϵ������
			//��center�ĸ����������ѡȡ
			subRow[0] = head.first.x + rand() % (center.x - head.first.x + 1);
			subRow[1] = center.x + 1 + rand() % (head.first.y - center.x);
			subCol[0] = head.second.x + rand() % (center.y - head.second.x + 1);
			subCol[1] = center.y + 1 + rand() % (head.second.y - center.y);
			//��ȡ�ĸ������ϵ�����Թ���Ԫ
			glm::ivec2 meta[4];
			meta[LEFT] = glm::ivec2(center.x, subCol[0]);
			meta[UP] = glm::ivec2(subRow[0], center.y);
			meta[RIGHT] = glm::ivec2(center.x, subCol[1]);
			meta[DOWN] = glm::ivec2(subRow[1], center.y);
			int notOpen = rand() % 4;//���ѡһ���Թ���Ԫ����ͨ��ʣ�µ���������ͨ
			for (auto x = 0; x < 4; ++x) {
				if (x != notOpen) {//���������Թ���Ԫ�ڶ�
					//���Ҵ�ͨ�������棬�ϡ��´�ͨ��������
					glm::ivec2 near = (x % 2 == 0) ? glm::ivec2(meta[x].x + 1, meta[x].y) 
						: glm::ivec2(meta[x].x, meta[x].y + 1);
					//����ǽ
					int which = (x % 2 == 0) ? DOWN : RIGHT;
					//��ͨmeta[x]��near֮���ǽ
					cells[meta[x].x][meta[x].y].neighbors[which] = 1;
					cells[near.x][near.y].neighbors[(which + 2) % 4] = 1;
				}
			}
			//Ȼ���ٶԵ�ǰ���ĸ��ӿռ����ͬ���������
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(head.first.x,center.x),
				glm::ivec2(head.second.x,center.y) }));
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(head.first.x,center.x),
				glm::ivec2(center.y + 1,head.second.y) }));
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(center.x + 1,head.first.y),
				glm::ivec2(head.second.x,center.y) }));
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(center.x + 1,head.first.y),
				glm::ivec2(center.y + 1,head.second.y) }));
		}
		else if (head.first.x < head.first.y) {//�ӿռ�����һ�����ߣ�����ͬ��
			int rm = head.first.x + rand() % (head.first.y - head.first.x);
			cells[rm][head.second.x].neighbors[DOWN] = 1;
			cells[rm + 1][head.second.x].neighbors[UP] = 1;
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(head.first.x,rm),
				glm::ivec2(head.second.x,head.second.x) }));
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(rm + 1,head.first.y),
				glm::ivec2(head.second.x,head.second.x) }));
		}
		else if (head.second.x < head.second.y) {//�ӿռ�����һ�����ߣ�����ͬ��
			int cm = head.second.x + rand() % (head.second.y - head.second.x);
			cells[head.first.x][cm].neighbors[RIGHT] = 1;
			cells[head.first.x][cm+1].neighbors[LEFT] = 1;
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(head.first.x,head.first.x),
				glm::ivec2(head.second.x,cm) }));
			recursive.push(std::pair<glm::ivec2, glm::ivec2>({ glm::ivec2(head.first.x,head.first.x),
				glm::ivec2(cm + 1,head.second.y) }));
		}
	}
	if (recursive.empty()) return true;
	else return false;
}

bool MazeAlgorithm::Pathfinding_Dfs() {
	//������ȱ���Ѱ·
	frame = 3;
	while (!path_dfs.empty() && frame--) {
		glm::ivec2 head = path_dfs.top();
		path_dfs.pop();
		//Ѱ���ھ�
		for (auto x = 0; x < 4; ++x) {
			glm::ivec2 loc(head.x + to[x][0], head.y + to[x][1]);
			if (CouldMove(loc) && !cells[loc.x][loc.y].visited //δ����
				&& cells[head.x][head.y].neighbors[x] == 1) {//����ǽ����
				cells[loc.x][loc.y].prev = head;//��¼ǰ����Ȼ��Ҫ����·��
				path_dfs.push(loc);
				cells[loc.x][loc.y].visited = 1;
				//�ҵ��յ���
				if (loc == glm::ivec2(row - 1, col - 1)) {
					//���dfsջ��ֹͣ����
					while (!path_dfs.empty())path_dfs.pop();
				}
			}
		}
	}
	if (path_dfs.empty()) {//���յ㵹��·�������
		GetSolution();
		return true;
	}
	else return false;
}

bool MazeAlgorithm::Pathfinding_Bfs() {
	//������ȱ���Ѱ·->���·��
	frame = 3;
	while (!path_bfs.empty() && frame--) {
		glm::ivec2 head = path_bfs.front();
		path_bfs.pop();
		//Ѱ���ھ�
		for (auto x = 0; x < 4; ++x) {
			glm::ivec2 loc(head.x + to[x][0], head.y + to[x][1]);
			if (CouldMove(loc) && !cells[loc.x][loc.y].visited //δ����
				&& cells[head.x][head.y].neighbors[x] == 1) {//����ǽ����
				cells[loc.x][loc.y].prev = head;//��¼ǰ����Ȼ��Ҫ����·��
				path_bfs.push(loc);
				cells[loc.x][loc.y].visited = 1;
				//�ҵ��յ���
				if (loc == glm::ivec2(row - 1, col - 1)) {
					//���dfsջ��ֹͣ����
					while (!path_bfs.empty())path_bfs.pop();
				}
			}
		}
	}
	if (path_bfs.empty()) {//���յ㵹��·�������
		GetSolution();
		return true;
	}
	else return false;
}

bool MazeAlgorithm::Pathfinding_Astar() {
	frame = 3;
	while (!path_astar.empty() && frame--) {
		Node head = path_astar.top();
		path_astar.pop();
		//���Ϊ����closed��
		cells[head.index.x][head.index.y].inOpen = false;
		cells[head.index.x][head.index.y].inClosed = true;
		cells[head.index.x][head.index.y].visited = 1;
		////�ҵ��յ���
		if (head.index == glm::ivec2(row - 1, col - 1)) {
			//���queue��ֹͣ����
			while (!path_astar.empty())path_astar.pop();
			break;
		}
		for (auto x = 0; x < 4; ++x) {//�鿴�ھ�
			glm::ivec2 loc(head.index.x + to[x][0], head.index.y + to[x][1]);
			if (CouldMove(loc) && cells[head.index.x][head.index.y].neighbors[x] == 1) {//��ǽ����
				if (cells[loc.x][loc.y].inClosed)continue;//����closed���У�������
				if (!cells[loc.x][loc.y].inOpen) {//����open���У�����open��
					path_astar.push(Node(loc.x, loc.y));
					cells[loc.x][loc.y].inOpen = true;
					cells[loc.x][loc.y].prev = head.index;
					cells[loc.x][loc.y].gn = cells[head.index.x][head.index.y].gn + 1;
					cells[loc.x][loc.y].hn = DirectLen(loc, glm::ivec2(row - 1, col - 1));
				}
				else {//����open���У����ǽ��бȽϣ�Ȼ���޸�ǰ��
					int orig = cells[loc.x][loc.y].gn + cells[loc.x][loc.y].hn;
					int nows = cells[head.index.x][head.index.y].gn + cells[loc.x][loc.y].hn + 1;
					if (nows < orig) {
						cells[loc.x][loc.y].prev = head.index;
						cells[loc.x][loc.y].gn = cells[head.index.x][head.index.y].gn + 1;
						cells[loc.x][loc.y].hn = cells[loc.x][loc.y].hn;
					}
				}
			}
		}
	}
	if (path_astar.empty()) {//���յ㵹��·�������
		GetSolution();
		return true;
	}
	else return false;
}

void MazeAlgorithm::GetSolution() {
	//���յ㵹��·�������
	glm::ivec2 index(row - 1, col - 1);
	while (index != glm::ivec2(-1, -1)) {
		solution.push_back(Index(index) + unitX*0.2f);
		index = cells[index.x][index.y].prev;
	}
	adven_index = solution.size() - 1;
}

bool MazeAlgorithm::AdventurerMove() {
	if (adven_index < 0 || solution.empty())return true;
	adventurer = solution[adven_index];
	--adven_index;
	return false;
}