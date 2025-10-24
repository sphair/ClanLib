/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

/// \addtogroup clanGL1_Display clanGL1 Display
/// \{

#pragma once
#include "api_gl1.h"

#include "../Core/System/cl_platform.h"

#ifdef __APPLE__
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#else
#include <GL/gl.h>
#endif

#ifndef CL_DISPLAYDEFINES_DEFINES
#define CL_DISPLAYDEFINES_DEFINES

#include <cstddef>

#ifndef __APPLE__
// Win32 and linux
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#else
typedef unsigned long GLenum;
typedef unsigned char GLboolean;
typedef unsigned long GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef long GLint;
typedef long GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif

/// \brief Display Defines
///
/// \xmlonly !group=GL1/Display! !header=gl1.h! \endxmlonly
enum CL_DisplayDefines
{
	// AccumOp:
#ifndef GL_ACCUM
	GL_ACCUM                          = 0x0100,
	GL_LOAD                           = 0x0101,
	GL_RETURN                         = 0x0102,
	GL_MULT                           = 0x0103,
	GL_ADD                            = 0x0104,
#endif
	// AlphaFunction:

#ifndef GL_NEVER
	GL_NEVER                          = 0x0200,
	GL_LESS                           = 0x0201,
	GL_EQUAL                          = 0x0202,
	GL_LEQUAL                         = 0x0203,
	GL_GREATER                        = 0x0204,
	GL_NOTEQUAL                       = 0x0205,
	GL_GEQUAL                         = 0x0206,
	GL_ALWAYS                         = 0x0207,
#endif
	// AttribMask:

#ifndef GL_CURRENT_BIT
	GL_CURRENT_BIT                    = 0x00000001,
	GL_POINT_BIT                      = 0x00000002,
	GL_LINE_BIT                       = 0x00000004,
	GL_POLYGON_BIT                    = 0x00000008,
	GL_POLYGON_STIPPLE_BIT            = 0x00000010,
	GL_PIXEL_MODE_BIT                 = 0x00000020,
	GL_LIGHTING_BIT                   = 0x00000040,
	GL_FOG_BIT                        = 0x00000080,
	GL_DEPTH_BUFFER_BIT               = 0x00000100,
	GL_ACCUM_BUFFER_BIT               = 0x00000200,
	GL_STENCIL_BUFFER_BIT             = 0x00000400,
	GL_VIEWPORT_BIT                   = 0x00000800,
	GL_TRANSFORM_BIT                  = 0x00001000,
	GL_ENABLE_BIT                     = 0x00002000,
	GL_COLOR_BUFFER_BIT               = 0x00004000,
	GL_HINT_BIT                       = 0x00008000,
	GL_EVAL_BIT                       = 0x00010000,
	GL_LIST_BIT                       = 0x00020000,
	GL_TEXTURE_BIT                    = 0x00040000,
	GL_SCISSOR_BIT                    = 0x00080000,
	GL_ALL_ATTRIB_BITS                = 0x000fffff,
#endif
	// BeginMode:
#ifndef GL_POINTS
	GL_POINTS                         = 0x0000,
	GL_LINES                          = 0x0001,
	GL_LINE_LOOP                      = 0x0002,
	GL_LINE_STRIP                     = 0x0003,
	GL_TRIANGLES                      = 0x0004,
	GL_TRIANGLE_STRIP                 = 0x0005,
	GL_TRIANGLE_FAN                   = 0x0006,
	GL_QUADS                          = 0x0007,
	GL_QUAD_STRIP                     = 0x0008,
	GL_POLYGON                        = 0x0009,
#endif
	// BlendEquationMode:

#ifndef GL_LOGIC_OP
	GL_LOGIC_OP                       = 0x0BF1,
#endif
	// BlendingFactorDest:

#ifndef GL_ZERO
	GL_ZERO                           = 0,
	GL_ONE                            = 1,
	GL_SRC_COLOR                      = 0x0300,
	GL_ONE_MINUS_SRC_COLOR            = 0x0301,
	GL_SRC_ALPHA                      = 0x0302,
	GL_ONE_MINUS_SRC_ALPHA            = 0x0303,
	GL_DST_ALPHA                      = 0x0304,
	GL_ONE_MINUS_DST_ALPHA            = 0x0305,
#endif
	// BlendingFactorSrc:

#ifndef GL_DST_COLOR
	GL_DST_COLOR                      = 0x0306,
	GL_ONE_MINUS_DST_COLOR            = 0x0307,
	GL_SRC_ALPHA_SATURATE             = 0x0308,
#endif
	// Boolean:

#ifndef GL_TRUE
	GL_TRUE                           = 1,
	GL_FALSE                          = 0,
#endif
	// ClipPlaneName:

#ifndef GL_CLIP_PLANE0
	GL_CLIP_PLANE0                    = 0x3000,
	GL_CLIP_PLANE1                    = 0x3001,
	GL_CLIP_PLANE2                    = 0x3002,
	GL_CLIP_PLANE3                    = 0x3003,
	GL_CLIP_PLANE4                    = 0x3004,
	GL_CLIP_PLANE5                    = 0x3005,
#endif
	// DataType:

#ifndef GL_BYTE
	GL_BYTE                           = 0x1400,
	GL_UNSIGNED_BYTE                  = 0x1401,
	GL_SHORT                          = 0x1402,
	GL_UNSIGNED_SHORT                 = 0x1403,
	GL_INT                            = 0x1404,
	GL_UNSIGNED_INT                   = 0x1405,
	GL_FLOAT                          = 0x1406,
	GL_2_BYTES                        = 0x1407,
	GL_3_BYTES                        = 0x1408,
	GL_4_BYTES                        = 0x1409,
	GL_DOUBLE                         = 0x140A,
#endif
	// DrawBufferMode:

#ifndef GL_NONE
	GL_NONE                           = 0,
	GL_FRONT_LEFT                     = 0x0400,
	GL_FRONT_RIGHT                    = 0x0401,
	GL_BACK_LEFT                      = 0x0402,
	GL_BACK_RIGHT                     = 0x0403,
	GL_FRONT                          = 0x0404,
	GL_BACK                           = 0x0405,
	GL_LEFT                           = 0x0406,
	GL_RIGHT                          = 0x0407,
	GL_FRONT_AND_BACK                 = 0x0408,
	GL_AUX0                           = 0x0409,
	GL_AUX1                           = 0x040A,
	GL_AUX2                           = 0x040B,
	GL_AUX3                           = 0x040C,
#endif
	// ErrorCode:

#ifndef GL_NO_ERROR
	GL_NO_ERROR                     = 0,
	GL_INVALID_ENUM                 = 0x0500,
	GL_INVALID_VALUE                = 0x0501,
	GL_INVALID_OPERATION            = 0x0502,
	GL_STACK_OVERFLOW               = 0x0503,
	GL_STACK_UNDERFLOW              = 0x0504,
	GL_OUT_OF_MEMORY                = 0x0505,
	// GL_TABLE_TOO_LARGE
#endif

	// FeedBackMode:
#ifndef GL_2D
	GL_2D                           = 0x0600,
	GL_3D                           = 0x0601,
	GL_3D_COLOR                     = 0x0602,
	GL_3D_COLOR_TEXTURE             = 0x0603,
	GL_4D_COLOR_TEXTURE             = 0x0604,
#endif
	// FeedBackToken:

#ifndef GL_PASS_THROUGH_TOKEN
	GL_PASS_THROUGH_TOKEN           = 0x0700,
	GL_POINT_TOKEN                  = 0x0701,
	GL_LINE_TOKEN                   = 0x0702,
	GL_POLYGON_TOKEN                = 0x0703,
	GL_BITMAP_TOKEN                 = 0x0704,
	GL_DRAW_PIXEL_TOKEN             = 0x0705,
	GL_COPY_PIXEL_TOKEN             = 0x0706,
	GL_LINE_RESET_TOKEN             = 0x0707,
#endif
	// FogMode:

