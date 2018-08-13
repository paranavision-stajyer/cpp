#include "stdafx.h"
#include "cppcmsService.h"

cppcmsService::~cppcmsService()
{

}

void cppcmsService::main()
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	response().out() <<
		"<html>\n"
		"<body>\n"
		"  <h1>Hello World</h1>\n"
		"</body>\n"
		"</html>\n";
}

void cppcmsService::foo(std::string x, std::string y)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	response().out() << "x:"<<x<<"y:"<<y;
}

void cppcmsService::assyXML(std::string userId, std::string assyName)
{	
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}

//return assyNames//
void cppcmsService::getAssyNames(std::string userId)
{
	std::cout << userId << std::endl;
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	std::vector<std::string> assyNames=furniture::controller::getAssyNames(std::stoi(userId));
	for (int i = 0; i < assyNames.size(); ++i){
		std::cout << assyNames.at(i) << std::endl;
		if (i != assyNames.size() - 1)
			response().out() << assyNames.at(i) << "@";
		else
			response().out() << assyNames.at(i);
	}

}

//return assyNames//
void cppcmsService::deleteAssy(std::string userId, std::string assyName)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	furniture::controller::deleteAssy(std::stoi(userId), assyName);
	
	std::vector<std::string> assyNames = furniture::controller::getAssyNames(std::stoi(userId));
	for (int i = 0; i < assyNames.size(); ++i) {
		if (i != assyNames.size() - 1)
			response().out() << assyNames.at(i) << "@";
		else
			response().out() << assyNames.at(i);
	}
}

//return assyNames//
void cppcmsService::addAssy(std::string userId, std::string assyName)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	furniture::controller::AddAssy(std::stoi(userId), assyName);
	std::vector<std::string> assyNames = furniture::controller::getAssyNames(std::stoi(userId));
	for (int i = 0; i < assyNames.size(); ++i) {
		if (i != assyNames.size() - 1)
			response().out() << assyNames.at(i) << "@";
		else
			response().out() << assyNames.at(i);
	}

}

//return assyXML//
void cppcmsService::updatePart(std::string userId, std::string assyName, std::string partName, std::string properties)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	std::vector<std::string> partProperties;
	std::string delimiter = "@";
	properties += "@";
	size_t pos = 0;
	std::string token;
	while ((pos = properties.find(delimiter)) != std::string::npos) {
		token = properties.substr(0, pos);
		properties.erase(0, pos + delimiter.length());
		partProperties.push_back(token);
	}
	
	furniture::controller::updatePart(std::stoi(userId), assyName, partName,std::stod(partProperties.at(0)), std::stod(partProperties.at(1)), std::stod(partProperties.at(2)), std::stod(partProperties.at(4)), std::stod(partProperties.at(5)), partProperties.at(3), std::stoi(partProperties.at(6)));
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}

//return assyXML//
void cppcmsService::deletePart(std::string userId, std::string assyName, std::string partName)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	furniture::controller::deletePart(std::stoi(userId), assyName, partName);
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}

//return assyXML//
void cppcmsService::addPart(std::string userId, std::string assyName, std::string partName)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	furniture::controller::addPart(std::stoi(userId), assyName, partName);
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}

//returnassyXML//
void cppcmsService::addJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string properties)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	
	std::vector<std::string> joinProperties;
	std::string delimiter = "@";
	properties += "@";
	size_t pos = 0;
	std::string token;
	while ((pos = properties.find(delimiter)) != std::string::npos) {
		token = properties.substr(0, pos);
		properties.erase(0, pos + delimiter.length());
		joinProperties.push_back(token);
	}
	furniture::controller::addJoin(std::stoi(userId), assyName, joiningPart, joinedPart, joinProperties[0], joinProperties[1], joinProperties[2]);
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}


void cppcmsService::deleteJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
		
	furniture::controller::deleteJoin(std::stoi(userId), assyName, joiningPart, joinedPart);
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());
}

/*Cagri*/
void cppcmsService::updateJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string properties)
{
	response().set_header("Access-Control-Allow-Origin", "*");
	response().set_header("Access-Control-Allow-Headers", "Content-Type");
	
	std::vector<std::string> joinProperties;
	std::string delimiter = "@";
	properties += "@";
	size_t pos = 0;
	std::string token;
	while ((pos = properties.find(delimiter)) != std::string::npos) {
		token = properties.substr(0, pos);
		properties.erase(0, pos + delimiter.length());
		joinProperties.push_back(token);
	}
	furniture::controller::updateJoin(std::stoi(userId), assyName, joiningPart, joinedPart, joinProperties[0], joinProperties[1], joinProperties[2]);
	response().out() << furniture::controller::getAssyXML(std::stoi(userId), assyName.data());



}



/*Cagri END*/