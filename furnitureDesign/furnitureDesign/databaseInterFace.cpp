#include "stdafx.h"
#include "databaseInterFace.h"
#include <boost/functional/hash.hpp>

using namespace furniture;
const char databaseInterFace::host[] = "localhost";
const char databaseInterFace::user[] = "root";
const char databaseInterFace::password[] = "123456";
const char databaseInterFace::db[] = "furnituredesign";

int databaseInterFace::insertPanel(Panel * panel)
{
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("INSERT INTO panels(width, panelName, height, thickness, stripthickness, material, stripes, direction) VALUES (?, ?,?, ? ,?, ?, ?, ?);");
		//set variable
		prep_stmt->setDouble(1, panel->getWidth());
		prep_stmt->setString(2, panel->panelName);
		prep_stmt->setDouble(3, panel->getHeight());
		prep_stmt->setDouble(4, panel->getThickness());
		prep_stmt->setDouble(5, panel->getStripThickness());
		prep_stmt->setInt(6, panel->getMaterial());
		prep_stmt->setString(7, panel->getStripesString());
		prep_stmt->setInt(8, panel->direction);
		//insert//
		prep_stmt->execute();
		con->commit();

		//get id of inserted panel
		sql::Statement *stmt;
		stmt = con->createStatement();
		res= stmt->executeQuery("Select last_insert_id() as id");
		res->next();
		int id =  res->getInt64("id");
		delete res;
		delete stmt;
		delete prep_stmt;
		delete con;
		return id;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
			//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
std::string databaseInterFace::getArrayString(int * arr, int lenght)
{
	std::string result = "";
	for (int i = 0; i < lenght; ++i)
	{
		if ((i + 1) == lenght)
			result += std::to_string(arr[i]);
		else
			result += std::to_string(arr[i]) + "-";
	}
	return result;
}
std::string databaseInterFace::getArrayString(float * arr, int lenght)
{
	std::string result = "";
	for (int i = 0; i < lenght; ++i)
	{
		if ((i + 1) == lenght)
			result += std::to_string(arr[i]);
		else
			result += std::to_string(arr[i]) + "-";
	}
	return result;
}
void databaseInterFace::addNewAssyToUser(int userId, std::string assyName)
{
	Assembly* assy = new Assembly();
	assy->assyName = assyName;
	int id=databaseInterFace::insertAssembly(assy);
	delete assy;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("INSERT INTO UserAssemblies(userId, Assemblies_idAssembly) VALUES (?, ?);");
		//set variable
		prep_stmt->setInt(1, userId);
		prep_stmt->setInt(2, id);
		//insert//
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
void databaseInterFace::getIntArrayFromString(std::string string, int* arr){
	std::string delimiter = "-";
	string += "-";
	size_t pos = 0;
	std::string token;
	int i = 0;
	while ((pos = string.find(delimiter)) != std::string::npos && i != 3) {
		token = string.substr(0, pos);
		string.erase(0, pos + delimiter.length());
		arr[i] = std::stoi(token);
		++i;
	}
}
void databaseInterFace::getFloatArrayFromString(std::string string, float* arr){
	std::string delimiter = "-";
	string += "-";
	size_t pos = 0;
	std::string token;
	int i = 0;
	while ((pos = string.find(delimiter)) != std::string::npos && i != 3) {
		token = string.substr(0, pos);
		string.erase(0, pos + delimiter.length());
		arr[i] = std::stof(token);
		++i;
	}
}
int databaseInterFace::insertAssembly(Assembly * assy){
	try {
		//add panels of the assy
		std::vector<int> panelIds;
		for (Panel* p : assy->panels){
			int panelId = databaseInterFace::insertPanel(p);
			p->setId(panelId);
			panelIds.push_back(panelId);
		}

		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("INSERT INTO assemblies (AssemblyName) values(?)");
		prep_stmt->setString(1, assy->assyName);
		prep_stmt->execute();
		con->commit();

		//get id of inserted assy
		sql::Statement *stmt;
		stmt = con->createStatement();
		res = stmt->executeQuery("Select last_insert_id() as id");
		res->next();
		int Assyid = res->getInt64("id");
		//add panels to assembly//
		for (int panelId : panelIds)
			databaseInterFace::addPanelToAssembly(Assyid,panelId);

		//add joins//
		for (Panel* p : assy->panels){
			if (p->join != NULL)
				databaseInterFace::addJoin(p->getId(),p->join->joinedPanel->getId(), p->getJoiningFaces(),p->getJoinedFaces(), p->getDistances());
		}

		delete res;
		delete stmt;
		delete prep_stmt;
		delete con;
		return Assyid;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return 0;
}
int databaseInterFace::getAssemblyId(int userId, std::string assyName)
{
	int id = 0;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("SELECT idAssembly as id FROM userassemblies as useassy, assemblies as assies where assies.idAssembly = useassy.Assemblies_idAssembly and useassy.userId = ? and assies.AssemblyName=?");
		//set variable
		prep_stmt->setInt(1, userId);
		prep_stmt->setString(2, assyName);


		//insert//
		res = prep_stmt->executeQuery();

		if (res->next())
			id=res->getInt("id");

		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return id;
}
int databaseInterFace::getPartId(int assyId, std::string partName)
{
	int id = 0;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("SELECT idPanel as id FROM assemblypanels as assyPanels, panels  where assyPanels.Panel_idPanel = panels.idPanel and panels.panelName = ? and assyPanels.Assembly_idAssembly=?;");
		//set variable
		prep_stmt->setString(1, partName);
		prep_stmt->setInt(2, assyId);


		//insert//
		res = prep_stmt->executeQuery();

		if (res->next())
			id = res->getInt("id");

		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return id;
}
std::vector<std::string> databaseInterFace::getUserAssemblyNames(int userId)
{
	std::vector<std::string> names;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("SELECT AssemblyName as name FROM userassemblies as useassy, assemblies as assies where assies.idAssembly = useassy.Assemblies_idAssembly and useassy.userId = ?");
		//set variable
		prep_stmt->setInt(1, userId);


		//insert//
		res = prep_stmt->executeQuery();

		while(res->next())
			names.push_back(res->getString("name"));
		
		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return names;
}
void databaseInterFace::deleteAssembly(int assyId) {
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("DELETE FROM USerAssemblies WHERE Assemblies_idAssembly=?");
		//set variable
		prep_stmt->setInt(1, assyId);
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
std::string databaseInterFace::getAssyName(int assyId)
{
	std::string result;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("Select * from assemblies where idAssembly=?;");
		//set variable
		prep_stmt->setInt(1, assyId);


		//insert//
		res = prep_stmt->executeQuery();

		if (res->next())
			result = res->getString("AssemblyName");
		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return result;
}
Assembly* databaseInterFace::getAssembly(int assyId){

	Assembly* assy = NULL;

	if (databaseInterFace::isAssyExist(assyId))
	{

		assy = new Assembly();
		assy->id = assyId;
		assy->assyName = databaseInterFace::getAssyName(assyId);
		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::PreparedStatement  *prep_stmt;
			sql::ResultSet *res;
			/* Create a connection */
			driver = get_driver_instance();
			con = driver->connect(host, user, password);
			/* Connect to the MySQL test database */
			con->setSchema(db);
			//get panelIds
			prep_stmt = con->prepareStatement("Select * from AssemblyPanels where Assembly_idAssembly=?;");
			prep_stmt->setInt(1, assyId);

			res = prep_stmt->executeQuery();
			std::vector<int> panelIds;
			while (res->next()) 
				panelIds.push_back(res->getInt("Panel_idPanel"));
			//
			//get panels and add to assy//
			for (int i : panelIds)
				assy->AddPanel(databaseInterFace::getPanel(i));
			//
			//get Joins//
			for (int i : panelIds){
				int* joiningFaces = new int[3];
				int* joinedFaces = new int[3];
				float* distances = new float[3];
				int joinedPanel = 0;
				if (databaseInterFace::getJoinProperties(i, joinedPanel, joiningFaces, joinedFaces, distances)){
					Panel* joiningPanelp = NULL;
					Panel* joinedPanelp = NULL;
					for (Panel* p : assy->panels){
						if (p->getId() == i)
							joiningPanelp = p;
						else if (p->getId() == joinedPanel)
							joinedPanelp = p;
					}
					if (joiningPanelp != NULL && joinedPanelp != NULL)
						joiningPanelp->setJoin(joinedPanelp,joiningFaces,joinedFaces,distances);
				}
				else {
					delete joiningFaces;
					delete joinedFaces;
					delete distances;
				}

			}
			delete prep_stmt;
			delete con;
			delete res;
		}
		catch (sql::SQLException &e) {
			std::cout << "# ERR: SQLException in " << __FILE__;
			//std::cout << "(" << __FUNCTION__ << ") on line " »
			//<< __LINE__ << endl;
			std::cout << "# ERR: " << e.what();
			std::cout << " (MySQL error code: " << e.getErrorCode();
			std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		}
	}
	return assy;
}
void databaseInterFace::addPanelToAssembly(int assyId, int panelId){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("INSERT INTO AssemblyPanels(Assembly_idAssembly, Panel_idPanel) VALUES (?, ?);");
		//set variable
		prep_stmt->setInt(1, assyId);
		prep_stmt->setInt(2, panelId);
		//insert//
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
void databaseInterFace::deletePanelFromAssembly(int assyId, int panelId){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("DELETE FROM AssemblyPanels WHERE Assembly_idAssembly=? and Panel_idPanel=?");
		//set variable
		prep_stmt->setInt(1,assyId);
		prep_stmt->setInt(2, panelId);

		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
void databaseInterFace::updatePanel(int panelId, Panel * panel){
	try {	
		panel->setId(panelId);
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("UPDATE panels SET `width`=?, `height`=?, `thickness`=?, `stripThickness`=?, `material`=?, `stripes`=?, `direction`=? WHERE `idPanel`=?;");
		
		//set variable
		prep_stmt->setDouble(1, panel->getWidth());
		prep_stmt->setDouble(2, panel->getHeight());
		prep_stmt->setDouble(3, panel->getThickness());
		prep_stmt->setDouble(4, panel->getStripThickness());
		prep_stmt->setInt(5, panel->getMaterial());
		prep_stmt->setString(6, panel->getStripesString());
		prep_stmt->setInt(7, panel->direction);
		prep_stmt->setInt(8, panel->getId());

		//insert//
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
Panel * databaseInterFace::getPanel(int panelId) {
	Panel* p = NULL;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("Select * from panels where idPanel=?;");
		//set variable
		prep_stmt->setInt(1, panelId);

		//insert//
		res=prep_stmt->executeQuery();

		if (res->next()){
			p = new Panel();
			p->setId(res->getInt("idPanel"));
			p->panelName = res->getString("panelName");
			p->setWidth(res->getDouble("width"));
			p->setHeight(res->getDouble("height"));
			p->setMaterial((Materials)res->getInt("material"));
			p->setThickness(res->getDouble("thickness"));
			p->setStripThickness(res->getDouble("stripThickness"));
			p->setStripesFromString(res->getString("stripes"));
			p->direction = res->getInt("direction");
		}
		
		con->commit();

		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return p;
}
void databaseInterFace::deletePanel(int panelId){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("DELETE FROM Panels WHERE idPanel=?");
		//set variable
		prep_stmt->setInt(1, panelId);
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
void databaseInterFace::addJoin(int joiningPanelId, int joinedPanelId, int * joiningFaces, int * joinedFaces, float * distances){
	try {
		//databaseInterFace::deleteJoin(joiningPanelId, joinedPanelId);
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("INSERT INTO Joins(joiningPanel, joinedPanel, joiningFaces, joinedFaces, distances) VALUES (?, ?, ? ,?, ?);");
		//set variable
		prep_stmt->setInt(1, joiningPanelId);
		prep_stmt->setInt(2, joinedPanelId);
		prep_stmt->setString(3, databaseInterFace::getArrayString(joiningFaces,3) );
		prep_stmt->setString(4, databaseInterFace::getArrayString(joinedFaces,3) );
		prep_stmt->setString(5, databaseInterFace::getArrayString(distances,3));
		//insert//
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
/*Cagri*/
void databaseInterFace::updateJoin(int joiningPanelId, int joinedPanelId, int * joiningFaces, int * joinedFaces, float * distances)
{
	try
	{
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("UPDATE Joins set(`joiningPanel`=?, `joinedPanel`=?, `joiningFaces`=?, `joinedFaces`=?, `distances`=?;");
		prep_stmt->setInt(1, joiningPanelId);
		prep_stmt->setInt(2, joinedPanelId);
		prep_stmt->setString(3, databaseInterFace::getArrayString(joiningFaces, 3));
		prep_stmt->setString(4, databaseInterFace::getArrayString(joinedFaces, 3));
		prep_stmt->setString(5, databaseInterFace::getArrayString(distances, 3));
		//insert//
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;


	}
	catch (sql::SQLException &e)
	{
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
/*Cagri - END*/
void databaseInterFace::deleteJoin(int joiningPanelId, int joinedPanelId){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;

		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("DELETE FROM Joins WHERE joiningPanel=? AND  joinedPanel=?");
		//set variable
		prep_stmt->setInt(0, joiningPanelId);
		prep_stmt->setInt(1, joinedPanelId);
		prep_stmt->execute();
		con->commit();

		delete prep_stmt;
		delete con;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}
bool databaseInterFace::isAssyExist(int assyId)
{
	bool result = false;
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("Select * from assemblies where idAssembly=?;");
		//set variable
		prep_stmt->setInt(1, assyId);


		//insert//
		res = prep_stmt->executeQuery();

		if (res->next())
			result = true;

		delete prep_stmt;
		delete con;
		delete res;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
	return result;
}
bool databaseInterFace::getJoinProperties(int joiningPanel, int& joinedPanel, int * joiningFaces, int * joinedFaces, float * distances){
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::PreparedStatement  *prep_stmt;
		sql::ResultSet *res;
		/* Create a connection */
		driver = get_driver_instance();
		con = driver->connect(host, user, password);
		/* Connect to the MySQL test database */
		con->setSchema(db);
		prep_stmt = con->prepareStatement("Select* from joins where joiningPanel=?;");
		//set variable
		//set variable
		prep_stmt->setInt(1, joiningPanel);

		//insert//
		res = prep_stmt->executeQuery();
		bool result = false;
		if (res->next()) {
			result = true;
			joinedPanel = res->getInt("joinedPanel");
			std::string joiningFacesS = res->getString("joiningFaces");
			std::string joinedFacesS = res->getString("joinedFaces");
			std::string distancesS = res->getString("distances");
			databaseInterFace::getIntArrayFromString(joiningFacesS, joiningFaces);
			databaseInterFace::getIntArrayFromString(joinedFacesS, joinedFaces);
			databaseInterFace::getFloatArrayFromString(distancesS, distances);
		}

		con->commit();

		delete prep_stmt;
		delete con;
		delete res;
		return result;
	}
	catch (sql::SQLException &e) {
		std::cout << "# ERR: SQLException in " << __FILE__;
		//std::cout << "(" << __FUNCTION__ << ") on line " »
		//<< __LINE__ << endl;
		std::cout << "# ERR: " << e.what();
		std::cout << " (MySQL error code: " << e.getErrorCode();
		std::cout << ", SQLState: " << e.getSQLState() << " )" << std::endl;
	}
}


