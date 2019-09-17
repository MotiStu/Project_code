//扫描管理模块
//目录是一个树形结构，windows是森林结构（多棵树）
//此项目下的树形结构是左孩子右兄弟表示方法，左边放孩子，右边是兄弟，方便
//为什么不用而二叉树，因为一层要是有n个孩子，表示不了
//windows下是FindFirstFile，麻烦因为是宽字符(wstring),但是你搜索的时候用的是string
//C++用的是_findfirst
#pragma once
#include "Common.h"
#include "DataManager.h"
//ScanManager设计成单例模式，整个程序只有一个扫描模块
//饿汉模式
class ScanManager   //扫描
{
public:
	void Scan(const string& path);
	void StartScan()
	{
		while (1)
		{
			Scan("D:\\C语言\\C++");
			std::this_thread::sleep_for(std::chrono::seconds(5));//休眠时间，每过5秒扫一次
		}
	}
	static ScanManager* CreateIntance()//声明为静态函数
	{
		static ScanManager scanmgr;
		static std::thread thd(&StartScan, &scanmgr);//搜索线程
		thd.detach();//线程分离

		return &scanmgr;
	}
private:
	ScanManager()//构造函数私有化，不能在类外通过new实例化
	{
		//_datamgr.Init();
	}
	ScanManager(const ScanManager&);

	//DataManager _datamgr;//数据库
};