	// GL_LINEAR
#ifndef GL_EXP
	GL_EXP                          = 0x0800,
	GL_EXP2                         = 0x0801,
#endif
	// FrontFaceDirection:

#ifndef GL_CW
	GL_CW                           = 0x0900,
	GL_CCW                          = 0x0901,
#endif
	// GetMapTarget:

#ifndef GL_COEFF
	GL_COEFF                        = 0x0A00,
	GL_ORDER                        = 0x0A01,
	GL_DOMAIN                       = 0x0A02,
#endif
	// GetTarget:

#ifndef GL_CURRENT_COLOR
	GL_CURRENT_COLOR                = 0x0B00,
	GL_CURRENT_INDEX                = 0x0B01,
	GL_CURRENT_NORMAL               = 0x0B02,
	GL_CURRENT_TEXTURE_COORDS       = 0x0B03,
	GL_CURRENT_RASTER_COLOR         = 0x0B04,
	GL_CURRENT_RASTER_INDEX         = 0x0B05,
	GL_CURRENT_RASTER_TEXTURE_COORDS= 0x0B06,
	GL_CURRENT_RASTER_POSITION      = 0x0B07,
	GL_CURRENT_RASTER_POSITION_VALID= 0x0B08,
	GL_CURRENT_RASTER_DISTANCE      = 0x0B09,
	GL_POINT_SMOOTH                 = 0x0B10,
	GL_POINT_SIZE                   = 0x0B11,
	GL_POINT_SIZE_RANGE             = 0x0B12,
	GL_POINT_SIZE_GRANULARITY       = 0x0B13,
	GL_LINE_SMOOTH                  = 0x0B20,
	GL_LINE_WIDTH                   = 0x0B21,
	GL_LINE_WIDTH_RANGE             = 0x0B22,
	GL_LINE_WIDTH_GRANULARITY       = 0x0B23,
	GL_LINE_STIPPLE                 = 0x0B24,
	GL_LINE_STIPPLE_PATTERN         = 0x0B25,
	GL_LINE_STIPPLE_REPEAT          = 0x0B26,
	GL_LIST_MODE                    = 0x0B30,
	GL_MAX_LIST_NESTING             = 0x0B31,
	GL_LIST_BASE                    = 0x0B32,
	GL_LIST_INDEX                   = 0x0B33,
	GL_POLYGON_MODE                 = 0x0B40,
	GL_POLYGON_SMOOTH               = 0x0B41,
	GL_POLYGON_STIPPLE              = 0x0B42,
	GL_EDGE_FLAG                    = 0x0B43,
	GL_CULL_FACE                    = 0x0B44,
	GL_CULL_FACE_MODE               = 0x0B45,
	GL_FRONT_FACE                   = 0x0B46,
	GL_LIGHTING                     = 0x0B50,
	GL_LIGHT_MODEL_LOCAL_VIEWER     = 0x0B51,
	GL_LIGHT_MODEL_TWO_SIDE         = 0x0B52,
	GL_LIGHT_MODEL_AMBIENT          = 0x0B53,
	GL_SHADE_MODEL                  = 0x0B54,
	GL_COLOR_MATERIAL_FACE          = 0x0B55,
	GL_COLOR_MATERIAL_PARAMETER     = 0x0B56,
	GL_COLOR_MATERIAL               = 0x0B57,
	GL_FOG                          = 0x0B60,
	GL_FOG_INDEX                    = 0x0B61,
	GL_FOG_DENSITY                  = 0x0B62,
	GL_FOG_START                    = 0x0B63,
	GL_FOG_END                      = 0x0B64,
	GL_FOG_MODE                     = 0x0B65,
	GL_FOG_COLOR                    = 0x0B66,
	GL_DEPTH_RANGE                  = 0x0B70,
	GL_DEPTH_TEST                   = 0x0B71,
	GL_DEPTH_WRITEMASK              = 0x0B72,
	GL_DEPTH_CLEAR_VALUE            = 0x0B73,
	GL_DEPTH_FUNC                   = 0x0B74,
	GL_ACCUM_CLEAR_VALUE            = 0x0B80,
	GL_STENCIL_TEST                 = 0x0B90,
	GL_STENCIL_CLEAR_VALUE          = 0x0B91,
	GL_STENCIL_FUNC                 = 0x0B92,
	GL_STENCIL_VALUE_MASK           = 0x0B93,
	GL_STENCIL_FAIL                 = 0x0B94,
	GL_STENCIL_PASS_DEPTH_FAIL      = 0x0B95,
	GL_STENCIL_PASS_DEPTH_PASS      = 0x0B96,
	GL_STENCIL_REF                  = 0x0B97,
	GL_STENCIL_WRITEMASK            = 0x0B98,
	GL_MATRIX_MODE                  = 0x0BA0,
	GL_NORMALIZE                    = 0x0BA1,
	GL_VIEWPORT                     = 0x0BA2,
	GL_MODELVIEW_STACK_DEPTH        = 0x0BA3,
	GL_PROJECTION_STACK_DEPTH       = 0x0BA4,
	GL_TEXTURE_STACK_DEPTH          = 0x0BA5,
	GL_MODELVIEW_MATRIX             = 0x0BA6,
	GL_PROJECTION_MATRIX            = 0x0BA7,
	GL_TEXTURE_MATRIX               = 0x0BA8,
	GL_ATTRIB_STACK_DEPTH           = 0x0BB0,
	GL_CLIENT_ATTRIB_STACK_DEPTH    = 0x0BB1,
	GL_ALPHA_TEST                   = 0x0BC0,
	GL_ALPHA_TEST_FUNC              = 0x0BC1,
	GL_ALPHA_TEST_REF               = 0x0BC2,
	GL_DITHER                       = 0x0BD0,
	GL_BLEND_DST                    = 0x0BE0,
	GL_BLEND_SRC                    = 0x0BE1,
	GL_BLEND                        = 0x0BE2,
	GL_LOGIC_OP_MODE                = 0x0BF0,
	GL_INDEX_LOGIC_OP               = 0x0BF1,
	GL_COLOR_LOGIC_OP               = 0x0BF2,
	GL_AUX_BUFFERS                  = 0x0C00,
	GL_DRAW_BUFFER                  = 0x0C01,
	GL_READ_BUFFER                  = 0x0C02,
	GL_SCISSOR_BOX                  = 0x0C10,
	GL_SCISSOR_TEST                 = 0x0C11,
	GL_INDEX_CLEAR_VALUE            = 0x0C20,
	GL_INDEX_WRITEMASK              = 0x0C21,
	GL_COLOR_CLEAR_VALUE            = 0x0C22,
	GL_COLOR_WRITEMASK              = 0x0C23,
	GL_INDEX_MODE                   = 0x0C30,
	GL_RGBA_MODE                    = 0x0C31,
	GL_DOUBLEBUFFER                 = 0x0C32,
	GL_STEREO                       = 0x0C33,
	GL_RENDER_MODE                  = 0x0C40,
	GL_PERSPECTIVE_CORRECTION_HINT  = 0x0C50,
	GL_POINT_SMOOTH_HINT            = 0x0C51,
	GL_LINE_SMOOTH_HINT             = 0x0C52,
	GL_POLYGON_SMOOTH_HINT          = 0x0C53,
	GL_FOG_HINT                     = 0x0C54,
	GL_TEXTURE_GEN_S                = 0x0C60,
	GL_TEXTURE_GEN_T                = 0x0C61,
	GL_TEXTURE_GEN_R                = 0x0C62,
	GL_TEXTURE_GEN_Q                = 0x0C63,
	GL_PIXEL_MAP_I_TO_I             = 0x0C70,
	GL_PIXEL_MAP_S_TO_S             = 0x0C71,
	GL_PIXEL_MAP_I_TO_R             = 0x0C72,
	GL_PIXEL_MAP_I_TO_G             = 0x0C73,
	GL_PIXEL_MAP_I_TO_B             = 0x0C74,
	GL_PIXEL_MAP_I_TO_A             = 0x0C75,
	GL_PIXEL_MAP_R_TO_R             = 0x0C76,
	GL_PIXEL_MAP_G_TO_G             = 0x0C77,
	GL_PIXEL_MAP_B_TO_B             = 0x0C78,
	GL_PIXEL_MAP_A_TO_A             = 0x0C79,
	GL_PIXEL_MAP_I_TO_I_SIZE        = 0x0CB0,
	GL_PIXEL_MAP_S_TO_S_SIZE        = 0x0CB1,
	GL_PIXEL_MAP_I_TO_R_SIZE        = 0x0CB2,
	GL_PIXEL_MAP_I_TO_G_SIZE        = 0x0CB3,
	GL_PIXEL_MAP_I_TO_B_SIZE        = 0x0CB4,
	GL_PIXEL_MAP_I_TO_A_SIZE        = 0x0CB5,
	GL_PIXEL_MAP_R_TO_R_SIZE        = 0x0CB6,
	GL_PIXEL_MAP_G_TO_G_SIZE        = 0x0CB7,
	GL_PIXEL_MAP_B_TO_B_SIZE        = 0x0CB8,
	GL_PIXEL_MAP_A_TO_A_SIZE        = 0x0CB9,
	GL_UNPACK_SWAP_BYTES            = 0x0CF0,
	GL_UNPACK_LSB_FIRST             = 0x0CF1,
	GL_UNPACK_ROW_LENGTH            = 0x0CF2,
	GL_UNPACK_SKIP_ROWS             = 0x0CF3,
	GL_UNPACK_SKIP_PIXELS           = 0x0CF4,
	GL_UNPACK_ALIGNMENT             = 0x0CF5,
	GL_PACK_SWAP_BYTES              = 0x0D00,
	GL_PACK_LSB_FIRST               = 0x0D01,
	GL_PACK_ROW_LENGTH              = 0x0D02,
	GL_PACK_SKIP_ROWS               = 0x0D03,
	GL_PACK_SKIP_PIXELS             = 0x0D04,
	GL_PACK_ALIGNMENT               = 0x0D05,
	GL_MAP_COLOR                    = 0x0D10,
	GL_MAP_STENCIL                  = 0x0D11,
	GL_INDEX_SHIFT                  = 0x0D12,
	GL_INDEX_OFFSET                 = 0x0D13,
	GL_RED_SCALE                    = 0x0D14,
	GL_RED_BIAS                     = 0x0D15,
	GL_ZOOM_X                       = 0x0D16,
	GL_ZOOM_Y                       = 0x0D17,
	GL_GREEN_SCALE                  = 0x0D18,
	GL_GREEN_BIAS                   = 0x0D19,
	GL_BLUE_SCALE                   = 0x0D1A,
	GL_BLUE_BIAS                    = 0x0D1B,
	GL_ALPHA_SCALE                  = 0x0D1C,
	GL_ALPHA_BIAS                   = 0x0D1D,
	GL_DEPTH_SCALE                  = 0x0D1E,
	GL_DEPTH_BIAS                   = 0x0D1F,
	GL_MAX_EVAL_ORDER               = 0x0D30,
	GL_MAX_LIGHTS                   = 0x0D31,
	GL_MAX_CLIP_PLANES              = 0x0D32,
	GL_MAX_TEXTURE_SIZE             = 0x0D33,
	GL_MAX_PIXEL_MAP_TABLE          = 0x0D34,
	GL_MAX_ATTRIB_STACK_DEPTH       = 0x0D35,
	GL_MAX_MODELVIEW_STACK_DEPTH    = 0x0D36,
	GL_MAX_NAME_STACK_DEPTH         = 0x0D37,
	GL_MAX_PROJECTION_STACK_DEPTH   = 0x0D38,
	GL_MAX_TEXTURE_STACK_DEPTH      = 0x0D39,
	GL_MAX_VIEWPORT_DIMS            = 0x0D3A,
	GL_MAX_CLIENT_ATTRIB_STACK_DEPTH= 0x0D3B,
	GL_SUBPIXEL_BITS                = 0x0D50,
	GL_INDEX_BITS                   = 0x0D51,
	GL_RED_BITS                     = 0x0D52,
	GL_GREEN_BITS                   = 0x0D53,
	GL_BLUE_BITS                    = 0x0D54,
	GL_ALPHA_BITS                   = 0x0D55,
	GL_DEPTH_BITS                   = 0x0D56,
	GL_STENCIL_BITS                 = 0x0D57,
	GL_ACCUM_RED_BITS               = 0x0D58,
	GL_ACCUM_GREEN_BITS             = 0x0D59,
	GL_ACCUM_BLUE_BITS              = 0x0D5A,
	GL_ACCUM_ALPHA_BITS             = 0x0D5B,
	GL_NAME_STACK_DEPTH             = 0x0D70,
	GL_AUTO_NORMAL                  = 0x0D80,
	GL_MAP1_COLOR_4                 = 0x0D90,
	GL_MAP1_INDEX                   = 0x0D91,
	GL_MAP1_NORMAL                  = 0x0D92,
	GL_MAP1_TEXTURE_COORD_1         = 0x0D93,
	GL_MAP1_TEXTURE_COORD_2         = 0x0D94,
	GL_MAP1_TEXTURE_COORD_3         = 0x0D95,
	GL_MAP1_TEXTURE_COORD_4         = 0x0D96,
	GL_MAP1_VERTEX_3                = 0x0D97,
	GL_MAP1_VERTEX_4                = 0x0D98,
	GL_MAP2_COLOR_4                 = 0x0DB0,
	GL_MAP2_INDEX                   = 0x0DB1,
	GL_MAP2_NORMAL                  = 0x0DB2,
	GL_MAP2_TEXTURE_COORD_1         = 0x0DB3,
	GL_MAP2_TEXTURE_COORD_2         = 0x0DB4,
	GL_MAP2_TEXTURE_COORD_3         = 0x0DB5,
	GL_MAP2_TEXTURE_COORD_4         = 0x0DB6,
	GL_MAP2_VERTEX_3                = 0x0DB7,
	GL_MAP2_VERTEX_4                = 0x0DB8,
	GL_MAP1_GRID_DOMAIN             = 0x0DD0,
	GL_MAP1_GRID_SEGMENTS           = 0x0DD1,
	GL_MAP2_GRID_DOMAIN             = 0x0DD2,
	GL_MAP2_GRID_SEGMENTS           = 0x0DD3,
	GL_TEXTURE_1D                   = 0x0DE0,
	GL_TEXTURE_2D                   = 0x0DE1,
	GL_FEEDBACK_BUFFER_POINTER      = 0x0DF0,
	GL_FEEDBACK_BUFFER_SIZE         = 0x0DF1,
	GL_FEEDBACK_BUFFER_TYPE         = 0x0DF2,
	GL_SELECTION_BUFFER_POINTER     = 0x0DF3,
	GL_SELECTION_BUFFER_SIZE        = 0x0DF4,
#endif
	// GetTextureParameter:

#ifndef GL_TEXTURE_WIDTH
	GL_TEXTURE_WIDTH                = 0x1000,
	GL_TEXTURE_HEIGHT               = 0x1001,
	GL_TEXTURE_INTERNAL_FORMAT      = 0x1003,
	GL_TEXTURE_BORDER_COLOR         = 0x1004,
	GL_TEXTURE_BORDER               = 0x1005,
#endif

