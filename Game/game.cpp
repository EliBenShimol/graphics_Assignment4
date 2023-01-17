#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <Bezier1D.h>
int countIndx = 0;
Bezier1D* b;
std::vector<glm::vec3> posCube;
int countGo = 0;
bool canStart = false;
int way = 0;
int insideCount = 1;
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

Game::Game(float angle ,float relationWH, float near1, float far1) : Scene(angle,relationWH,near1,far1)
{ 	
}

void Game::Init()
{		

	AddShader("../res/shaders/pickingShader");	
	AddShader("../res/shaders/basicShader");
	
	AddTexture("../res/textures/bricks.jpg",false);

	restartScene(3);
	pickedShape = 0;
	
	SetShapeTex(0,0);
	MoveCamera(0,zTranslate,10);
	pickedShape = -1;
	
	//ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::changeSeg(int indx, bool checkCont, int type) {
	if (indx != -1) {
		int segIndx = indx;
		if (indx < 4)
			segIndx = 4;
		else {
			while (segIndx % 3 != 0)
				segIndx++;
		}
		if (segIndx < countIndx) {
			int place = segIndx - 3;
			if(segIndx == 4)
				place = segIndx - 4;

			if (segIndx == 4)
				segIndx = 0;
			else
				segIndx = segIndx/3 - 1;
			glm::mat4 ind;
			int count = 0;
			for (int i = place; i < place + 4; i++) {
				glm::vec4 tmp = glm::vec4(shapes[i]->delta, 0);
				ind[count] = tmp;
				count++;
			}
			
			b->updateIndx(segIndx, ind);
			if (indx % 3 == 0)
				b->changePoint(segIndx + 1, glm::vec3(ind[3][0], ind[3][1], ind[3][2]));

			IndexedModel m = b->GetLine();
			posCube = m.positions;
			shapes[shapes.size() - 1]->setMeshForBezier(m);
			if (checkCont && type == 1) {
				if (indx % 3 == 1) {
					if (indx - 2 > 0)
						changeSeg(indx - 2, checkCont, 0);
				}
				else if(indx % 3 == 2)
					if (indx + 2 < shapes.size())
						changeSeg(indx + 2, checkCont, 0);
			}

			if (type == 1 && indx % 3 == 0) {
				if (indx + 1 < shapes.size())
					changeSeg(indx + 1, checkCont, 0);
				if (indx - 1 > 0)
					changeSeg(indx - 1, checkCont, 0);

			}

			shapes[shapes.size() - 2]->ZeroTrans();
			shapes[shapes.size() - 2]->MyScale(glm::vec3(0.3, 0.3, 0.3));
			shapes[shapes.size() - 2]->MyTranslate(glm::vec3(shapes[0]->delta[0], shapes[0]->delta[1], 0), 0);
			countGo = 0;
			way = 0;
			insideCount = 0;
		}
	}

}

void Game::controlMov() {
	canStart = !canStart;
}

void Game::addLine() {
	glm::vec3 delta = shapes[countIndx-1]->delta;

	for (int i = 0; i < 3; i++) {
		AddShape(Octahedron, -1, TRIANGLES);
		SetShapeTex(1, 0);
		shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
		shapes[countIndx]->MyTranslate(glm::vec3(delta[0] + i +  1, delta[1], 0), 0);
		countIndx++;
	}

}

void Game::closeLine() {
	glm::vec3 delta = shapes[countIndx - 1]->delta;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(delta[0] + 4, delta[1], 0), 0);
	countIndx++;

	delta = shapes[countIndx - 1]->delta;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(delta[0] + 1.5, delta[1] - 3, 0), 0);
	countIndx++;

	delta = shapes[countIndx - 1]->delta;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(delta[0] - 0.5, delta[1] - 4, 0), 0);
	countIndx++;
}

glm::mat4 Game::getMat() {
	glm::mat4 ind;
	int count = 0;
	for (int i = countIndx - 4; i < countIndx; i++) {
		glm::vec4 tmp = glm::vec4(shapes[i]->delta, 0);
		ind[count] = tmp;
		count++;
	}
	return ind;

}


