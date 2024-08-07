/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Render/transfer_texture.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"

namespace clan
{
	TransferTexture::TransferTexture()
	{
	}

	TransferTexture::TransferTexture(std::unique_ptr<PixelBufferProvider> provider) : PixelBuffer(std::move(provider))
	{
	}

	TransferTexture::TransferTexture(GraphicContext &gc, int width, int height, PixelBufferDirection direction, TextureFormat texture_format, const void *data, BufferUsage usage)
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		std::unique_ptr<PixelBufferProvider> provider = gc_provider->alloc_pixel_buffer();
		provider->create(data, Size(width, height), direction, texture_format, usage);
		*this = TransferTexture(std::move(provider));
	}

	TransferTexture::TransferTexture(GraphicContext &gc, const PixelBuffer &pbuff, PixelBufferDirection direction, BufferUsage usage)
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		std::unique_ptr<PixelBufferProvider> provider = gc_provider->alloc_pixel_buffer();
	
		provider->create(pbuff.get_data(), pbuff.get_size(), direction, pbuff.get_format(), usage);
		*this = TransferTexture(std::move(provider));
	}

	TransferTexture::~TransferTexture()
	{
	}
}
