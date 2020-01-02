#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

static void printMat(const glm::mat4 mat)
{
	std::cout<<" matrix:"<<std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout<< mat[j][i]<<" ";
		std::cout<<std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle ,float relationWH, float near, float far) : Scene(angle,relationWH,near,far)
{ 	
}


  
void Game::Init()
{		
	unsigned int texIDs[1] = { 0 };
	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/plane.png",false);
	AddMaterial(texIDs, 1);

	// Rubik cube
	InitializeCube();

	MoveCamera(0,zTranslate,16);

	pickedShape = -1;
	
	ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	textures[materials[shapes[pickedShape]->GetMaterial()]->GetTexture(0)]->Bind(0);
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	//s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);

	float mult = pow(pickedShape,1.5);//*(pickedShape%2)

	glm::vec3 clr = glm::vec3((pickedShape%9)/3.0, (pickedShape % 3)/3.0,
		pickedShape/3.0);
	s->SetUniform4f("theColor", clr.x/3.0, clr.y / 3.0, clr.z/3.0, 1.0f);

	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}


//float count = 0.0;
int iterations;



int lastAxis = -1;
int lastStart = -1;

void Game::Motion()
{
	//
	//while (!operation_q.empty()) {
	//	glm::mat4 operation = operation_q.front();
	//	int cube_id = cubes_id.front();

	//	//shapes[cube_id]->MyRotate(operation);

	//	operation_q.pop();
	//	cubes_id.pop();
	//}
	
	if(isActive)
	{

		int totalCobes = RUBIC_DIMENSION*pow(RUBIC_DIMENSION, 2);
		int totalCobesWall = pow(RUBIC_DIMENSION, 2);


		for (int i = 0; i < totalCobesWall; ++i) {
			//makeRotate(i,0.4, 0);



			int realI;
			int arrValue;
			//scn->makeRotate(arrValue, angle, numAxis);



			switch (operation_qChar.front()/*currentTurn*/) {
			case 'R':
				realI = getAxisValue(X_AXIS, i);
				arrValue = getArrayValue(realI);


				lastStart = 0;
				lastAxis = X_AXIS;

				makeRotate(arrValue, (90.0 / rotation_speed) *direction, X_AXIS);
				break;


			case 'L':
				lastStart = totalCobes - totalCobesWall;
				realI = getAxisValue(X_AXIS, i+ lastStart);
				arrValue = getArrayValue(realI);

				lastAxis = X_AXIS;
				
				makeRotate(arrValue, (90.0 / rotation_speed) *direction, X_AXIS);
				break;


			case 'U':
				realI = getAxisValue(Y_AXIS, i);
				arrValue = getArrayValue(realI);

				lastAxis = Y_AXIS;
				lastStart = 0;
				makeRotate(arrValue, (90.0 / rotation_speed) *direction, Y_AXIS);
				break;


			case 'D':
				lastStart = totalCobes - totalCobesWall;
				realI = getAxisValue(Y_AXIS, i + lastStart);
				arrValue = getArrayValue(realI);

				lastAxis = Y_AXIS;
				
				makeRotate(arrValue, (-(90.0 / rotation_speed))*direction, Y_AXIS);
				break;



			case 'B':
				lastStart = totalCobes - totalCobesWall;
				realI = getAxisValue(Z_AXIS, i+ lastStart);
				arrValue = getArrayValue(realI);


				lastAxis = Z_AXIS;
				
				makeRotate(arrValue, (-(90.0 / rotation_speed))*direction, Z_AXIS);
				break;


			case 'F':
				realI = getAxisValue(Z_AXIS, i );
				arrValue = getArrayValue(realI);


				lastAxis = Z_AXIS;
				lastStart = 0;
				makeRotate(arrValue, (90.0 /rotation_speed) *direction, Z_AXIS);
				break;
			default: break;


			}

			
		}
		iterations++;
		if (iterations == rotation_speed) {


			if (operation_qChar.front() == 'B' || operation_qChar.front() == 'D') {
				updateArrayIndexes(lastStart, lastStart + totalCobesWall, -90 * direction, lastAxis);
			}
			else {
				updateArrayIndexes(lastStart, lastStart + totalCobesWall, 90 * direction, lastAxis);
			}
			

			operation_qChar.pop();

			//char popQueue = operation_qChar.front();
			if (operation_qChar.empty()) {
				isActive = false;
			}

			iterations = 0;
			currentTurn = 0;
		}
	}
}

