#include "Game.h"
#include "ResourceManager.h"
#include "Menu.h"
#include <iostream>
#include <sstream>

Game::Game(GLuint width, GLuint height)
	:Width(width),Height(height),State(GAME_MENU), unitX(15), unitY(15),
	gridX(Width /unitX),gridY(Height/unitY)
{
	maze = std::make_shared<MazeAlgorithm>(gridY - 1, gridX - 1, unitX, unitY);
}

//��Ϸ��ʼ��
void Game::Init() {
	// ������ɫ��
	ResourceManager::LoadShader("../res/Shaders/Sprite.vert",
		"../res/Shaders/Sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("../res/Shaders/Particle.vert",
		"../res/Shaders/Particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("../res/Shaders/Effect.vert",
		"../res/Shaders/Effect.frag", nullptr, "postprocessing");

	// ������ɫ��
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", projection);

	//��Ϸ�ؼ�
	sound = std::shared_ptr<ISoundEngine>(createIrrKlangDevice());
	sprite = std::make_shared<SpriteRenderer>(ResourceManager::GetShader("sprite"));
	wall = std::make_shared<GameObject>(glm::vec2(0, 0), glm::vec2(unitX, unitY*0.2f),
		ResourceManager::GetTexture("wall"), glm::vec3(1.0, 0.0, 0.0));

	// ��������
	ResourceManager::LoadTexture("../res/Textures/background.png", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Textures/brick.png", GL_FALSE, "wall");
	ResourceManager::LoadTexture("../res/Textures/Light.png", GL_FALSE, "particle");

	//������Ч
	effects = std::make_shared<PostProcessor>(ResourceManager::GetShader("postprocessing"),
		this->Width, this->Height);
	//�ı���ʾ
	text = std::make_shared<TextRenderer>(this->Width, this->Height);
	text->Load("../res/Fonts/ocraext.TTF", 24);
	//��������
	sound->play2D("../res/Audio/background.mp3", GL_TRUE);
}

//��������
void Game::ProcessInput(GLfloat dt) {
	if (this->State == GAME_MENU) {
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {//ȷ��
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Reset();//����
		}
	}
	if (this->State == GAME_LOST || this->State == GAME_WIN) {//��Ϸ��Ӯ֮��ȷ�����¿�ʼ
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]){
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			this->State = GAME_MENU;
			//Reset();
		}
	}
	if (this->State == GAME_ACTIVE) {
		if (this->Keys[GLFW_KEY_P] && !this->KeysProcessed[GLFW_KEY_P]) {//��ͣ
			this->State = GAME_PAUSE;
			this->KeysProcessed[GLFW_KEY_P] = GL_TRUE;
		}
	}
	if (this->State == GAME_PAUSE) {
		if (this->Keys[GLFW_KEY_B]) {
			this->State = GAME_ACTIVE;
		}
	}
}

//��Ϸ����
void Game::Update(GLfloat dt) {
	
	if (this->State == GAME_ACTIVE ) {//����ģʽ
		maze->GetWallFromCells();//��ȡשǽ��λ��
		if (maze->Generator_Dfs()) {//�����Թ���һ�������
			this->State = GAME_WIN;
		}
	}
}

//��Ⱦ
void Game::Render() {
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU
		|| this->State == GAME_WIN || this->State == GAME_LOST || this->State == GAME_PAUSE) {
		// ���Ʊ���
		sprite->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);

		//�����Թ�
		Texture2D &tmp = ResourceManager::GetTexture("wall");
		for (auto it = maze->walls.begin(); it != maze->walls.end(); ++it) {
			sprite->DrawSprite(tmp, it->Pos, wall->Size, glm::radians(it->angle), wall->Color);
		}

		//����
		//text->RenderText("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU) {
		Menu::Selection(*text, this->Width, this->Height);
	}
	if (this->State == GAME_WIN) {
		Menu::Win(*text, this->Width, this->Height);
	}
	if(this->State == GAME_LOST){
		//����
		Menu::Lost(*text, this->Width, this->Height, std::string("Over"));
	}
}

//��������ʼ״̬
void Game::Reset(){
	maze->Init();
	//firetimer[0] = firetimer[1] = firetimer[2] = 0.0f;
	//algorithm->ResetSnakeAndFood();
	//food->Position = Index(algorithm->food->Index);
	//snake->Position = Index(algorithm->snake.front().Index);
}
