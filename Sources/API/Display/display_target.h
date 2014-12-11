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

#include <memory>

namespace clan
{
/// \addtogroup clanDisplay_Display clanDisplay Display
/// \{

class DisplayTargetProvider;
class DisplayTarget_Impl;

/// \brief Display target for clanDisplay.
class DisplayTarget
{
/// \name Construction
/// \{

public:
	/// \brief Constructs a DisplayTarget
	///
	/// \param provider = Display Target Provider
	DisplayTarget(DisplayTargetProvider *provider);

	virtual ~DisplayTarget();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the provider for the display target.
	DisplayTargetProvider *get_provider();

	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Set this display target to be the current target
	void set_current();

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a null DisplayTarget
	DisplayTarget();

	/// \brief Constructs a DisplayTarget
	///
	/// \param DisplayTarget_Impl = Weak Ptr
	DisplayTarget(const std::weak_ptr<DisplayTarget_Impl> impl);

	std::shared_ptr<DisplayTarget_Impl> impl;

	friend class DisplayTarget_Impl;
/// \}
};

}

/// \}
