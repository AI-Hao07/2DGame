#include "Game.h"
#include "ResourceManager.h"

Game::Game(GLuint width, GLuint height)
	:Width(width),Height(height),State(GAME_ACTIVE), unitX(50), unitY(50),
	gridX(Width /unitX),gridY(Height/unitY),timer(0.0f)
{
	InitVelocity = glm::vec2(+unitX, +0);
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
	ResourceManager::LoadTexture("../res/Textures/timg.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("../res/Textures/body.png", GL_FALSE, "snakebody");
	ResourceManager::LoadTexture("../res/Textures/food.png", GL_FALSE, "food");

	//��
	glm::vec2 headPos = glm::vec2(
		(gridX - 1) / 2 * unitX, (gridY - 1) / 2 * unitY
	);
	snake = std::make_shared<SnakeObject>(headPos, unitX / 2, InitVelocity,
		ResourceManager::GetTexture("snakebody"));

	//ʳ��
	food = std::make_shared<GameObject>(glm::vec2(0.0, 0.0), glm::vec2(unitX, unitY),
		ResourceManager::GetTexture("food"));
	SetFoodPos();

	//������Ч
	effects = std::make_shared<PostProcessor>(ResourceManager::GetShader("postprocessing"),
		this->Width, this->Height);
	//�ı���ʾ
	text = std::make_shared<TextRenderer>(this->Width, this->Height);
	text->Load("../res/Fonts/ocraext.TTF", 24);
	//��������
	sound->play2D("../res/Audio/breakout.mp3", GL_TRUE);
}

//��������
void Game::ProcessInput(GLfloat dt) {
	if (this->State == GAME_ACTIVE) {
		// �ƶ�����
		if (this->Keys[GLFW_KEY_A]) {//����
			snake->Velocity = glm::vec2(-unitX, 0);
			snake->ChangeHeadDir();
		}
		if (this->Keys[GLFW_KEY_D]) {//����
			snake->Velocity = glm::vec2(+unitX, 0);
			snake->ChangeHeadDir();
		}
		if (this->Keys[GLFW_KEY_W]) {//����
			snake->Velocity = glm::vec2(0, -unitY);
			snake->ChangeHeadDir();
		}
		if (this->Keys[GLFW_KEY_S]) {//����
			snake->Velocity = glm::vec2(0, +unitY);
			snake->ChangeHeadDir();
		}
	}
}

//��Ϸ����
void Game::Update(GLfloat dt) {
	timer += dt;
	snake->Move(dt);
}

//��Ⱦ
void Game::Render() {
	// ���Ʊ���
	sprite->DrawSprite(ResourceManager::GetTexture("background"),
		glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f
	);
	//����
	snake->Draw(*sprite);
	//ʳ��
	food->Draw(*sprite);
}

//��ײ���
void Game::DoCollisions() {

}

//����ʳ��λ��
void Game::SetFoodPos() {
	int x1 = rand() % gridX;
	int y1 = rand() % gridY;
	food->Position = glm::vec2(x1*unitX, y1*unitY);
}