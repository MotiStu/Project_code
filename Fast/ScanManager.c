#define _CRT_SECURE_NO_WARNINGS 1
#include "ScanManager.h"

void ScanManager::Scan(const string& path)
{
	//比对  文件系统和数据库
	vector<string> localdirs;//存目录容器
	vector<string> localfiles;//存文件容器
	DirectoryList(path, localdirs, localfiles); //遍历文件
	
	std::set<string> localset;
	localset.insert(localfiles.begin(), localfiles.end());
	localset.insert(localdirs.begin(), localdirs.end());

	//数据库：涉及到增添数据-----需要用一个线程
	std::set<string> dbset;//这个刚开始是空的
	DataManager::GetInstanse()->GetDoc(path, dbset);//文件扫描后和数据库对比

	auto localit = localset.begin();
	auto dbit = dbset.begin();
	while (localit != localset.end() && dbit != dbset.end())
	{
		if (*localit < *dbit)//本地有，数据库没有，新增数据
		{
			DataManager::GetInstanse()->InsertDoc(path, *localit);//对应到中的数据库
			++localit;
		}
		else if (*localit>*dbit)//本地没有，数据库有，删除数据
		{
			DataManager::GetInstanse()->DeleteDoc(path, *dbit);//对应到中的数据库
			++dbit;
		}
		else
		{
			++localit;
			++dbit;
		}
	}
	while (localit != localset.end())//新增数据
	{
		DataManager::GetInstanse()->InsertDoc(path, *localit);//对应到中的数据库
		++localit;
	}
	while (dbit != dbset.end())//删除数据
	{
		DataManager::GetInstanse()->DeleteDoc(path, *dbit);//对应到中的数据库中删除应该注意整个文件夹都删了怎么办
		++dbit;
	}

	//递归比对子目录数据
	for (const auto& subdirs : localdirs)
	{
		string subpath = path;
		subpath += '\\';
		subpath += subdirs;
		Scan(subpath);//要是绝对路径
	}
}