Game::~Game(void)
{
	//if (this->arrayValue != NULL) {
	//	delete this->arrayValue;
	//	this->arrayValue = NULL;
	//}
}


void Game::InitializeCube()
{
	// Rubik cube data structures

	int totalCobes = RUBIC_DIMENSION *pow(RUBIC_DIMENSION, 2);





	for (int i = 0; i < totalCobes; ++i) {
		this->axises[Z_AXIS][i] = i;
	}
	int counter = 0;
	for (int k = (RUBIC_DIMENSION -1); k >= 0; --k) {
		for (int i = 0; i < RUBIC_DIMENSION; ++i) {
			for (int j = (RUBIC_DIMENSION - 1); j >= 0; --j) {
				int localIndex = k + RUBIC_DIMENSION * (j)+pow(RUBIC_DIMENSION, 2) * (i);
				//axises[1][localIndex] = j + pow(RUBIC_DIMENSION, 2)*i+ RUBIC_DIMENSION*k;
				axises[X_AXIS][counter++] = localIndex;
			}
		}
	}
	counter = 0;
	for (int k = 0; k < RUBIC_DIMENSION; ++k) {
		for (int i = 0; i < RUBIC_DIMENSION; ++i) {
			for (int j = 0; j < RUBIC_DIMENSION; ++j) {
				int localIndex = j + RUBIC_DIMENSION * (k)+pow(RUBIC_DIMENSION, 2) * i;
				//axises[1][localIndex] = j + pow(RUBIC_DIMENSION, 2)*i+ RUBIC_DIMENSION*k;
				axises[Y_AXIS][counter++] = localIndex;
			}
		}
	}

	if (RUBIC_DIMENSION == 2) {
		for (int k = 0; k < RUBIC_DIMENSION; ++k) {
			for (int i = 0; i < RUBIC_DIMENSION; ++i) {
				for (int j = 0; j < RUBIC_DIMENSION; ++j) {
					AddShape(Cube, -1, TRIANGLES);
					//AddShape(Cube, -1, 0);
					int localIndex = j + RUBIC_DIMENSION * (i)+pow(RUBIC_DIMENSION, 2) * k;
					shapes[localIndex]->MyTranslate(glm::vec3((1) * (j * 2 - 1), (1) * (1 - (i * 2)), (1) * (1 - (k * 2))), 0); // The center.
					this->arrayValue[localIndex] = localIndex;
				}

			}
		}
	}
	else
	{


		for (int k = 0; k < RUBIC_DIMENSION; ++k) {
			for (int i = 0; i < RUBIC_DIMENSION; ++i) {
				for (int j = 0; j < RUBIC_DIMENSION; ++j) {
					AddShape(Cube, -1, TRIANGLES);
					//AddShape(Cube, -1, 0);
					int localIndex = j + RUBIC_DIMENSION * (i)+pow(RUBIC_DIMENSION, 2) * k;
					shapes[localIndex]->MyTranslate(glm::vec3((2) * (+j  - RUBIC_DIMENSION/2), (2) * (RUBIC_DIMENSION/2 - i), (2) * (RUBIC_DIMENSION/2 - k)), 0); // The center.
					this->arrayValue[localIndex] = localIndex;
				}

			}
		}
	}

	//AddShape(Axis, -1, LINES);
	//SetShapeMaterial(0, 0);
}