	// HintMode:

#ifndef GL_DONT_CARE
	GL_DONT_CARE                    = 0x1100,
	GL_FASTEST                      = 0x1101,
	GL_NICEST                       = 0x1102,
#endif
	// LightName:

#ifndef GL_LIGHT0
	GL_LIGHT0                       = 0x4000,
	GL_LIGHT1                       = 0x4001,
	GL_LIGHT2                       = 0x4002,
	GL_LIGHT3                       = 0x4003,
	GL_LIGHT4                       = 0x4004,
	GL_LIGHT5                       = 0x4005,
	GL_LIGHT6                       = 0x4006,
	GL_LIGHT7                       = 0x4007,
#endif
	// LightParameter:

#ifndef GL_AMBIENT
	GL_AMBIENT                      = 0x1200,
	GL_DIFFUSE                      = 0x1201,
	GL_SPECULAR                     = 0x1202,
	GL_POSITION                     = 0x1203,
	GL_SPOT_DIRECTION               = 0x1204,
	GL_SPOT_EXPONENT                = 0x1205,
	GL_SPOT_CUTOFF                  = 0x1206,
	GL_CONSTANT_ATTENUATION         = 0x1207,
	GL_LINEAR_ATTENUATION           = 0x1208,
	GL_QUADRATIC_ATTENUATION        = 0x1209,
#endif
	// ListMode:

#ifndef GL_COMPILE
	GL_COMPILE                      = 0x1300,
	GL_COMPILE_AND_EXECUTE          = 0x1301,
#endif
	// LogicOp:

#ifndef GL_CLEAR
	GL_CLEAR                        = 0x1500,
	GL_AND                          = 0x1501,
	GL_AND_REVERSE                  = 0x1502,
	GL_COPY                         = 0x1503,
	GL_AND_INVERTED                 = 0x1504,
	GL_NOOP                         = 0x1505,
	GL_XOR                          = 0x1506,
	GL_OR                           = 0x1507,
	GL_NOR                          = 0x1508,
	GL_EQUIV                        = 0x1509,
	GL_INVERT                       = 0x150A,
	GL_OR_REVERSE                   = 0x150B,
	GL_COPY_INVERTED                = 0x150C,
	GL_OR_INVERTED                  = 0x150D,
	GL_NAND                         = 0x150E,
	GL_SET                          = 0x150F,
#endif
	// MaterialParameter:

#ifndef GL_EMISSION
	GL_EMISSION                     = 0x1600,
	GL_SHININESS                    = 0x1601,
	GL_AMBIENT_AND_DIFFUSE          = 0x1602,
	GL_COLOR_INDEXES                = 0x1603,
#endif
	// MatrixMode:

#ifndef GL_MODELVIEW
	GL_MODELVIEW                    = 0x1700,
	GL_PROJECTION                   = 0x1701,
	GL_TEXTURE                      = 0x1702,
#endif

	// PixelCopyType:

#ifndef GL_COLOR
	GL_COLOR                        = 0x1800,
	GL_DEPTH                        = 0x1801,
	GL_STENCIL                      = 0x1802,
#endif
	// PixelFormat:

#ifndef GL_COLOR_INDEX
	GL_COLOR_INDEX                  = 0x1900,
	GL_STENCIL_INDEX                = 0x1901,
	GL_DEPTH_COMPONENT              = 0x1902,
	GL_RED                          = 0x1903,
	GL_GREEN                        = 0x1904,
	GL_BLUE                         = 0x1905,
	GL_ALPHA                        = 0x1906,
	GL_RGB                          = 0x1907,
	GL_RGBA                         = 0x1908,
	GL_LUMINANCE                    = 0x1909,
	GL_LUMINANCE_ALPHA              = 0x190A,
#endif
	// PixelType:

#ifndef GL_BITMAP
	GL_BITMAP                       = 0x1A00,
#endif	
	// PolygonMode:

#ifndef GL_POINT
	GL_POINT                        = 0x1B00,
	GL_LINE                         = 0x1B01,
	GL_FILL                         = 0x1B02,
#endif
	// RenderingMode:

#ifndef GL_RENDER
	GL_RENDER                       = 0x1C00,
	GL_FEEDBACK                     = 0x1C01,
	GL_SELECT                       = 0x1C02,
#endif
	// ShadingModel:

#ifndef GL_FLAT
	GL_FLAT                         = 0x1D00,
	GL_SMOOTH                       = 0x1D01,
#endif
	// StencilOp:

