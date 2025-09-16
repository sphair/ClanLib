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
*/

#pragma once

#include <memory>

namespace clan
{
	/// \addtogroup clanDisplay_Display clanDisplay Display
	/// \{

	class GraphicContext;
	class RasterizerStateDescription;
	class RasterizerStateProvider;
	class GraphicContextProvider;

	/// \brief Rasterizer state setup.
	class RasterizerState
	{
	public:
		/// \brief Constructs a null instance.
		RasterizerState();

		/// \brief Constructs a Rasterizer State
		///
		/// \param context = Graphic context
		/// \param desc = Rasterizer state description
		RasterizerState(GraphicContext& context, const RasterizerStateDescription& desc);

		/// \brief Constructs a Rasterizer State
		///
		/// \param gc_provider = Graphic content provider
		/// \param desc = Rasterizer state description
		RasterizerState(GraphicContextProvider* gc_provider, const RasterizerStateDescription &desc);

		/// \brief Returns true if this object is invalid.
		bool is_null() const { return !provider; }
		explicit operator bool() const { return bool(provider); }

		RasterizerStateProvider *get_provider() const;

	private:
		std::shared_ptr<RasterizerStateProvider> provider;
	};

	/// \}
}
