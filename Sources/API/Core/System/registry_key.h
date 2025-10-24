/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once

#ifdef WIN32

#include "../api_core.h"
#include "databuffer.h"
#include <vector>

class CL_RegistryKey_Impl;

/// \brief Registry key class.
/** <p>This class is only available on Windows.<p>
    !group=Core/System! !header=core.h!*/
class CL_API_CORE CL_RegistryKey
{
/// \name Construction
/// \{

public:
	enum PredefinedKey
	{
		key_classes_root,
		key_current_config,
		key_current_user,
		key_local_machine,
		key_users
	};

	enum CreateFlags
	{
		create_always    = 0,
		create_new       = 1,
		create_volatile  = 2
	};

	// Construct a null instance
	CL_RegistryKey();

	CL_RegistryKey(PredefinedKey key, const CL_StringRef &subkey, unsigned int access_rights = KEY_ALL_ACCESS, unsigned int create_flags = create_always);

	CL_RegistryKey(HKEY key);

	~CL_RegistryKey();


/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return impl.is_null(); }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	HKEY get_key() const;

	std::vector<CL_String> get_subkey_names() const;

	std::vector<CL_String> get_value_names() const;

	int get_value_int(const CL_StringRef &name, int default_value = 0) const;

	CL_DataBuffer get_value_binary(const CL_StringRef &name, const CL_DataBuffer &default_value = CL_DataBuffer()) const;

	CL_String get_value_string(const CL_StringRef &name, const CL_StringRef &default_value = CL_StringRef()) const;

	std::vector<CL_String> get_value_multi_string(const CL_StringRef &name, const std::vector<CL_String> &default_value = std::vector<CL_String>()) const;


/// \}
/// \name Operations
/// \{

public:
	CL_RegistryKey open_key(const CL_StringRef &subkey, unsigned int access_rights = KEY_ALL_ACCESS);

	CL_RegistryKey create_key(const CL_StringRef &subkey, unsigned int access_rights = KEY_ALL_ACCESS, CreateFlags create_flags = create_always);

	void delete_key(const CL_StringRef &subkey, bool recursive);

	static void delete_key(PredefinedKey key, const CL_StringRef &subkey, bool recursive);

	void set_value_int(const CL_StringRef &name, int value);

	void set_value_binary(const CL_StringRef &name, const CL_DataBuffer &value);

	void set_value_string(const CL_StringRef &name, const CL_StringRef &value);

//	void set_value_multi_string(const CL_StringRef &name, const std::vector<CL_String> &value);

	void delete_value(const CL_StringRef &name);


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_RegistryKey_Impl> impl;
/// \}
};

#endif

/// \}
