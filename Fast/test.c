#define _CRT_SECURE_NO_WARNINGS 1
#include "Common.h"
#include "DataManager.h"
#include "ScanManager.h"
void TestDirectoryList()//找一层目录下的所有文档
{
	vector<string> dirs;
	vector<string> files;
	DirectoryList("D:\\C语言\\C++",dirs,files);//注意转义字符，所以有两个“\\”
	for (const auto& e : dirs)
	{
		cout << e << endl;
	}
	for (const auto& e : files)
	{
		cout << e << endl;
	}
}

void TestSqlite()
{
	SqliteManager sq;
	sq.Open("test.db");
	string createtb_sql = "create table tb_doc(id INTEGER PRIMARY KEY AUTOINCREMENT, doc_path text, doc_name text)";
	sq.ExecuteSql(createtb_sql);

	string insert_sql = "insert into tb_doc values(NULL, '比特课件','C++')";
	sq.ExecuteSql(insert_sql);

	string insert_sql2 = "insert into tb_doc values(NULL, 'snajio','C++')";
	sq.ExecuteSql(insert_sql2);

	string query_sql = "select * from tb_doc where doc_path = 'snajio'";
	int row, col;
	char** ppRet;
	/*sq.GetTable(query_sql, row, col, ppRet);
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; i < col; ++j)
		{
			cout << ppRet[i*col+j]<<" ";
		}
		cout << endl;
	}*/
	//sqlite3_free_table(ppRet);

	AutoGetTable agt(sq, query_sql, row, col, ppRet);//存在问题，越界？？、------》行列错
	for (int i = 0; i < row-1; ++i)
	{
		for (int j = 0; i < col; ++j)
		{
			cout << ppRet[i*col + j] << " ";
		}
		cout << endl;
	}
}

void TestScanManager()
{
	//ScanManager scanmgr;
	//scanmgr.Scan("D:\\");
	//scanmgr.Scan("D:\\C语言C++\\C++1");
}
void TestSarch()
{
	ScanManager::CreateIntance();//用于搜索的线程，可以不退出去进行搜索更新

	//DataManager datamgr;
	//datamgr.Init();	//初始化
	vector<std::pair<string, string>> docinfos;
	string key;
	cout << "===================" << endl;
	/*cout << "please enter fing thing :";*/
	while (std::cin >> key)
	{
		
		docinfos.clear();//因为是push_back，所以每次要清一下
		DataManager::GetInstanse()->Search(key, docinfos);
		printf("%-50s %-50s\n", "名称", "路径");
		for (const auto& e : docinfos)
		{
			//cout << e.first << "   " << e.second << endl;
			//printf("%-50s %-50s\n",e.first.c_str() , e.second.c_str());//之前那个是对象，要转成字符串
			
			//控制高亮部分输出
			string prefix, highlight, suffix;
			const string& name = e.first;
			const string& path = e.second;

			DataManager::GetInstanse()->SplitHighlight(e.first, key, prefix, highlight, suffix);
			cout << prefix;
			ColourPrintf(highlight.c_str());
			cout << suffix;
			//补齐空格，对其
			for (size_t i = name.size(); i <= 50; ++i)
			{
				cout << " ";
			}
			printf("%-50s\n", path.c_str());
		}
		cout << "+++++++++++++++++" << endl;
		cout << "please enter fing thing :";
	}
}

void TestHighlight()
{
	ColourPrintf("wo");
}
int main()
{
	//TestDirectoryList();
	//TestSqlite();

	//下面两个要在一起
	TestScanManager();
	TestSarch();
	
	//TestHighlight();

	system("pause");
	return 0;
}
