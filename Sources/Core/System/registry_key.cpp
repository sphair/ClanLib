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

namespace clan
{
	class RegistryKey_Impl
	{
	public:
		RegistryKey_Impl(HKEY key)
			: key(key)
		{
		}

		~RegistryKey_Impl()
		{
			RegCloseKey(key);
		}

		static HKEY get_predefined_hkey(RegistryKey::PredefinedKey key)
		{
			HKEY hkey = 0;
			switch (key)
			{
			case RegistryKey::key_classes_root:
				hkey = HKEY_CLASSES_ROOT;
				break;
			case RegistryKey::key_current_config:
				hkey = HKEY_CURRENT_CONFIG;
				break;
			case RegistryKey::key_current_user:
				hkey = HKEY_CURRENT_USER;
				break;
			case RegistryKey::key_local_machine:
				hkey = HKEY_LOCAL_MACHINE;
				break;
			case RegistryKey::key_users:
				hkey = HKEY_USERS;
				break;
			}
			return hkey;
		}

		HKEY key;
	};

	RegistryKey::RegistryKey()
	{
	}

	RegistryKey::RegistryKey(PredefinedKey key, const std::string &subkey, unsigned int access_rights, unsigned int create_flags)
	{
		HKEY hkey = RegistryKey_Impl::get_predefined_hkey(key);
		DWORD disposition = 0;
		HKEY new_key = 0;
		LONG result = RegCreateKeyEx(hkey, StringHelp::utf8_to_ucs2(subkey).c_str(), 0, 0, (create_flags & create_volatile) ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE, access_rights, 0, &new_key, &disposition);
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to create registry key %1", subkey));

		if (disposition != REG_CREATED_NEW_KEY && (create_flags & create_new))
		{
			RegCloseKey(new_key);
			throw Exception(string_format("Key already exists: %1", subkey));
		}

		impl = std::shared_ptr<RegistryKey_Impl>(new RegistryKey_Impl(new_key));
	}

	RegistryKey::RegistryKey(HKEY key)
		: impl(std::make_shared<RegistryKey_Impl>(key))
	{
	}

	RegistryKey::~RegistryKey()
	{
	}

	void RegistryKey::throw_if_null() const
	{
		if (!impl)
			throw Exception("RegistryKey is null");
	}

	HKEY RegistryKey::get_key() const
	{
		return impl->key;
	}

	std::vector<std::string> RegistryKey::get_subkey_names() const
	{
		std::vector<std::string> results;
		DWORD index = 0;
		while (index < 100000)
		{
			WCHAR name[MAX_PATH];
			DWORD name_size = MAX_PATH;
			FILETIME last_write_time;
			LONG result = RegEnumKeyEx(impl->key, index++, name, &name_size, 0, 0, 0, &last_write_time);
			if (result == ERROR_NO_MORE_ITEMS)
				break;
			if (result != ERROR_SUCCESS)
				continue;
			results.push_back(StringHelp::ucs2_to_utf8(name));
		}
		return results;
	}

	std::vector<std::string> RegistryKey::get_value_names() const
	{
		std::vector<std::string> results;
		DWORD index = 0;
		while (index < 100000)
		{
			WCHAR name[MAX_PATH];
			DWORD name_size = MAX_PATH;
			LONG result = RegEnumValue(impl->key, index++, name, &name_size, 0, 0, 0, 0);
			if (result == ERROR_NO_MORE_ITEMS)
				break;
			if (result != ERROR_SUCCESS)
				continue;
			results.push_back(StringHelp::ucs2_to_utf8(name));
		}
		return results;
	}

