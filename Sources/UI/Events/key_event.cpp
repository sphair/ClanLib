
#include "UI/precomp.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/View/view.h"

namespace clan
{
	Pointf KeyEvent::pointer_pos(View *view) const
	{
		return view->from_root_pos(_pointer_pos);
	}
}
