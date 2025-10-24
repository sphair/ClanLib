/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Ingo Ruhnke
*/

/// \addtogroup clanCore_System clanCore System
/// \{

#pragma once

#include "../api_core.h"
#include "../Text/string_types.h"
#include <vector>

class CL_CommandLine_Generic;

/// \brief Helper class to parse command line arguments
///
///    <p>The CL_CommandLine class helps to parse command line arguments, namely
///    the argc/argv pair that you get from main(). CL_CommandLine mimics
///    getopt_long() behaviour as closely as possible, while providing a
///    cleaner interface and a few additional features, like automatic
///    generation of '--help' output. CL_CommandLine can parse long arguments
///    in the following styles:</p>
///    -
///    <p>program --file FILENAME<br>
///    program --file=FILENAME</p>
///    -
///    <p>Short arguments are handled like this:</p>
///    -
///    <p>program -f FILENAME</p>
///    -
///    <p>Concatenating short arguments is also supported, so that:</p>
///
///    <p>program -f -a -b FILENAME</p>
///    -
///    <p>is equivalent to:</p>
///    -
///    <p>program -fab FILENAME</p>
///    -
///    <p>Non-option arguments (aka rest arguments) are supported as well:</p>
///    -
///    <p>program SOMEFILE SOMEOTHERFILE ...</p>
///
///    <p>To avoid ambiguity when a filename starts with '-' CL_CommandLine stops
///    parsing arguments after the first encounter of a '--', so in</p>
///    -
///    <p>program -f -b -- -f -b</p>
///    -
///    <p>In the above example the first '-f -b' options are treated as
///    normal while the ones after the '--' are treated as rest arguments
///    (aka filenames in most programs).</p> 
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_CommandLine
{
/// \name Construction
/// \{

public:
	CL_CommandLine();

	~CL_CommandLine();


/// \}
/// \name Attributes
/// \{

public:
	enum
	{
		REST_ARG  = -2
	};

	/// \brief Return the key of the current option
	int get_key();

	/// \brief Return the argument to the current option
	CL_String8 get_argument();


/// \}
/// \name Operations
/// \{

public:
	/// \brief Change the indention used for the help message, default is 18
	void set_help_indent(int i);

	/// \brief Add a usage line to the help output, usage is for example
	/// \brief "[OPTIONS]... FILE", the program name will get appended
	/// \brief automatically
	void add_usage(const CL_String8& usage);

	/// \brief Adds extra documentation to the help output, should only be
	/// \brief used at the beginning or at the end, to
	void add_doc(const CL_String8& doc);

	/// \brief Starts a new group of options, the grouptopic gets printed
	/// \brief above the group of options in the print_help() output
	void add_group(const CL_String8& grouptopic);

	/// \brief Adds a option to the parser
	///
	/// \param key a letter for a short-option or a numeric value
	/// \param key larger than 255 that identifies the option
	/// \param long_option the long name of this option or "" if non
	/// \param long_option should be used for this option
	/// \param argument the type of the argument that this option
	/// \param argument requires (i.e. FILE, SIZE, WIDTH, etc.) or "" if no argument is
	/// \param argument required
	/// \param help the help string for this option
	/// \param visible true if the option should be listed in the help
	/// \param visible output, false will not list it in the help output which might
	/// \param visible be usefull for cheat-options or backward-compability options
	/// \param visible than would only clutter the help output
	void add_option(
		int key,
		const CL_String8& long_option,
		const CL_String8& argument,
		const CL_String8& help,
		bool visible = true);

	/// \brief Parse the options arcording to the options added with
	/// \brief add_option(), result of the parsing is accessible via
	/// \brief next() and get_key()/get_argument()
	void parse_args(int argc, char** argv);

	/// \brief Print the help output, normaly done via a --help option
	void print_help();

	/// \brief Goto the next option
	bool next();


/// \}
/// \name Implementation
/// \{

private:
	CL_CommandLine_Generic* impl;
/// \}
};


/// \}