	int RegistryKey::get_value_int(const std::string &name, int default_value) const
	{
		DWORD type = 0, data = 0, size_data = sizeof(DWORD);
		LONG result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE)&data, &size_data);
		if (result != ERROR_SUCCESS || type != REG_DWORD)
			return default_value;
		else
			return data;
	}

	DataBuffer RegistryKey::get_value_binary(const std::string &name, const DataBuffer &default_value) const
	{
		DWORD type = 0, size_data = 0;
		LONG result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
		if (result != ERROR_SUCCESS || type != REG_BINARY)
			return default_value;

		DataBuffer buffer(size_data);
		size_data = buffer.get_size();
		result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE)buffer.get_data(), &size_data);
		if (result != ERROR_SUCCESS || type != REG_BINARY)
			return default_value;
		return buffer;
	}

	std::string RegistryKey::get_value_string(const std::string &name, const std::string &default_value) const
	{
		DWORD type = 0, size_data = 0;
		LONG result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
		if (result != ERROR_SUCCESS || type != REG_SZ)
			return default_value;

		DataBuffer buffer(size_data);
		size_data = buffer.get_size();
		result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE)buffer.get_data(), &size_data);
		if (result != ERROR_SUCCESS || type != REG_SZ)
			return default_value;
		return StringHelp::ucs2_to_utf8((WCHAR *)buffer.get_data());
	}

	std::vector<std::string> RegistryKey::get_value_multi_string(const std::string &name, const std::vector<std::string> &default_value) const
	{
		DWORD type = 0, size_data = 0;
		LONG result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, 0, &size_data);
		if (result != ERROR_SUCCESS || type != REG_MULTI_SZ)
			return default_value;

		DataBuffer buffer(size_data);
		size_data = buffer.get_size();
		result = RegQueryValueEx(impl->key, StringHelp::utf8_to_ucs2(name).c_str(), 0, &type, (LPBYTE)buffer.get_data(), &size_data);
		if (result != ERROR_SUCCESS || type != REG_MULTI_SZ)
			return default_value;

		std::vector<std::string> results;
		WCHAR *pos = (TCHAR *)buffer.get_data();
		while (*pos != 0)
		{
			std::string s = StringHelp::ucs2_to_utf8(pos);
			results.push_back(s);
			pos += s.length() + 1;
		}

		return results;
	}

	RegistryKey RegistryKey::open_key(const std::string &subkey, unsigned int access_rights)
	{
		HKEY new_key = 0;
		LONG result = RegOpenKeyEx(impl->key, StringHelp::utf8_to_ucs2(subkey).c_str(), 0, access_rights, &new_key);
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to open registry key %1", subkey));
		return RegistryKey(new_key);
	}

	RegistryKey RegistryKey::create_key(const std::string &subkey, unsigned int access_rights, CreateFlags create_flags)
	{
		DWORD disposition = 0;
		HKEY new_key = 0;
		LONG result = RegCreateKeyEx(impl->key, StringHelp::utf8_to_ucs2(subkey).c_str(), 0, 0, (create_flags & create_volatile) ? REG_OPTION_VOLATILE : REG_OPTION_NON_VOLATILE, access_rights, 0, &new_key, &disposition);
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to create registry key %1", subkey));

		if (disposition != REG_CREATED_NEW_KEY && (create_flags & create_new))
		{
			RegCloseKey(new_key);
			throw Exception(string_format("Key already exists: %1", subkey));
		}

		return RegistryKey(new_key);
	}

	void RegistryKey::delete_key(const std::string &subkey, bool recursive)
	{
		if (recursive)
		{
			DWORD result = SHDeleteKey(impl->key, StringHelp::utf8_to_ucs2(subkey).c_str());
			if (result != ERROR_SUCCESS)
				throw Exception(string_format("Unable to delete registry key %1", subkey));
		}
		else
		{
			LONG result = RegDeleteKey(impl->key, StringHelp::utf8_to_ucs2(subkey).c_str());
			if (result != ERROR_SUCCESS)
				throw Exception(string_format("Unable to delete registry key %1", subkey));
		}
	}

	void RegistryKey::delete_key(PredefinedKey key, const std::string &subkey, bool recursive)
	{
		HKEY hkey = RegistryKey_Impl::get_predefined_hkey(key);
		if (recursive)
		{
			DWORD result = SHDeleteKey(hkey, StringHelp::utf8_to_ucs2(subkey).c_str());
			if (result != ERROR_SUCCESS)
				throw Exception(string_format("Unable to delete registry key %1", subkey));
		}
		else
		{
			LONG result = RegDeleteKey(hkey, StringHelp::utf8_to_ucs2(subkey).c_str());
			if (result != ERROR_SUCCESS)
				throw Exception(string_format("Unable to delete registry key %1", subkey));
		}
	}

	void RegistryKey::set_value_int(const std::string &name, int value)
	{
		DWORD v = value;
		LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_DWORD, (const BYTE *)&v, sizeof(DWORD));
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to set registry key value %1", name));
	}

	void RegistryKey::set_value_binary(const std::string &name, const DataBuffer &value)
	{
		LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_BINARY, (const BYTE *)value.get_data(), value.get_size());
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to set registry key value %1", name));
	}

	void RegistryKey::set_value_string(const std::string &name, const std::string &value)
	{
		std::wstring value_str = StringHelp::utf8_to_ucs2(value);
		LONG result = RegSetValueEx(impl->key, name.empty() ? 0 : StringHelp::utf8_to_ucs2(name).c_str(), 0, REG_SZ, (const BYTE *)value_str.c_str(), (value_str.length() + 1) * sizeof(WCHAR));
		if (result != ERROR_SUCCESS)
			throw Exception(string_format("Unable to set registry key value %1", name));
	}
	/*
	void RegistryKey::set_value_multi_string(const std::string &name, const std::vector<std::string> &value)
	{
		int size = 1;
		for (std::vector<std::string>::size_type i = 0; i < value.size(); i++)
		size += value[i].length()+1;
		DataBuffer buffer(size * sizeof(WCHAR));
		int pos = 0;
		// gosh this is too boring..
	}
	*/
	void RegistryKey::delete_value(const std::string &name)
	{
		RegDeleteValue(impl->key, StringHelp::utf8_to_ucs2(name).c_str());
	}
}

#endif