void Game::restartScene(int segments){
	while (shapes.size() > 0)
		shapes.pop_back();
	countIndx = 0;

	AddShape(Octahedron, -1, TRIANGLES);
	float scale = 400.0 / 12.0;
	shapes[countIndx]->MyTranslate(glm::vec3(-11, -7, 0), 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	countIndx++;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(-11.5, -3, 0), 0);
	countIndx++;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(-10, 0, 0), 0);
	countIndx++;
	AddShape(Octahedron, -1, TRIANGLES);
	SetShapeTex(1, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(-6, 0, 0), 0);
	countIndx++;
	std::vector<glm::mat4> inds;
	glm::mat4 ind = getMat();
	inds.push_back(ind);

	for (int i = 1; i < segments - 1; i++) {
		addLine();
		inds.push_back(getMat());
	}

	closeLine();
	glm::mat4 ind4 = getMat();
	inds.push_back(ind4);

	
	AddShape(Cube, -1, TRIANGLES);
	SetShapeTex(countIndx, 0);
	shapes[countIndx]->MyScale(glm::vec3(0.3, 0.3, 0.3));
	shapes[countIndx]->MyTranslate(glm::vec3(shapes[0]->delta[0], shapes[0]->delta[1], 0), 0);
	countIndx++;
	

	AddShape(LineCopy, -1, LINE_STRIP);
	b = new Bezier1D(segments, 100, 0, inds, 0);
	IndexedModel m = b->GetLine();
	posCube = m.positions;
	shapes[countIndx]->setMeshForBezier(m);
	countIndx++;
	way = 0;
	countGo = 0;
	insideCount = 0;
	canStart = true;



}

void Game::Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape+1) & 0x000000FF) >>  0;
	int g = ((pickedShape+1) & 0x0000FF00) >>  8;
	int b = ((pickedShape+1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal",Model);
	s->SetUniform4f("lightDirection", 0.0f , 0.0f, -1.0f, 0.0f);
	if(shaderIndx == 0)
		s->SetUniform4f("lightColor",r/255.0f, g/255.0f, b/255.0f,1.0f);
	else 
		s->SetUniform4f("lightColor",0.7f,0.8f,0.1f,1.0f);
	if (canStart) {
		if (way == 0) {
			if (countGo < posCube.size()) {
				glm::vec3 tmp = posCube[countGo];
				if (insideCount == 80) {
					shapes[shapes.size() - 2]->ZeroTrans();
					shapes[shapes.size() - 2]->MyScale(glm::vec3(0.3, 0.3, 0.3));
					shapes[shapes.size() - 2]->MyTranslate(glm::vec3(tmp[0], tmp[1], 0), 0);
					if (countGo < posCube.size()-1) {
						glm::vec3 tmp2 = posCube[countGo + 1];
						glm::vec3 rotVec = tmp - tmp2;
						glm::vec3 norm = glm::vec3(0, 1, 0);
						float sizeVec = sqrt(rotVec[0] * rotVec[0] + rotVec[1] * rotVec[1]);
						float ang2 = acos(glm::dot(rotVec, norm) / sizeVec);
						float pi = 3.1415928;
						ang2 = ang2 * 180 / pi;
						shapes[shapes.size() - 2]->MyRotate(ang2, glm::vec3(0, 0, 1), 0);
					}
					countGo++;
					insideCount = 0;
				}
				else
					insideCount++;
			}
			else {
				way = 1;
				countGo = countGo - 1;
			}
		}
		else if (way == 1) {
			if (countGo > -1) {
				if (insideCount == 80) {
					glm::vec3 tmp = posCube[countGo];
					shapes[shapes.size() - 2]->ZeroTrans();
					shapes[shapes.size() - 2]->MyScale(glm::vec3(0.3, 0.3, 0.3));
					shapes[shapes.size() - 2]->MyTranslate(glm::vec3(tmp[0], tmp[1], 0), 0);
					if (countGo > 0) {
						glm::vec3 tmp2 = posCube[countGo - 1];
						glm::vec3 rotVec = tmp2 - tmp;
						glm::vec3 norm = glm::vec3(0, 1, 0);
						float sizeVec = sqrt(rotVec[0] * rotVec[0] + rotVec[1] * rotVec[1]);
						float ang2 = acos(glm::dot(rotVec, norm) / sizeVec);
						float pi = 3.1415928;
						ang2 = ang2 * 180 / pi;
						shapes[shapes.size() - 2]->MyRotate(ang2, glm::vec3(0, 0, 1), 0);
					}
					countGo--;
					insideCount = 0;
				}
				else
					insideCount++;
			}
			else {
				way = 0;
				countGo++;
			}
		}
	}

	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if(isActive)
	{
	}
}

Game::~Game(void)
{
}
