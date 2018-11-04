#ifndef CONTROLER_H
#define CONTROLER_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

enum ControlerMovement{
	FRONT,
	BACK,
	FRONT_AND_LEFT,
	FRONT_AND_RIGHT,
	BACK_AND_LEFT,
	BACK_AND_RIGHT
};

//default values of controler
const float ROTATE_ANGLE = -90.0f;
const glm::vec3 FRONT_VECTOR = glm::vec3(0.0f, 0.0f, -1.0f);

//parameter of the movement and rotate
float sensitivityOfMove = 30.0f;
float speed = 3.5f;
float sensitivityOfRotate = 0.1f;

class Controler {
public:
	glm::vec3 objectPosition;//opsition of the controned object
	glm::vec3 objectFront;//direction vector of the controled object
	float rotateAngleOfBottom;
	float rotateAngleOfTop;


	Controler(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : objectFront(FRONT_VECTOR), rotateAngleOfBottom(ROTATE_ANGLE) , rotateAngleOfTop(ROTATE_ANGLE){
		this->objectPosition = position;
		updateFrontVector();
	}

	glm::vec3 getObjectPositon() {
		return this->objectPosition;
	}
	glm::vec3 getObjectFront() {
		return this->objectFront;
	}

	//return rotate angle of the top
	//-----------------------------------
	float getRotateAngleTop() {
		return this->rotateAngleOfTop;
	}
	float getRotateAngleBottom() {
		return this->rotateAngleOfBottom;
	}

	//proess the movement of the tank from keyboard
	//--------------------------------------------------------------
	void move(ControlerMovement move, float const & deltaTime) {

		if (move == FRONT) {
			this->objectPosition += this->objectFront*deltaTime*speed;
		}
		else if (move == BACK) {
			this->objectPosition -= this->objectFront*deltaTime*speed;
		}
		else if(move == FRONT_AND_LEFT){
			this->rotateAngleOfBottom -= deltaTime*sensitivityOfMove;
			updateFrontVector();
			this->objectPosition += this->objectFront*deltaTime*speed;
		}
		else if (move == FRONT_AND_RIGHT) {
			this->rotateAngleOfBottom += deltaTime*sensitivityOfMove;
			updateFrontVector();
			this->objectPosition += this->objectFront*deltaTime*speed;
		}
		else if (move == BACK_AND_LEFT) {
			this->rotateAngleOfBottom += deltaTime*sensitivityOfMove;
			updateFrontVector();
			this->objectPosition -= this->objectFront*deltaTime*speed;
		}
		else {
			this->rotateAngleOfBottom -= deltaTime*sensitivityOfMove;
			updateFrontVector();
			this->objectPosition -= this->objectFront*deltaTime*speed;
		}
	}

	//proess the movement of the tank from mouse
	//-----------------------------------------------
	void ProcessMouseMovement(float const & xoffset, float const & yoffset) {
		rotateAngleOfTop += xoffset * sensitivityOfRotate;
	}

private:
	void updateFrontVector() {
		glm::vec3 temp;
		temp.x = glm::cos(glm::radians(rotateAngleOfBottom));
		temp.y = 0.0f;
		temp.z = glm::sin(glm::radians(rotateAngleOfBottom));
		this->objectFront = temp;
	}
	 
};


#endif // !CONTROLER_H

