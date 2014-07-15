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

#include "Scene3D/precomp.h"
#include "API/Scene3D/Performance/gpu_timer.h"
#include "API/Core/System/comptr.h"
#ifdef WIN32
#include <d3d11.h>
#endif

namespace clan
{

class GPUTimer_Impl
{
public:
#ifdef WIN32
	struct Frame
	{
		std::vector<std::string> names;
		std::vector<ComPtr<ID3D11Query> > queries;
		ComPtr<ID3D11Query> disjoint_query;
	};

	void timestamp(GraphicContext &gc);

	std::vector<ComPtr<ID3D11Query> > unused_queries;
	std::vector<ComPtr<ID3D11Query> > unused_disjoint_queries;

	std::vector<std::shared_ptr<Frame> > frames;
	std::vector<GPUTimer::Result> last_results;
#endif
};


GPUTimer::GPUTimer()
	: impl(std::make_shared<GPUTimer_Impl>())
{
}

void GPUTimer::begin_frame(GraphicContext &gc)
{
#ifdef WIN32
	if (impl->unused_disjoint_queries.empty())
	{
		ID3D11Device *device = D3DTarget::get_device_handle(gc);
		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
		desc.MiscFlags = 0;

		ComPtr<ID3D11Query> disjoint_query;
		HRESULT result = device->CreateQuery(&desc, disjoint_query.output_variable());
		if (FAILED(result))
			throw Exception("D3D11Device.CreateQuery(D3D11_QUERY_TIMESTAMP_DISJOINT) failed");

		impl->unused_disjoint_queries.push_back(disjoint_query);
	}

	impl->frames.push_back(std::shared_ptr<GPUTimer_Impl::Frame>(new GPUTimer_Impl::Frame()));
	impl->frames.back()->queries.reserve(impl->unused_queries.capacity());
	impl->frames.back()->disjoint_query = impl->unused_disjoint_queries.back();
	impl->unused_disjoint_queries.pop_back();

	ID3D11DeviceContext *context = D3DTarget::get_device_context_handle(gc);
	context->Begin(impl->frames.back()->disjoint_query.get());
#endif
}

void GPUTimer::begin_time(GraphicContext &gc, const std::string &name)
{
#ifdef WIN32
	impl->frames.back()->names.push_back(name);
	impl->timestamp(gc);
#endif
}

void GPUTimer::end_time(GraphicContext &gc)
{
#ifdef WIN32
	impl->timestamp(gc);
#endif
}

void GPUTimer::end_frame(GraphicContext &gc)
{
#ifdef WIN32
	ID3D11DeviceContext *context = D3DTarget::get_device_context_handle(gc);
	context->End(impl->frames.back()->disjoint_query.get());
#endif
}

std::vector<GPUTimer::Result> GPUTimer::get_results(GraphicContext &gc)
{
#ifdef WIN32
	ID3D11DeviceContext *context = D3DTarget::get_device_context_handle(gc);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjoint_data;
	if (context->GetData(impl->frames.front()->disjoint_query.get(), &disjoint_data, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0) != S_OK)
		return impl->last_results;

	std::vector<Result> results;
	for (size_t i = 0; i < impl->frames.front()->names.size(); i++)
	{
		UINT64 start = 0;
		if (context->GetData(impl->frames.front()->queries[i * 2].get(), &start, sizeof(UINT64), 0) != S_OK)
			return impl->last_results;

		UINT64 end = 0;
		if (context->GetData(impl->frames.front()->queries[i * 2 + 1].get(), &end, sizeof(UINT64), 0) != S_OK)
			return impl->last_results;

		float time_elapsed = (float)((end - start) / (double)disjoint_data.Frequency);
		results.push_back(Result(impl->frames.front()->names[i], time_elapsed));
	}
	impl->last_results = results;

	impl->unused_queries.insert(impl->unused_queries.end(), impl->frames.front()->queries.begin(), impl->frames.front()->queries.end());
	impl->unused_disjoint_queries.push_back(impl->frames.front()->disjoint_query);
	impl->frames.erase(impl->frames.begin());

	return results;
#else
	return std::vector<GPUTimer::Result>();
#endif
}

#ifdef WIN32
void GPUTimer_Impl::timestamp(GraphicContext &gc)
{
	if (unused_queries.empty())
	{
		ID3D11Device *device = D3DTarget::get_device_handle(gc);
		D3D11_QUERY_DESC desc;
		desc.Query = D3D11_QUERY_TIMESTAMP;
		desc.MiscFlags = 0;

		ComPtr<ID3D11Query> query;
		HRESULT result = device->CreateQuery(&desc, query.output_variable());
		if (FAILED(result))
			throw Exception("D3D11Device.CreateQuery(D3D11_QUERY_TIMESTAMP) failed");

		unused_queries.push_back(query);
	}

	frames.back()->queries.push_back(unused_queries.back());
	unused_queries.pop_back();

	ID3D11DeviceContext *context = D3DTarget::get_device_context_handle(gc);
	context->End(frames.back()->queries.back().get());
}
#endif

}
