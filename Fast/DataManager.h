//数据库相关操作
#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include "Common.h"
class SqliteManager   //库的操作
{
public:
	SqliteManager()
		:_db(nullptr)
	{}
	~SqliteManager()
	{
		Close();
	}
	void Open(const string& path);
	void Close();
	void ExecuteSql(const string& sql);
	void GetTable(const string& sql, int& row, int& col, char**& ppRet);//
	SqliteManager(const SqliteManager&) = delete;
	SqliteManager& operator=(const SqliteManager&) = delete;
private:
	sqlite3* _db; // 数据库对象
};

//RAII
class AutoGetTable//智能
{
public:
	AutoGetTable(SqliteManager& sm, const string& sql, int& row, int& col, char**& ppRet)
	{
		sm.GetTable(sql, row, col, ppRet);//GetTable是库的操作
		_ppRet = ppRet;
	}

	~AutoGetTable()
	{
		sqlite3_free_table(_ppRet);
	}

	AutoGetTable(const AutoGetTable&) = delete;
	AutoGetTable operator = (const AutoGetTable&) = delete;
private:
	char** _ppRet;
};


///////////////////////////////////////////
//数据管理模块
#define TB_NAME "tb_doc" //这里是表的名字table
#define DB_NAME "db_name.db"  //这里是一个数据库的名字，可在这里直接修改 database

//表应该也只有一个对象
//为了方便进行加锁，设计成单例模式
class DataManager //表的操作
{
public:
	//e汉
	static DataManager* GetInstanse()
	{
		static DataManager datamgr;
		datamgr.Init();//建个表就好

		return &datamgr;
	}
	void Init();//建表
	void GetDoc(const string& path, std::set<string>& dbset);//查找path目录下的文档
	void InsertDoc(const string& path,const string& doc);
	void DeleteDoc(const string& path, const string& doc);
	void Search(const string& key, vector<std::pair<string, string>>&docinfos);//查找
	void SplitHighlight(const string& str, const string& key, string& prefix, string& highlight, string& suffix);//切割高亮部分


private:
	DataManager()
	{}
	SqliteManager _dbmgr;//数据库对象，对其操作
	std::mutex _mutex;
};