	// GL_ZERO
#ifndef GL_KEEP
	GL_KEEP                         = 0x1E00,
	GL_REPLACE                      = 0x1E01,
	GL_INCR                         = 0x1E02,
	GL_DECR                         = 0x1E03,
#endif
	// GL_INVERT

	// StringName:

#ifndef GL_VENDOR
	GL_VENDOR                       = 0x1F00,
	GL_RENDERER                     = 0x1F01,
	GL_VERSION                      = 0x1F02,
	GL_EXTENSIONS                   = 0x1F03,
#endif
	// TextureCoordName:
#ifndef GL_S
	GL_S                            = 0x2000,
	GL_T                            = 0x2001,
	GL_R                            = 0x2002,
	GL_Q                            = 0x2003,
#endif
	// TextureEnvMode:
#ifndef GL_MODULATE
	GL_MODULATE                     = 0x2100,
	GL_DECAL                        = 0x2101,
	// GL_BLEND
	// GL_REPLACE
#endif

	// TextureEnvParameter:

#ifndef GL_TEXTURE_ENV_MODE
	GL_TEXTURE_ENV_MODE             = 0x2200,
	GL_TEXTURE_ENV_COLOR            = 0x2201,
#endif
	// TextureEnvTarget:

#ifndef GL_TEXTURE_ENV
	GL_TEXTURE_ENV                  = 0x2300,
#endif
	// TextureGenMode:

#ifndef GL_EYE_LINEAR
	GL_EYE_LINEAR                   = 0x2400,
	GL_OBJECT_LINEAR                = 0x2401,
	GL_SPHERE_MAP                   = 0x2402,
#endif

	// TextureGenParameter:

#ifndef GL_TEXTURE_GEN_MODE
	GL_TEXTURE_GEN_MODE             = 0x2500,
	GL_OBJECT_PLANE                 = 0x2501,
	GL_EYE_PLANE                    = 0x2502,
#endif

	// TextureMagFilter:

#ifndef GL_NEAREST
	GL_NEAREST                      = 0x2600,
	GL_LINEAR                       = 0x2601,
#endif
	// TextureMinFilter:

#ifndef GL_NEAREST_MIPMAP_NEAREST
	GL_NEAREST_MIPMAP_NEAREST       = 0x2700,
	GL_LINEAR_MIPMAP_NEAREST        = 0x2701,
	GL_NEAREST_MIPMAP_LINEAR        = 0x2702,
	GL_LINEAR_MIPMAP_LINEAR         = 0x2703,
#endif
	// TextureParameterName:

#ifndef GL_TEXTURE_MAG_FILTER
	GL_TEXTURE_MAG_FILTER           = 0x2800,
	GL_TEXTURE_MIN_FILTER           = 0x2801,
	GL_TEXTURE_WRAP_S               = 0x2802,
	GL_TEXTURE_WRAP_T               = 0x2803,
#endif
	// TextureWrapMode:

#ifndef GL_CLAMP
	GL_CLAMP                        = 0x2900,
	GL_REPEAT                       = 0x2901,
#endif
	// ClientAttribMask:

#ifndef GL_CLIENT_PIXEL_STORE_BIT
	GL_CLIENT_PIXEL_STORE_BIT       = 0x00000001,
	GL_CLIENT_VERTEX_ARRAY_BIT      = 0x00000002,
	GL_CLIENT_ALL_ATTRIB_BITS       = 0xffffffff,
#endif
	// polygon_offset:

#ifndef GL_POLYGON_OFFSET_FACTOR
	GL_POLYGON_OFFSET_FACTOR        = 0x8038,
	GL_POLYGON_OFFSET_UNITS         = 0x2A00,
	GL_POLYGON_OFFSET_POINT         = 0x2A01,
	GL_POLYGON_OFFSET_LINE          = 0x2A02,
	GL_POLYGON_OFFSET_FILL          = 0x8037,
#endif
	// texture:

#ifndef GL_ALPHA4
	GL_ALPHA4                       = 0x803B,
	GL_ALPHA8                       = 0x803C,
	GL_ALPHA12                      = 0x803D,
	GL_ALPHA16                      = 0x803E,
	GL_LUMINANCE4                   = 0x803F,
	GL_LUMINANCE8                   = 0x8040,
	GL_LUMINANCE12                  = 0x8041,
	GL_LUMINANCE16                  = 0x8042,
	GL_LUMINANCE4_ALPHA4            = 0x8043,
	GL_LUMINANCE6_ALPHA2            = 0x8044,
	GL_LUMINANCE8_ALPHA8            = 0x8045,
	GL_LUMINANCE12_ALPHA4           = 0x8046,
	GL_LUMINANCE12_ALPHA12          = 0x8047,
	GL_LUMINANCE16_ALPHA16          = 0x8048,
	GL_INTENSITY                    = 0x8049,
	GL_INTENSITY4                   = 0x804A,
	GL_INTENSITY8                   = 0x804B,
	GL_INTENSITY12                  = 0x804C,
	GL_INTENSITY16                  = 0x804D,
	GL_R3_G3_B2                     = 0x2A10,
	GL_RGB4                         = 0x804F,
	GL_RGB5                         = 0x8050,
	GL_RGB8                         = 0x8051,
	GL_RGB10                        = 0x8052,
	GL_RGB12                        = 0x8053,
	GL_RGB16                        = 0x8054,
	GL_RGBA2                        = 0x8055,
	GL_RGBA4                        = 0x8056,
	GL_RGB5_A1                      = 0x8057,
	GL_RGBA8                        = 0x8058,
	GL_RGB10_A2                     = 0x8059,
	GL_RGBA12                       = 0x805A,
	GL_RGBA16                       = 0x805B,
	GL_TEXTURE_RED_SIZE             = 0x805C,
	GL_TEXTURE_GREEN_SIZE           = 0x805D,
	GL_TEXTURE_BLUE_SIZE            = 0x805E,
	GL_TEXTURE_ALPHA_SIZE           = 0x805F,
	GL_TEXTURE_LUMINANCE_SIZE       = 0x8060,
	GL_TEXTURE_INTENSITY_SIZE       = 0x8061,
	GL_PROXY_TEXTURE_1D             = 0x8063,
	GL_PROXY_TEXTURE_2D             = 0x8064,
#endif
	// texture_object:

#ifndef GL_TEXTURE_PRIORITY
	GL_TEXTURE_PRIORITY             = 0x8066,
	GL_TEXTURE_RESIDENT             = 0x8067,
	GL_TEXTURE_BINDING_1D           = 0x8068,
	GL_TEXTURE_1D_BINDING           = 0x8068,
	GL_TEXTURE_BINDING_2D           = 0x8069,
	GL_TEXTURE_2D_BINDING           = 0x8069,
#endif
#ifndef GL_TEXTURE_BINDING_3D
	GL_TEXTURE_BINDING_3D           = 0x806A,
	GL_TEXTURE_3D_BINDING           = 0x806A,
#endif
	// vertex_array:

#ifndef GL_VERTEX_ARRAY
	GL_VERTEX_ARRAY                 = 0x8074,
	GL_NORMAL_ARRAY                 = 0x8075,
	GL_COLOR_ARRAY                  = 0x8076,
	GL_INDEX_ARRAY                  = 0x8077,
	GL_TEXTURE_COORD_ARRAY          = 0x8078,
	GL_EDGE_FLAG_ARRAY              = 0x8079,
	GL_VERTEX_ARRAY_SIZE            = 0x807A,
	GL_VERTEX_ARRAY_TYPE            = 0x807B,
	GL_VERTEX_ARRAY_STRIDE          = 0x807C,
	GL_NORMAL_ARRAY_TYPE            = 0x807E,
	GL_NORMAL_ARRAY_STRIDE          = 0x807F,
	GL_COLOR_ARRAY_SIZE             = 0x8081,
	GL_COLOR_ARRAY_TYPE             = 0x8082,
	GL_COLOR_ARRAY_STRIDE           = 0x8083,
	GL_INDEX_ARRAY_TYPE             = 0x8085,
	GL_INDEX_ARRAY_STRIDE           = 0x8086,
	GL_TEXTURE_COORD_ARRAY_SIZE     = 0x8088,
	GL_TEXTURE_COORD_ARRAY_TYPE     = 0x8089,
	GL_TEXTURE_COORD_ARRAY_STRIDE   = 0x808A,
	GL_EDGE_FLAG_ARRAY_STRIDE       = 0x808C,
	GL_VERTEX_ARRAY_POINTER         = 0x808E,
	GL_NORMAL_ARRAY_POINTER         = 0x808F,
	GL_COLOR_ARRAY_POINTER          = 0x8090,
	GL_INDEX_ARRAY_POINTER          = 0x8091,
	GL_TEXTURE_COORD_ARRAY_POINTER  = 0x8092,
	GL_EDGE_FLAG_ARRAY_POINTER      = 0x8093,
	GL_V2F                          = 0x2A20,
	GL_V3F                          = 0x2A21,
	GL_C4UB_V2F                     = 0x2A22,
	GL_C4UB_V3F                     = 0x2A23,
	GL_C3F_V3F                      = 0x2A24,
	GL_N3F_V3F                      = 0x2A25,
	GL_C4F_N3F_V3F                  = 0x2A26,
	GL_T2F_V3F                      = 0x2A27,
	GL_T4F_V4F                      = 0x2A28,
	GL_T2F_C4UB_V3F                 = 0x2A29,
	GL_T2F_C3F_V3F                  = 0x2A2A,
	GL_T2F_N3F_V3F                  = 0x2A2B,
	GL_T2F_C4F_N3F_V3F              = 0x2A2C,
	GL_T4F_C4F_N3F_V4F              = 0x2A2D,
#endif
#ifndef GL_BGR
	// bgra

