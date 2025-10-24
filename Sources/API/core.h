/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

/// \brief <p>This is the heart of ClanLib. The core library contain the glue that
/// \brief binds all other clanlib libraries together.</p>
//! Global=Core

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif

#define CL_VERSION(x,y,z)	( (x << 16) | (y << 8) | (z) )
#define CL_CURRENT_VERSION	CL_VERSION(2,0,2)
#define CL_VERSION_STRING "2.0.2"

#ifdef _MSC_VER
#pragma warning( disable : 4786)
#endif

#ifdef __BORLANDC__
#define BORLAND
#endif

#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif

#ifdef WIN32
#include <windows.h>  // this is a temporary workaround. Fix mutex and others :)
#endif

#include "Core/Text/string_types.h"
#include "Core/Text/file_logger.h"
#include "Core/Text/console.h"
#include "Core/Text/console_logger.h"
#include "Core/Text/logger.h"
#include "Core/Text/string_format.h"
#include "Core/Text/string_help.h"
#include "Core/Text/string_allocator.h"
#include "Core/System/databuffer.h"
#include "Core/System/block_allocator.h"
#include "Core/System/console_window.h"
#include "Core/System/datetime.h"
#include "Core/System/event.h"
#include "Core/System/event_provider.h"
#include "Core/System/exception.h"
#include "Core/System/mutex.h"
#include "Core/System/runnable.h"
#include "Core/System/service.h"
#include "Core/System/setup_core.h"
#include "Core/System/thread.h"
#include "Core/System/thread_local_storage.h"
#include "Core/System/autoptr.h"
#include "Core/System/weakptr.h"
#include "Core/System/sharedptr.h"
#include "Core/System/system.h"
#include "Core/System/command_line.h"
#include "Core/System/memory_pool.h"
#include "Core/System/static_memory_pool.h"
#include "Core/System/fixed_memory_pool.h"
#include "Core/System/keep_alive.h"
#include "Core/System/registry_key.h"
#include "Core/Signals/callback_0.h"
#include "Core/Signals/callback_1.h"
#include "Core/Signals/callback_2.h"
#include "Core/Signals/callback_3.h"
#include "Core/Signals/callback_4.h"
#include "Core/Signals/callback_5.h"
#include "Core/Signals/callback_v0.h"
#include "Core/Signals/callback_v1.h"
#include "Core/Signals/callback_v2.h"
#include "Core/Signals/callback_v3.h"
#include "Core/Signals/callback_v4.h"
#include "Core/Signals/callback_v5.h"
#include "Core/Signals/signal_v0.h"
#include "Core/Signals/signal_v1.h"
#include "Core/Signals/signal_v2.h"
#include "Core/Signals/signal_v3.h"
#include "Core/Signals/signal_v4.h"
#include "Core/Signals/signal_v5.h"
#include "Core/Signals/virtual_function_0.h"
#include "Core/Signals/virtual_function_1.h"
#include "Core/Signals/virtual_function_2.h"
#include "Core/Signals/virtual_function_3.h"
#include "Core/Signals/virtual_function_4.h"
#include "Core/Signals/virtual_function_5.h"
#include "Core/Signals/slot.h"
#include "Core/Signals/slot_container.h"
#include "Core/Resources/resource.h"
#include "Core/Resources/resource_manager.h"
#include "Core/Resources/resource_data_session.h"
#include "Core/XML/dom_processing_instruction.h"
#include "Core/XML/dom_entity_reference.h"
#include "Core/XML/dom_notation.h"
#include "Core/XML/dom_exception.h"
#include "Core/XML/dom_cdata_section.h"
#include "Core/XML/dom_document_fragment.h"
#include "Core/XML/dom_comment.h"
#include "Core/XML/dom_attr.h"
#include "Core/XML/dom_entity.h"
#include "Core/XML/dom_document_type.h"
#include "Core/XML/dom_document.h"
#include "Core/XML/dom_implementation.h"
#include "Core/XML/dom_node_list.h"
#include "Core/XML/dom_text.h"
#include "Core/XML/dom_node.h"
#include "Core/XML/dom_character_data.h"
#include "Core/XML/dom_named_node_map.h"
#include "Core/XML/dom_element.h"
#include "Core/XML/dom_string.h"
#include "Core/XML/xml_tokenizer.h"
#include "Core/XML/xml_writer.h"
#include "Core/XML/xml_token.h"
#include "Core/XML/xpath_evaluator.h"
#include "Core/XML/xpath_object.h"
#include "Core/CSS/css_document.h"
#include "Core/CSS/css_import.h"
#include "Core/CSS/css_property.h"
#include "Core/CSS/css_ruleset.h"
#include "Core/CSS/css_selector.h"
#include "Core/IOData/access_control_list.h"
#include "Core/IOData/file.h"
#include "Core/IOData/file_help.h"
#include "Core/IOData/path_help.h"
#include "Core/IOData/iodevice.h"
#include "Core/IOData/pipe_connection.h"
#include "Core/IOData/pipe_listen.h"
#include "Core/IOData/security_descriptor.h"
#include "Core/IOData/datatypes.h"
#include "Core/IOData/directory.h"
#include "Core/IOData/security_identifier.h"
#include "Core/IOData/cl_endian.h"
#include "Core/IOData/directory_scanner.h"
#include "Core/IOData/virtual_directory.h"
#include "Core/IOData/virtual_file_system.h"
#include "Core/IOData/virtual_file_source.h"
#include "Core/IOData/iodevice_memory.h"
#include "Core/IOData/virtual_directory_listing.h"
#include "Core/Zip/zip_archive.h"
#include "Core/Zip/zip_writer.h"
#include "Core/Zip/zip_reader.h"
#include "Core/Zip/zip_file_entry.h"
#include "Core/Math/angle.h"
#include "Core/Math/base64_encoder.h"
#include "Core/Math/base64_decoder.h"
#include "Core/Math/bezier_curve.h"
#include "Core/Math/circle.h"
#include "Core/Math/vec1.h"
#include "Core/Math/vec2.h"
#include "Core/Math/vec3.h"
#include "Core/Math/vec4.h"
#include "Core/Math/delauney_triangulator.h"
#include "Core/Math/ear_clip_triangulator.h"
#include "Core/Math/ear_clip_result.h"
#include "Core/Math/line_math.h"
#include "Core/Math/math.h"
#include "Core/Math/mat2.h"
#include "Core/Math/mat3.h"
#include "Core/Math/mat4.h"
#include "Core/Math/origin.h"
#include "Core/Math/outline_triangulator.h"
#include "Core/Math/point.h"
#include "Core/Math/pointset_math.h"
#include "Core/Math/quad.h"
#include "Core/Math/rect.h"
#include "Core/Math/sha1.h"
#include "Core/Math/size.h"
#include "Core/Math/triangle_math.h"
#include "Core/Math/line.h"
#include "Core/Math/line_ray.h"
#include "Core/Math/line_segment.h"
#include "Core/core_iostream.h"

