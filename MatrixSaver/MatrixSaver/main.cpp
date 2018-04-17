//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ResourceManager.h"
// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// The Width of the screen
GLuint SCREEN_WIDTH = 1400;
// The height of the screen
GLuint SCREEN_HEIGHT = 800;


int main(int argc, char *argv[]){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);   //û�б߿�ͱ�����  

	//ȫ����ʾ
	GLFWmonitor *pMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * mode = glfwGetVideoMode(pMonitor);
	SCREEN_WIDTH = mode->width;
	SCREEN_HEIGHT = mode->height;

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FunnyMaze", pMonitor, nullptr);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);//���̻ص�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	// OpenGL configuration
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;


	while (!glfwWindowShouldClose(window)){
		// Calculate delta time����������֮֡���ʱ��
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();//����¼�
		//�����û�����
		
		//����
		

		// Render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);//����Ҫ������Ȳ���
		

		glfwSwapBuffers(window);
	} 
	 
	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();//ж�ؼ��ص�ȫ����Դ

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	//closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024){
		//if (action == GLFW_PRESS)
			//snake.Keys[key] = GL_TRUE;
		//else if (action == GLFW_RELEASE) {
			//snake.Keys[key] = GL_FALSE;
			//snake.KeysProcessed[key] = GL_FALSE;
		//}
	}
}