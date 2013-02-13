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

#pragma once

namespace clan
{

class GPUTimer
{
public:
	GPUTimer();
	~GPUTimer();

	void begin_frame(clan::GraphicContext &gc);

	void begin_time(clan::GraphicContext &gc, const std::string &name);
	void end_time(clan::GraphicContext &gc);

	void end_frame(clan::GraphicContext &gc);

	struct Result
	{
		Result(std::string name, float time_elapsed) : name(name), time_elapsed(time_elapsed) { }
		std::string name;
		float time_elapsed;
	};

	std::vector<Result> get_results(clan::GraphicContext &gc);

private:
	struct Frame
	{
		std::vector<std::string> names;
		std::vector<clan::ComPtr<ID3D11Query> > queries;
		clan::ComPtr<ID3D11Query> disjoint_query;
	};

	void timestamp(clan::GraphicContext &gc);

	std::vector<clan::ComPtr<ID3D11Query> > unused_queries;
	std::vector<clan::ComPtr<ID3D11Query> > unused_disjoint_queries;

	std::vector<std::shared_ptr<Frame> > frames;
	std::vector<Result> last_results;
};

}
