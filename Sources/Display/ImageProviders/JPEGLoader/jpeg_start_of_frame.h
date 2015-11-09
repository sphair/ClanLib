/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

namespace clan
{
	class JPEGStartOfFrameComponent;

	class JPEGStartOfFrame
	{
	public:
		JPEGStartOfFrame() : sample_precision(0), height(0), width(0) { }

		uint8_t sample_precision;
		uint16_t height;
		uint16_t width;
		std::vector<JPEGStartOfFrameComponent> components;
	};

	class JPEGStartOfFrameComponent
	{
	public:
		JPEGStartOfFrameComponent() : id(0), horz_sampling_factor(0), vert_sampling_factor(0), quantization_table_selector(0) { }

		// Component identifier – Assigns a unique label to the component in the sequence of frame component specification parameters.
		uint8_t id;

		// specifies the number of horizontal data units of the component in each MCU, when more than one component is encoded in a scan.
		uint8_t horz_sampling_factor;

		// Specifies the number of vertical data units of the component in each MCU, when more than one component is encoded in a scan.
		uint8_t vert_sampling_factor;

		// Specifies one of four possible quantization table destinations from which the quantization table to use for dequantization of DCT coefficients of the component.
		uint8_t quantization_table_selector;
	};
}
