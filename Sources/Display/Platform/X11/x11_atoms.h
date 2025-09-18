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
**    Chu Chin Kuan
*/
#include <algorithm>
#include <cassert>
#include <cstring> // memset
#include <map>
#include <string>
#include <vector>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "API/Core/Text/logger.h"

namespace clan
{
	using X11AtomMap = std::map< std::string, Atom >;

	class X11Atoms
	{
	public:
		X11Atoms() : _display_(nullptr) {  }
		X11Atoms(::Display *display) : _display_(display)
		{
			log_event("debug", "Initializing X11 Display Atoms...");
			for (const auto &elem : _atoms_)
			{
				_map_[elem] = XInternAtom(_display_, elem.c_str(), True);
				log_event("debug", "  %1\t: %2", elem, (_map_[elem] == None) ? "None" : "OK");
			}
		}

		Atom &operator[](const std::string &elem)
		{
			auto iter = _map_.find(elem);
			assert(iter != _map_.end());
			return iter->second;
		}

		const Atom &operator[](const std::string &elem) const
		{
			auto iter = _map_.find(elem);
			assert(iter != _map_.end());
			return iter->second;
		}

		bool exists(const std::string &elem) const
		{
			auto iter = _map_.find(elem);
			if (iter != _map_.end())
				return iter->second != None;
			else
				return false;
		}

		Atom get_atom(::Display *display, const char *elem, bool only_if_exists)
		{
			assert(display == _display_); // Safety check.
			auto iter = _map_.find(elem);
			if (iter != _map_.end())
			{
				return iter->second;
			}
			else
			{
				Atom atom = XInternAtom(display, elem, only_if_exists);
				_map_[std::string(elem)] = atom;
				return atom;
			}
		}

		void clear()
		{
			_map_.clear();
		}

		// Important: Use XFree() on the returned pointer (if not NULL)
		static unsigned char *get_property(::Display *display, Window window, Atom property, Atom &actual_type, int &actual_format, unsigned long &item_count);
		static unsigned char *get_property(::Display *display, Window window, Atom property, unsigned long &item_count);
		unsigned char *get_property(Window window, const std::string &property, unsigned long &item_count) const
		{
			return get_property(_display_, window, (*this)[property], item_count);
		}

		std::vector<bool> check_net_wm_state(Window window, std::vector<std::string> state_atoms) const;
		bool modify_net_wm_state(Window window, long action, const std::string &atom1, const std::string &atom2);

	public:
		//! List of all atoms handled by ClanLib.
		static const std::vector< std::string > _atoms_;

	private:
		::Display* _display_;
		X11AtomMap _map_;
	};
}