	GL_BGR                          = 0x80E0,
	GL_BGRA                         = 0x80E1,

#endif
#ifndef GL_CONSTANT_COLOR
	// blend_color

	GL_CONSTANT_COLOR               = 0x8001,
	GL_ONE_MINUS_CONSTANT_COLOR     = 0x8002,
	GL_CONSTANT_ALPHA               = 0x8003,
	GL_ONE_MINUS_CONSTANT_ALPHA     = 0x8004,
	GL_BLEND_COLOR                  = 0x8005,

#endif
#ifndef GL_FUNC_ADD
	// blend_minmax

	GL_FUNC_ADD                     = 0x8006,
	GL_MIN                          = 0x8007,
	GL_MAX                          = 0x8008,
	GL_BLEND_EQUATION               = 0x8009,

#endif
#ifndef GL_FUNC_SUBTRACT
	// blend_subtract

	GL_FUNC_SUBTRACT                = 0x800A,
	GL_FUNC_REVERSE_SUBTRACT        = 0x800B,

#endif
#ifndef GL_COLOR_MATRIX
	// color_matrix

	GL_COLOR_MATRIX                 = 0x80B1,
	GL_COLOR_MATRIX_STACK_DEPTH     = 0x80B2,
	GL_MAX_COLOR_MATRIX_STACK_DEPTH = 0x80B3,
	GL_POST_COLOR_MATRIX_RED_SCALE  = 0x80B4,
	GL_POST_COLOR_MATRIX_GREEN_SCALE= 0x80B5,
	GL_POST_COLOR_MATRIX_BLUE_SCALE = 0x80B6,
	GL_POST_COLOR_MATRIX_ALPHA_SCALE= 0x80B7,
	GL_POST_COLOR_MATRIX_RED_BIAS   = 0x80B8,
	GL_POST_COLOR_MATRIX_GREEN_BIAS = 0x80B9,
	GL_POST_COLOR_MATRIX_BLUE_BIAS  = 0x80BA,
	GL_POST_COLOR_MATRIX_ALPHA_BIAS = 0x80BB,

#endif
#ifndef GL_COLOR_TABLE
	// color_table

	GL_COLOR_TABLE                  = 0x80D0,
	GL_POST_CONVOLUTION_COLOR_TABLE = 0x80D1,
	GL_POST_COLOR_MATRIX_COLOR_TABLE= 0x80D2,
	GL_PROXY_COLOR_TABLE            = 0x80D3,
	GL_PROXY_POST_CONVOLUTION_COLOR_TABLE = 0x80D4,
	GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80D5,
	GL_COLOR_TABLE_SCALE            = 0x80D6,
	GL_COLOR_TABLE_BIAS             = 0x80D7,
	GL_COLOR_TABLE_FORMAT           = 0x80D8,
	GL_COLOR_TABLE_WIDTH            = 0x80D9,
	GL_COLOR_TABLE_RED_SIZE         = 0x80DA,
	GL_COLOR_TABLE_GREEN_SIZE       = 0x80DB,
	GL_COLOR_TABLE_BLUE_SIZE        = 0x80DC,
	GL_COLOR_TABLE_ALPHA_SIZE       = 0x80DD,
	GL_COLOR_TABLE_LUMINANCE_SIZE   = 0x80DE,
	GL_COLOR_TABLE_INTENSITY_SIZE   = 0x80DF,

#endif
#ifndef GL_CONVOLUTION_1D
	// convolution

	GL_CONVOLUTION_1D               = 0x8010,
	GL_CONVOLUTION_2D               = 0x8011,
	GL_SEPARABLE_2D                 = 0x8012,
	GL_CONVOLUTION_BORDER_MODE      = 0x8013,
	GL_CONVOLUTION_FILTER_SCALE     = 0x8014,
	GL_CONVOLUTION_FILTER_BIAS      = 0x8015,
	GL_REDUCE                       = 0x8016,
	GL_CONVOLUTION_FORMAT           = 0x8017,
	GL_CONVOLUTION_WIDTH            = 0x8018,
	GL_CONVOLUTION_HEIGHT           = 0x8019,
	GL_MAX_CONVOLUTION_WIDTH        = 0x801A,
	GL_MAX_CONVOLUTION_HEIGHT       = 0x801B,
	GL_POST_CONVOLUTION_RED_SCALE   = 0x801C,
	GL_POST_CONVOLUTION_GREEN_SCALE = 0x801D,
	GL_POST_CONVOLUTION_BLUE_SCALE  = 0x801E,
	GL_POST_CONVOLUTION_ALPHA_SCALE = 0x801F,
	GL_POST_CONVOLUTION_RED_BIAS    = 0x8020,
	GL_POST_CONVOLUTION_GREEN_BIAS  = 0x8021,
	GL_POST_CONVOLUTION_BLUE_BIAS   = 0x8022,
	GL_POST_CONVOLUTION_ALPHA_BIAS  = 0x8023,
	GL_CONSTANT_BORDER              = 0x8151,
	GL_REPLICATE_BORDER             = 0x8153,
	GL_CONVOLUTION_BORDER_COLOR     = 0x8154,

#endif
#ifndef GL_MAX_ELEMENTS_VERTICES
	// draw_range_elements

	GL_MAX_ELEMENTS_VERTICES        = 0x80E8,
	GL_MAX_ELEMENTS_INDICES         = 0x80E9,

#endif
#ifndef GL_HISTOGRAM
	// histogram

	GL_HISTOGRAM                    = 0x8024,
	GL_PROXY_HISTOGRAM              = 0x8025,
	GL_HISTOGRAM_WIDTH              = 0x8026,
	GL_HISTOGRAM_FORMAT             = 0x8027,
	GL_HISTOGRAM_RED_SIZE           = 0x8028,
	GL_HISTOGRAM_GREEN_SIZE         = 0x8029,
	GL_HISTOGRAM_BLUE_SIZE          = 0x802A,
	GL_HISTOGRAM_ALPHA_SIZE         = 0x802B,
	GL_HISTOGRAM_LUMINANCE_SIZE     = 0x802C,
	GL_HISTOGRAM_SINK               = 0x802D,
	GL_MINMAX                       = 0x802E,
	GL_MINMAX_FORMAT                = 0x802F,
	GL_MINMAX_SINK                  = 0x8030,
	GL_TABLE_TOO_LARGE              = 0x8031,

#endif
#ifndef GL_UNSIGNED_BYTE_3_3_2
	// packed_pixels

	GL_UNSIGNED_BYTE_3_3_2          = 0x8032,
	GL_UNSIGNED_SHORT_4_4_4_4       = 0x8033,
	GL_UNSIGNED_SHORT_5_5_5_1       = 0x8034,
	GL_UNSIGNED_INT_8_8_8_8         = 0x8035,
	GL_UNSIGNED_INT_10_10_10_2      = 0x8036,
	GL_UNSIGNED_BYTE_2_3_3_REV      = 0x8362,
	GL_UNSIGNED_SHORT_5_6_5         = 0x8363,
	GL_UNSIGNED_SHORT_5_6_5_REV     = 0x8364,
	GL_UNSIGNED_SHORT_4_4_4_4_REV   = 0x8365,
	GL_UNSIGNED_SHORT_1_5_5_5_REV   = 0x8366,
	GL_UNSIGNED_INT_8_8_8_8_REV     = 0x8367,
	GL_UNSIGNED_INT_2_10_10_10_REV  = 0x8368,

	GL_MIRRORED_REPEAT              = 0x8370,

#endif
#ifndef GL_RESCALE_NORMAL
	// rescale_normal

	GL_RESCALE_NORMAL               = 0x803A,

#endif
#ifndef GL_LIGHT_MODEL_COLOR_CONTROL
	// separate_specular_color

	GL_LIGHT_MODEL_COLOR_CONTROL    = 0x81F8,
	GL_SINGLE_COLOR                 = 0x81F9,
	GL_SEPARATE_SPECULAR_COLOR      = 0x81FA,

#endif
#ifndef GL_PACK_SKIP_IMAGES
	// texture3D

	GL_PACK_SKIP_IMAGES             = 0x806B,
	GL_PACK_IMAGE_HEIGHT            = 0x806C,
	GL_UNPACK_SKIP_IMAGES           = 0x806D,
	GL_UNPACK_IMAGE_HEIGHT          = 0x806E,
	GL_TEXTURE_3D                   = 0x806F,
	GL_PROXY_TEXTURE_3D             = 0x8070,
	GL_TEXTURE_DEPTH                = 0x8071,
	GL_TEXTURE_WRAP_R               = 0x8072,
	GL_MAX_3D_TEXTURE_SIZE          = 0x8073,

#endif
#ifndef GL_CLAMP_TO_BORDER
	// texture_edge_clamp

