#include "Game.h"
#include "CollisionDetect.h"
#include "ResourceManager.h"
#include "Menu.h"
#include <iostream>
#include <sstream>

Game::Game(GLuint width, GLuint height)
	:Width(width),Height(height),State(GAME_MENU), unitX(30), unitY(30),
	gridX(Width /unitX),gridY(Height/unitY),timer(0.0f),fireindex(0)
	,mode(2),score(0)
{
	firetimer[0] = firetimer[1] = firetimer[2] = 0;
	InitVelocity = glm::vec2(+unitX, +0);
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
	snake = std::make_shared<SnakeObject>(headPos, unitX / 2, InitVelocity,
		ResourceManager::GetTexture("snakebody"));

	//ʳ��
	food = std::make_shared<GameObject>(glm::vec2(0.0, 0.0), glm::vec2(unitX, unitY),
		ResourceManager::GetTexture("snakebody"));
	SetFoodPos();
	food->Velocity = glm::vec2(300, 300);
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
			unitX*1.5,//���Ӵ�С
			2.0f,//������������
			1.0f/2.0f//͸����˥���ٶ�
			);
}

//��������
void Game::ProcessInput(GLfloat dt) {
	if (this->State == GAME_ACTIVE && this->mode == 1) {//���ģʽ�����ƶ�
		// �ƶ�����
		if (this->Keys[GLFW_KEY_LEFT]) {//����
			if (snake->Velocity != glm::vec2(+unitX, 0)) {//�������෴�ķ�����
				snake->nextdir = glm::vec2(-unitX, 0);
				KeysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_RIGHT]) {//����
			if (snake->Velocity != glm::vec2(-unitX, 0)) {//�������෴�ķ�����
				snake->nextdir = glm::vec2(+unitX, 0);
				KeysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_UP]) {//����
			if (snake->Velocity != glm::vec2(0, +unitY)) {//�������෴�ķ�����
				snake->nextdir = glm::vec2(0, -unitY);
				KeysProcessed[GLFW_KEY_UP] = GL_TRUE;
			}
		}
		if (this->Keys[GLFW_KEY_DOWN]) {//����
			if (snake->Velocity != glm::vec2(0, -unitY)) {//�������෴�ķ�����
				snake->nextdir = glm::vec2(0, +unitY);
				KeysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
			}
		}
	}
	if (this->State == GAME_MENU) {
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]) {//ȷ��
			this->State = GAME_ACTIVE;
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			Reset();//����
		}
		if (this->Keys[GLFW_KEY_1] && !this->KeysProcessed[GLFW_KEY_1]) {//���ģʽ
			this->mode = 1;
			this->KeysProcessed[GLFW_KEY_1] = GL_TRUE;
		}
		if (this->Keys[GLFW_KEY_2] && !this->KeysProcessed[GLFW_KEY_2]) {//AIģʽ
			this->mode = 2;
			this->KeysProcessed[GLFW_KEY_2] = GL_TRUE;
		}
	}
	if (this->State == GAME_LOST || this->State == GAME_WIN) {//��Ϸ��Ӯ֮��ȷ��������Ϸ
		if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER]){
			this->KeysProcessed[GLFW_KEY_ENTER] = GL_TRUE;
			this->State = GAME_MENU;
			Reset();
		}
	}
	if (this->State == GAME_ACTIVE && this->mode == 2) {
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
	if (this->State == GAME_ACTIVE && this->mode == 1) {//���ģʽ
		//�����ƶ�
		snake->Move(dt, *algorithm);
		//����ײ���
		DoCollisions();
	}
	
	if (this->State == GAME_ACTIVE && this->mode == 2) {//AIģʽ
		//Ҫע��x��y��˳�����෴��
		//��ͷλ��
		std::pair<int, int> src(snake->GetHeadPos().y / snake->Size.y,
			snake->GetHeadPos().x / snake->Size.x);
		//ʳ��λ��
		std::pair<int, int>det(food->Position.y / food->Size.y,
			food->Position.x / food->Size.x);
		//β��λ��
		std::pair<int, int> tail(snake->GetTailPos().y / snake->Size.y,
			snake->GetTailPos().x / snake->Size.x);
		//β��ǰ���λ��
		std::pair<int,int> tailPrev(snake->GetTailPrevPos().y / snake->Size.y,
			snake->GetTailPrevPos().x / snake->Size.x);
		if (algorithm->Search(src, det, 1) 
			&& algorithm->IsSafe(src,tail,tailPrev,algorithm->path == det)) {//�ҵ���·
			GoAhead();
		}
		else {//û�ҵ�·�򲻰�ȫ,ȥ��β�͵�·
			if (algorithm->Search(src, tail, 2)) {//�ҵ���ȥβ�͵�·
			//	std::cout << "go tail" << std::endl;
				GoAhead();
			}
			else if (algorithm->JustFindOne(src)) {//�������
				std::cout << "just go" << std::endl;
				GoAhead();
			}
			else {//û·��ʧ����
				State = GAME_LOST;
			}
		}
		//����ײ���
		//DoCollisions();
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
		snake->DrawHead(*sprite, ResourceManager::GetTexture("snakehead"));
		snake->Draw(*sprite);
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

//��ײ���
void Game::DoCollisions() {
	if (State == GAME_ACTIVE) {
		//̰���߳Ե�ʳ��
		if (CollisionDetect::CheckCollision2(*snake, *food)) {
			if(this->mode == 1)snake->AddBody(food->Position);//����һ������
			//���䱬ը�̻�
			fireindex = (fireindex + 1) % 3;
			firetimer[fireindex] = 2.0f;
			firework->Position = food->Position;
			boom[fireindex]->Reset();
			boom[fireindex]->Update(0.f, *firework, 400, firework->Size / 2.0f, 3, fireindex);
			SetFoodPos();//��������ʳ��
			sound->play2D("../res/Audio/get.wav", GL_FALSE);
			//��ȡһ��
			++score;
		}

		//ײ��ǽ��
		if (CollisionDetect::CheckCollisionWidthWall(*snake, this->Width, this->Height)) {
			this->State = GAME_LOST;
			sound->play2D("../res/Audio/dead.mp3", GL_FALSE);
		}
		
		//�Լ����Լ�
		if (snake->isCollisionSelf()) {
			this->State = GAME_LOST;
			sound->play2D("../res/Audio/dead.mp3", GL_FALSE);
		}
	}
}

//����ʳ��λ��
void Game::SetFoodPos() {
	int x1 = rand() % gridY;
	int y1 = rand() % gridX;
	while (!algorithm->CouldPlaceFood(x1, y1)) {//һֱ�ҵ��ܹ�����ʳ��ĵط�
		x1 = rand() % gridY;
		y1 = rand() % gridX;
	}
	food->Position = glm::vec2(y1*unitX, x1*unitY);
}

//��������ʼ״̬
void Game::Reset(){
	timer = 0.0f;
	fireindex = 0;
	firetimer[0] = firetimer[1] = firetimer[2] = 0.0f;
	score = 0;
	glm::vec2 headPos = glm::vec2(
		(gridX - 1) / 2 * unitX, (gridY - 1) / 2 * unitY
	);
	snake->Reset(headPos, InitVelocity, *algorithm);
}

void Game::GoAhead() {
	std::pair<int, int> next = algorithm->path;
	glm::vec2 step = glm::vec2(next.second*snake->Size.x, next.first*snake->Size.y);
	if (step == food->Position) {//�����һ����ʳ��Ļ�ֱ������һ������
		snake->AddBody(step, true);
		algorithm->Place(next.second, next.first);

		//��ײ,boom
		fireindex = (fireindex + 1) % 3;
		firetimer[fireindex] = 2.0f;
		firework->Position = food->Position;
		boom[fireindex]->Reset();
		boom[fireindex]->Update(0.f, *firework, 400, firework->Size / 2.0f, 3, fireindex);
		SetFoodPos();//��������ʳ��
		sound->play2D("../res/Audio/get.wav", GL_FALSE);
		//��ȡһ��
		++score;
	}
	else snake->MoveByAi(step, *algorithm);
}