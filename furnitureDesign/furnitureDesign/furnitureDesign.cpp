// furnitureDesign.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "cppcmsService.h"
using namespace furniture;


int main(int argc, char ** argv)
{

	try {

		cppcms::service srv(argc, argv);
		srv.applications_pool().mount(
			cppcms::applications_factory<cppcmsService>()
		);
		srv.run();
	}
	catch (std::exception const &e) {
		std::cerr << e.what() << std::endl;
	}
}

