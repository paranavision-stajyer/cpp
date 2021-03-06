// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef furnitureDesignDLL_EXPORTS
#define furnitureDesign_API __declspec(dllexport) 
#else
#define furnitureDesign_API __declspec(dllimport) 
#endif
#include <stdio.h>
#include "targetver.h"
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <tchar.h>
#include <string>

#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <MySQL Server 5.5/include/mysql.h>




#include "Assembly.h"
#include "databaseInterFace.h"
#include "Panel.h"
#include "controller.h"
// TODO: reference additional headers your program requires here
