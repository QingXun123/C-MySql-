# include <stdio.h>
# include <iostream>
# include <malloc.h>
# include <stdlib.h>
# include <string.h>
# include <sstream>
#include <mysql.h>
using namespace std;
MYSQL mysql;
char messageprintf[11][100] = {"NULL", "学号", "姓名", "性别", "班级", "C语言成绩", "高数成绩", "英语成绩"};

struct every
{
	int id;
	char name[100], sex[10];
	int grade;//year, month, day;
	float C_program, math, English;
	float sum, pjnum;
};

struct student
{
	struct every stu;
	struct student* pNext;
};

bool sql_execute(MYSQL m_mysql, const char* sql, string id)
{
	if (mysql_query(&m_mysql, sql))
	{
		string letter = "delete from student where id = " + id;
		const char* stl = letter.c_str();
		if (mysql_query(&mysql, stl))
			cerr << "数据存储失败，错误信息为： " << mysql_error(&m_mysql) << endl;// 打错误log，直接显示到控制台
		else
		{
			mysql_query(&m_mysql, sql);
			cout << "数据存储成功！" << endl;
		}
		return false;
	}
	else
		cout << "数据存储成功！" << endl;
	return true;
}

void sql_login(void)//sql登录(还缺一个手动登录功能)
{
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, "localhost", "root", "sz2003gd78439851", "test", 3306, NULL, 0) == NULL)
		cout << "连接失败！" << endl;
	else
		cout << "连接成功！" << endl;
	return;
}

struct student * sql_message_read(struct student * pHead)
{
	struct student* pTail = pHead;
	pTail->pNext = NULL;
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_query(&mysql, "select * from student");
	res = mysql_store_result(&mysql);
	while (row = mysql_fetch_row(res))
	{
		stringstream ss0, ss1, ss2, ss3, ss4, ss5, ss6, ss7, ss8;
		struct student* pNew = (struct student*)malloc(sizeof(struct student));
		ss0 << row[0]; ss0 >> pNew->stu.id;
		ss1 << row[1]; ss1 >> pNew->stu.name;
		ss2 << row[2]; ss2 >> pNew->stu.sex;
		ss3 << row[3]; ss3 >> pNew->stu.grade;
		ss4 << row[4]; ss4 >> pNew->stu.C_program;
		ss5 << row[5]; ss5 >> pNew->stu.math;
		ss6 << row[6]; ss6 >> pNew->stu.English;
		ss7 << row[7]; ss7 >> pNew->stu.sum;
		ss8 << row[8]; ss8 >> pNew->stu.pjnum;
		pTail->pNext = pNew;
		pNew->pNext = NULL;
		pTail = pNew;
	}
	mysql_free_result(res);
	return pTail;
}

void sql_message_save(struct student * pHead)//功能一：存入学生信息
{
	pHead = pHead->pNext;
	while (pHead != NULL)
	{
		string letter = "insert into student (id, `name`, sex, grade, C_program, math, English, `sum`, pjnum) values (";
		string t[10];
		stringstream ss0, ss1, ss2, ss3, ss4, ss5, ss6, ss7, ss8;
		ss0 << pHead->stu.id; ss0 >> t[0];
		ss1 << pHead->stu.name; ss1 >> t[1];
		ss2 << pHead->stu.sex; ss2 >> t[2];
		ss3 << pHead->stu.grade; ss3 >> t[3];
		ss4 << pHead->stu.C_program; ss4 >> t[4];
		ss5 << pHead->stu.math; ss5 >> t[5];
		ss6 << pHead->stu.English; ss6 >> t[6];
		ss7 << pHead->stu.sum; ss7 >> t[7];
		ss8 << pHead->stu.pjnum; ss8 >> t[8];
		letter = letter + t[0] + " , '" + t[1] + "', '" + t[2] + "', " + t[3] + ", " + t[4] + ", " + t[5] + ", " + t[6] + ", " + t[7] + ", " + t[8] + ')';
		const char* sql1 = letter.c_str();
		sql_execute(mysql, sql1, t[0]);
		pHead = pHead->pNext;
	}
	return;
}

