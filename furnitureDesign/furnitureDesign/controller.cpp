#include "stdafx.h"
#include "controller.h"
using namespace furniture;

std::vector<std::string> controller::getAssyNames(int userId)
{
	return databaseInterFace::getUserAssemblyNames(userId);
}

std::string controller::getAssyXML(int userId, std::string assyName)
{
	std::string result = "";
	int id=databaseInterFace::getAssemblyId(userId, assyName);
	if (id != 0){
		Assembly* assy=	databaseInterFace::getAssembly(id);
		result=assy->getXML();
	}
	return result;
}

bool controller::AddAssy(int userId, std::string assyName)
{
	std::vector<std::string> names=databaseInterFace::getUserAssemblyNames(userId);
	for (int i = 0; i < names.size(); ++i)
		if (names[i] == assyName)
			return false;
	databaseInterFace::addNewAssyToUser(userId, assyName);
	return true;
}

bool controller::updatePart(int userId, std::string assyName, std::string partName, double width, double height, double thickness, double stripThickness, int material, std::string stripes, int direction)
{
	int assyid= databaseInterFace::getAssemblyId(userId, assyName);
	if (assyid == 0)
		return false;
	int partid = databaseInterFace::getPartId(assyid, partName);
	if (partid == 0)
		return false;
	Panel* p=new Panel();
	p->panelName = partName;
	p->setWidth( width);
	p->setHeight(height);
	p->setThickness(thickness);
	p->setStripesFromString(stripes);
	p->direction = direction;
	p->setMaterial((Materials)material);
	p->setStripThickness(stripThickness);
	databaseInterFace::updatePanel(partid,p);
	delete p;
	return true;
}

bool controller::deletePart(int userId, std::string assyName, std::string partName)
{
	int assyid = databaseInterFace::getAssemblyId(userId, assyName);
	if (assyid == 0)
		return false;
	int partid = databaseInterFace::getPartId(assyid, partName);
	if (partid == 0)
		return false;
	databaseInterFace::deletePanelFromAssembly(assyid, partid);
	databaseInterFace::deletePanel(partid);
	return true;
}

bool controller::addPart(int userId, std::string assyName, std::string partName)
{
	int assyid = databaseInterFace::getAssemblyId(userId, assyName);
	if (assyid == 0)
		return false;
	int partid = databaseInterFace::getPartId(assyid, partName);
	if (partid == 0)
	{
		Panel* p=new Panel();
		p->panelName = partName;
		int panelid=databaseInterFace::insertPanel(p);
		delete p;
		databaseInterFace::addPanelToAssembly(assyid, panelid);
		return true;
	}
	else
		return false;
}

bool controller::addJoin(int userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string joiningFaces, std::string joinedFaces, std::string distances)
{
	int assyid=databaseInterFace::getAssemblyId(userId, assyName);
	if (assyid == 0)
		return false;
	int joiningid=databaseInterFace::getPartId(assyid, joiningPart);
	if (joiningid == 0)
		return false;
	int joinedid = databaseInterFace::getPartId(assyid, joinedPart);
	if (joinedid == 0)
		return false;
	if (joiningid != joinedid){
		int* joiningFacesarr = new int[3];
		int* joinedFacesarr = new int[3];
		float* distancesarr = new float[3];
		databaseInterFace::getIntArrayFromString(joiningFaces, joiningFacesarr);
		databaseInterFace::getIntArrayFromString(joinedFaces, joinedFacesarr);
		databaseInterFace::getFloatArrayFromString(distances, distancesarr);
		databaseInterFace::addJoin(joiningid, joinedid, joiningFacesarr, joinedFacesarr, distancesarr);
		delete[] joinedFacesarr;
		delete[] joiningFacesarr;
		delete[] distancesarr;
		return true;
	}
}

void controller::deleteAssy(int userId, std::string assyName)
{
	int id = databaseInterFace::getAssemblyId(userId, assyName);
	if (id != 0) {
		databaseInterFace::deleteAssembly(id);
	}
}
