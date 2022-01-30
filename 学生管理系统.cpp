# include <stdio.h>
# include <iostream>
# include <malloc.h>
# include <stdlib.h>
# include <string.h>
# include <sstream>
#include <mysql.h>
using namespace std;
MYSQL mysql;
string aim[2][10] = { "NULL", "id", "name", "sex", "grade", "C_program", "math", "English", "sum", "pjnum", "NULL", "学号", "姓名", "性别", "班级", "C语言成绩", "高数成绩", "英语成绩", "总分", "平均分"};
void two(void);


bool sql_execute(MYSQL m_mysql, const char* sql)//存储表数据
{
	if (mysql_query(&m_mysql, sql))
	{
		cerr << "数据存储失败，错误信息为： " << mysql_error(&m_mysql) << endl;// 打错误log，直接显示到控制台
		return false;
	}
	else
		cout << "数据存储成功！" << endl;
	return true;
}

void sql_login(void)//sql登录(还缺一个密码登录功能)
{
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, "localhost", "root", "sz2003gd78439851", "test", 3306, NULL, 0) == NULL)
		cout << "数据库连接失败！" << endl;
	else
		cout << "数据库连接成功！" << endl;
	return;
}

void sql_printf(MYSQL_RES* res)//输出表
{
	MYSQL_ROW row;
	cout << "学号\t\t姓名\t性别\t班级\tC语言成绩\t高数成绩\t英语成绩\t总分\t平均分\n";
	while (row = mysql_fetch_row(res))
	{
		cout << row[0] << '\t';//打印学号
		cout << row[1] << '\t';//打印姓名
		cout << row[2] << '\t';//打印性别
		cout << row[3] << '\t';//打印班级
		cout << row[4] << "\t\t";//打印C语言成绩
		cout << row[5] << "\t\t";//打印高数成绩
		cout << row[6] << "\t\t";//打印英语成绩
		cout << row[7] << '\t';//打印总分
		cout << row[8] << endl;//打印平均分
	}

	return;
}

MYSQL_RES* four(int judge)//功能四：查询学生信息
{
	MYSQL_RES* res;
	cout << endl;
	cout << "+-------------------------------+\n";
	cout << "|         学生管理系统          |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 1 查询学号    | 2 查询姓名    |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 3 查询班级    | 0 返回        |\n";
	cout << "+---------------+---------------+\n";
	cout << "功能：";
	int n;
	string message;
	cin >> n;
	if (!n) return 0;
	if (n == 3) n++;
	cout << "请输入想要查询的" << aim[1][n] << "：";
	cin >> message;
	string letter = "select * from student where " + aim[0][n] + '=' + '\'' + message + '\'';
	const char* stl = letter.c_str();
	mysql_query(&mysql, stl);
	res = mysql_store_result(&mysql);
	if (!judge)
	{
		sql_printf(res);
		mysql_free_result(res);
	}
	else
	{
		system("cls");
		sql_printf(res);
		mysql_query(&mysql, stl);
		res = mysql_store_result(&mysql);
		return res;
	}
	return 0;
}

