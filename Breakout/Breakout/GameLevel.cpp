#include "GameLevel.h"

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight){
	// ��չ�������
	this->Bricks.clear();
	// ���ļ��м���
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);//�ļ���
	std::vector<std::vector<GLuint>> tileData;
	if (fstream){//һ��һ�ж�ȡ
		while (std::getline(fstream, line)) // ��ȡ�ؿ��ļ���ÿһ��
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) // ��ȡ���ո�ָ���ÿ������
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)//��ש�����ݳ�ʼ���ؿ�
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint lvlWidth, GLuint lvlHeight){
	// ����ÿ��ά�ȵĴ�С
	GLuint height = tileData.size();
	GLuint width = tileData[0].size();
	//���ȷ���ÿ��ש��Ĵ�С
	GLfloat unit_width = lvlWidth / static_cast<GLfloat>(width);
	GLfloat unit_height = lvlHeight / height;
	// ����tileDataC��ʼ���ؿ�     
	for (GLuint y = 0; y < height; ++y){
		for (GLuint x = 0; x < width; ++x){
			// ���ש������
			if (tileData[y][x] == 1){//���ɴݻٵ�ש��
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				GameObject obj(pos, size,
					ResourceManager::GetTexture("block_solid"),
					glm::vec3(0.8f, 0.8f, 0.7f)
				);
				obj.IsSolid = GL_TRUE;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1){//һ���ɱ��ݻٵ�ש��
				glm::vec3 color = glm::vec3(1.0f); // Ĭ��Ϊ��ɫ
				//��ͬ����������ש�����ɫ
				if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(unit_width, unit_height);
				glm::vec2 velocity = glm::vec2(150, -450);
				this->Bricks.push_back(
					GameObject(pos, size, ResourceManager::GetTexture("block"), color)
				);
			}
		}
	}
}

void GameLevel::Draw(SpriteRenderer &renderer){
	for (GameObject &tile : this->Bricks)//��Ⱦ����δ���ƻ���ש��
		if (!tile.Destroyed)
			tile.Draw(renderer);
}

GLboolean GameLevel::IsCompleted(){
	for (GameObject &tile : this->Bricks)//�����������ש���Ƿ�ȫ�����ƻ�
		if (!tile.IsSolid && !tile.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}