	GL_CLAMP_TO_BORDER              = 0x812D,
	GL_CLAMP_TO_EDGE                = 0x812F,

#endif
#ifndef GL_TEXTURE_MIN_LOD
	// texture_lod

	GL_TEXTURE_MIN_LOD              = 0x813A,
	GL_TEXTURE_MAX_LOD              = 0x813B,
	GL_TEXTURE_BASE_LEVEL           = 0x813C,
	GL_TEXTURE_MAX_LEVEL            = 0x813D,

#endif
#ifndef GL_SMOOTH_POINT_SIZE_RANGE
	// GetTarget1_2:

	GL_SMOOTH_POINT_SIZE_RANGE      = 0x0B12,
	GL_SMOOTH_POINT_SIZE_GRANULARITY= 0x0B13,
	GL_SMOOTH_LINE_WIDTH_RANGE      = 0x0B22,
	GL_SMOOTH_LINE_WIDTH_GRANULARITY= 0x0B23,
	GL_ALIASED_POINT_SIZE_RANGE     = 0x846D,
	GL_ALIASED_LINE_WIDTH_RANGE     = 0x846E,

#endif
#ifndef GL_TEXTURE0
	// multitexture:

	GL_TEXTURE0                 = 0x84C0,
	GL_TEXTURE1                 = 0x84C1,
	GL_TEXTURE2                 = 0x84C2,
	GL_TEXTURE3                 = 0x84C3,
	GL_TEXTURE4                 = 0x84C4,
	GL_TEXTURE5                 = 0x84C5,
	GL_TEXTURE6                 = 0x84C6,
	GL_TEXTURE7                 = 0x84C7,
	GL_TEXTURE8                 = 0x84C8,
	GL_TEXTURE9                 = 0x84C9,
	GL_TEXTURE10                = 0x84CA,
	GL_TEXTURE11                = 0x84CB,
	GL_TEXTURE12                = 0x84CC,
	GL_TEXTURE13                = 0x84CD,
	GL_TEXTURE14                = 0x84CE,
	GL_TEXTURE15                = 0x84CF,
	GL_TEXTURE16                = 0x84D0,
	GL_TEXTURE17                = 0x84D1,
	GL_TEXTURE18                = 0x84D2,
	GL_TEXTURE19                = 0x84D3,
	GL_TEXTURE20                = 0x84D4,
	GL_TEXTURE21                = 0x84D5,
	GL_TEXTURE22                = 0x84D6,
	GL_TEXTURE23                = 0x84D7,
	GL_TEXTURE24                = 0x84D8,
	GL_TEXTURE25                = 0x84D9,
	GL_TEXTURE26                = 0x84DA,
	GL_TEXTURE27                = 0x84DB,
	GL_TEXTURE28                = 0x84DC,
	GL_TEXTURE29                = 0x84DD,
	GL_TEXTURE30                = 0x84DE,
	GL_TEXTURE31                = 0x84DF,
	GL_ACTIVE_TEXTURE           = 0x84E0,
	GL_CLIENT_ACTIVE_TEXTURE    = 0x84E1,
	GL_MAX_TEXTURE_UNITS        = 0x84E2,

#endif
#ifndef GL_ABGR
	// EXT_abgr:

	GL_ABGR                     = 0x8000,

#endif
#ifndef GL_COMBINE
	// EXT_texture_env_combine:

	GL_COMBINE                  = 0x8570,
	GL_COMBINE_RGB              = 0x8571,
	GL_COMBINE_ALPHA            = 0x8572,
	GL_RGB_SCALE                = 0x8573,
	GL_ADD_SIGNED               = 0x8574,
	GL_INTERPOLATE              = 0x8575,
	GL_CONSTANT                 = 0x8576,
	GL_PRIMARY_COLOR            = 0x8577,
	GL_PREVIOUS                 = 0x8578,
	GL_SOURCE0_RGB              = 0x8580,
	GL_SOURCE1_RGB              = 0x8581,
	GL_SOURCE2_RGB              = 0x8582,
	GL_SOURCE0_ALPHA            = 0x8588,
	GL_SOURCE1_ALPHA            = 0x8589,
	GL_SOURCE2_ALPHA            = 0x858A,
	GL_OPERAND0_RGB             = 0x8590,
	GL_OPERAND1_RGB             = 0x8591,
	GL_OPERAND2_RGB             = 0x8592,
	GL_OPERAND0_ALPHA           = 0x8598,
	GL_OPERAND1_ALPHA           = 0x8599,
	GL_OPERAND2_ALPHA           = 0x859A,

#endif
#ifndef GL_SRC0_RGB
	// OpenGL 1.5 token renames:
	GL_SRC0_RGB                 = GL_SOURCE0_RGB,
	GL_SRC1_RGB                 = GL_SOURCE1_RGB,
	GL_SRC2_RGB                 = GL_SOURCE2_RGB,
	GL_SRC0_ALPHA               = GL_SOURCE0_ALPHA,
	GL_SRC1_ALPHA               = GL_SOURCE1_ALPHA,
	GL_SRC2_ALPHA               = GL_SOURCE2_ALPHA,

#endif
#ifndef GL_COMPRESSED_ALPHA
	// ARB_texture_compression:

	GL_COMPRESSED_ALPHA           = 0x84e9,
	GL_COMPRESSED_LUMINANCE       = 0x84ea,
	GL_COMPRESSED_LUMINANCE_ALPHA = 0x84eb,
	GL_COMPRESSED_INTENSITY       = 0x84ec,
	GL_COMPRESSED_RGB             = 0x84ed,
	GL_COMPRESSED_RGBA            = 0x84ee,
	GL_TEXTURE_COMPRESSION_HINT   = 0x84ef,
	GL_TEXTURE_COMPRESSED_IMAGE_SIZE = 0x86a0,
	GL_TEXTURE_COMPRESSED         = 0x86a1,
	GL_NUM_COMPRESSED_TEXTURE_FORMATS = 0x86a2,
	GL_COMPRESSED_TEXTURE_FORMATS = 0x86a3,

#endif
#ifndef GL_NORMAL_MAP
	// ARB_texture_cube_map:

	GL_NORMAL_MAP                 = 0x8511,
	GL_REFLECTION_MAP             = 0x8512,
	GL_TEXTURE_CUBE_MAP           = 0x8513,
	GL_TEXTURE_BINDING_CUBE_MAP   = 0x8514,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851a,
	GL_PROXY_TEXTURE_CUBE_MAP     = 0x851b,
	GL_MAX_CUBE_MAP_TEXTURE_SIZE  = 0x851c,

#endif
#ifndef GL_MULTISAMPLE
	// ARB_multisample:

	GL_MULTISAMPLE                = 0x809d,
	GL_SAMPLE_ALPHA_TO_COVERAGE   = 0x809e,
	GL_SAMPLE_ALPHA_TO_ONE        = 0x809f,
	GL_SAMPLE_COVERAGE            = 0x80a0,
	GL_SAMPLE_BUFFERS             = 0x80a8,
	GL_SAMPLES                    = 0x80a9,
	GL_SAMPLE_COVERAGE_VALUE      = 0x80aa,
	GL_SAMPLE_COVERAGE_INVERT     = 0x80ab,
	GL_MULTISAMPLE_BIT            = 0x20000000,

#endif
#ifndef GL_SUBTRACT
	// ARB_texture_env_combine:

	GL_SUBTRACT                   = 0x84e7,

#endif
#ifndef GL_DOT3_RGB
	// ARB_texture_env_dot3:

	GL_DOT3_RGB                   = 0x86ae,
	GL_DOT3_RGBA                  = 0x86af,

#endif
#ifndef GL_TRANSPOSE_MODELVIEW_MATRIX
	// ARB_transpose_matrix:

	GL_TRANSPOSE_MODELVIEW_MATRIX  = 0x84e3,
	GL_TRANSPOSE_PROJECTION_MATRIX = 0x84e4,
	GL_TRANSPOSE_TEXTURE_MATRIX    = 0x84e5,
	GL_TRANSPOSE_COLOR_MATRIX      = 0x84e6,

#endif
#ifndef GL_GENERATE_MIPMAP
	// SGIS_generate_mipmap:

	GL_GENERATE_MIPMAP             = 0x8191,
	GL_GENERATE_MIPMAP_HINT        = 0x8192,

#endif
#ifndef GL_DEPTH_COMPONENT16
	// ARB_depth_texture:

	GL_DEPTH_COMPONENT16           = 0x81a5,
	GL_DEPTH_COMPONENT24           = 0x81a6,
	GL_DEPTH_COMPONENT32           = 0x81a7,
	GL_TEXTURE_DEPTH_SIZE          = 0x884a,
	GL_DEPTH_TEXTURE_MODE          = 0x884b,

#endif
#ifndef GL_TEXTURE_COMPARE_MODE
	// ARB_shadow:

