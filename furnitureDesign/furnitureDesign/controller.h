#pragma once
namespace furniture
{
	static class controller
	{
		//Use Cases 
	public:
		static std::vector<std::string> getAssyNames(int userId);
		static std::string getAssyXML(int userId, std::string assyName);
		static void deleteAssy(int userId, std::string assyName);
		static bool AddAssy(int userId, std::string assyName);
		static bool updatePart(int userId, std::string assyName, std::string partName, double width, double height, double thickness, double stripThickness, int material, std::string stripes, int direction);
		static bool deletePart(int userId, std::string assyName, std::string partName);
		static bool addPart(int userId, std::string assyName, std::string partName);
		static bool addJoin(int userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string joiningFaces, std::string joinedFaces, std::string distances);
	};
}