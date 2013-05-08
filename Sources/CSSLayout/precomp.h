/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#pragma once

#include <map>
#include <vector>
#include <string>
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/System/exception.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/rect.h"
#include "API/Core/Math/size.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_node.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/XML/dom_text.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/image.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/graphic_context.h"
#include "API/GUI/gui_component.h"
#include "API/CSSLayout/CSSTokenizer/css_tokenizer.h"
#include "API/CSSLayout/CSSTokenizer/css_token.h"
