#include "Game.h"
#include "SpriteRenderer.h"
#include "ResourceManager.h"
#include "BallObject.h"

// Game-related State data
SpriteRenderer  *Renderer;

// ��ʼ������Ĵ�С
const glm::vec2 PLAYER_SIZE(200, 40);
// ��ʼ�����������
const GLfloat PLAYER_VELOCITY(500.0f);
//��ҵ���
GameObject      *Player;

// ��ʼ������ٶ�
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// ��İ뾶
const GLfloat BALL_RADIUS = 12.5f;
//�˶���С��
BallObject     *Ball;

Game::Game(GLuint width, GLuint height)
	:State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game(){

}

void Game::Init(){
	// ������ɫ��
	ResourceManager::LoadShader("../res/Shaders/Sprite.vert", 
		"../res/Shaders/Sprite.frag", nullptr, "sprite");
	// ������ɫ��
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
		static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// ����ר������Ⱦ�Ŀ���
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	// ��������
	ResourceManager::LoadTexture("../res/Textures/background.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Textures/huaji.jpg", GL_TRUE, "face");
	ResourceManager::LoadTexture("../res/Textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("../res/Textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("../res/Textures/paddle.png", GL_TRUE, "paddle");
	// ���عؿ�
	GameLevel one; one.Load("../res/levels/one.lvl", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("../res/levels/two.lvl", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("../res/levels/three.lvl", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("../res/levels/four.lvl", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 1;

	//���
	glm::vec2 playerPos = glm::vec2(
		this->Width / 2 - PLAYER_SIZE.x / 2,
		this->Height - PLAYER_SIZE.y
	);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));

	//С��
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,
		ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt){
	 Ball->Move(dt, this->Width);
}

void Game::ProcessInput(GLfloat dt){
	if (this->State == GAME_ACTIVE) {
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// �ƶ�����
		if (this->Keys[GLFW_KEY_A]) {//����
			if (Player->Position.x >= 0)
				Player->Position.x -= velocity;
			if (Ball->Stuck)//С�����ŵ����ƶ�
				Ball->Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D]) {//����
			if (Player->Position.x <= this->Width - Player->Size.x)
				Player->Position.x += velocity;
			if (Ball->Stuck)//С�����ŵ����ƶ�
				Ball->Position.x += velocity;
		}
		if (this->Keys[GLFW_KEY_SPACE])//�ͷ�С��
			Ball->Stuck = false;
	}
}

void Game::Render(){
	if (this->State == GAME_ACTIVE){
		// ���Ʊ���
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
		);
		// ���ƹؿ�
		this->Levels[this->Level].Draw(*Renderer);
		//��ҵ����С��
		Player->Draw(*Renderer);
		Ball->Draw(*Renderer);
	}
}