void two(struct student * pHead)//输出学生信息
{
	struct student* pTemp = pHead->pNext;
	if (pTemp == NULL)
	{
		cout << "没有数据！" << endl;
		return;
	}
	cout << "学号\t\t姓名\t性别\t班级\tC语言成绩\t高数成绩\t英语成绩\t总分\t平均分\n";
	while (pTemp != NULL)
	{
		cout << pTemp->stu.id << '\t';//打印学号
		cout << pTemp->stu.name << '\t';//打印姓名
		cout << pTemp->stu.sex << '\t';//打印性别
		cout << pTemp->stu.grade << '\t';//打印班级
		cout << pTemp->stu.C_program << "\t\t";//打印C语言成绩
		cout << pTemp->stu.math << "\t\t";//打印高数成绩
		cout << pTemp->stu.English << "\t\t";//打印英语成绩
		cout << pTemp->stu.sum << '\t';//打印总分
		cout << pTemp->stu.pjnum << endl;//打印平均分
		pTemp = pTemp->pNext;
	}

	return;
}

void menu(void)//菜单
{
	cout << endl;
	cout << "+---------------------------------+\n";
	cout << "|          学生管理系统           |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 1 存入学生信息 | 2 输出学生信息 |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 3 修改学生信息 | 4 查询学生信息 |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 5 更改排序方式 | 0 退出系统     |\n";
	cout << "+----------------+----------------+\n";

	return;
}

struct student* creat(struct student* pHead, int len)//建立链表
{
	struct student* pTail = pHead;
	pTail->pNext = NULL;
	if (pHead == NULL)
	{
		cout << "分配失败！" << endl;
		exit(-1);
	}
	for (int begin = 1; begin <= len; begin++)
	{
		struct student* pNew = (struct student*)malloc(sizeof(struct student));
		cout << "学号: ";
		cin >> pNew->stu.id;
		cout << "姓名: ";
		cin >> pNew->stu.name;
		cout << "性别: ";
		cin >> pNew->stu.sex;
		cout << "班级: ";
		cin >> pNew->stu.grade;
		cout << "C语言成绩: ";
		cin >> pNew->stu.C_program;
		cout << "高等数学成绩: ";
		cin >> pNew->stu.math;
		cout << "英语成绩: ";
		cin >> pNew->stu.English;
		pNew->stu.sum = pNew->stu.C_program + pNew->stu.math + pNew->stu.English;
		pNew->stu.pjnum = pNew->stu.sum / 3;
		pTail->pNext = pNew;
		pNew->pNext = NULL;
		pTail = pNew;
	}

	return pTail;
}

void one(struct student * pHead, struct student * pTail)//功能一：输入学生信息
{
	cout << "请输入需要存入的学生信息数量：";
	int num;
	cin >> num;
	cout << "请输入数据" << endl;
	struct student* pTemp = (struct student*)malloc(sizeof(struct student));
	struct student* pTemp_Tail = creat(pTemp, num);
	struct student* temp = pHead->pNext;
	pTemp_Tail->pNext = temp;
	*pHead = *pTemp;
	
	return;
}

int main(void)
{
	sql_login();
	struct student* pHead = (struct student*)malloc(sizeof(struct student));
	struct student* pTail = sql_message_read(pHead);
	int i = 1, n;
	while (i)
	{
		menu();
		cout << "请输入想要的功能:";
		cin >> n;
		switch (n)
		{
			case 0: cout << "程序已退出！\n"; i = 0; break;
			case 1: one(pHead, pTail);
			case 2: two(pHead); break;
			default: cout << "该程序没有这个功能！\n";
		}

		system("pause");
		system("cls");
	}

	sql_message_save(pHead);
	mysql_close(&mysql);
	return 0;
}