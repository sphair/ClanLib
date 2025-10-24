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

#include "Core/precomp.h"
#include "API/Core/System/registry_key.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"

#ifdef WIN32

#include <windows.h>
#include <shlwapi.h>

#if defined (_MSC_VER)
#pragma comment(lib, "shlwapi.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_RegistryKey_Impl Class:

class CL_RegistryKey_Impl
{
public:
	CL_RegistryKey_Impl(HKEY key)
	: key(key)
	{
	}

	~CL_RegistryKey_Impl()
	{
		RegCloseKey(key);
	}

	static HKEY get_predefined_hkey(CL_RegistryKey::PredefinedKey key)
	{
		HKEY hkey = 0;
		switch (key)
		{
		case CL_RegistryKey::key_classes_root:
			hkey = HKEY_CLASSES_ROOT;
			break;
		case CL_RegistryKey::key_current_config:
			hkey = HKEY_CURRENT_CONFIG;
			break;
		case CL_RegistryKey::key_current_user:
			hkey = HKEY_CURRENT_USER;
			break;
		case CL_RegistryKey::key_local_machine:
			hkey = HKEY_LOCAL_MACHINE;
			break;
		case CL_RegistryKey::key_users:
			hkey = HKEY_USERS;
			break;
		}
		return hkey;
	}

	HKEY key;
};

/////////////////////////////////////////////////////////////////////////////
// CL_RegistryKey Construction:

CL_RegistryKey::CL_RegistryKey()
{
}

CL_RegistryKey::CL_RegistryKey(PredefinedKey key, const CL_StringRef &subkey, unsigned int access_rights, unsigned int create_flags)
{
	HKEY hkey = CL_RegistryKey_Impl::get_predefined_hkey(key);
	DWORD disposition = 0;
	HKEY new_key = 0;
	LONG result = RegCreateKeyEx(hkey, CL_StringHelp::utf8_to_ucs2(subkey).c_str(), 0, 0, (create_flags & create_volatile) ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE, access_rights, 0, &new_key, &disposition);
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to create registry key %1", subkey));

	if (disposition != REG_CREATED_NEW_KEY && (create_flags & create_new))
	{
		RegCloseKey(new_key);
		throw CL_Exception(cl_format("Key already exists: %1", subkey));
	}

	impl = CL_SharedPtr<CL_RegistryKey_Impl>(new CL_RegistryKey_Impl(new_key));
}

CL_RegistryKey::CL_RegistryKey(HKEY key)
: impl(new CL_RegistryKey_Impl(key))
{
}

CL_RegistryKey::~CL_RegistryKey()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegistryKey Attributes:

void CL_RegistryKey::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_RegistryKey is null");
}

HKEY CL_RegistryKey::get_key() const
{
	return impl->key;
}

std::vector<CL_String> CL_RegistryKey::get_subkey_names() const
{
	std::vector<CL_String> results;
	DWORD index = 0;
	while (index < 100000)
	{
		TCHAR name[MAX_PATH];
		DWORD name_size = MAX_PATH;
		FILETIME last_write_time;
		LONG result = RegEnumKeyEx(impl->key, index++, name, &name_size, 0, 0, 0, &last_write_time);
		if (result == ERROR_NO_MORE_ITEMS)
			break;
		if (result != ERROR_SUCCESS)
			continue;
		results.push_back(name);
	}
	return results;
}

std::vector<CL_String> CL_RegistryKey::get_value_names() const
{
	std::vector<CL_String> results;
	DWORD index = 0;
	while (index < 100000)
	{
		TCHAR name[MAX_PATH];
		DWORD name_size = MAX_PATH;
		LONG result = RegEnumValue(impl->key, index++, name, &name_size, 0, 0, 0, 0);
		if (result == ERROR_NO_MORE_ITEMS)
			break;
		if (result != ERROR_SUCCESS)
			continue;
		results.push_back(name);
	}
	return results;
}

int CL_RegistryKey::get_value_int(const CL_StringRef &name, int default_value) const
{
	DWORD type = 0, data = 0, size_data = sizeof(DWORD);
	LONG result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE) &data, &size_data);
	if (result != ERROR_SUCCESS || type != REG_DWORD)
		return default_value;
	else
		return data;
}

CL_DataBuffer CL_RegistryKey::get_value_binary(const CL_StringRef &name, const CL_DataBuffer &default_value) const
{
	DWORD type = 0, size_data = 0;
	LONG result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
	if (result != ERROR_SUCCESS || type != REG_BINARY)
		return default_value;

	CL_DataBuffer buffer(size_data);
	size_data = buffer.get_size();
	result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE) buffer.get_data(), &size_data);
	if (result != ERROR_SUCCESS || type != REG_BINARY)
		return default_value;
	return buffer;
}

CL_String CL_RegistryKey::get_value_string(const CL_StringRef &name, const CL_StringRef &default_value) const
{
	DWORD type = 0, size_data = 0;
	LONG result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
	if (result != ERROR_SUCCESS || type != REG_SZ)
		return default_value;

	CL_DataBuffer buffer(size_data);
	size_data = buffer.get_size();
	result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE) buffer.get_data(), &size_data);
	if (result != ERROR_SUCCESS || type != REG_SZ)
		return default_value;
	return (TCHAR *) buffer.get_data();
}

