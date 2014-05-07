/*
 * *  ClanLib SDK
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

#include <vector>
#include "callback.h"
#include "signal.h"

namespace clan {

// Disabled below to stop the Suse Linux warning: 'clan::Callback<void()>' has a field 'clan::Callback<void()>::impl' whose type uses the anonymous namespace [enabled by default]
//namespace {

class AutoDisconnector
{
public:
    virtual ~AutoDisconnector() {}
};

template<class... Args>
class AutoDisconnectorT : public AutoDisconnector
{
public:
    AutoDisconnectorT(const Signal<Args...> &signal, const Callback<void(Args...)> &callback) :
        signal(signal), callback(callback)
    {
        this->signal.connect(callback);
    }
    virtual ~AutoDisconnectorT()
    {
        signal.disconnect(callback);
        callback.clear();
    }
private:
    Signal<Args...> signal;
    Callback<void(Args...)> callback;
};

//}

class CallbackContainer
{
public:
    CallbackContainer() : callbacks() {}

    template<class... Args>
    void connect(const Signal<Args...> &signal, const Callback<void(Args...)> &callback)
    {
        callbacks.emplace_back(new AutoDisconnectorT<Args...>(signal, callback));
    }

    ~CallbackContainer()
    {
    }
private:
    std::vector<std::unique_ptr<AutoDisconnector>> callbacks;
};

}
