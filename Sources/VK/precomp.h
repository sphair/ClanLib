/*
**  ClanLib SDK
**  Copyright (c) 1997-2026 The ClanLib Team
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
**	claim that you wrote the original software. If you use this software
**	in a product, an acknowledgment in the product documentation would be
**	appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**	misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**	Magnus Norddahl
**	Mark Page
*/

#pragma once

// ---- Platform ----
#ifdef _WIN32
#  ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#  endif
#  ifndef NOMINMAX
#	define NOMINMAX
#  endif
#  include <windows.h>
#endif

// ---- Vulkan (core only – platform surfaces included per-file) ----
#include <vulkan/vulkan.h>

// ---- Standard library ----
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <unordered_map>

// ---- ClanLib core ----
#include "API/Core/System/exception.h"
#include "API/Core/Text/logger.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Image/pixel_buffer.h"
