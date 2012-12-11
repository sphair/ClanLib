
#include "precomp.h"
#include "file_cache.h"
#include <ClanLib/sqlite.h>

using namespace clan;

FileCache::FileCache()
{
	db = create_db_connection();
	init_database();
}

DataBuffer FileCache::get_file(const std::string &url, std::string &out_content_type)
{
	DBCommand cmd = db.create_command("SELECT Data, ContentType FROM File WHERE Url=?1 LIMIT 1;");
	cmd.set_input_parameter_string(1, url);
	DBReader reader = db.execute_reader(cmd);
	if (reader.retrieve_row())
	{
		DataBuffer buffer = reader.get_column_binary(0);
		out_content_type = reader.get_column_string(1);
		reader.close();
		return buffer;
	}
	else
	{
		return DataBuffer();
	}
}

void FileCache::set_file(const std::string &url, const DataBuffer &buffer, const std::string &content_type)
{
	DBCommand cmd = db.create_command("INSERT INTO File(Url, Data, ContentType) VALUES(?1, ?2, ?3);");
	cmd.set_input_parameter_string(1, url);
	cmd.set_input_parameter_binary(2, buffer);
	cmd.set_input_parameter_string(3, content_type);
	db.execute_non_query(cmd);
}

void FileCache::init_database()
{
	std::string version_string;
	int version_major = 0, version_minor = 0;
	int servicepack_major = 0, servicepack_minor = 0;
	bool exists = check_database_exists(
		db,
		version_string,
		version_major,
		version_minor,
		servicepack_major,
		servicepack_minor);
	if (!exists)
	{
		create_tables(db);
	}
	else if (version_major != 1 || version_minor != 0 || servicepack_major != 0 || servicepack_minor != 0)
	{
		upgrade_tables(
			db,
			version_string,
			version_major,
			version_minor,
			servicepack_major,
			servicepack_minor);
	}
}

bool FileCache::check_database_exists(
	DBConnection db,
	std::string &version_string,
	int &version_major,
	int &version_minor,
	int &servicepack_major,
	int &servicepack_minor)
{
	try
	{
		DBCommand query = db.create_command(
			"SELECT "
			"	VersionString,"
			"	VersionMajor,"
			"	VersionMinor,"
			"	ServicePackMajor,"
			"	ServicePackMinor "
			"FROM VersionInfo "
			"WHERE Application = 'CarambolaBrowser';");
		DBReader reader = db.execute_reader(query);
		if (reader.retrieve_row())
		{
			version_string = reader.get_column_string(0);
			version_major = reader.get_column_int(1);
			version_minor = reader.get_column_int(2);
			servicepack_major = reader.get_column_int(3);
			servicepack_minor = reader.get_column_int(4);
			reader.close();
			return true;
		}
		reader.close();
		return false;
	}
	catch (Exception e)
	{
		return false;
	}
}

void FileCache::create_tables(DBConnection db)
{
	DBCommand query = db.create_command(
		"CREATE TABLE VersionInfo("
		"	VersionInfoId INTEGER PRIMARY KEY,"
		"	Application STRING,"
		"	VersionString STRING,"
		"	VersionMajor INTEGER,"
		"	VersionMinor INTEGER,"
		"	ServicePackMajor INTEGER,"
		"	ServicePackMinor INTEGER);");
	db.execute_non_query(query);

	query = db.create_command(
		"CREATE TABLE File("
		"	FileId INTEGER PRIMARY KEY,"
		"	Url STRING,"
		"	ContentType STRING,"
		"	Data BLOB);");
	db.execute_non_query(query);

	query = db.create_command(
		"INSERT INTO VersionInfo("
		"	Application,"
		"	VersionString,"
		"	VersionMajor,"
		"	VersionMinor,"
		"	ServicePackMajor,"
		"	ServicePackMinor)"
		"VALUES("
		"	'CarambolaBrowser',"
		"	'Carambola Browser 1.0',"
		"	'1',"
		"	'0',"
		"	'0',"
		"	'0');");
	db.execute_non_query(query);
}

void FileCache::upgrade_tables(
	DBConnection db,
	std::string &version_string,
	int &version_major,
	int &version_minor,
	int &servicepack_major,
	int &servicepack_minor)
{
	throw Exception(string_format("Database is version %1, unable to upgrade", version_string));
}

DBConnection FileCache::create_db_connection()
{
	return SqliteConnection("file_cache.db");
}
