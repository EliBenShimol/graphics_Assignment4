#include "scene.h"
#include "glad/include/glad/glad.h"
#include <iostream>

	static void printMat(const glm::mat4 mat)
	{
		printf(" matrix: \n");
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				printf("%f ", mat[j][i]);
			printf("\n");
		}
	}

	std::vector<float> rotSizes;
	std::vector<glm::vec2> lineSizes;
	bool checkIndx = false;
	int saveIndx = -1;
	bool checkIndx2 = false;
	int saveIndx2 = -1;
	Scene::Scene()
	{
		//
		glLineWidth(5);
	
		cameras.push_back(new Camera(60.0f,1.0,0.1f,100.0f));		
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	Scene::Scene(float angle,float relationWH,float near1, float far1)
	{
		//glLineWidth(5);
		cameras.push_back(new Camera(angle,relationWH,near1,far1));
		pickedShape = -1;
		depth = 0;
		cameraIndx = 0;
		xold = 0;
		yold = 0;
		isActive = false;
	}

	void Scene::AddShapeFromFile(const std::string& fileName,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(fileName,mode));
	}

	void Scene::AddShape(int type, int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(type,mode));
	}

	void Scene::AddShapeCopy(int indx,int parent,unsigned int mode)
	{
		chainParents.push_back(parent);
		shapes.push_back(new Shape(*shapes[indx],mode));
	}

	void Scene::AddShader(const std::string& fileName)
	{
		shaders.push_back(new Shader(fileName));
	}

	void Scene::AddTexture(const std::string& textureFileName,bool for2D)
	{
		textures.push_back(new Texture(textureFileName));
	}

	void Scene::AddTexture(int width,int height, unsigned char *data)
	{
		textures.push_back(new Texture(width,height,data));
	}

	void Scene::AddCamera(glm::vec3& pos , float fov,float relationWH , float zNear, float zFar)
	{
		cameras.push_back(new Camera(fov,relationWH,zNear,zFar));
		cameras.back()->MyTranslate(pos,0);
	}

	void Scene::Draw(int shaderIndx,int cameraIndx,int buffer,bool toClear,bool debugMode)
	{
		glEnable(GL_DEPTH_TEST);
		glm::mat4 Normal = MakeTrans();
	
		glm::mat4 MVP = cameras[cameraIndx]->GetViewProjection()*glm::inverse(cameras[cameraIndx]->MakeTrans());
		int p = pickedShape;
		if(toClear)
		{
			if(shaderIndx>0)
				Clear(1,0,1,1);
			else
				Clear(0,0,0,0);
		}

		for (unsigned int i=0; i<shapes.size();i++)
		{
			if(shapes[i]->Is2Render())
			{
				glm::mat4 Model = Normal * shapes[i]->MakeTrans();
				
				if(shaderIndx > 0)
				{
					Update(MVP,Model,shapes[i]->GetShader());
					shapes[i]->Draw(shaders,textures,false);	
				}
				else 
				{ //picking
					Update(MVP,Model,0);
					shapes[i]->Draw(shaders,textures,true);
				}
			}
		}
		pickedShape = p;
	}

	void Scene::MoveCamera(int cameraIndx,int type,float amt)
	{
		switch (type)
		{
			case xTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				cameras[cameraIndx]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				cameras[cameraIndx]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;

			case -1:
				cameras[cameraIndx]->ZeroTrans();
				break;
			default:
				break;
		}
	}

	void Scene::ShapeTransformation(int type,float amt)
	{
		if(glm::abs(amt)>1e-5)
		{
			switch (type)
			{
			case xTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(amt,0,0),0);
			break;
			case yTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,amt,0),0);
			break;
			case zTranslate:
				shapes[pickedShape]->MyTranslate(glm::vec3(0,0,amt),0);
			break;
			case xRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(1,0,0),0);
			break;
			case yRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,1,0),0);
			break;
			case zRotate:
				shapes[pickedShape]->MyRotate(amt,glm::vec3(0,0,1),0);
			break;
			default:
				break;
			}
		}

	}

	void Scene::Resize(int width,int height)
	{
	
		cameras[0]->SetProjection(cameras[0]->GetAngle(),(float)width/height);
		glViewport(0,0,width,height);
		std::cout<<cameras[0]->GetRelationWH()<<std::endl;
	}
	int Scene::getClosestOct(float newX, float newY) {
		int ans = -1;
		float pickedShapeX = 2.0;
		float pickedShapeY = 2.0;
		int count = 0;
		for (Shape* s : shapes){
			if (s->shapeType == 3) {
				float placeX = s->delta[0];
				float placeY = -s->delta[1];
				//std::cout << s->shapeType << std::endl;
				float checkX = newX - placeX;
				float checkY = newY - placeY;
				//std::cout << checkX << std::endl;
				//std::cout << checkY << std::endl;
				if (checkX < 0)
					checkX = -1 * checkX;
				if (checkY < 0)
					checkY = -1 * checkY;
				if (checkX < 0.5 && checkY < 0.5)
					if (checkX <= pickedShapeX && checkY <= pickedShapeY) {
						pickedShapeX = checkX;
						pickedShapeY = checkY;
						ans = count;
					}
			}
			count++;
		}
		return ans;
	}


	void Scene::restart(int checkCont) {
		checkIndx = false;
		saveIndx = -1;
		if (checkCont == 1)
			checkIndx2 = false;
	}
	float Scene::Picking(int x,int y, int mode, bool checkCont)
	{
		float part = 1.0 / 3.0;
		float scale = 33.0 + part;
		int xTmp = x - 400;
		int yTmp = y - 400;
		float newX = xTmp / scale;
		float newY = yTmp / scale;
		int indx = -1;
		indx = getClosestOct(newX, newY);
		if (checkCont) {
			if (!checkIndx2) {
				if (indx != -1) {
					saveIndx2 = indx;
					checkIndx2 = true;
				}
			}
			else
				indx = saveIndx2;
		}
		else if (mode == 0) {
			indx = getClosestOct(newX, newY);
			checkIndx = false;
		}
		else if (mode == 1) {
			//std::cout << "here" << std::endl;
			if (!checkIndx) {
				if (indx != -1) {
					saveIndx = indx;
					checkIndx = true;
				}
			}
			else
				indx = saveIndx;


		}

		if (mode == 0) {
			if (indx != -1) {
				if (!checkCont || indx % 3 == 0) {
					shapes[indx]->ZeroTrans();
					shapes[indx]->MyTranslate(glm::vec3(newX, -newY, 0), 0);
					shapes[indx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
				}
				else {
					int rotIndx = -1;
					if (indx % 3 == 1)
						rotIndx = indx - 1;
					else if (indx % 3 == 2)
						rotIndx = indx + 1;
					sameVector(rotIndx, indx, newX, newY);
				}
				return indx;
			}
		}

		else if (mode == 1 && indx != -1) {
			//std::cout << indx << std::endl;
			int rotIndx = -1;
			if (indx % 3 == 1) {
				rotIndx = indx - 1;
				rotAround(rotIndx, indx, newX, newY);
				if (checkCont) {
					if ((indx - 2) > 0) {
						glm::vec3 rotDelta = shapes[rotIndx]->delta;
						float lengthX = newX - rotDelta[0];
						if (lengthX < 0)
							lengthX = -1 * lengthX;
						float lengthY = newY - rotDelta[1];
						if (lengthY < 0)
							lengthY = -1 * lengthY;

						float newX2 = newX - 2 * lengthX;
						if(newX - rotDelta[0] < 0) 
							newX2 = newX + 2 * lengthX;
						float  newY2 = newY - 2 * lengthY;
						if (newY - rotDelta[1] < 0)
							newY2 = newY + 2 * lengthY;
						rotAround(rotIndx, indx - 2, newX2, newY2);
					}
					
				}
				return indx;
			}
			else if (indx % 3 == 2) {
				rotIndx = indx + 1;
				rotAround(rotIndx, indx, newX, newY);
				if (checkCont) {
					if ((indx + 2) < shapes.size()-2) {
						glm::vec3 rotDelta = shapes[rotIndx]->delta;
						float lengthX = newX - rotDelta[0];
						float lengthY = newY - rotDelta[1];
						rotAround(rotIndx, indx + 2, newX - 2 * lengthX, newY - 2 * lengthY);
					}

				}
				return indx;
			}
			else {
				if (indx != -1) {
					glm::vec3 delta = shapes[indx]->delta;
					float dx = newX - delta[0];
					float dy = newY + delta[1];
					if (indx - 1 > 0) {
						glm::vec3 tmpDelta = shapes[indx-1]->delta;
						shapes[indx-1]->ZeroTrans();
						shapes[indx-1]->MyTranslate(glm::vec3(tmpDelta[0] + dx, tmpDelta[1] - dy, 0), 0);
						shapes[indx-1]->MyScale(glm::vec3(0.3, 0.3, 0.3));
					}
					if (indx + 1 < shapes.size() - 1) {
						glm::vec3 tmpDelta = shapes[indx + 1]->delta;
						shapes[indx + 1]->ZeroTrans();
						shapes[indx + 1]->MyTranslate(glm::vec3(tmpDelta[0] + dx, tmpDelta[1] - dy, 0), 0);
						shapes[indx + 1]->MyScale(glm::vec3(0.3, 0.3, 0.3));
					}


					shapes[indx]->ZeroTrans();
					shapes[indx]->MyTranslate(glm::vec3(newX, -newY, 0), 0);
					shapes[indx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
					return indx;
				}
			}

		}

		return -1;
	}


	void Scene::sameVector(int rotIndx, int myIndx, float x, float y) {
		float newX = -1.0f;
		float newY = -1.0f;

		int addPlace = 2 * (myIndx / 3);
		int morePlace = myIndx % 3 - 1;
		int newPlace = addPlace + morePlace;
		glm::vec2 myLine = lineSizes[newPlace];
		float m = myLine[0];
		float b = myLine[1];
		
		//std::cout << m << std::endl;
		//std::cout << b << std::endl;
		//std::cout << "here" << std::endl;
		if (m != INFINITE) {
			newX = x;
			newY = m * newX + b;
		}
		else {
			newX = delta[0];
			newY = y;
		}
		//std::cout << newX << std::endl;
		//std::cout << newY << std::endl;
		shapes[myIndx]->ZeroTrans();
		shapes[myIndx]->MyTranslate(glm::vec3(newX, -newY, 0), 0);
		shapes[myIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	}

	void Scene::changeLines() {
		while (lineSizes.size() > 0)
			lineSizes.pop_back();
		for (int i = 0; i < shapes.size(); i++) {
			if (shapes[i]->shapeType == 3) {
				if (i % 3 != 0) {
					glm::vec3 rotDelta;
					if (i % 3 == 1)
						rotDelta = shapes[i - 1]->delta;
					else if (i % 3 == 2)
						rotDelta = shapes[i + 1]->delta;
					glm::vec3 delta = shapes[i]->delta;

					float m = (-rotDelta[1] + delta[1]) / (rotDelta[0] - delta[0]);
					float b = -rotDelta[1] - m * rotDelta[0];

					glm::vec2 adding = glm::vec2(m, b);

					lineSizes.push_back(adding);

				}
			}
		}
	}
	void Scene::changeRotSize() {
		while (rotSizes.size() > 0)
			rotSizes.pop_back();
		for (int i = 0; i < shapes.size(); i++) {
			if (shapes[i]->shapeType == 3) {
				if (i % 3 != 0) {
					glm::vec3 rotDelta;
					if (i % 3 == 1)
						rotDelta = shapes[i-1]->delta;
					else if (i % 3 == 2)
						rotDelta = shapes[i+1]->delta;
					glm::vec3 delta = shapes[i]->delta;
					float myLength = sqrt((delta[0] - rotDelta[0]) * (delta[0] - rotDelta[0]) + (delta[1] - rotDelta[1]) * (delta[1] - rotDelta[1]));
					rotSizes.push_back(myLength);
				}
			}
		}

	}

	void Scene::rotAround(int rotIndx, int myIndx, float x, float y) {
		glm::vec3 rotDelta = shapes[rotIndx]->delta;
		int addPlace = 2* (myIndx / 3);
		int morePlace = myIndx % 3 - 1;
		int newPlace = addPlace + morePlace;
		float myLength = rotSizes[newPlace];
		float mouseLength = sqrt((x - rotDelta[0]) * (x - rotDelta[0]) + (y + rotDelta[1]) * (y + rotDelta[1]));
		
		float newX = 0.0f;
		float newY = 0.0f;
		float tmpX = x - rotDelta[0];
		float tmpY = -y - rotDelta[1];
		float getDiff = myLength / mouseLength;
		newX = tmpX * getDiff + rotDelta[0];
		newY = tmpY * getDiff + rotDelta[1];
		//std::cout << newX << std::endl;
		//std::cout << newY << std::endl;
		//std::cout << "here" << std::endl;
		shapes[myIndx]->ZeroTrans();
		shapes[myIndx]->MyTranslate(glm::vec3(newX, newY, 0), 0);
		shapes[myIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
		



	}
	//return coordinates in global system for a tip of arm position is local system 
	void Scene::MouseProccessing(int button)
	{
		if(pickedShape == -1)
		{
			if(button == 1 )
			{				

				MyTranslate(glm::vec3(-xrel/20.0f,0,0),0);
				MyTranslate(glm::vec3(0,yrel/20.0f,0),0);
				WhenTranslate();
			}
			else
			{
				MyRotate(xrel/2.0f,glm::vec3(1,0,0),0);
				MyRotate(yrel/2.0f,glm::vec3(0,0,1),0);
				WhenRotate();
			}
		}
	}

	void Scene::ZeroShapesTrans()
	{
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			shapes[i]->ZeroTrans();
		}
	}

	void Scene::ReadPixel()
	{
		glReadPixels(1,1,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&depth);
	}

	void Scene::UpdatePosition(float xpos, float ypos)
	{
		xrel = xold - xpos;
		yrel = yold - ypos;
		xold = xpos;
		yold = ypos;
	}

	void Scene::HideShape(int shpIndx)
	{
			shapes[shpIndx]->Hide();
	}

	void Scene::UnhideShape(int shpIndx)
	{
			
			shapes[shpIndx]->Unhide();
	}

	void Scene::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	Scene::~Scene(void)
{
	for (Shape* shp : shapes)
		{
			delete shp;
		}
	for (Camera* cam : cameras)
		{
			delete cam;
		}
	for(Shader* sdr: shaders)
		{
			delete sdr;
		}
	for(Texture* tex: textures)
		{
			delete tex;
		}

}


	 
	