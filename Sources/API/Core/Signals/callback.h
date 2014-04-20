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

#include <functional>
#include <memory>

namespace clan
{
    /// \addtogroup clanCore_Signals clanCore Signals
    /// \{

    namespace
    {
        /// (Internal ClanLib Class)
        template<class Res, class... Params>
        class Callback_impl
        {
        public:
            virtual Res invoke(const Params & ... params) const = 0;
        };

        template<class Res, class... Params>
        class Callback_impl_without : public Callback_impl<Res, Params...>
        {
        public:
            Callback_impl_without(const std::function<Res(Params...)> &function)
            : func(function) { return; }

            virtual Res invoke(const Params & ... params) const override
            {
                return func(params...);
            }
        private:
            const std::function<Res(Params...)> func;
        };

        /// (Internal ClanLib Class)
        template<class Res, class UserData, class... Params>
        class Callback_impl_with : public Callback_impl<Res, Params...>
        {
        public:
            Callback_impl_with(const std::function<Res(Params..., UserData)> &function, const UserData &userdata)
            : func(function), userdata(userdata) { return; }

            virtual Res invoke(const Params & ... params) const override
            {
                return func(params..., userdata);
            }
        private:
            const std::function<Res(Params..., UserData)> func;
            const UserData userdata;
        };
    }

    /// (Internal ClanLib Class)
    template<class... Args>
    class Callback;

    /// \brief Callback
    template<class Res, class... Params>
    class Callback<Res(Params...)>
    {
    public:
        Callback()
        : impl() { return; }

        Callback(const Callback& other)
        : impl(other.impl) { return; }

        Callback(const std::function<Res(Params...)> &function)
        : impl(new Callback_impl_without<Res, Params...>(function)) { return; }

        template<class UserData>
        Callback(const std::function<Res(Params..., UserData)> &function, const UserData &userdata)
        : impl(new Callback_impl_with<Res, UserData, Params...>(function, userdata)) { return; }

        template<class Instance>
        Callback(Instance *instance, Res(Instance::*function)(Params...))
        : Callback([=](const Params & ... params){ return (instance->*function)(params...); }) { return; }

        template<class Instance, class UserData>
        Callback(Instance *instance, Res(Instance::*function)(Params..., UserData), const UserData &userdata)
        : Callback([=](const Params & ... params, const UserData &userdata)
                   { return (instance->*function)(params..., userdata); },userdata) { return; }

        Res invoke(const Params & ... params) const
        {
            return impl->invoke(params...);
        }

        void set(const std::function<Res(Params...)> &function)
        {
            impl = std::make_shared<Callback_impl_without<Res, Params...>>(function);
        }

        template<class UserData>
        void set(const std::function<Res(Params..., UserData)> &function, const UserData &userdata)
        {
            impl = std::make_shared<Callback_impl_with<Res, UserData, Params...>>(function, userdata);
        }

        template<class Instance>
        void set(Instance *instance, Res(Instance::*function)(Params...))
        {
            set([instance, function](const Params & ... params){ return (instance->*function)(params...); });
        }

        template<class Instance, class UserData>
        void set(Instance *instance, Res(Instance::*function)(Params..., UserData), const UserData &userdata)
        {
            set<UserData>([instance, function](const Params & ... params, const UserData &ud)
                          { return (instance->*function)(params..., ud); }, userdata);
        }

        void clear() const
        {
            impl = nullptr;
        }

        bool is_null() const
        {
            return !impl;
        }
    private:
        std::shared_ptr<Callback_impl<Res, Params...>> impl;
    };


    template<class Res> using Callback_0 = Callback<Res()>;
    using Callback_v0 = Callback_0<void>;

    template<class Res, class A> using Callback_1 = Callback<Res(A)>;
    template<class A> using Callback_v1 = Callback_1<void, A>;

    template<class Res, class A, class B> using Callback_2 = Callback<Res(A, B)>;
    template<class A, class B> using Callback_v2 = Callback_2<void, A, B>;

    template<class Res, class A, class B, class C> using Callback_3 = Callback<Res(A, B, C)>;
    template<class A, class B, class C> using Callback_v3 = Callback_3<void, A, B, C>;

    template<class Res, class A, class B, class C, class D> using Callback_4 = Callback<Res(A, B, C, D)>;
    template<class A, class B, class C, class D> using Callback_v4 = Callback_4<void, A, B, C, D>;

    template<class Res, class A, class B, class C, class D, class E> using Callback_5 = Callback<Res(A, B, C, D, E)>;
    template<class A, class B, class C, class D, class E> using Callback_v5 = Callback_5<void, A, B, C, D, E>;

    template<class Res, class A, class B, class C, class D, class E, class F> using Callback_6 = Callback<Res(A, B, C, D, E, F)>;
    template<class A, class B, class C, class D, class E, class F> using Callback_v6 = Callback_6<void, A, B, C, D, E, F>;
}
