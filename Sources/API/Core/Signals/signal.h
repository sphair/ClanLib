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
**    Marcel Hellwig
*/

#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include "callback.h"

namespace clan
{
/// \addtogroup clanCore_Signals clanCore Signals
/// \{

/// \brief Signal
template<class... Params>
class CL_API_CORE Signal
{
public:
    /// \name Construction
    /// \{
    Signal()
    : impl(new std::vector<Callback<void(Params...)>>) { return; }

    Signal(const Signal &copy)
    : impl(copy.impl) { return; }

    /// \}
    /// \name Operations
    /// \{

    void connect(const Callback<void(Params...)> &callback)
    {
        impl->push_back(callback);
    }

    void disconnect(const Callback<void(Params...)> &callback)
    {
        impl->erase(std::remove(impl->begin(), impl->end(), callback), impl->end());
    }

    void invoke(Params & ... params) const
    {
        for(auto &cb : *impl)
            if(!cb.is_null())
                cb.invoke(params...);
    }

    /// \}
    /// \name Implementation
    /// \{

private:
    std::shared_ptr<std::vector<Callback<void(Params...)>>> impl;
/// \}
};


using Signal_v0 = Signal<>;
template<class A> using Signal_v1 = Signal<A>;
template<class A, class B> using Signal_v2 = Signal<A, B>;
template<class A, class B, class C> using Signal_v3 = Signal<A, B, C>;
template<class A, class B, class C, class D> using Signal_v4 = Signal<A, B, C, D>;
template<class A, class B, class C, class D, class E> using Signal_v5 = Signal<A, B, C, D, E>;
template<class A, class B, class C, class D, class E, class F> using Signal_v6 = Signal<A, B, C, D, E, F>;

}
/// \}
