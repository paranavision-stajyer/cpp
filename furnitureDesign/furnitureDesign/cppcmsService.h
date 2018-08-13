#pragma once
#include <cppcms/application.h>
#include <cppcms/service.h>
#include <cppcms/http_response.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <cppcms/applications_pool.h>
#include <cppcms/rpc_json.h>
#include <iostream>
#include <stdlib.h>

class cppcmsService : public cppcms::application
{
public:
	 cppcmsService(cppcms::service &srv):
		cppcms::application(srv) {

		dispatcher().assign("/foo/(\\d+)/(\\d+)", &cppcmsService::foo, this, 1,2);
		mapper().assign("foo", "/foo/{1}/{2}");

		dispatcher().assign("/assyXML/(\\d+)/(.*)", &cppcmsService::assyXML, this, 1,2);
		mapper().assign("assyXML", "/assyXML/{1}/{2}");

		dispatcher().assign("/getAssyNames/(\\d+)", &cppcmsService::getAssyNames, this, 1);
		mapper().assign("getAssyNames", "/getAssyNames/{1}");

		dispatcher().assign("/deleteAssy/(\\d+)/(.*)", &cppcmsService::deleteAssy, this, 1,2);
		mapper().assign("deleteAssy", "/deleteAssy/{1}/{2}");

		dispatcher().assign("/addAssy/(\\d+)/(.*)", &cppcmsService::addAssy, this, 1, 2);
		mapper().assign("addAssy", "/addAssy/{1}/{2}");

		dispatcher().assign("/updatePart/(\\d+)/(.*)/(.*)/(.*)", &cppcmsService::updatePart, this, 1, 2,3,4);
		mapper().assign("updatePart", "/updatePart/{1}/{2}/{3}/{4}");
			
		dispatcher().assign("/deletePart/(\\d+)/(.*)/(.*)", &cppcmsService::deletePart, this, 1, 2, 3);
		mapper().assign("deletePart", "/deletePart/{1}/{2}/{3}");

		dispatcher().assign("/addPart/(\\d+)/(.*)/(.*)", &cppcmsService::addPart, this, 1, 2, 3);
		mapper().assign("addPart", "/addPart/{1}/{2}/{3}");

		dispatcher().assign("/addJoin/(\\d+)/(.*)/(.*)/(.*)/(.*)", &cppcmsService::addJoin, this, 1, 2, 3, 4, 5);
		mapper().assign("addJoin", "/addJoin/{1}/{2}/{3}/{4}/{5}");

		dispatcher().assign("/deleteJoin/(\\d+)/(.*)/(.*)/(.*)", &cppcmsService::deleteJoin, this, 1, 2, 3, 4);
		mapper().assign("deleteJoin", "/deleteJoin/{1}/{2}/{3}/{4}");

		dispatcher().assign("/updateJoin/(\\d+)/(.*)/(.*)/(.*)/(.*)", &cppcmsService::updateJoin, this, 1, 2, 3, 4, 5);
		mapper().assign("updateJoin", "/addJoin/{1}/{2}/{3}/{4}/{5}");

		dispatcher().assign("", &cppcmsService::main, this);
		mapper().assign( "" );

		mapper().root( "/cppcmsService" );
	};
	~cppcmsService();

	void main();

	void foo(std::string x, std::string y);
	//virtual void main(std::string url);
	void assyXML(std::string userId, std::string assyName);

	void getAssyNames(std::string userId);

	void deleteAssy(std::string userId, std::string assyName);

	void addAssy(std::string userId, std::string assyName);

	void updatePart(std::string userId, std::string assyName,std::string partName, std::string properties);

	void deletePart(std::string userId, std::string assyName, std::string partName);

	void addPart(std::string userId, std::string assyName, std::string partName);

	void addJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string properties);

	void deleteJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart);
	
	void updateJoin(std::string userId, std::string assyName, std::string joiningPart, std::string joinedPart, std::string properties);
};

