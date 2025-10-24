/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

/// \addtogroup clanCore_Text clanCore Text
/// \{

#ifndef _cl_header_string_container_
#define _cl_header_string_container_

#if defined(_MSC_VER)
#pragma once
#endif

#include "../api_core.h"
#include "string_data.h"

class CL_MemoryPool;

/// \brief String container template.
///
///  Basic string container.  Extends StringData with operations that work on string data.
/// \xmlonly !group=Core/Text! !header=core.h! \endxmlonly
template<typename CharType, typename ReferenceClass, typename StdString>
class CL_API_CORE CL_StringContainer : public CL_StringData<CharType, ReferenceClass, StdString>
{
public:

#ifndef WIN32

	// Fix for GCC bug for not finding CL_StringDataTypes::size_type and so on .... grrrrrrr!
	// TODO: Remove this fix when GCC is a good compiler. As from 13 December 2007, SVN GCC does not work
	// See: http:/** gcc.gnu.org/bugzilla/show_bug.cgi?id=34453*/
	typedef unsigned int size_type;
	typedef CharType char_type;
	typedef CharType *iterator;
	typedef const CharType *const_iterator;

	// These could and maybe should be replaced with "this->function_name()"
	iterator begin() { return CL_StringData<CharType, ReferenceClass, StdString>::begin(); }

	/// \brief End
	///
	/// \return iterator
	iterator end() { return CL_StringData<CharType, ReferenceClass, StdString>::end(); }

	/// \brief Size
	///
	/// \return size_type
	size_type size() const { return CL_StringData<CharType, ReferenceClass, StdString>::size(); }

	/// \brief Length
	///
	/// \return size_type
	size_type length() const { return CL_StringData<CharType, ReferenceClass, StdString>::length(); }

	/// \brief Empty
	///
	/// \return bool
	bool empty() const { return CL_StringData<CharType, ReferenceClass, StdString>::empty(); }
	const CharType *data() const { return CL_StringData<CharType, ReferenceClass, StdString>::data(); }

	/// \brief Data
	///
	/// \return Char Type
	CharType *data() { return CL_StringData<CharType, ReferenceClass, StdString>::data(); }

#endif

	CL_StringContainer();

	/// \brief Constructs a StringContainer
	///
	/// \param source = Std String
	CL_StringContainer(const StdString &source);

	/// \brief Constructs a StringContainer
	///
	/// \param source = String Container
	CL_StringContainer(const CL_StringContainer &source);

	/// \brief Constructs a StringContainer
	///
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	CL_StringContainer(const CL_StringData<CharType, ReferenceClass, StdString> &source);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	CL_StringContainer(const char *c_str);

