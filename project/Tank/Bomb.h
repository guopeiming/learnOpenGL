#ifndef BOMB_H
#define BOMB_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <learnopengl/MyCube.h>
#include <learnopengl/shader.h>


class Bomb {
public:
	glm::vec3 front;
	MyCube cube;
	float const speed = 9.0f;


	Bomb(glm::vec3 position, glm::vec3 front) {
		this->cube = MyCube(position);
		this->front = front;
		this->cube.flag = true;
	}
	Bomb() {
		this->cube = MyCube(glm::vec3(0.0f, 0.0f, 0.0f));
		this->cube.flag = false;
	}
	void updatePosition(float & deltaTime) {
		this->cube.position += this->front*(deltaTime*speed);
	}
	void Draw(Shader & shader) {
		this->cube.Draw(shader);
	}

};


#endif // !BOMB_H

#pragma once