void three(void)//功能三：修改学生信息
{
	cout << endl;
	cout << "+---------------------------------+\n";
	cout << "|          学生管理系统           |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 1 修改单行数据 | 2 清空所有数据 |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 0 返回         |                |\n";
	cout << "+----------------+----------------+\n";
	cout << "功能：";
	int n;
	cin >> n;
	switch (n)
	{
	case 0: return;
	case 1:
	{
		MYSQL_RES* res = four(1);
		if (res == 0) return;
		MYSQL_ROW row;
		cout << endl;
		cout << "+---------------------------------+\n";
		cout << "|          学生管理系统           |\n";
		cout << "+----------------+----------------+\n";
		cout << "| 1 修改学号     | 2 修改姓名     |\n";
		cout << "+----------------+----------------+\n";
		cout << "| 3 修改性别     | 4 修改班级     |\n";
		cout << "+----------------+----------------+\n";
		cout << "| 5 修改C成绩    | 6 修改高数成绩 |\n";
		cout << "+----------------+----------------+\n";
		cout << "| 7 修改英语成绩 | 8 删除该数据   |\n";
		cout << "+----------------+----------------+\n";
		cout << "| 9 不修改       | 0 返回         |\n";
		cout << "+----------------+----------------+\n";
		string Newmessage;
		while (row = mysql_fetch_row(res))
		{
			cout << "对" << row[0] << "进行修改" << endl;
			cout << "功能：";
			cin >> n;
			if (!n) return;
			if (n == 9) continue;
			string letter;
			if (n == 8)
			{
				letter = "DELETE FROM student WHERE id = ";
				letter += row[0];
			}
			else
			{
				cout << "请输入学号为" << row[0] << "的" << aim[1][n] << "新数据：";
				cin >> Newmessage;
				if (5 <= n && n <= 7)
				{
					stringstream ss1, ss2, ss3;
					float sum, t;
					ss1 << row[n-1]; ss1 >> sum;
					for (int begin = 5; begin <= 7; begin++)
					{
						if (begin == n)
							continue;
						stringstream ss[3];
						ss[begin-5] << row[begin-1]; ss[begin-5] >> t;
						sum += t;
					}
					string temp, pjtemp;
					ss2 << sum; ss2 >> temp;
					ss3 << sum / 3; ss3 >> pjtemp;
					letter = "update student set " + aim[0][8] + " = \'" + temp + "\' where id = " + row[0];
					const char* stl = letter.c_str();
					sql_execute(mysql, stl);
					letter = "update student set " + aim[0][9] + " = \'" + pjtemp + "\' where id = " + row[0];
					stl = letter.c_str();
					sql_execute(mysql, stl);
				}
				letter = "update student set " + aim[0][n] + " = \'" + Newmessage + "\' where id = " + row[0];
			}
			const char* stl = letter.c_str();
			sql_execute(mysql, stl);
			cout << endl;
		}
		mysql_free_result(res);
		two();
	} break;
	case 2:
	{
		string letter = "DELETE FROM student";
		const char* stl = letter.c_str();
		sql_execute(mysql, stl);
	}break;
	default: cout << "该程序没有这个功能！\n";
	}
	return;
}

void two()//功能二：输出学生信息
{
	MYSQL_RES* res;
	MYSQL_ROW row;
	mysql_query(&mysql, "select * from student");
	res = mysql_store_result(&mysql);
	sql_printf(res);
	mysql_free_result(res);
	return;
}

void one(void)//功能一：存入学生信息
{
	cout << "请输入需要存入的学生信息数量：";
	int num;
	cin >> num;
	for (int begin = 1; begin <= num; begin++)
	{
		stringstream ss1, ss2, ss3, ss4, ss5;
		string id, name, sex, grade, sid, sC, sm, sE, ss, spj;
		float C_program, math, English;
		cout << "请输入第" << begin << "行数据" << endl;
		cout << "学号："; cin >> id;
		cout << "姓名："; cin >> name;
		cout << "性别："; cin >> sex;
		cout << "班级："; cin >> grade;
		cout << "C语言成绩："; cin >> C_program; ss1 << C_program; ss1 >> sC;
		cout << "高等数学成绩："; cin >> math; ss2 << math; ss2 >> sm;
		cout << "英语成绩："; cin >> English; ss3 << English; ss3 >> sE;
		ss4 << C_program + math + English; ss4 >> ss;
		ss5 << (C_program + math + English) / 3; ss5 >> spj;
		string letter = "INSERT INTO student (id, `name`, sex, grade, C_program, math, English, `sum`, pjnum) VALUES (" + id + ",'" + name + "','" + sex + "'," + grade + ',' + sC + ',' + sm + ',' + sE + ',' + ss + ',' + spj + ')';
		//fprintf(letter, "INSERT INTO student (id, `name`, sex, grade, C_program, math, English, `sum`, pjnum) VALUES (%d, \'%s\', \'%s\', %d, %f, %f, %f, %f, %f)" , id, name, sex, grade, C_program, math, English, sum, pjnum);
		const char* sql1 = letter.c_str();
		sql_execute(mysql, sql1);
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

int main(void)
{
	sql_login();
	int i = 1, n;
	while (i)
	{
		menu();
		cout << "请输入想要的功能:";
		cin >> n;
		switch (n)
		{
		case 0: cout << "程序已退出！\n"; i = 0; break;
		case 1: one();
		case 2: two(); break;
		case 3: three(); break;
		case 4: four(0); break;
		default: cout << "该程序没有这个功能！\n";
		}
		system("pause");
		system("cls");
	}


	mysql_close(&mysql);
	return 0;
}