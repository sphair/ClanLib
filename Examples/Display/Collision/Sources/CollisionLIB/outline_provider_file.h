/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
*/


#pragma once

class Contour;
class OutlineProviderFile_Impl;

/// \brief File outline provider is used to load precompiled outlines.
///
/// <p>A OutlineProviderFile is used to load precompiled outlines.</p> 
class OutlineProviderFile
{
/// \name Construction
/// \{

 public:
	/// \brief Construct a outline provider
	///
	/// \param file = file to load.
	OutlineProviderFile(clan::IODevice &file);

	~OutlineProviderFile();

/// \}
/// \name Attributes
/// \{

 public:
	/// \brief return the countours that make up the outline
	std::vector<Contour> get_contours();

	/// \brief Not used for file provider. Returns -1.
	clan::Size get_size();

/// \}
/// \name Operations
/// \{

public:

/// \}
/// \name Implementation
/// \{

 private:
	std::shared_ptr<OutlineProviderFile_Impl> impl;
/// \}
};
