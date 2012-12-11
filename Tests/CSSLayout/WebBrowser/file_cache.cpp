
#include "precomp.h"
#include "file_cache.h"
#include <ClanLib/sqlite.h>

FileCache::FileCache()
{
	db = create_db_connection();
	init_database();
}

CL_DataBuffer FileCache::get_file(const CL_String &url, CL_String &out_content_type)
{
	CL_DBCommand cmd = db.create_command("SELECT Data, ContentType FROM File WHERE Url=?1 LIMIT 1;");
	cmd.set_input_parameter_string(1, url);
	CL_DBReader reader = db.execute_reader(cmd);
	if (reader.retrieve_row())
	{
		CL_DataBuffer buffer = reader.get_column_binary(0);
		out_content_type = reader.get_column_string(1);
		reader.close();
		return buffer;
	}
	else
	{
		return CL_DataBuffer();
	}
}

void FileCache::set_file(const CL_String &url, const CL_DataBuffer &buffer, const CL_String &content_type)
{
	CL_DBCommand cmd = db.create_command("INSERT INTO File(Url, Data, ContentType) VALUES(?1, ?2, ?3);");
	cmd.set_input_parameter_string(1, url);
	cmd.set_input_parameter_binary(2, buffer);
	cmd.set_input_parameter_string(3, content_type);
	db.execute_non_query(cmd);
}

void FileCache::init_database()
{
	CL_String version_string;
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
	CL_DBConnection db,
	CL_String &version_string,
	int &version_major,
	int &version_minor,
	int &servicepack_major,
	int &servicepack_minor)
{
	try
	{
		CL_DBCommand query = db.create_command(
			"SELECT "
			"	VersionString,"
			"	VersionMajor,"
			"	VersionMinor,"
			"	ServicePackMajor,"
			"	ServicePackMinor "
			"FROM VersionInfo "
			"WHERE Application = 'CarambolaBrowser';");
		CL_DBReader reader = db.execute_reader(query);
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
	catch (CL_Exception e)
	{
		return false;
	}
}

void FileCache::create_tables(CL_DBConnection db)
{
	CL_DBCommand query = db.create_command(
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
	CL_DBConnection db,
	CL_String &version_string,
	int &version_major,
	int &version_minor,
	int &servicepack_major,
	int &servicepack_minor)
{
	throw CL_Exception(cl_format("Database is version %1, unable to upgrade", version_string));
}

CL_DBConnection FileCache::create_db_connection()
{
	return CL_SqliteConnection("file_cache.db");
}
