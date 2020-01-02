#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include "iostream"

void runOnCubes(int theStart, int theStop, float angle, int numAxis, Game* scn);
int direction = -1;

	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			int totalCobesWall = pow(RUBIC_DIMENSION, 2);
			int totalCobes = pow(RUBIC_DIMENSION, 3);
			glfwGetCursorPos(window,&x2,&y2);
			int pickedId = scn->Picking((int)x2,(int)y2);
			//std::cout << "picked object = " << pickedId << std::endl;

			if (scn->IsActive()) {
				return;
			}
			switch (pickedId) {
			case 4: // front face
				scn->setQueueNewOperation('F');
				scn->setSturn('F');
				runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, Z_AXIS, scn);
				break;
			case 10: // upper face
				scn->setQueueNewOperation('U');
				scn->setSturn('U');
				runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, Y_AXIS, scn);
				break;
			case 12: // left face
				scn->setQueueNewOperation('L');
				scn->setSturn('L');
				runOnCubes(totalCobes - totalCobesWall, totalCobes, direction * MOVING_ANGLE, X_AXIS, scn);
				break;
			case 16: // bottom face
				scn->setQueueNewOperation('D');
				scn->setSturn('D');
				runOnCubes(totalCobes - totalCobesWall, totalCobes, -direction * MOVING_ANGLE, Y_AXIS, scn);
				break;
			case 14: // right face
				scn->setQueueNewOperation('R');
				scn->setSturn('R');
				runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, X_AXIS, scn);
				break;
			case 22: // back face
				scn->setQueueNewOperation('B');
				scn->setSturn('B');
				runOnCubes(totalCobes - totalCobesWall, totalCobes, -direction * MOVING_ANGLE, Z_AXIS, scn);
				break;
			default:
				break;
			}
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		if (yoffset > 0) {
			scn->MoveCamera(0, scn->zTranslate, 0.4f);
		}
		else {
			scn->MoveCamera(0, scn->zTranslate, -0.4f);
		}
	}


	void printScnArray(Game *scn) {
		int a[27] = { 0 };
		int b[27] = { 0 };
		int c[27] = { 0 };
		int d[27] = { 0 };
		for (int i = 0; i < 27; ++i) {
			a[i] = scn->getArrayValue(i);
			b[i] = scn->getAxisValue(Z_AXIS, i);
			c[i] = scn->getAxisValue(Y_AXIS, i);
			d[i] = scn->getAxisValue(X_AXIS, i);

		}
			

		printf("array \n");
		printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n",a[0],a[1],a[2], a[9], a[10], a[11], a[18], a[19], a[20]);
		printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n", a[3], a[4], a[5], a[12], a[13], a[14], a[21], a[22], a[23]);
		printf("%2d %2d %2d     %2d %2d %2d     %2d %2d %2d\n\n", a[6], a[7], a[8], a[15], a[16], a[17], a[24], a[25], a[26]);
	}
	int randomRotate(Game* scn);

	void runOnCubes(int theStart, int theStop, float angle,int numAxis, Game *scn) {
		int arr[9];

		scn->Activate();
		scn->setDirection(direction);


	}



	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		int totalCobes =   pow(RUBIC_DIMENSION, 3);
		int totalCobesWall = pow(RUBIC_DIMENSION, 2);


		char* ranLetter = "RLUFDB";


		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{

			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				case GLFW_KEY_SPACE:
					direction = -direction;
				break;
				case GLFW_KEY_R: // RIGHT FACE
					scn->setQueueNewOperation('R');
					scn->setSturn('R');
					runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, X_AXIS, scn);
					break;
				case GLFW_KEY_L: // LEFT FACE
					scn->setQueueNewOperation('L');
					
					scn->setSturn('L');
					runOnCubes(totalCobes - totalCobesWall, totalCobes, direction * MOVING_ANGLE, X_AXIS, scn);
					break;
				case GLFW_KEY_U: // UPPER FACE
					scn->setQueueNewOperation('U');
					scn->setSturn('U');
					runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, Y_AXIS, scn);
					break;
				case GLFW_KEY_D: // BOTTOM FACE, direction switched
					scn->setQueueNewOperation('D');
					scn->setSturn('D');
					runOnCubes(totalCobes - totalCobesWall, totalCobes, -direction * MOVING_ANGLE, Y_AXIS, scn);
					break;
				case GLFW_KEY_B: // BACK FACE, direction switched
					scn->setQueueNewOperation('B');
					scn->setSturn('B');
					runOnCubes(totalCobes - totalCobesWall, totalCobes, -direction * MOVING_ANGLE, Z_AXIS, scn);
					break;
				case GLFW_KEY_F: // Front face
					scn->setQueueNewOperation('F');
					scn->setSturn('F');
					runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, Z_AXIS, scn);
					break;

				case GLFW_KEY_M: // RANDOM ROTATION


					for (int i = 0; i < 10; ++i) {
						int rnNum = rand() % 6;
						scn->setQueueNewOperation(ranLetter[rnNum]);
					}
					scn->setSturn('M');
					runOnCubes(0, totalCobesWall, direction * MOVING_ANGLE, Z_AXIS, scn);
					break;

				case GLFW_KEY_A: // slower rotation
					scn->rotation_speed -= 10; //
					break;

				case GLFW_KEY_Z: // faster rotation
					scn->rotation_speed += 10;
					break;
				

			default:
				break;
			}
		}
	}
	
	void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->UpdatePosition((float)xpos,(float)ypos);
		if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
		}

	}

	void window_size_callback(GLFWwindow* window, int width, int height)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		scn->Resize(width,height);
		
	}

	void Init(Display &display)
	{
		display.AddKeyCallBack(key_callback);
		display.AddMouseCallBacks(mouse_callback,scroll_callback,cursor_position_callback);
		display.AddResizeCallBack(window_size_callback);
	}