//数据
#define _CRT_SECURE_NO_WARNINGS 1
#include "DataManager.h"
#include <assert.h>
void SqliteManager::Open(const string& path)
{
	int ret = sqlite3_open(path.c_str(), &_db);
	if (ret != SQLITE_OK)
	{
		ERROE_LOG("sqlite3_open\n");
	}
	else
	{
		TRACE_LOG("sqlite3_open success");
	}
}
void SqliteManager::Close()
{
	int ret = sqlite3_close(_db);
	if (ret != SQLITE_OK)
	{
		ERROE_LOG("sqlite3_close\n");
	}
	else
	{
		TRACE_LOG("sqlite3_close success");
	}
}
void SqliteManager::ExecuteSql(const string& sql)//执行数据库语句
{
	assert(_db);

	char* errmsg;
	int ret = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &errmsg);
	if (ret != SQLITE_OK)
	{
		ERROE_LOG("sqlite3_exec:(%s) errmsg:%s\n", sql.c_str(), errmsg);
		sqlite3_free(errmsg);

	}
	else
	{
		TRACE_LOG("sqlite3_exec:(%s) success",sql.c_str());
	}
}
void SqliteManager::GetTable(const string& sql, int& row, int& col, char**& ppRet)//
{
	assert(_db);  //断言表是否打开，没打开就不能拿到表
	char* errmsg;
	int ret = sqlite3_get_table(_db, sql.c_str(), &ppRet, &row, &col, &errmsg);

	if (ret != SQLITE_OK)
	{
		ERROE_LOG("sqlite3_get_table:(%s) errmsg:%s\n", sql.c_str(), errmsg);
		sqlite3_free(errmsg);
	}
	else
	{
		TRACE_LOG("sqlite3_get_table:(%s) success", sql.c_str());
	}
}

void DataManager::Init()//想要有一个表，所以打开数据库，建表。。。。
{
	std::unique_lock<std::mutex> lock(_mutex);
	_dbmgr.Open(DB_NAME);//打开数据库
	
	char sql[256];
	sprintf(sql, "create table if not exists tb_doc (id INTEGER PRIMARY KEY, path text, name text,\
										name_pinyin text, name_initials)");//建表
	_dbmgr.ExecuteSql(sql);//执行建表语句

}
void DataManager::GetDoc(const string& path,std::set<string>& dbset)//查找path目录下的文档
{
	char sql[256];
	sprintf(sql, "select name from %s where path = '%s'", TB_NAME,  path.c_str());
	int row, col;
	char** ppRet;
	
	std::unique_lock<std::mutex> lock(_mutex);
	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);//获取表用的是RAII的思想
	lock.unlock();
	
	for (int i = 1; i < row; ++i)//注意行列关系
	{
		for (int j = 0; j <= col; ++j)
		{
			dbset.insert(ppRet[i*col + j]);
		}
	}
}
void DataManager::InsertDoc(const string& path, const string& name)
{
	char sql[256];
	string pinyin = ChineseConvertPinYinAllSpell(name);
	string initials = ChineseConvertPinYinInitials(name);
	sprintf(sql, "insert into %s(path,name,name_pinyin,name_initials) values('%s', '%s','%s','%s')", TB_NAME, path.c_str(), name.c_str(), pinyin.c_str(), initials.c_str());

	std::unique_lock<std::mutex> lock(_mutex);

	_dbmgr.ExecuteSql(sql);
}
void DataManager::DeleteDoc(const string& path, const string& name)//删除注意整个文件夹都删了怎么办
{
	char sql[256];
	sprintf(sql, "delete from %s where path = '%s' and name = '%s'", TB_NAME, path.c_str(), name.c_str());

	string path_ = path;
	path_ += '\\';
	path_ += name;
	sprintf(sql, "delete from %s where path like '%s%'", TB_NAME, path_.c_str());//这里删的是文件夹下的子文件

	std::unique_lock<std::mutex> lock(_mutex);
	_dbmgr.ExecuteSql(sql);
}