std::vector<CL_String> CL_RegistryKey::get_value_multi_string(const CL_StringRef &name, const std::vector<CL_String> &default_value) const
{
	DWORD type = 0, size_data = 0;
	LONG result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
	if (result != ERROR_SUCCESS || type != REG_MULTI_SZ)
		return default_value;

	CL_DataBuffer buffer(size_data);
	size_data = buffer.get_size();
	result = RegQueryValueEx(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE) buffer.get_data(), &size_data);
	if (result != ERROR_SUCCESS || type != REG_MULTI_SZ)
		return default_value;

	std::vector<CL_String> results;
	TCHAR *pos = (TCHAR *) buffer.get_data();
	while (*pos != 0)
	{
		CL_StringRef s = pos;
		results.push_back(s);
		pos += s.length()+1;
	}

	return results;
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegistryKey Operations:

CL_RegistryKey CL_RegistryKey::open_key(const CL_StringRef &subkey, unsigned int access_rights)
{
	HKEY new_key = 0;
	LONG result = RegOpenKeyEx(impl->key, CL_StringHelp::utf8_to_ucs2(subkey).c_str(), 0, access_rights, &new_key);
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to open registry key %1", subkey));
	return CL_RegistryKey(new_key);
}

CL_RegistryKey CL_RegistryKey::create_key(const CL_StringRef &subkey, unsigned int access_rights, CreateFlags create_flags)
{
	DWORD disposition = 0;
	HKEY new_key = 0;
	LONG result = RegCreateKeyEx(impl->key, CL_StringHelp::utf8_to_ucs2(subkey).c_str(), 0, 0, (create_flags & create_volatile) ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE, access_rights, 0, &new_key, &disposition);
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to create registry key %1", subkey));

	if (disposition != REG_CREATED_NEW_KEY && (create_flags & create_new))
	{
		RegCloseKey(new_key);
		throw CL_Exception(cl_format("Key already exists: %1", subkey));
	}

	return CL_RegistryKey(new_key);
}

void CL_RegistryKey::delete_key(const CL_StringRef &subkey, bool recursive)
{
	if (recursive)
	{
		DWORD result = SHDeleteKey(impl->key, CL_StringHelp::utf8_to_ucs2(subkey).c_str());
		if (result != ERROR_SUCCESS)
			throw CL_Exception(cl_format("Unable to delete registry key %1", subkey));
	}
	else
	{
		LONG result = RegDeleteKey(impl->key, CL_StringHelp::utf8_to_ucs2(subkey).c_str());
		if (result != ERROR_SUCCESS)
			throw CL_Exception(cl_format("Unable to delete registry key %1", subkey));
	}
}

void CL_RegistryKey::delete_key(PredefinedKey key, const CL_StringRef &subkey, bool recursive)
{
	HKEY hkey = CL_RegistryKey_Impl::get_predefined_hkey(key);
	if (recursive)
	{
		DWORD result = SHDeleteKey(hkey, CL_StringHelp::utf8_to_ucs2(subkey).c_str());
		if (result != ERROR_SUCCESS)
			throw CL_Exception(cl_format("Unable to delete registry key %1", subkey));
	}
	else
	{
		LONG result = RegDeleteKey(hkey, CL_StringHelp::utf8_to_ucs2(subkey).c_str());
		if (result != ERROR_SUCCESS)
			throw CL_Exception(cl_format("Unable to delete registry key %1", subkey));
	}
}

void CL_RegistryKey::set_value_int(const CL_StringRef &name, int value)
{
	DWORD v = value;
	LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_DWORD, (const BYTE *) &v, sizeof(DWORD));
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to set registry key value %1", name));
}

void CL_RegistryKey::set_value_binary(const CL_StringRef &name, const CL_DataBuffer &value)
{
	LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_BINARY, (const BYTE *) value.get_data(), value.get_size());
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to set registry key value %1", name));
}

void CL_RegistryKey::set_value_string(const CL_StringRef &name, const CL_StringRef &value)
{
	LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : CL_StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_SZ, (const BYTE *) value.c_str(), (value.length()+1) * sizeof(TCHAR));
	if (result != ERROR_SUCCESS)
		throw CL_Exception(cl_format("Unable to set registry key value %1", name));
}
/*
void CL_RegistryKey::set_value_multi_string(const CL_StringRef &name, const std::vector<CL_String> &value)
{
	int size = 1;
	for (std::vector<CL_String>::size_type i = 0; i < value.size(); i++)
		size += value[i].length()+1;
	CL_DataBuffer buffer(size * sizeof(TCHAR));
	int pos = 0;
	// gosh this is too boring..
}
*/
void CL_RegistryKey::delete_value(const CL_StringRef &name)
{
	RegDeleteValue(impl->key, CL_StringHelp::utf8_to_ucs2(name).c_str());
}

/////////////////////////////////////////////////////////////////////////////
// CL_RegistryKey Implementation:

#endif
