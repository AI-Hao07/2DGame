#include "Game.h"
#include "CollisionDetect.h"
#include "ResourceManager.h"
#include "Menu.h"
#include <iostream>
#include <sstream>

Game::Game(GLuint width, GLuint height)
	:Width(width),Height(height),State(GAME_MENU), unitX(30), unitY(30),
	gridX(Width /unitX),gridY(Height/unitY),fireindex(0),score(0)
{
	firetimer[0] = firetimer[1] = firetimer[2] = 0;
	algorithm = std::make_shared<Algorithm>(this->gridY, this->gridX);
}

Game::~Game()
{
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

	// ��������
	ResourceManager::LoadTexture("../res/Textures/background.png", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Textures/particle.bmp", GL_FALSE, "snakebody");
	ResourceManager::LoadTexture("../res/Textures/head.png", GL_FALSE, "snakehead");
	ResourceManager::LoadTexture("../res/Textures/Light.png", GL_FALSE, "particle");

	//��
	glm::vec2 headPos = glm::vec2(
		(gridX - 1) / 2 * unitX, (gridY - 1) / 2 * unitY
	);
	snake = std::make_shared<SnakeObject>(headPos, unitX / 2, glm::vec2(unitX,unitY),
		ResourceManager::GetTexture("snakebody"));

	//ʳ��
	food = std::make_shared<GameObject>(glm::vec2(0.0, 0.0), glm::vec2(unitX, unitY),
		ResourceManager::GetTexture("snakebody"));
	food->Velocity = glm::vec2(300, 300);
	food->Position = Index(algorithm->food->Index);
	firework = std::make_shared<GameObject>(glm::vec2(0.0, 0.0), glm::vec2(unitX, unitY),
		ResourceManager::GetTexture("snakebody"));
	firework->Velocity = glm::vec2(200, 200);
	//������Ч
	effects = std::make_shared<PostProcessor>(ResourceManager::GetShader("postprocessing"),
		this->Width, this->Height);
	//�ı���ʾ
	text = std::make_shared<TextRenderer>(this->Width, this->Height);
	text->Load("../res/Fonts/ocraext.TTF", 24);
	//��������
	sound->play2D("../res/Audio/greedysnake.mp3", GL_TRUE);
	//������Ч
	temptation = std::make_shared<ParticleGenerator>(
		ResourceManager::GetShader("particle"),
		ResourceManager::GetTexture("particle"),
		800,//��������
		unitX,//���Ӵ�С
		1.5f,//������������
		1.0/1.5f//͸����˥���ٶ�
		);
	for(int x = 0;x < 3;++x)
		boom[x] = std::make_shared<ParticleGenerator>(
			ResourceManager::GetShader("particle"),
			ResourceManager::GetTexture("particle"),
			410,//��������
			unitX*1.1,//���Ӵ�С
			2.0f,//������������
			1.0f/2.0f//͸����˥���ٶ�
			);
	this->Reset();
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
			Reset();
		}
	}
	if (this->State == GAME_ACTIVE) {
		if (this->Keys[GLFW_KEY_P] && !this->KeysProcessed[GLFW_KEY_P]) {//��ͣ
			this->State = GAME_PAUSE;
			this->KeysProcessed[GLFW_KEY_P] = GL_TRUE;
			algorithm->Display();
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
	
	if (this->State == GAME_ACTIVE ) {//AIģʽ
		//AI����
		glm::ivec2 move = algorithm->AIThinking();
		//û�ҵ��κ�·������Ϸ����
		if (move == glm::ivec2(-1, -1))this->State = GAME_LOST;
		else {
			//�߳�һ��
			bool isCollision = algorithm->make_move(move);
			if (isCollision) {//��ײ,boom
				fireindex = (fireindex + 1) % 3;
				firetimer[fireindex] = 2.0f;
				firework->Position = food->Position;
				boom[fireindex]->Reset();
				boom[fireindex]->Update(0.f, *firework, 400, firework->Size / 2.0f, 3, fireindex);
				sound->play2D("../res/Audio/get.wav", GL_FALSE);
				//��ȡһ��
				++score;
			}
			if (algorithm->win) {//������
				State = GAME_WIN;
				return;
			}
			food->Position = Index(algorithm->food->Index);//����ʳ��λ��
		}
	}
	//���Ӹ���
	temptation->Update(dt, *food, 8, food->Size / 20.f, 2);
	//�̻���ը
	for (int x = 0; x < 3; ++x) {
		if (firetimer[x] > 0) {
			firetimer[x] -= dt;
			boom[x]->Update(dt, *firework, 0, firework->Size / 2.0f, 3, fireindex);
		}
	}
	if (firetimer[fireindex] <= 0 && State != GAME_ACTIVE && State != GAME_PAUSE) {//�Ǽ���״̬���λ�������̻�
		firetimer[fireindex] = 2.0f;
		//���λ��
		int x1 = rand() % gridX;
		int y1 = rand() % gridY;
		firework->Position = glm::vec2(x1*unitX, y1*unitY);
		boom[fireindex]->Reset();
		boom[fireindex]->Update(0.f, *firework, 400, firework->Size / 2.0f, 3, fireindex);
		fireindex = (fireindex + rand()%3) % 3;
	}
}

//��Ⱦ
void Game::Render() {
	std::stringstream ss; ss << this->score;
	if (this->State == GAME_ACTIVE || this->State == GAME_MENU
		|| this->State == GAME_WIN || this->State == GAME_LOST || this->State == GAME_PAUSE) {
		// ���Ʊ���
		sprite->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		//����
		snake->DrawSnake(algorithm->snake, *sprite, ResourceManager::GetTexture("snakehead"));
		//ʳ��
		food->Draw(*sprite);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//����
		temptation->Draw();
		//�̻���ը
		if(firetimer[0]>0)boom[0]->Draw();
		if (firetimer[1]>0)boom[1]->Draw();
		if (firetimer[2]>0)boom[2]->Draw();
		//����
		text->RenderText("Score:" + ss.str(), 5.0f, 5.0f, 1.0f);
	}
	if (this->State == GAME_MENU) {
		Menu::Selection(*text, this->Width, this->Height);
	}
	if (this->State == GAME_WIN) {
		Menu::Win(*text, this->Width, this->Height);
	}
	if(this->State == GAME_LOST){
		//����
		Menu::Lost(*text, this->Width, this->Height, ss.str());
	}
}

//��������ʼ״̬
void Game::Reset(){
	fireindex = 0;
	firetimer[0] = firetimer[1] = firetimer[2] = 0.0f;
	score = 0;
	algorithm->ResetSnakeAndFood();
	food->Position = Index(algorithm->food->Index);
	snake->Position = Index(algorithm->snake.front().Index);
}
