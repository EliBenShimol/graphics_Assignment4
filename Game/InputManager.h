#pragma once   //maybe should be static class
#include "display.h"
#include "game.h"
#include <iostream>

float yMove = 0.0f;
float xMove = 0.0f;
float zMove = 0.0f;
bool check = false;
bool checkCont = false;
	void mouse_callback(GLFWwindow* window,int button, int action, int mods)
	{	
		if(action == GLFW_PRESS )
		{
			Game *scn = (Game*)glfwGetWindowUserPointer(window);
			double x2,y2;
			glfwGetCursorPos(window,&x2,&y2);
		}
	}
	
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);
		scn->MyTranslate(glm::vec3(0,0,xoffset),0);
		
	}
	
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Game *scn = (Game*)glfwGetWindowUserPointer(window);

		if(action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			switch (key)
			{
				case GLFW_KEY_ESCAPE:			
					glfwSetWindowShouldClose(window,GLFW_TRUE);
				break;
				case GLFW_KEY_SPACE:
					if(scn->IsActive())
						scn->Deactivate();
					else
						scn->Activate();
					scn->controlMov();
				break;

				case GLFW_KEY_UP:
					scn->MoveCamera(0,scn->yTranslate,0.4f);
					yMove = yMove + 0.4f;
					//scn->MouseProccessing(GLFW_KEY_UP);
					break;
				case GLFW_KEY_F:
					scn->MoveCamera(0, scn->zTranslate, 0.4f);
					zMove = zMove + 0.4f;
					break;

				case GLFW_KEY_B:
					scn->MoveCamera(0, scn->zTranslate, -0.4f);
					zMove = zMove - 0.4f;
					break;
				case GLFW_KEY_DOWN:
					//scn->shapeTransformation(scn->xGlobalRotate,-5.f);
					//cout<< "down: "<<endl;
					scn->MoveCamera(0,scn->yTranslate,-0.4f);
					yMove = yMove - 0.4f;
					break;

				case GLFW_KEY_LEFT:
					scn->MoveCamera(0, scn->xTranslate, 0.4f);
					xMove = xMove + 0.4f;
					break;
				case GLFW_KEY_RIGHT:
					scn->MoveCamera(0, scn->xTranslate, -0.4f);
					xMove = xMove - 0.4f;
					break;
				case GLFW_KEY_C:
					checkCont = !checkCont;
					//std::cout << checkCont << std::endl;
					scn->changeLines();
					break;
				case GLFW_KEY_2:
					scn->restartScene(2);
					scn->MoveCamera(0, scn->xTranslate, -xMove);
					scn->MoveCamera(0, scn->yTranslate, -yMove);
					scn->MoveCamera(0, scn->zTranslate, -zMove);
					xMove = 0;
					yMove = 0;
					zMove = 0;
					check = false;
					scn->restart(1);
					break;
				case GLFW_KEY_3:
					scn->restartScene(3);
					scn->MoveCamera(0, scn->xTranslate, -xMove);
					scn->MoveCamera(0, scn->yTranslate, -yMove);
					scn->MoveCamera(0, scn->zTranslate, -zMove);
					xMove = 0;
					yMove = 0;
					zMove = 0;
					check = false;
					scn->restart(1);
					break;
				case GLFW_KEY_4:
					scn->restartScene(4);
					scn->MoveCamera(0, scn->xTranslate, -xMove);
					scn->MoveCamera(0, scn->yTranslate, -yMove);
					scn->MoveCamera(0, scn->zTranslate, -zMove);
					xMove = 0;
					yMove = 0;
					zMove = 0;
					check = false;
					scn->restart(1);
					break;
				case GLFW_KEY_5:
					scn->restartScene(5);
					scn->MoveCamera(0, scn->xTranslate, -xMove);
					scn->MoveCamera(0, scn->yTranslate, -yMove);
					scn->MoveCamera(0, scn->zTranslate, -zMove);
					xMove = 0;
					yMove = 0;
					zMove = 0;
					check = false;
					scn->restart(1);
					break;
				case GLFW_KEY_6:
					scn->restartScene(6);
					scn->MoveCamera(0, scn->xTranslate, -xMove);
					scn->MoveCamera(0, scn->yTranslate, -yMove);
					scn->MoveCamera(0, scn->zTranslate, -zMove);
					xMove = 0;
					yMove = 0;
					zMove = 0;
					check = false;
					scn->restart(1);
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
			if (!check) {
				scn->changeRotSize();
				check = true;
			}
			//scn->MouseProccessing(GLFW_MOUSE_BUTTON_RIGHT);
			float part = 1.0 / 3.0;
			float scale = 33.0 + part;
			float yAdd = yMove * scale;
			float xAdd = xMove * scale;
			int indx = scn->Picking((int)(xpos + xAdd), (int)(ypos - yAdd), 1, checkCont);
			if (indx != -1)
				scn->changeSeg(indx, checkCont, 1);
		}
		else if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			//std::cout << xpos << std::endl;
			//std::cout << ypos << std::endl;
			check = false;
			scn->restart(0);
			float part = 1.0 / 3.0;
			float scale = 33.0 + part;
			float yAdd = yMove * scale;
			float xAdd = xMove * scale;
			int indx = scn->Picking((int)(xpos + xAdd), (int)(ypos - yAdd), 0, checkCont);
			if(indx != -1)
				scn->changeSeg(indx, checkCont, 0);
			//else
				//scn->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
			//std::cout << xpos << std::endl;
			//std::cout << ypos << std::endl;
		}
		else {
			check = false;
			scn->restart(1);
			if(checkCont)
				scn->changeLines();
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
