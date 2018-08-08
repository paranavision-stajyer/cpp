#pragma once
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
namespace furniture {
	enum Materials { MDF2 = 0, MDF1 = 1, LDF2 = 2, LDF1 = 3 };

	class Panel
	{
		class  Join {
		public:
			glm::mat4 transformMatrix;
			int* joiningFaces;
			int* joinedFaces;
			float* distances;
			Panel* joiningPanel;
			Panel* joinedPanel;
			Join(Panel* joiningPanel, Panel* joinedPanel, int* joiningFaces, int* joinedFaces, float* distances) {
				this->joiningPanel = joiningPanel;
				this->joinedPanel = joinedPanel;
				this->joiningFaces = joiningFaces;
				this->joinedFaces = joinedFaces;
				this->distances = distances;
				this->calculateTransformMatrix();
			}
			~Join() {
				delete[] joinedFaces;
				delete[] joiningFaces;
				delete[] distances;
			}
		private:
			void calculateTransformMatrix() {
				glm::mat4 transform = glm::mat4();
				if (joinedPanel != NULL) {
					std::vector<glm::vec3> joiningNormals(0);
					std::vector<glm::vec3> joinedNormals(0);
					std::vector<glm::vec3> joiningPoints(0);
					std::vector<glm::vec3> joinedPoints(0);
					for (int i = 0; i < 3; ++i) {
						joiningNormals.push_back(this->joiningPanel->getFaceNormalVector(joiningFaces[i]));
						joinedNormals.push_back(this->joinedPanel->getFaceNormalVector(joinedFaces[i]));
						joiningPoints.push_back(this->joiningPanel->getPointFromFace(joiningFaces[i]));
						joinedPoints.push_back(this->joinedPanel->getPointFromFace(joinedFaces[i]));
					}
					//calculating the rotations//
					for (int i = 0; i < 3; ++i) {
						glm::vec3 joiningNormal = transform * glm::vec4(joiningNormals[i], 0);
						if (i == 0)
							joiningNormal = glm::vec3(-joiningNormal.x, -joiningNormal.y, -joiningNormal.z);
						double fcrossx = joiningNormal.y*joinedNormals[i].z - joiningNormal.z*joinedNormals[i].y;
						double fcrossy = joiningNormal.z*joinedNormals[i].x - joiningNormal.x*joinedNormals[i].z;
						double fcrossz = joiningNormal.x*joinedNormals[i].y - joiningNormal.y*joinedNormals[i].x;
						double fcross = std::sqrt(std::pow(fcrossx, 2) + std::pow(fcrossy, 2) + std::pow(fcrossz, 2));
						float angle = (float)std::atan2(fcross, glm::dot(joiningNormal, joinedNormals[i]));
						//if angle is zero do not apply transform//
						if ((angle<0.01&&angle>-0.01))
							continue;

						glm::vec3 axis = glm::cross(joiningNormal, joinedNormals[i]);
						//if angle is pi(180 degree) use any axis that is perpendicular to facenormals as an axis
						if ((angle<((glm::pi<float>()) + 0.01) && angle>(glm::pi<float>()) - 0.01)) {
							float x = 0;
							float y = 0;
							float z = 0;
							for (int i = 0; i < 3; ++i) {
								if (i == 0)
									if (joinedNormals[i].x == 0) {
										x = 1;
										break;
									}
									else if (i == 1)
										if (joinedNormals[i].y == 0) {
											y = 1;
											break;
										}
										else if (i == 2)
											if (joinedNormals[i].y == 0) {
												z = 1;
												break;
											}
							}
							if (x == 0 && y == 0 && z == 0)
								x = 1;
							axis = glm::vec3(x, y, z);
						}
						glm::mat4 rotationTransform = glm::rotate(angle, axis);
						transform = rotationTransform * transform;
					}
					//calculating translations//
					for (int i = 0; i < 3; ++i) {
						glm::vec3 joiningPoint = transform * glm::vec4(joiningPoints[i], 1);
						glm::vec3 p2p1 = glm::vec3(-joiningPoint.x + joinedPoints[i].x, -joiningPoint.y + joinedPoints[i].y, -joiningPoint.z + joinedPoints[i].z);
						float distance = glm::dot(p2p1, joinedNormals[i]) + this->distances[i];
						glm::mat4 translationTransform = glm::translate(glm::vec3(distance*joinedNormals[i].x, distance*joinedNormals[i].y, distance*joinedNormals[i].z));
						transform = translationTransform * transform;
					}
				}
				this->transformMatrix = transform;
			}
		};
	private:
		int id;
		double width;
		double height;
		double thickness;
		double stripThickness;
		Materials material;
		bool stripes[4] = { false,false,false,false };
		static const int coeffs[];

	public:
		int direction;
		std::string panelName;
		Join* join;
		Panel();
		~Panel();
		std::string getStripesString();
		int* getJoiningFaces();
		int* getJoinedFaces();
		float* getDistances();
		void setId(int id);
		int getId();
		double getStripThickness();
		void setStripThickness(double stripThickness);
		void setStripesFromString(std::string stripes);



		/**create new int* with the size 3 for joined and joining faces, create new float* for the distances
		* if Panel has already join property it removes the Join and create new one
		* Example:
		*  Panel* p1 = new Panel();
		*  Panel* p2 = new Panel();
		*  int* joiningFaces = new int[3]{ 3,5,2 };
		*  int* joinedFaces = new int[3]{ 3,5,2 };
		*  float* distances = new float[3]{ 0, 10, 20 };
		*  p1->setJoin(p2, joiningFaces, joinedFaces, distances);
		*/
		void setJoin(Panel* joinedPanel, int* joiningFaces, int* joinedFaces, float* distances);
		/** Removes Existin Join */
		void removeJoin();
		/** faceId should be in range [0-5]*/
		glm::vec3 getFaceNormalVector(int faceId);
		/** faceId should be in range [0-5]*/
		glm::vec3 getPointFromFace(int faceId);
		double getWidth();
		void setWidth(double width);
		double getHeight();
		void setHeight(double height);
		double getThickness();
		void setThickness(double thickness);
		int getMaterial();
		void setMaterial(Materials material);
		glm::mat4 getTransformMatrix();
	};
}