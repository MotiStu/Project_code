#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include "DoCommentConvert.h"  
void test()
{
	FILE* pfIn = NULL;                 //输入流，修改前的源文件
	FILE* pfOut = NULL;                //输出流，修改之后的文件
	pfIn = fopen("input.c", "r");    
	pfOut = fopen("output.c", "w");       
	if (pfIn == NULL)
	{
		perror("error opening file");
		exit(EXIT_FAILURE);
	}
	if (pfOut == NULL)
	{
		perror("error opening file");
		fclose(pfIn);
		exit(EXIT_FAILURE);
	}
	DoCommentConvert(pfIn, pfOut);
	fclose(pfIn);
	fclose(pfOut);
}
int main()
{
	test();
	printf("转换成功\n");
	system("pause");
	return 0;
}
