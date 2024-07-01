#pragma once

#include <shader/Shader.h>
#include <glm/glm.hpp>

class Sprite
{
public:
	Sprite() {}
	~Sprite();
	void inicializar(GLuint texID,glm::vec3 pos = glm::vec3(0.0,0.0,0.0), glm::vec3 escala = glm::vec3(1.0,1.0,1.0),float angulo = 0.0);
	void desenhar();
	void finalizar();
	void moverParaDireita();
	void moverParaEsquerda();
	void cair();

	inline void setShader(Shader* shader) { this->shader = shader; }
	inline void setJumping(bool isJumping) { this->isJumping = isJumping; }
	inline void setOnGround(bool onGround) { this->onGround = onGround;  }
	inline bool getJumping() { return isJumping; }
	inline void setJumpVel(glm::vec2 jumpVel) { this->jumpVel = jumpVel; }
protected:
	void atualizar();
	GLuint VAO; //identificador do buffer de geometria, indicando os atributos dos v�rtices
	GLuint texID; //identificador do buffer de textura

	//Vari�veis com as infos para aplicar as transforma��es no objeto
	glm::vec3 pos, escala;
	float angulo;

	//Uma refer�ncia ao programa de shader que a sprite ir� usar para seu desenho
	Shader *shader;

	//Atributos para controle das a��es
	bool isJumping, onGround;
	glm::vec2 jumpVel;
	float vel;

	float lastTime; 
	float FPS;

};