	GL_TEXTURE_COMPARE_MODE        = 0x884c,
	GL_TEXTURE_COMPARE_FUNC        = 0x884d,
	GL_COMPARE_R_TO_TEXTURE        = 0x884e,

#endif
#ifndef GL_FOG_COORDINATE_SOURCE
	// EXT_fog_coord:

	GL_FOG_COORDINATE_SOURCE       = 0x8450,
	GL_FOG_COORDINATE              = 0x8451,
	GL_FRAGMENT_DEPTH              = 0x8452,
	GL_CURRENT_FOG_COORDINATE      = 0x8453,
	GL_FOG_COORDINATE_ARRAY_TYPE   = 0x8454,
	GL_FOG_COORDINATE_ARRAY_STRIDE = 0x8455,
	GL_FOG_COORDINATE_ARRAY_POINTER = 0x8456,
	GL_FOG_COORDINATE_ARRAY        = 0x8457,

#endif
#ifndef GL_FOG_COORD_SRC
	// OpenGL 1.5 token renames:
	GL_FOG_COORD_SRC               = GL_FOG_COORDINATE_SOURCE,
	GL_FOG_COORD                   = GL_FOG_COORDINATE,
	GL_CURRENT_FOG_COORD           = GL_CURRENT_FOG_COORDINATE,
	GL_FOG_COORD_ARRAY_TYPE        = GL_FOG_COORDINATE_ARRAY_TYPE,
	GL_FOG_COORD_ARRAY_STRIDE      = GL_FOG_COORDINATE_ARRAY_STRIDE,
	GL_FOG_COORD_ARRAY_POINTER     = GL_FOG_COORDINATE_ARRAY_POINTER,
	GL_FOG_COORD_ARRAY             = GL_FOG_COORDINATE_ARRAY,

#endif
#ifndef GL_POINT_SIZE_MIN
	// ARB_point_parameters:

	GL_POINT_SIZE_MIN              = 0x8126,
	GL_POINT_SIZE_MAX              = 0x8127,
	GL_POINT_FADE_THRESHOLD_SIZE   = 0x8128,
	GL_POINT_DISTANCE_ATTENUATION  = 0x8129,

#endif
#ifndef GL_COLOR_SUM
	// EXT_secondary_color:

	GL_COLOR_SUM                   = 0x8458,
	GL_CURRENT_SECONDARY_COLOR     = 0x8459,
	GL_SECONDARY_COLOR_ARRAY_SIZE  = 0x845a,
	GL_SECONDARY_COLOR_ARRAY_TYPE  = 0x845b,
	GL_SECONDARY_COLOR_ARRAY_STRIDE = 0x845c,
	GL_SECONDARY_COLOR_ARRAY_POINTER = 0x845d,
	GL_SECONDARY_COLOR_ARRAY       = 0x845e,

#endif
#ifndef GL_BLEND_DST_RGB
	// EXT_blend_func_separate:

	GL_BLEND_DST_RGB               = 0x80c8,
	GL_BLEND_SRC_RGB               = 0x80c9,
	GL_BLEND_DST_ALPHA             = 0x80ca,
	GL_BLEND_SRC_ALPHA             = 0x80cb,

#endif
#ifndef GL_INCR_WRAP
	// EXT_stencil_wrap:

	GL_INCR_WRAP                   = 0x8507,
	GL_DECR_WRAP                   = 0x8508,

#endif
#ifndef GL_MAX_TEXTURE_LOD_BIAS
	// EXT_texture_lod_bias:

	GL_MAX_TEXTURE_LOD_BIAS        = 0x84fd,
	GL_TEXTURE_FILTER_CONTROL      = 0x8500,
	GL_TEXTURE_LOD_BIAS            = 0x8501,

#endif
#ifndef GL_ARRAY_BUFFER
	// ARB_vertex_buffer_object:

	GL_ARRAY_BUFFER                        = 0x8892,
	GL_ELEMENT_ARRAY_BUFFER                = 0x8893,
	GL_ARRAY_BUFFER_BINDING                = 0x8894,
	GL_ELEMENT_ARRAY_BUFFER_BINDING        = 0x8895,
	GL_VERTEX_ARRAY_BUFFER_BINDING         = 0x8896,
	GL_NORMAL_ARRAY_BUFFER_BINDING         = 0x8897,
	GL_COLOR_ARRAY_BUFFER_BINDING          = 0x8898,
	GL_INDEX_ARRAY_BUFFER_BINDING          = 0x8899,
	GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING  = 0x889A,
	GL_EDGE_FLAG_ARRAY_BUFFER_BINDING      = 0x889B,
	GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING = 0x889C,
	GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING = 0x889D,
	GL_FOG_COORD_ARRAY_BUFFER_BINDING      = GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING,
	GL_WEIGHT_ARRAY_BUFFER_BINDING         = 0x889E,
	GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING  = 0x889F,
	GL_STREAM_DRAW                         = 0x88E0,
	GL_STREAM_READ                         = 0x88E1,
	GL_STREAM_COPY                         = 0x88E2,
	GL_STATIC_DRAW                         = 0x88E4,
	GL_STATIC_READ                         = 0x88E5,
	GL_STATIC_COPY                         = 0x88E6,
	GL_DYNAMIC_DRAW                        = 0x88E8,
	GL_DYNAMIC_READ                        = 0x88E9,
	GL_DYNAMIC_COPY                        = 0x88EA,
	GL_READ_ONLY                           = 0x88B8,
	GL_WRITE_ONLY                          = 0x88B9,
	GL_READ_WRITE                          = 0x88BA,
	GL_BUFFER_SIZE                         = 0x8764,
	GL_BUFFER_USAGE                        = 0x8765,
	GL_BUFFER_ACCESS                       = 0x88BB,
	GL_BUFFER_MAPPED                       = 0x88BC,
	GL_BUFFER_MAP_POINTER                  = 0x88BD,

#endif
#ifndef GL_SAMPLES_PASSED
	// ARB_occlusion_query:

	GL_SAMPLES_PASSED                      = 0x8914,
	GL_QUERY_COUNTER_BITS                  = 0x8864,
	GL_CURRENT_QUERY                       = 0x8865,
	GL_QUERY_RESULT                        = 0x8866,
	GL_QUERY_RESULT_AVAILABLE              = 0x8867,

#endif
#ifndef GL_PROGRAM_OBJECT
	// ARB_shader_objects:

	GL_PROGRAM_OBJECT                      = 0x8B40,
	GL_TYPE                                = 0x8B4E,
	GL_SUBTYPE                             = 0x8B4F,
#endif
#ifndef GL_DELETE_STATUS 
	GL_DELETE_STATUS                       = 0x8B80,
#endif
#ifndef GL_COMPILE_STATUS
	GL_COMPILE_STATUS                      = 0x8B81,
	GL_LINK_STATUS                         = 0x8B82,
	GL_VALIDATE_STATUS                     = 0x8B83,
	GL_INFO_LOG_LENGTH                     = 0x8B84,
	GL_ATTACHED_OBJECTS                    = 0x8B85,
	GL_ACTIVE_UNIFORMS                     = 0x8B86,
	GL_ACTIVE_UNIFORM_MAX_LENGTH           = 0x8B87,
	GL_SHADER_SOURCE_LENGTH                = 0x8B88,
	GL_CURRENT_PROGRAM                     = 0x8B8D,
	GL_OBJECT_TYPE                         = GL_TYPE,
	GL_OBJECT_SUBTYPE                      = GL_SUBTYPE,
	GL_OBJECT_DELETE_STATUS                = GL_DELETE_STATUS,
	GL_OBJECT_COMPILE_STATUS               = GL_COMPILE_STATUS,
	GL_OBJECT_LINK_STATUS                  = GL_LINK_STATUS,
	GL_OBJECT_VALIDATE_STATUS              = GL_VALIDATE_STATUS,
	GL_OBJECT_INFO_LOG_LENGTH              = GL_INFO_LOG_LENGTH,
	GL_OBJECT_ATTACHED_OBJECTS             = GL_ATTACHED_OBJECTS,
	GL_OBJECT_ACTIVE_UNIFORMS              = GL_ACTIVE_UNIFORMS,
	GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH    = GL_ACTIVE_UNIFORM_MAX_LENGTH,
	GL_OBJECT_SHADER_SOURCE_LENGTH         = GL_SHADER_SOURCE_LENGTH,
	GL_SHADER_OBJECT                       = 0x8B48,
	GL_INT_VEC2                            = 0x8B53,
	GL_INT_VEC3                            = 0x8B54,
	GL_INT_VEC4                            = 0x8B55,
	GL_BOOL                                = 0x8B56,
	GL_BOOL_VEC2                           = 0x8B57,
	GL_BOOL_VEC3                           = 0x8B58,
	GL_BOOL_VEC4                           = 0x8B59,
	GL_FLOAT_MAT2                          = 0x8B5A,
	GL_FLOAT_MAT3                          = 0x8B5B,
	GL_FLOAT_MAT4                          = 0x8B5C,
	GL_SAMPLER_1D                          = 0x8B5D,
	GL_SAMPLER_2D                          = 0x8B5E,
	GL_SAMPLER_3D                          = 0x8B5F,
	GL_SAMPLER_CUBE                        = 0x8B60,
	GL_SAMPLER_1D_SHADOW                   = 0x8B61,
	GL_SAMPLER_2D_SHADOW                   = 0x8B62,
	GL_SAMPLER_2D_RECT                     = 0x8B63,
	GL_SAMPLER_2D_RECT_SHADOW              = 0x8B64,

#endif
#ifndef GL_VERTEX_SHADER
	// ARB_vertex_shader:

