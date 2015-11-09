
#include "UI/precomp.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/View/view.h"

namespace clan
{
	Pointf PointerEvent::pos(View *view) const
	{
		return view->from_root_pos(_pos);
	}

	Pointf PointerEvent::pos(const std::shared_ptr<View> &view) const
	{
		return view->from_root_pos(_pos);
	}

	void PointerEvent::set_pos(View *view, const Pointf &pos)
	{
		_pos = view->to_root_pos(pos);
	}
}
