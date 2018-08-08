#include "stdafx.h"
#include "Assembly.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using namespace furniture;
void Assembly::AddPanel(Panel * panel) {
	panels.push_back(panel);
}

void Assembly::RemovePanel(Panel * panel) {
	int i = 0;
	for (Panel* p : panels){
		if (p == panel)
		{
			panels.erase(panels.begin() + i);
			break;
		}
		++i;
	}
}

std::string Assembly::getXML()
{
	XMLDocument doc;
	XMLElement* msg;

	doc.NewDeclaration();
	XMLElement* root = doc.NewElement("assy");
	root->SetAttribute("name", (&(this->assyName))->data());
	doc.InsertFirstChild(root);
	for (Panel* p : this->panels) {
		//panel
		XMLElement* panel = doc.NewElement("panel");
		panel->SetAttribute("name", p->panelName.data());
		panel->SetAttribute("height", std::to_string(p->getHeight()).data());
		panel->SetAttribute("width", std::to_string(p->getWidth()).data());
		panel->SetAttribute("stripes", p->getStripesString().data());
		panel->SetAttribute("thickness", std::to_string(p->getThickness()).data());
		panel->SetAttribute("stripThickness", std::to_string(p->getStripThickness()).data());
		panel->SetAttribute("direction", std::to_string(p->direction).data());

		//transform information//
		XMLElement* tr = doc.NewElement("tr");
		glm::mat4 transformMatrix = this->getTransformMatrix(p);
		std::string values = "";
		const float *pSource= (const float*)glm::value_ptr(transformMatrix);
		//for some reason glm developers decided to take values on collumn based like below//
		//we need values row based
		/*0 4 8  12
		* 1 5 9  13
		  2 6 10 14
		  3 7 11 15
		*/
		//that's why we take values on different order//
		for(int c=0;c<4;++c)
			for (int r = 0; r < 4; ++r)
			{
				values += std::to_string(pSource[r*4+c]) + "*";
			}
		tr->SetAttribute("values", values.data());
		panel->InsertEndChild(tr);
		root->InsertEndChild(panel);
	}
	// Declare a printer    
	XMLPrinter printer;

	// attach it to the document you want to convert in to a std::string 
	doc.Accept(&printer);

	// Create a std::string and copy your document data in to the string    
	return printer.CStr();
}

glm::mat4 Assembly::getTransformMatrix(Panel * panel){
	if (panel->join == NULL)
		return glm::mat4();
	else
		return getTransformMatrix(panel->join->joinedPanel)*panel->getTransformMatrix();
}

Assembly::Assembly() {
}

Assembly::~Assembly() {
	for (Panel* p : panels) {
		delete p;
	}
}
