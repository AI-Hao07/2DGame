#include "MazeAlgorithm.h"

MazeAlgorithm::MazeAlgorithm(int r, int c, int ux, int uy)
	:row(r), col(c), unitX(ux), unitY(uy), frame(1)
{
	cells.resize(row, std::vector<Cell>(col));
	//�ĸ��ڽ�ǽ������
	dir[LEFT] = { glm::vec2(-unitX*0.5,+unitY*0.4),90.0f };
	dir[UP] = { glm::vec2(0 - 0.1*unitX,0),0.0f };
	dir[RIGHT] = { glm::vec2(+0.5*unitX,+unitY*0.4),90.0f };
	dir[DOWN] = { glm::vec2(0 - 0.1*unitX,unitY),0.0f };
	GetWallFromCells();
}

void MazeAlgorithm::Init() {
	//�Թ���ʼ������ʼȫ������ǽ
	for (auto it = cells.begin(); it != cells.end(); ++it) {
		for (auto in = it->begin(); in != it->end(); ++in) {
			in->neighbors[LEFT] = in->neighbors[UP] = in->neighbors[RIGHT] = in->neighbors[DOWN] = 0;
			in->visited = false;
		}
	}
	//dfs�Թ����ɵĸ���stack��գ�Ȼ������һ��Ԫ��-->���Ͻ�
	while (!record.empty())record.pop();
	cur = glm::ivec2(0, 0);
	record.push(cur);
}

void MazeAlgorithm::GetWallFromCells() {
	walls.clear();
	for (auto x = 0; x < row; ++x) {
		for (auto y = 0; y < col; ++y) {
			//UI�����µ����Ͻ�����
			glm::vec2 leftTop = Index(glm::ivec2(x, y));
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

bool MazeAlgorithm::Generator_Dfs() {
	frame = 10;
	while (!record.empty() && frame--) {//�����л���frame������0ʱ
		cells[cur.x][cur.y].visited = true;//��ǵ�ǰ��λ��Ϊ���ʹ�����
		bool hasNeigh = false;//�Ƿ����ھ�δ����
		std::vector<std::pair<glm::ivec2,int>> tmp; //��¼δ���ʵ��ھ�
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
	frame = 1;
	//ջβ�մ����Ѿ�������
	if (record.empty())return true;
	else return false;
}