#ifndef  __COMMENT_CONVERT_H__
#define  __COMMENT_CONVERT_H__
#include <stdio.h>
#include <stdlib.h>

enum State    //注释中的状态
{
	NUL_STATE,     
	C_STATE,        //C注释
	CPP_STATE,      //C++注释
	END_STATE,      //文件结束
};



void DoCommentConvert(FILE* pfIn, FILE* pfOut);
void DoNulState(FILE* pfIn, FILE* pfOut, enum State* ps);
void DoCState(FILE* pfIn, FILE* pfOut, enum State* ps);
void DoCppState(FILE* pfIn, FILE* pfOut, enum State* ps);


#endif
