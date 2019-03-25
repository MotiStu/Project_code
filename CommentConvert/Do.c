#define _CRT_SECURE_NO_WARNINGS 1
#include "DoCommentConvert.h"
#include <stdio.h>
#include <stdlib.h>


void DoCommentConvert(FILE* pfIn, FILE* pfOut)
{
	enum State state = NUL_STATE;        //刚进入文件按未知处理
	while (state != END_STATE)
	{
		switch (state)
		{
		case NUL_STATE:
		{
			DoNulState(pfIn, pfOut, &state);
		}
			break;
		case C_STATE:
		{
			DoCState(pfIn, pfOut, &state);
		}
			break;
		case CPP_STATE:
		{
			DoCppState(pfIn, pfOut, &state);
		}
			break;
		default:
			break;

		}

	}

}

void DoNulState(FILE* pfIn, FILE* pfOut,enum State* ps)
{
	int first = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '/':
	{
				int second = 0;
				second = fgetc(pfIn);
				switch (second)
				{
				case '/':
				{
						*ps = CPP_STATE;
						fputc(first, pfOut);
						fputc(second, pfOut);
							
				}
					break;
				case '*':
				{
						*ps = C_STATE;
						fputc('/', pfOut);
						fputc('/', pfOut);          //将C注释中的“/*”改为“//”
							
				}
					break;
				default:
				{
					   fputc(first, pfOut);
					   fputc(second, pfOut);
				}
					break;
				}
	}
			break;
	case EOF:
	{	
				*ps = END_STATE;
	}
			break;
	default:
	{
			   fputc(first, pfOut);
	}
		break;
	}

	
}

void DoCState(FILE* pfIn, FILE* pfOut,enum State* ps)
{
	int first = 0;
	int second = 0;
	first = fgetc(pfIn);
	switch (first)
	{
	case '*':        //C注释结束的*
	{
				int second = fgetc(pfIn);
				switch (second)
				{
				case '/':
				{
							*ps = NUL_STATE;
							fputc('\n', pfOut);
				}
					break;
				case '*':       //形似/*sfdf***/中的多*问题找到最后一个*
				{
							fputc(first, pfOut);
							ungetc(second, pfIn);       //ungetc    必须是输入流而不是输出流
				}
					break;
				default:
				{
						   fputc(first, pfOut);
						   fputc(second, pfOut);
				}
					break;
				}
				
	}
		break;
	case '\n':          //当在c注释中遇到了换行时，每行加上"//"
	{
				 fputc(first, pfOut);
				 fputc('/', pfOut);
				 fputc('/', pfOut);	
				 
	}
		break;
	default:
	{
			   fputc(first, pfOut);
	}
		break;
	}
}
void DoCppState(FILE* pfIn, FILE* pfOut,enum State* ps)
{
	int first = fgetc(pfIn);
	switch (first)
	{
	case '\n':
	{
				 fputc(first, pfOut);
				 *ps = NUL_STATE;
	}
		break;
	case EOF:
	{
				*ps = END_STATE;
	}
		break;
	default:
	{
			   fputc(first, pfOut);
	}
		break;
	}

}
