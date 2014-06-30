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

#pragma once
#include <functional>

namespace clan
{
/// \addtogroup clanCore_Signals clanCore Signals
/// \{

	template<typename R, typename T>
	std::function<R()> bind_member(T *instance, R(T::*func)()) { return [=]() -> R { return (instance->*func)(); }; }

	template<typename R, typename T>
	std::function<void()> bind_member(T *instance, void(T::*func)()) { return [=]() { (instance->*func)(); }; }

	template<typename R, typename T, typename Arg1>
	std::function<R(Arg1)> bind_member(T *instance, R(T::*func)(Arg1)) { return [=](Arg1 arg1) -> R { return (instance->*func)(arg1); }; }

	template<typename R, typename T, typename Arg1>
	std::function<void(Arg1)> bind_member(T *instance, void(T::*func)(Arg1)) { return [=](Arg1 arg1) { (instance->*func)(arg1); }; }

	template<typename R, typename T, typename Arg1, typename Arg2>
	std::function<R(Arg1, Arg2)> bind_member(T *instance, R(T::*func)(Arg1, Arg2)) { return [=](Arg1 arg1, Arg2 arg2) -> R { return (instance->*func)(arg1, arg2); }; }

	template<typename R, typename T, typename Arg1, typename Arg2>
	std::function<void(Arg1, Arg2)> bind_member(T *instance, void(T::*func)(Arg1, Arg2)) { return [=](Arg1 arg1, Arg2 arg2) { (instance->*func)(arg1, arg2); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3>
	std::function<R(Arg1, Arg2, Arg3)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) -> R { return (instance->*func)(arg1, arg2, arg3); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3>
	std::function<void(Arg1, Arg2, Arg3)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3) { (instance->*func)(arg1, arg2, arg3); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	std::function<R(Arg1, Arg2, Arg3, Arg4)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) -> R { return (instance->*func)(arg1, arg2, arg3, arg4); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
	std::function<void(Arg1, Arg2, Arg3, Arg4)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4) { (instance->*func)(arg1, arg2, arg3, arg4); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5) { (instance->*func)(arg1, arg2, arg3, arg4, arg5); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6) { (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7) { (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8) { (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9) { (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	std::function<R(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)> bind_member(T *instance, R(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) -> R { return (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); }; }

	template<typename R, typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Arg10>
	std::function<void(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)> bind_member(T *instance, void(T::*func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10)) { return [=](Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Arg10 arg10) { (instance->*func)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10); }; }
}
/// \}
