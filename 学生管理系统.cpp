# include <stdio.h>
# include <iostream>
# include <malloc.h>
# include <stdlib.h>
# include <string.h>
using namespace std;

struct every
{
	int xuehao;
	string name, sex;
	int year, month, day;
	float C_program, math, English;
	float sum, pjnum;
};

struct student
{
	struct every stu;
	struct student* pNext;
};

struct student* pHead = (struct student*)malloc(sizeof(struct student));
int len = 0;

void menu(void)
{
	printf("\n");
	printf("+---------------------------------+\n");
	printf("|          学生管理系统           |\n");
	printf("+----------------+----------------+\n");
	printf("| 1 存入学生信息 | 2 输出学生信息 |\n");
	printf("+----------------+----------------+\n");
	printf("| 3 修改学生信息 | 4 查询学生信息 |\n");
	printf("+----------------+----------------+\n");
	printf("| 5 更改排序方式 | 0 退出系统     |\n");
	printf("+----------------+----------------+\n");

	return;
}

void creat(void)
{
	struct student* pTail = pHead;
	pTail->pNext = NULL;
	if (pHead == NULL)
	{
		cout << "分配失败！" << endl;
		exit(-1);
	}
	int len;
	cin >> len;
	for (int begin = 1; begin <= len; begin++)
	{
		struct student* pNew = (struct student*)malloc(sizeof(struct student));
		cin >> pNew->stu.xuehao >> pNew->stu.name >> pNew->stu.sex >> pNew->stu.C_program >> pNew->stu.math >> pNew->stu.English;
		pNew->stu.sum = pNew->stu.C_program + pNew->stu.math + pNew->stu.English;
		pNew->stu.pjnum = pNew->stu.sum / 3;
		pTail->pNext = pNew;
		pNew->pNext = NULL;
		pTail = pNew;
	}
}

int main(void)
{
	pHead->pNext = NULL;
	int i = 1, n;
	while (i)
	{
		menu();
		printf("请输入想要的功能:");
		cin >> n;
		switch (n)
		{
			case 0: printf ("程序已退出！\n"); return 0;
			default : printf ("该程序没有这个功能！\n");
		}
		
		system("pause");
		system("cls");
	}

	return 0;
}