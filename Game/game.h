#pragma once
#include "scene.h"
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

#define RUBIC_DIMENSION 3

#define Z_AXIS 0
#define Y_AXIS 1
#define X_AXIS 2


#define TOTAL_CUBES RUBIC_DIMENSION* RUBIC_DIMENSION*RUBIC_DIMENSION
#define TOTAL_CUBE_WALL RUBIC_DIMENSION*RUBIC_DIMENSION



#define ORIGIN_RELATIVE 7 // used in MyRotate mode, to rotate relative to origin axis

#define MOVING_ANGLE 90	

# define M_PI           3.14159265358979323846  /* pi */

class Game : public Scene
{
private:
	int arrayValue[TOTAL_CUBES] = { 0 };
	int axises[3][TOTAL_CUBES] = { 0 };
public:
	
	void setAxis(int axisNum, int location, int value) {
		axises[axisNum][location] = value;
	}

	void setArrayValue(int location, int value) {
		arrayValue[location] = value;
	}

	int getAxisValue(int axisNum, int location) {
		return axises[axisNum][location];
	}
	int getArrayValue(int location) {
		return arrayValue[location];
	}
	

	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx);
	void ControlPointUpdate();
	void WhenRotate();
	void WhenTranslate();
	void Motion();
	void InitializeCube();
	void setPickedShape(int newPickedShape){
       	pickedShape = newPickedShape;;
	}
	int getPickedShape() {
		return pickedShape;
	}
	float rotation_speed = 225;
	

	void updateArrayIndexes(int theStart, int theStop, float angle, int numAxis) {
		int arr[TOTAL_CUBE_WALL];

		for (int i = theStart; i < theStop; ++i) {
			int realI = getAxisValue(numAxis, i);
			int arrValue = getArrayValue(realI);
			//scn->makeRotate(arrValue, angle, numAxis);
			arr[i - theStart] = arrValue;// realI;// scn->getArrayValue(scn->getAxisValue(numAxis, i));
		}


		for (int i = 0; i < RUBIC_DIMENSION; ++i) {
			for (int j = 0; j < RUBIC_DIMENSION; ++j) {

				int realIndex = getAxisValue(numAxis, i * RUBIC_DIMENSION + j + theStart);
				if ((numAxis == 0  /* || numAxis == 2*/)) {
					if (angle > 0) {
						setArrayValue(realIndex, arr[i + RUBIC_DIMENSION * ((RUBIC_DIMENSION - 1) - j)]);
					}
					else {
						setArrayValue(realIndex, arr[(RUBIC_DIMENSION - 1) - i + RUBIC_DIMENSION * (j)]);
					}
				}
				else {
					if (angle > 0) {
						setArrayValue(realIndex, arr[(RUBIC_DIMENSION-1) - i + RUBIC_DIMENSION * (j)]);
					}
					else {
						setArrayValue(realIndex, arr[i + RUBIC_DIMENSION * ((RUBIC_DIMENSION - 1) - j)]);
					}



				}
			}
		}









	}

	void makeRotate(int numShape,float rotateAngle,int axisNum) {
		glm::mat4 T = glm::mat4(1);
		T[3] = glm::vec4(-1,-1,-1,1); //-shapes[numShape]->MakeTrans()[3];
		glm::vec4 p = shapes[numShape]->MakeTrans()[3];

		glm::mat4 R = glm::mat4(1);

		float oneRad = 360/( 2*M_PI) ;

		glm::vec3 ZVec = glm::vec3(0, 0, 1);
		
		glm::vec4 newP;
		glm::mat4 cube_transform = glm::mat4(1);

		switch (axisNum) {
		case Z_AXIS :
			R[0][0] = cos(rotateAngle / oneRad);
			R[0][1] = -sin(rotateAngle / oneRad);
			R[1][0] = sin(rotateAngle / oneRad);
			R[1][1] = cos(rotateAngle / oneRad);

			newP = R*p;
			/*
			float rotAng = 0;
			while (abs(rotAng) < rotateAngle) {
				cube_transform = glm::translate(cube_transform, glm::vec3(-p));
				cube_transform = glm::rotate(cube_transform, -rotateAngle, ZVec);
				cube_transform = glm::translate(cube_transform, glm::vec3(newP));
				operation_q.emplace(cube_transform);
				cubes_id.emplace(numShape);
			}
			*/

			shapes[numShape]->MyTranslate(glm::vec3(-p), -1);
			shapes[numShape]->MyRotate(-rotateAngle, glm::vec3(0, 0, 1), ORIGIN_RELATIVE);
			shapes[numShape]->MyTranslate(glm::vec3(newP), -1);
			break;
		case Y_AXIS:
			R[0][0] = cos(rotateAngle / oneRad);
			R[0][2] = sin(rotateAngle / oneRad);
			R[2][0] = -sin(rotateAngle / oneRad);
			R[2][2] = cos(rotateAngle / oneRad);
			newP =R*p;

		    shapes[numShape]->MyTranslate(glm::vec3(-p), -1);
			shapes[numShape]->MyRotate(-rotateAngle, glm::vec3(0, 1, 0), ORIGIN_RELATIVE);
			shapes[numShape]->MyTranslate(glm::vec3(newP), -1);
			break;
		case X_AXIS:
			R[1][1] = cos(rotateAngle / oneRad);
			R[1][2] = sin(rotateAngle / oneRad);
			R[2][1] = -sin(rotateAngle / oneRad);
			R[2][2] = cos(rotateAngle / oneRad);

			newP = R*p;

			shapes[numShape]->MyTranslate(glm::vec3(-p), -1);
			shapes[numShape]->MyRotate(rotateAngle, glm::vec3(1, 0, 0), ORIGIN_RELATIVE);
			shapes[numShape]->MyTranslate(glm::vec3(newP), -1);
			
			break;
		}
	}

	~Game(void);
	
};

