#pragma once
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
namespace furniture
{
	static class databaseInterFace
	{
	private:
		static const char host[];
		static const char user[];
		static const char password[];
		static const char db[];
		static std::string getArrayString(int* arr, int lenght);
		static std::string getArrayString(float* arr, int lenght);
	public:

		/*static bool insertUser(std::string userName, std::string userPassword, std::string name, std::string surName, std::string mailAddress, std::string address, std::string phone);*/
		/*static bool loginValid(std::string userName, std::string userPassword);*/
		static void addNewAssyToUser(int userId, std::string assyName);
		static std::string getAssyName(int assyId);
		static void getIntArrayFromString(std::string string, int * arr);
		static void getFloatArrayFromString(std::string string, float * arr);
		static int insertAssembly(Assembly* assy);
		static int getAssemblyId(int userId, std::string assyName);
		static int getPartId(int assyId, std::string partName);
		static std::vector<std::string> getUserAssemblyNames(int userId);
		//static void updateAssembly(int assyId, Assembly* newAssy);
		static void deleteAssembly(int assyId);
		static Assembly* getAssembly(int assyId);
		static void addPanelToAssembly(int assyId, int panelId);
		static void deletePanelFromAssembly(int assyId, int panelId);
		static int insertPanel(Panel* panel);
		static void updatePanel(int panelId, Panel* panel);
		static Panel* getPanel(int panelId);
		static void deletePanel(int panelId);
		static void addJoin(int joiningPanelId, int joinedPanelId, int* joiningFaces, int* joinedFaces, float* distances);
		static void deleteJoin(int joiningPanelId, int joinedPanelId);
		static bool isAssyExist(int assyId);
		static bool getJoinProperties(int joiningPanel, int& joinedPanel, int* joiningFaces, int* joinedFaces, float* distances);
	};
}