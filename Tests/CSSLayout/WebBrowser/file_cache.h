
#pragma once

class FileCache
{
public:
	FileCache();
	CL_DataBuffer get_file(const CL_String &url, CL_String &out_content_type);
	void set_file(const CL_String &url, const CL_DataBuffer &buffer, const CL_String &content_type);

private:
	void init_database();
	bool check_database_exists(
		CL_DBConnection db,
		CL_String &version_string,
		int &version_major,
		int &version_minor,
		int &servicepack_major,
		int &servicepack_minor);

	void create_tables(CL_DBConnection db);
	void upgrade_tables(
		CL_DBConnection db,
		CL_String &version_string,
		int &version_major,
		int &version_minor,
		int &servicepack_major,
		int &servicepack_minor);

	static CL_DBConnection create_db_connection();

	CL_DBConnection db;
};