	GL_VERTEX_SHADER                       = 0x8B31,
	GL_MAX_VERTEX_UNIFORM_COMPONENTS       = 0x8B4A,
	GL_MAX_VARYING_FLOATS                  = 0x8B4B,
	GL_MAX_VERTEX_ATTRIBS                  = 0x8869,
	GL_MAX_TEXTURE_IMAGE_UNITS             = 0x8872,
	GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS      = 0x8B4C,
	GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS    = 0x8B4D,
	GL_MAX_TEXTURE_COORDS                  = 0x8871,
	GL_VERTEX_PROGRAM_POINT_SIZE           = 0x8642,
	GL_VERTEX_PROGRAM_TWO_SIDE             = 0x8643,
	GL_ACTIVE_ATTRIBUTES                   = 0x8B89,
	GL_ACTIVE_ATTRIBUTE_MAX_LENGTH         = 0x8B8A,
	GL_OBJECT_ACTIVE_ATTRIBUTES            = GL_ACTIVE_ATTRIBUTES,
	GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH  = GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
	GL_VERTEX_ATTRIB_ARRAY_ENABLED         = 0x8622,
	GL_VERTEX_ATTRIB_ARRAY_SIZE            = 0x8623,
	GL_VERTEX_ATTRIB_ARRAY_STRIDE          = 0x8624,
	GL_VERTEX_ATTRIB_ARRAY_TYPE            = 0x8625,
	GL_VERTEX_ATTRIB_ARRAY_NORMALIZED      = 0x886A,
	GL_CURRENT_VERTEX_ATTRIB               = 0x8626,
	GL_VERTEX_ATTRIB_ARRAY_POINTER         = 0x8645,
	GL_FLOAT_VEC2                          = 0x8B50,
	GL_FLOAT_VEC3                          = 0x8B51,
	GL_FLOAT_VEC4                          = 0x8B52,

#endif
#ifndef GL_FRAGMENT_SHADER
	// ARB_fragment_shader:

	GL_FRAGMENT_SHADER                     = 0x8B30,
	GL_MAX_FRAGMENT_UNIFORM_COMPONENTS     = 0x8B49,
	GL_FRAGMENT_SHADER_DERIVATIVE_HINT     = 0x8B8B,

#endif
#ifndef GL_MAX_DRAW_BUFFERS
	// ARB_draw_buffers:

	GL_MAX_DRAW_BUFFERS                    = 0x8824,
	GL_DRAW_BUFFER0                        = 0x8825,
	GL_DRAW_BUFFER1                        = 0x8826,
	GL_DRAW_BUFFER2                        = 0x8827,
	GL_DRAW_BUFFER3                        = 0x8828,
	GL_DRAW_BUFFER4                        = 0x8829,
	GL_DRAW_BUFFER5                        = 0x882A,
	GL_DRAW_BUFFER6                        = 0x882B,
	GL_DRAW_BUFFER7                        = 0x882C,
	GL_DRAW_BUFFER8                        = 0x882D,
	GL_DRAW_BUFFER9                        = 0x882E,
	GL_DRAW_BUFFER10                       = 0x882F,
	GL_DRAW_BUFFER11                       = 0x8830,
	GL_DRAW_BUFFER12                       = 0x8831,
	GL_DRAW_BUFFER13                       = 0x8832,
	GL_DRAW_BUFFER14                       = 0x8833,
	GL_DRAW_BUFFER15                       = 0x8834,

#endif
#ifndef GL_POINT_SPRITE
	// ARB_point_sprite:

	GL_POINT_SPRITE                        = 0x8861,
	GL_COORD_REPLACE                       = 0x8862,
	GL_POINT_SPRITE_COORD_ORIGIN           = 0x8CA0,
	GL_LOWER_LEFT                          = 0x8CA1,
	GL_UPPER_LEFT                          = 0x8CA2,

#endif
#ifndef GL_STENCIL_BACK_FUNC
	// ATI_separate_stencil:

	GL_STENCIL_BACK_FUNC                   = 0x8800,
	GL_STENCIL_BACK_FAIL                   = 0x8801,
	GL_STENCIL_BACK_PASS_DEPTH_FAIL        = 0x8802,
	GL_STENCIL_BACK_PASS_DEPTH_PASS        = 0x8803,

#endif
#ifndef GL_STENCIL_TEST_TWO_SIDE
	// EXT_stencil_two_side additional states:

	GL_STENCIL_TEST_TWO_SIDE               = 0x8910,
	GL_ACTIVE_STENCIL_FACE                 = 0x8911,

#endif
#ifndef GL_FRAMEBUFFER
	// EXT_framebuffer_object:

	GL_FRAMEBUFFER                         = 0x8D40,
	GL_RENDERBUFFER                        = 0x8D41,
	GL_STENCIL_INDEX1                      = 0x8D46,
	GL_STENCIL_INDEX4                      = 0x8D47,
	GL_STENCIL_INDEX8                      = 0x8D48,
	GL_STENCIL_INDEX16                     = 0x8D49,
	GL_RENDERBUFFER_WIDTH                  = 0x8D42,
	GL_RENDERBUFFER_HEIGHT                 = 0x8D43,
	GL_RENDERBUFFER_INTERNAL_FORMAT        = 0x8D44,
	GL_RENDERBUFFER_RED_SIZE               = 0x8D50,
	GL_RENDERBUFFER_GREEN_SIZE             = 0x8D51,
	GL_RENDERBUFFER_BLUE_SIZE              = 0x8D52,
	GL_RENDERBUFFER_ALPHA_SIZE             = 0x8D53,
	GL_RENDERBUFFER_DEPTH_SIZE             = 0x8D54,
	GL_RENDERBUFFER_STENCIL_SIZE           = 0x8D55,
	GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE              = 0x8CD0,
	GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME              = 0x8CD1,
	GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL            = 0x8CD2,
	GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE    = 0x8CD3,
	GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET       = 0x8CD4,
	GL_COLOR_ATTACHMENT0                   = 0x8CE0,
	GL_COLOR_ATTACHMENT1                   = 0x8CE1,
	GL_COLOR_ATTACHMENT2                   = 0x8CE2,
	GL_COLOR_ATTACHMENT3                   = 0x8CE3,
	GL_COLOR_ATTACHMENT4                   = 0x8CE4,
	GL_COLOR_ATTACHMENT5                   = 0x8CE5,
	GL_COLOR_ATTACHMENT6                   = 0x8CE6,
	GL_COLOR_ATTACHMENT7                   = 0x8CE7,
	GL_COLOR_ATTACHMENT8                   = 0x8CE8,
	GL_COLOR_ATTACHMENT9                   = 0x8CE9,
	GL_COLOR_ATTACHMENT10                  = 0x8CEA,
	GL_COLOR_ATTACHMENT11                  = 0x8CEB,
	GL_COLOR_ATTACHMENT12                  = 0x8CEC,
	GL_COLOR_ATTACHMENT13                  = 0x8CED,
	GL_COLOR_ATTACHMENT14                  = 0x8CEE,
	GL_COLOR_ATTACHMENT15                  = 0x8CEF,
	GL_DEPTH_ATTACHMENT                    = 0x8D00,
	GL_STENCIL_ATTACHMENT                  = 0x8D20,
	GL_FRAMEBUFFER_COMPLETE                            = 0x8CD5,
	GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT               = 0x8CD6,
	GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT       = 0x8CD7,
	GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS               = 0x8CD9,
	GL_FRAMEBUFFER_INCOMPLETE_FORMATS                  = 0x8CDA,
	GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER              = 0x8CDB,
	GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER              = 0x8CDC,
	GL_FRAMEBUFFER_UNSUPPORTED                         = 0x8CDD,
	GL_FRAMEBUFFER_BINDING                 = 0x8CA6,
	GL_RENDERBUFFER_BINDING                = 0x8CA7,
	GL_MAX_COLOR_ATTACHMENTS               = 0x8CDF,
	GL_MAX_RENDERBUFFER_SIZE               = 0x84E8,
	GL_INVALID_FRAMEBUFFER_OPERATION       = 0x0506,

#endif
#ifndef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
	// EXT_texture_compression_s3tc:

	GL_COMPRESSED_RGB_S3TC_DXT1_EXT        = 0x83F0,
	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT       = 0x83F1,
	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT       = 0x83F2,
	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT       = 0x83F3,

#endif
#ifndef GL_READ_FRAMEBUFFER
	// EXT_framebuffer_blit:

	GL_READ_FRAMEBUFFER                    = 0x8CA8,
	GL_DRAW_FRAMEBUFFER                    = 0x8CA9,
	GL_DRAW_FRAMEBUFFER_BINDING            = 0x8CA6, // alias GL_FRAMEBUFFER_BINDING
	GL_READ_FRAMEBUFFER_BINDING            = 0x8CAA
#endif
};

#endif

/// \}
