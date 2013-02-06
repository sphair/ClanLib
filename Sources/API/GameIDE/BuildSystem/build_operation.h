/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

/// \addtogroup clanGameIDE_System clanGameIDE System
/// \{

#pragma once
namespace clan
{
class SolutionModel;

class BuildOperation
{
public:
	BuildOperation(SolutionModel *solution);
	void build();
	void stop_build();
	bool is_building() const;
	std::string get_info_log();

	void write_log_line(const std::string &text);
	void add_file(const std::string &file);
	std::vector<std::string> get_files_with_extension(const std::string &extension);

private:
	std::map<std::string, bool> files;
	std::string info_log;
};
}

/// \}
