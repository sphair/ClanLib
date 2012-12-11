
#pragma once

class FileCache
{
public:
	FileCache();
	clan::DataBuffer get_file(const std::string &url, std::string &out_content_type);
	void set_file(const std::string &url, const clan::DataBuffer &buffer, const std::string &content_type);

private:
	void init_database();
	bool check_database_exists(
		clan::DBConnection db,
		std::string &version_string,
		int &version_major,
		int &version_minor,
		int &servicepack_major,
		int &servicepack_minor);

	void create_tables(clan::DBConnection db);
	void upgrade_tables(
		clan::DBConnection db,
		std::string &version_string,
		int &version_major,
		int &version_minor,
		int &servicepack_major,
		int &servicepack_minor);

	static clan::DBConnection create_db_connection();

	clan::DBConnection db;
};
