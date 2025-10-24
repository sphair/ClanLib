/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//: <p>This is the heart of ClanLib. The core library contain the glue that
//: binds all other clanlib libraries together. It contain general platform
//: independence classes and functions. It also setup the keep alive pump and
//: other fundamental attributes of ClanLib.</p>
//! Global=Core

#ifndef header_core
#define header_core

#if _MSC_VER > 1000
#pragma once
#endif

#define CL_VERSION(x,y,z)	( (x << 16) | (y << 8) | (z) )
#define CL_CURRENT_VERSION	CL_VERSION(1,0,0)
#define CL_VERSION_STRING "1.0.0"

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#ifdef __BORLANDC__
#define BORLAND
#endif

#include "Core/core_iostream.h"
#include "Core/System/cl_assert.h"
#include "Core/System/command_line.h"
#include "Core/System/setupcore.h"
#include "Core/System/keep_alive.h"
#include "Core/System/system.h"
#include "Core/System/error.h"
#include "Core/System/thread.h"
#include "Core/System/threadfunc_v0.h"
#include "Core/System/threadfunc_v1.h"
#include "Core/System/threadfunc_v2.h"
#include "Core/System/mutex.h"
#include "Core/System/clanstring.h"
#include "Core/System/timer.h"
#include "Core/System/event_listener.h"
#include "Core/System/event_trigger.h"
#include "Core/System/console_window.h"
#include "Core/System/log.h"
#include "Core/System/clipboard.h"
#include "Core/System/cl_library.h"
#include "Core/System/crash_reporter.h"
#include "Core/System/call_stack.h"
#include "Core/System/clonable.h"
#include "Core/System/lazycopyptr.h"
#include "Core/System/owningptr.h"
#include "Core/System/sharedptr.h"
#include "Core/System/weakptr.h"
#include "Core/System/mutexsharedptr.h"
#include "Core/System/mutexweakptr.h"

#include "Core/IOData/datatypes.h"
#include "Core/IOData/cl_endian.h"
#include "Core/IOData/inputsource.h"
#include "Core/IOData/inputsource_file.h"
#include "Core/IOData/inputsource_memory.h"
#include "Core/IOData/inputsource_provider.h"
#include "Core/IOData/inputsource_provider_file.h"
#include "Core/IOData/outputsource.h"
#include "Core/IOData/outputsource_file.h"
#include "Core/IOData/outputsource_memory.h"
#include "Core/IOData/outputsource_provider.h"
#include "Core/IOData/directory_scanner.h"
#include "Core/IOData/directory.h"
#include "Core/IOData/zip_archive.h"
#include "Core/IOData/zip_file_entry.h"

#include "Core/Resources/resource_manager.h"
#include "Core/Resources/resource.h"
#include "Core/Resources/resourcetype_boolean.h"
#include "Core/Resources/resourcetype_integer.h"
#include "Core/Resources/resourcetype_float.h"
#include "Core/Resources/resourcetype_string.h"
#include "Core/Resources/resourcetype_raw.h"

#include "Core/Math/matrix4x4.h"
#include "Core/Math/cl_vector.h"
#include "Core/Math/vector2.h"
#include "Core/Math/math.h"
#include "Core/Math/line_math.h"
#include "Core/Math/triangle_math.h"
#include "Core/Math/rect.h"
#include "Core/Math/quad.h"
#include "Core/Math/size.h"
#include "Core/Math/point.h"
#include "Core/Math/number_pool.h"
#include "Core/Math/delauney_triangulator.h"
#include "Core/Math/outline_triangulator.h"

#include "Core/XML/xml_token.h"
#include "Core/XML/xml_tokenizer.h"
#include "Core/XML/xml_writer.h"
#include "Core/XML/dom_attr.h"
#include "Core/XML/dom_cdata_section.h"
#include "Core/XML/dom_character_data.h"
#include "Core/XML/dom_comment.h"
#include "Core/XML/dom_document.h"
#include "Core/XML/dom_document_fragment.h"
#include "Core/XML/dom_document_type.h"
#include "Core/XML/dom_element.h"
#include "Core/XML/dom_entity.h"
#include "Core/XML/dom_entity_reference.h"
#include "Core/XML/dom_exception.h"
#include "Core/XML/dom_implementation.h"
#include "Core/XML/dom_named_node_map.h"
#include "Core/XML/dom_node.h"
#include "Core/XML/dom_node_list.h"
#include "Core/XML/dom_notation.h"
#include "Core/XML/dom_processing_instruction.h"
#include "Core/XML/dom_text.h"

#if defined (_MSC_VER)
#if !defined (_MT)
#error Your application is set to link with the single threaded version of the run-time library. Go to project settings, in the C++ section, and change it from single threaded to multithreaded.
#endif
#if !defined (_DEBUG)
#pragma comment(lib, "clanCore-static-mt.lib")
#pragma comment(lib, "zlib-static-mt.lib")
#else
#pragma comment(lib, "clanCore-static-mt-debug.lib")
#pragma comment(lib, "zlib-static-mt-debug.lib")
#endif

#pragma comment(lib, "winmm.lib")

#endif

#endif