	/// \brief Constructs a StringContainer
	///
	/// \param c_str = char
	/// \param length = size_type
	CL_StringContainer(const char *c_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	CL_StringContainer(const wchar_t *wc_str);

	/// \brief Constructs a StringContainer
	///
	/// \param wc_str = wchar_t
	/// \param length = size_type
	CL_StringContainer(const wchar_t *wc_str, size_type length);

	/// \brief Constructs a StringContainer
	///
	/// \param n = size_type
	/// \param c = Char Type
	CL_StringContainer(size_type n, CharType c);
	~CL_StringContainer();

	/// \brief ReferenceClass
	///
	/// \return operator
	operator ReferenceClass() const;

	const CharType *c_str() const;

	/// \brief Max size
	///
	/// \return size_type
	size_type max_size() const { return (size_type) -1; }

	/// \brief Capacity
	///
	/// \return size_type
	size_type capacity() const { return data_capacity; }

//	void swap(CL_StringContainer &other);
//	static inline swap(CL_StringContainer &s1, CL_StringContainer &s2);

	/// \brief Clear
	void clear();

	/// \brief Reserve
	///
	/// \param size = size_type
	void reserve(size_type size);

	/// \brief Resize
	///
	/// \param n = size_type
	void resize(size_type n);

	/// \brief Resize
	///
	/// \param n = size_type
	/// \param c = Char Type
	void resize(size_type n, CharType c);

//	template <class InputIterator>
//	CL_StringContainer &assign(InputIterator first, InputIterator last);
	CL_StringContainer &assign(const CL_StringData<CharType, ReferenceClass, StdString> &s);

	/// \brief Assign
	///
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &assign(const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos, size_type n);

	/// \brief Assign
	///
	/// \param s = Char Type
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &assign(const CharType *s, size_type n);

	/// \brief Assign
	///
	/// \param s = Char Type
	///
	/// \return String Container
	CL_StringContainer &assign(const CharType *s);

	/// \brief Assign
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_StringContainer &assign(size_type n, CharType c);

	/// \brief Insert
	///
	/// \param pos = iterator
	/// \param item = Char Type
	///
	/// \return iterator
	iterator insert(iterator pos, const CharType &item)
	{
		size_type insert_pos = (size_type) (pos - begin());
		size_type n = length();
		if (insert_pos > n)
			insert_pos = n;
		resize(n + 1);
		CharType *d = data();
		for (size_type i = n+1; i > insert_pos; i--)
			d[i] = d[i-1];
		d[insert_pos] = item;

		return begin() + insert_pos;
	}

	/// \brief Insert
	///
	/// \param pos = iterator
	/// \param num_copies = size_type
	/// \param item = Char Type
	void insert(iterator pos, size_type num_copies, const CharType &item);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	///
	/// \return String Container
	CL_StringContainer &insert(size_type pos, const CL_StringData<CharType, ReferenceClass, StdString> &s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	/// \param pos1 = size_type
	/// \param length = size_type
	///
	/// \return String Container
	CL_StringContainer &insert(size_type pos, const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos1, size_type length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_StringContainer &insert(size_type pos, const CharType *s);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param s = Char Type
	/// \param s_length = size_type
	///
	/// \return String Container
	CL_StringContainer &insert(size_type pos, const CharType *s, size_type s_length);

	/// \brief Insert
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_StringContainer &insert(size_type pos, size_type n, CharType c);

	/// \brief Append
	///
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	///
	/// \return String Container
	CL_StringContainer &append(const CL_StringData<CharType, ReferenceClass, StdString> &s);

	/// \brief Append
	///
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &append(const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos, size_type n);

	CL_StringContainer<CharType, ReferenceClass, StdString> &append(const char *s);

	/// \brief Append
	///
	/// \param s = char
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &append(const char *s, size_type n);

	/// \brief Append
	///
	/// \param s = wchar_t
	///
	/// \return String Container
	CL_StringContainer &append(const wchar_t *s);

	/// \brief Append
	///
	/// \param s = wchar_t
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &append(const wchar_t *s, size_type n);

	/// \brief Append
	///
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_StringContainer &append(size_type n, CharType c);

	/// \brief Push back
	///
	/// \param c = Char Type
	void push_back(CharType c);

	/// \brief Erase
	///
	/// \param p = iterator
	///
	/// \return iterator
	iterator erase(iterator p)
	{
		iterator e = end();
		if (p == e)
			return e;
		return erase(p, p+1);
	}

	/// \brief Erase
	///
	/// \param first = iterator
	/// \param last = iterator
	///
	/// \return iterator
	iterator erase(iterator first, iterator last)
	{
		size_type pos = (size_type) (first - begin());
		iterator e = end();
		iterator p1 = first;
		iterator p2 = last;
		while (p2 != e)
		{
			*p1 = *p2;
			p1++;
			p2++;
		}
		size_type new_size = (size_type) (p1 - begin());
		resize(new_size);
		return begin() + pos;
	}

	/// \brief Erase
	///
	/// \param pos = size_type
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &erase(size_type pos = 0, size_type n = CL_StringDataTypes::npos);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	///
	/// \return String Container
	CL_StringContainer &replace(size_type pos, size_type n, const CL_StringData<CharType, ReferenceClass, StdString> &s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	/// \param pos1 = size_type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_StringContainer &replace(size_type pos, size_type n, const CL_StringData<CharType, ReferenceClass, StdString> &s, size_type pos1, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	/// \param n1 = size_type
	///
	/// \return String Container
	CL_StringContainer &replace(size_type pos, size_type n, const CharType *s, size_type n1);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param s = Char Type
	///
	/// \return String Container
	CL_StringContainer &replace(size_type pos, size_type n, const CharType *s);

	/// \brief Replace
	///
	/// \param pos = size_type
	/// \param n = size_type
	/// \param n1 = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_StringContainer &replace(size_type pos, size_type n, size_type n1, CharType c);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param CharType = String Data
	/// \param StdString = Reference Class
	///
	/// \return String Container
	CL_StringContainer &replace(iterator first, iterator last, const CL_StringData<CharType, ReferenceClass, StdString> &s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	/// \param n = size_type
	///
	/// \return String Container
	CL_StringContainer &replace(iterator first, iterator last, const CharType *s, size_type n);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param s = Char Type
	///
	/// \return String Container
	CL_StringContainer &replace(iterator first, iterator last, const CharType *s);

	/// \brief Replace
	///
	/// \param first = iterator
	/// \param last = iterator
	/// \param n = size_type
	/// \param c = Char Type
	///
	/// \return String Container
	CL_StringContainer &replace(iterator first, iterator last, size_type n, CharType c);

	/// \brief Copy
	///
	/// \param buf = Char Type
	/// \param n = size_type
	/// \param pos = size_type
	///
	/// \return size_type
	size_type copy(CharType *buf, size_type n, size_type pos = 0) const;

	CL_StringContainer &operator =(const CL_StringContainer &source);
	CL_StringContainer &operator =(const CL_StringData<CharType, ReferenceClass, StdString> &source);
	CL_StringContainer &operator =(const char *c_str);
	CL_StringContainer &operator =(const wchar_t *c_str);
	CL_StringContainer &operator +=(const CL_StringData<CharType, ReferenceClass, StdString> &s);
	CL_StringContainer &operator +=(const char *c_str);
	CL_StringContainer &operator +=(const wchar_t *c_str);
	CL_StringContainer &operator +=(CharType c);

protected:

	/// \brief Init
	void init();

	CL_MemoryPool *pool;
	size_type data_capacity;
	enum { local_string_length = 63 };
	CharType local_string[local_string_length + 1];
};
/*
template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CharType *s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator==(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CharType *s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator!=(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CharType *s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator<(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CharType *s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CL_StringData<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CL_StringData<CharType, ReferenceClass, StdString> &s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CharType *s1, const CL_StringContainer<CharType, ReferenceClass, StdString> &s2);

template <typename CharType, typename ReferenceClass, typename StdString>
CL_API_CORE bool operator>(const CL_StringContainer<CharType, ReferenceClass, StdString> &s1, const CharType *s2);
*/
#endif
/// \}
