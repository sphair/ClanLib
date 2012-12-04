#pragma once

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

// Choose the target renderer
#define USE_OPENGL_2
//define USE_OPENGL_1
//#define USE_D3D

#ifdef USE_D3D
#include <ClanLib/d3d.h>
#endif

#ifdef USE_OPENGL_1
#include <ClanLib/gl1.h>
#endif

#ifdef USE_OPENGL_2
#include <ClanLib/gl.h>
#endif
