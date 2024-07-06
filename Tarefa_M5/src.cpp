#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;
#include <shader/Sprite.h>
#include <shader/Shader.h>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

bool movimento(Sprite* car,GLFWwindow* window);

GLuint loadText(string path);


const GLuint WIDTH = 800, HEIGHT = 600;

int main () {
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "instacia textura", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//regitrando a função de callback qnd key_polling
	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		cout << "Failed to initialize GLAD" << endl;
	}

	int w,h;
	glfwGetFramebufferSize(window, &w, &h);

	glViewport(0,0,w,h);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("../shaders/HelloTriangle.vs", "../shaders/HelloTriangle.fs");


	GLuint charac = loadText("../resources/text/Idle.png");
	GLuint characW = loadText("../resources/text/Walk.png");

	

	Sprite idle;
	idle.setShader(&shader);
	idle.inicializar(charac, glm::vec3(400,300,0.0),glm::vec3(768, 128, 1.0),0.0,1,6);


	Sprite walking;
	walking.setShader(&shader);
	walking.inicializar(characW,glm::vec3(150,300,0.0),glm::vec3(1280, 128, 1.0),0.0,1,10);

	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);

	shader.setMat4("projection", glm::value_ptr(projection));

	shader.setInt("texBuffer", 0);

	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();

		

		glClearColor(0.5f, 0.7f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (movimento(&walking,window)){
			walking.desenhar();
		}
		else {
			idle.setPos(walking.getPos());
			idle.desenhar();
		}

		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLuint loadText(string filePath)
{
	GLuint texID;

	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
    	if (nrChannels == 3) //jpg, bmp
    	{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    	}
    	else //png
    	{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    	}
    	glGenerateMipmap(GL_TEXTURE_2D);


		stbi_image_free(data);

		glBindTexture(GL_TEXTURE_2D, 0);

	}
	else
	{
    	 std::cout << "Failed to load texture" << std::endl;
	}


	return texID;
}

bool movimento(Sprite* car,GLFWwindow* window){
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_LEFT))
		{
			car->moverParaEsquerda();
			return true;
		}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_RIGHT))
	{
		car->moverParaDireita();
		return true;
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_UP))
	{
		car->moverParaCima();
		return true;
	}
	if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_DOWN))
	{
		car->moverParaBaixo();
		return true;
	}

	return false;
}