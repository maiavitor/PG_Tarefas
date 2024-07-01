#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

#include <shader/Shader.h>
#include <shader/Sprite.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader shader("../shaders/tex.vs", "../shaders/tex.fs");

	GLuint back = loadText("../resources/text/Cartoon_Forest_BG_02.png");
	GLuint charac = loadText("../resources/text/X-removebg-preview.png");
	GLuint obj = loadText("../resources/text/Gates_dark_shadow3.png");

	Sprite background;
	background.setShader(&shader);
	background.inicializar(back, glm::vec3(400,300,0.0),glm::vec3(800, 600, 1.0));

	Sprite character;
	character.setShader(&shader);
	character.inicializar(charac, glm::vec3(270,90,0.0),glm::vec3(110, 110, 1.0));

	Sprite objt;
	objt.setShader(&shader);
	objt.inicializar(obj, glm::vec3(500,80,0.0),glm::vec3(150, 150, 1.0));



	glActiveTexture(GL_TEXTURE0);

	shader.Use();

	glm::mat4 projection = glm::ortho(0.0,800.0,0.0,600.0,-1.0,1.0);

	shader.setMat4("projection", glm::value_ptr(projection));

	shader.setInt("texBuffer", 0);

	while (!glfwWindowShouldClose(window)){

		glfwPollEvents();

		int w,h;
		glfwGetFramebufferSize(window, &w, &h);

		glViewport(0,0,w,h);

		glClearColor(0.5f, 0.7f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		background.desenhar();
		character.desenhar();
		objt.desenhar();

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

GLuint loadText(string path)
{
	GLuint texID;

	// Gera o identificador da textura na mem�ria 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Configura��o do par�metro WRAPPING nas coords s e t
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Confiruga��o do par�metro FILTERING na minifica��o e magnifica��o da textura
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

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
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}