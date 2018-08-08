#include "stdafx.h"
#include "Panel.h"
using namespace furniture;
const int Panel::coeffs[] = {
		// front face
		1, -1,  1,
		1,  1,  1,
		-1,  1,  1,
		-1, -1,  1,
		// back face
		-1, -1, -1,
		-1,  1, -1,
		1,  1, -1,
		1, -1, -1,
		// right face
		1, -1, -1,
		1,  1, -1,
		1,  1,  1,
		1, -1,  1,
		// left face
		-1, -1,  1,
		-1,  1,  1,
		-1,  1, -1,
		-1, -1, -1,
		// top face
		1,  1,  1,
		1,  1, -1,
		-1,  1, -1,
		-1,  1,  1,
		// bottom face
		-1, -1,  1,
		-1, -1, -1,
		1, -1, -1,
		1, -1,  1,
	};

Panel::Panel() {
	this->width = 1000;
	this->height = 700;
	this->stripThickness = 2;
	this->thickness = 18;
	this->material = MDF2;
	this->direction = 0;
	this->join = NULL;
}


Panel::~Panel(){
	delete join;
	this->join = NULL;
}

std::string Panel::getStripesString(){
	return std::string(this->stripes[0] ? "1-" : "0-") + std::string(this->stripes[1] ? "1-" : "0-") + std::string(this->stripes[2] ? "1-" : "0-") + std::string( this->stripes[3] ? "1" : "0");
}

int * Panel::getJoiningFaces()
{
	if (this->join != NULL)
		return this->join->joiningFaces;
	else
		return NULL;
}

int * Panel::getJoinedFaces()
{

	if (this->join != NULL)
		return this->join->joinedFaces;
	else
		return NULL;
}

float * Panel::getDistances()
{
	if (this->join != NULL)
		return this->join->distances;
	else
		return NULL;
}

void Panel::setId(int id)
{
	this->id = id;
}

int Panel::getId()
{
	return this->id;
}

double Panel::getStripThickness(){
	return this->stripThickness;
}

void Panel::setStripThickness(double stripThickness){
	double height=this->getHeight();
	double width = this->getWidth();
	this->stripThickness = stripThickness;
	this->setWidth(width);
	this->setHeight(height);
}

void Panel::setStripesFromString(std::string stripes){
	double width=this->getWidth();
	double height=this->getHeight();
	stripes += "-";
	std::string delimiter = "-";

	size_t pos = 0;
	std::string token;
	int arr[4];
	int i = 0;
	while ((pos = stripes.find(delimiter)) != std::string::npos && i!=4 ) {
		token = stripes.substr(0, pos);
		stripes.erase(0, pos + delimiter.length());
		this->stripes[i] = false;
		if (token == "1")
			this->stripes[i] = true;
		++i;
	}
	this->setWidth(width);
	this->setHeight(height);
}

void Panel::setJoin(Panel* joinedPanel,int* joiningFaces, int* joinedFaces, float* distances) {
	//for preventing joining each other//
	if (joinedPanel->join != NULL && joinedPanel->join->joiningPanel == this)
		return;
	//can not join to itself//
	if (joinedPanel == this)
		return;
	if (this->join == NULL)
		this->join = new Join(this, joinedPanel, joiningFaces, joinedFaces, distances);
	else {
		this->removeJoin();
		this->join = new Join(this, joinedPanel, joiningFaces, joinedFaces, distances);
	}
}

void Panel::removeJoin() {
	delete this->join;
	this->join = NULL;
}

glm::vec3 Panel::getFaceNormalVector(int faceId) {
	std::vector<glm::vec3> points(0);
	for (int i = 0; i < 3; ++i) 
		points.push_back(glm::vec3(coeffs[faceId*12+i*3],coeffs[faceId*12+1+ i * 3],coeffs[faceId*12+2+ i * 3]));
	glm::vec3 bma = glm::vec3(points[1].x-points[0].x, points[1].y - points[0].y, points[1].z - points[0].z);
	glm::vec3 cma = glm::vec3(points[2].x - points[0].x, points[2].y - points[0].y, points[2].z - points[0].z);
	return glm::normalize(glm::cross(bma, cma));
}

glm::vec3 Panel::getPointFromFace(int faceId) {
	return glm::vec3(coeffs[faceId * 12]*this->getWidth()/2, coeffs[faceId * 12 + 1]*this->getHeight()/2, coeffs[faceId * 12 + 2]*this->getThickness()/2);
}

double Panel::getWidth() {
	return width + (stripes[0] + stripes[2])*stripThickness;
}

void Panel::setWidth(double width){
	this->width = width - (stripes[0] + stripes[2])*stripThickness;
}

double Panel::getHeight(){
	return height + (stripes[1] + stripes[2])*stripThickness;
}

void Panel::setHeight(double height){
	this->height = height - (stripes[1] + stripes[2])*stripThickness;
}

double Panel::getThickness(){
	return thickness;
}

void Panel::setThickness(double thickness){
	this->thickness = thickness;
}

int Panel::getMaterial(){
	return this->material;
}
void Panel::setMaterial(Materials material){
	this->material = material;
}

glm::mat4 Panel::getTransformMatrix(){
	if (this->join != NULL)
		return this->join->transformMatrix;
	else
		return glm::mat4();
}