void DataManager::Search(const string& key, vector<std::pair<string, string>>&docinfos)
{
	//char sql[256];
	//string like_key = "%";
	//like_key += key;
	//like_key += "%";
	//sprintf(sql, "select name, path from %s where name like '%s'", TB_NAME,like_key.c_str());
	////like 语句模糊匹配用%，还是转义字符，所以如上
	//
	//int row, col;
	//char** ppRet;
	//AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	//for (int i = 1; i < row; ++i)//注意行列关系
	//{
	//	docinfos.push_back(std::make_pair(ppRet[i*col + 0], ppRet[i*col + 1]));
	//}


	char sql[256];
	{
		string pinyin = ChineseConvertPinYinAllSpell(key);//拼音搜
		string initials = ChineseConvertPinYinInitials(key);//首字母搜
		sprintf(sql, "select name, path from %s where name_pinyin like '%%%s%%' or name_initials like '%%%s%%'", TB_NAME, pinyin.c_str(),initials.c_str());
		//like 语句模糊匹配用%，还是转义字符，所以如上
		//用or匹配两个，解决了用汉字搜，会有2次结果，因为大写和拼音都搜了

		int row, col;
		char** ppRet;

		std::unique_lock<std::mutex> lock(_mutex);
		AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
		lock.unlock();

		for (int i = 1; i < row; ++i)//注意行列关系
		{
			docinfos.push_back(std::make_pair(ppRet[i*col + 0], ppRet[i*col + 1]));
		}
	}

	//以下用不到了
	//用首字母搜
	//{
	//	string initials = ChineseConvertPinYinInitials(key);
	//	sprintf(sql, "select name, path from %s where name_initials like '%%%s%%'", TB_NAME, initials.c_str());
	//	int row, col;
	//	char** ppRet;
	//	AutoGetTable agt(_dbmgr, sql, row, col, ppRet);
	//	for (int i = 1; i < row; ++i)//注意行列关系
	//	{

	//		docinfos.push_back(std::make_pair(ppRet[i*col + 0], ppRet[i*col + 1]));
	//	}
	//}

}

void DataManager::SplitHighlight(const string& str, const string& key, string& prefix, string& highlight, string& suffix)//切割高亮部分
{
	//汉字直接匹配
	{
		size_t ht_start = str.find(key);
		if (ht_start != string::npos)
		{
			prefix = str.substr(0, ht_start);
			highlight = key;//高亮部分
			suffix = str.substr(ht_start + key.size(), string::npos);
			return;
		}
	}

	//拼音全拼匹配
	{
		string str_allspell = ChineseConvertPinYinAllSpell(str);//文件名转为拼音
		string key_allspell = ChineseConvertPinYinAllSpell(key);//中英结合搜

		size_t ht_index = 0;//
		size_t allspell_index = 0;
		size_t ht_start = 0, ht_len = 0;//ht_start控制中文名高亮部分

		size_t allspell_start = str_allspell.find(key);
		if (allspell_start != string::npos)
		{
			size_t allspell_end = allspell_start + key_allspell.size();

			while (allspell_index < allspell_end)
			{
				//检测是何时开始高亮
				if (allspell_index == allspell_start)//start是指文件全拼中key的其实位置
				{
					ht_start = ht_index;//对应汉字的位置
				}

				//如果是ASCII字符，跳过
				if (str[ht_index] >= 0 && str[ht_index] <= 127)
				{
					++ht_index;
					++allspell_index;
				}
				else//跳汉字，因为GBK下一个汉字2个字节
				{
					char chinses[3] = { '\0' };
					chinses[0] = str[ht_index];
					chinses[1] = str[ht_index + 1];
					string ap_str = ChineseConvertPinYinAllSpell(chinses);
					//中文文件名ht_index向后跳。因为GBK下一个汉字2个字节，跳一个汉字的长度
					ht_index += 2;
					//文件全拼指向allspell_index全拼跳过汉字的拼音长度
					allspell_index += ap_str.size();

				}

			}
			ht_len = ht_index - ht_start;
			prefix = str.substr(0, ht_start);
			highlight = str.substr(ht_start, ht_len);
			suffix = str.substr(ht_start + ht_len, string::npos);
			return;
		}
	}

	//拼音首字母
	{
		string ial_str = ChineseConvertPinYinInitials(str);
		string ial_key = ChineseConvertPinYinInitials(key);
		size_t ial_strat = ial_str.find(key);
		
		if (ial_strat != string::npos)
		{
			size_t ial_end = ial_strat + ial_key.size();
			size_t ial_index = 0 , ht_index = 0;
			size_t ht_start = 0, ht_len = 0;
			while (ial_index < ial_end)
			{
				if (ial_index == ial_strat)
				{
					ht_start = ht_index;
				}
				//ascii
				if (str[ht_index] >= 0 && str[ht_index] <= 127)
				{
					++ht_index;
					++ial_index;
				}
				else
				{
					ht_index += 2;//汉字跳两个字节
					++ial_index;//首字母只用跳一个
				}
			}
			ht_len = ht_index - ht_start;
			prefix = str.substr(0, ht_start);
			highlight = str.substr(ht_start, ht_len);
			suffix = str.substr(ht_start + ht_len, string::npos);
			return;
		}
	}
	//TRACE_LOG("split Highlight no match. str:%s, key:%s\n", str.c_str(), key.c_str());
	prefix = str;
}

