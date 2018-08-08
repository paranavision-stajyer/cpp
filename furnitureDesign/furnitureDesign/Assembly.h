#pragma once
#include "Panel.h"
namespace furniture
{
	class Assembly
	{
	public:
		void AddPanel(Panel* panel);
		void RemovePanel(Panel* panel);
		std::string getXML();
		std::vector<Panel*> panels;
		glm::mat4 getTransformMatrix(Panel* panel);
		int id;
		std::string assyName;
		Assembly();
		~Assembly();
	private:

	};
}

