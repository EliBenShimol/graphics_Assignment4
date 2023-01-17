#include "../Engine3D/Bezier1D.h"
#include <iostream>
static void printMat(const glm::mat4 mat)
{
    std::cout << " matrix:" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            std::cout << mat[j][i] << " ";
        std::cout << std::endl;
    }
}
Bezier1D::Bezier1D(int segNum,int res,int mode, std::vector<glm::mat4> mat, int viewport)
{ 
    this->segmentsNum = segNum;
    this->resT = res;
    //printMat(mat);
    for (int i = 0; i < segmentsNum; i++) {
        segments.push_back(mat[i]);
    }
}

IndexedModel Bezier1D::GetLine()
{
    IndexedModel model;
    for (int i = 0; i <= segmentsNum * resT+segmentsNum-1; i++)
        model.indices.push_back(i);
    float t = 1.0 / resT;
    for (int j = 0; j < segmentsNum; j++) {
        for (int i = 0; i <= resT; i++) {
            glm::vec4 tAns = GetPointOnCurve(j, t*i);
            glm::vec3 tmp = glm::vec3(tAns[0], tAns[1], tAns[2]);
            model.positions.push_back(tmp);
            model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
            model.normals.push_back(glm::vec3(1.f, 1.f, 1.f));
        }
    }
    glm::vec4 tAns = GetPointOnCurve(segmentsNum-1, 1);
    glm::vec3 tmp = glm::vec3(tAns[0], tAns[1], tAns[2]);
    model.positions.push_back(tmp);
    model.colors.push_back(glm::vec3(1.f, 1.f, 1.f));
    model.normals.push_back(glm::vec3(1.f, 1.f, 1.f));

    //model.CalcNormals();

    return model;
}

glm::vec4 Bezier1D::GetControlPoint(int segment, int indx) const
{
    if (segment < segmentsNum)
        return segments[segment][indx];
    return segments[segmentsNum - 1][3];
}

glm::vec4 Bezier1D::GetPointOnCurve(int segment, float t)
{
    glm::vec4 p0 = segments[segment][0];
    glm::vec4 p1 = segments[segment][1];
    glm::vec4 p2 = segments[segment][2];
    glm::vec4 p3 = segments[segment][3];
    float x = pow(1-t, 3) * p0[0] +
        pow(1 - t, 2) * 3 * t * p1[0] +
        (1 - t) * 3 * t * t * p2[0] +
        t * t * t * p3[0];
    float y = pow(1 - t, 3) * p0[1] +
        pow(1 - t, 2) * 3 * t * p1[1] +
        (1 - t) * 3 * t * t * p2[1] +
        t * t * t * p3[1];
    glm::vec4 pFinal = glm::vec4(x, y, p0[2], p0[3]);
    return pFinal;
}

glm::vec3 Bezier1D::GetVelosity(int segment, int t)
{
    return glm::vec3();
}

void Bezier1D::SplitSegment(int segment, int t)
{
}

void Bezier1D::AddSegment(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments.back()[3];
    segments.push_back(glm::mat4(p0, p1, p2, p3));
}

void Bezier1D::ChangeSegment(int segIndx,glm::vec4 p1, glm::vec4 p2, glm::vec4 p3)
{
    glm::vec4 p0 = segments[segIndx-1][3];
    segments[segIndx] = glm::mat4(p0, p1, p2, p3);
}


void Bezier1D::updateIndx(int segIndx, glm::mat4 mat) {
    segments[segIndx] = mat;
}

float Bezier1D::MoveControlPoint(int segment, int indx, float dx,float dy,bool preserveC1)
{
    return 0; //not suppose to reach here
}

void Bezier1D::changePoint(int segIndx, glm::vec3 newP0) {
    if (segIndx < segmentsNum)
        segments[segIndx][0] = glm::vec4(newP0, 0);
}

void Bezier1D::CurveUpdate(int pointIndx, float dx, float dy, bool preserveC1)
{
}

void Bezier1D::ResetCurve(int segNum)
{

}

float Bezier1D::newPow(float a, int b) {
    float ans = 1.0;
    for (int i = 0; i < b; i++)
        ans = ans * a;
    return ans;
}

Bezier1D::~Bezier1D(void)
{

}