#ifdef __cplusplus_cli
#pragma managed(pop)
#endif

#if defined (_MSC_VER)
	#if !defined (_MT)
		#error Your application is set to link with the single threaded version of the run-time library. Go to project settings, in the C++ section, and change it from single threaded to multi threaded.
	#endif
	#if !defined (UNICODE)
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-dll.lib")
				#else
					#pragma comment(lib, "clanCore-dll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-dll-debug.lib")
				#else
					#pragma comment(lib, "clanCore-dll-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mtdll.lib")
					#pragma comment(lib, "zlib-x64-static-mtdll.lib")
				#else
					#pragma comment(lib, "clanCore-static-mtdll.lib")
					#pragma comment(lib, "zlib-static-mtdll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mtdll-debug.lib")
					#pragma comment(lib, "zlib-x64-static-mtdll-debug.lib")
				#else
					#pragma comment(lib, "clanCore-static-mtdll-debug.lib")
					#pragma comment(lib, "zlib-static-mtdll-debug.lib")
				#endif
			#endif
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mt.lib")
					#pragma comment(lib, "zlib-x64-static-mt.lib")
				#else
					#pragma comment(lib, "clanCore-static-mt.lib")
					#pragma comment(lib, "zlib-static-mt.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mt-debug.lib")
					#pragma comment(lib, "zlib-x64-static-mt-debug.lib")
				#else
					#pragma comment(lib, "clanCore-static-mt-debug.lib")
					#pragma comment(lib, "zlib-static-mt-debug.lib")
				#endif
			#endif
		#endif
	#else
		#if defined (CL_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-dll-uc.lib")
				#else
					#pragma comment(lib, "clanCore-dll-uc.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-dll-uc-debug.lib")
				#else
					#pragma comment(lib, "clanCore-dll-uc-debug.lib")
				#endif
			#endif
		#elif defined (_DLL)
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mtdll-uc.lib")
					#pragma comment(lib, "zlib-x64-static-mtdll.lib")
				#else
					#pragma comment(lib, "clanCore-static-mtdll-uc.lib")
					#pragma comment(lib, "zlib-static-mtdll.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "zlib-x64-static-mtdll-debug.lib")
				#else
					#pragma comment(lib, "clanCore-static-mtdll-uc-debug.lib")
					#pragma comment(lib, "zlib-static-mtdll-debug.lib")
				#endif
			#endif
		#else
			#if !defined (_DEBUG)
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mt-uc.lib")
					#pragma comment(lib, "zlib-x64-static-mt.lib")
				#else
					#pragma comment(lib, "clanCore-static-mt-uc.lib")
					#pragma comment(lib, "zlib-static-mt.lib")
				#endif
			#else
				#if defined(_M_X64)
					#pragma comment(lib, "clanCore-x64-static-mt-uc-debug.lib")
					#pragma comment(lib, "zlib-x64-static-mt-debug.lib")
				#else
					#pragma comment(lib, "clanCore-static-mt-uc-debug.lib")
					#pragma comment(lib, "zlib-static-mt-debug.lib")
				#endif
			#endif
		#endif
	#endif
#endif

