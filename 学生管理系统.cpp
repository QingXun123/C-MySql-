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
string order[2] = { "ASC", "DESC" }, ordertemp = order[0], order_main = "id";
int od = 0;
void two(void);
void three(void);

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

void sql_login(void)//sql登录
{
	string account, password;
	cout << "请输入账号密码" << endl;
	cout << "账号：";
	cin >> account;
	cout << "密码：";
	cin >> password;
	const char* taccount = account.c_str();
	const char* tpassword = password.c_str();
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, "localhost", taccount, tpassword, "test", 3306, NULL, 0) == NULL)
	{
		cout << "数据库连接失败，请重新登录！" << endl;
		system("pause");
		system("cls");
		sql_login();
	}
	else
	{
		system("cls");
		cout << "数据库连接成功！" << endl;
	}
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

void four_menu(int judge)
{
	cout << "+---------------------------------+\n";
	cout << "|          ";
	if (judge)
		cout << "修改";
	else
		cout << "查询";
	cout << "学生信息           | \n";
	cout << "+----------------+----------------+\n";
	cout << "| 1 查询学号     | 2 查询姓名     |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 3 查询班级     | 4 按分段查找   |\n";
	cout << "+----------------+----------------+\n";
	if (judge)
		cout << "|                ";
	else
		cout << "| 5 统计学生信息 ";
	cout << "| 0 返回         |\n";
	cout << "+----------------+----------------+\n";
	return;
}

string four_fenduan(void)
{
	string letter;
	float a, b;
	string sa, sb;
	stringstream ssa, ssb;
	cout << "请输入两个分段" << endl << "第一个分段：";
	cin >> a;
	cout << "第二个分段：";
	cin >> b;
	if (a > b)
		swap(a, b);
	ssa << a; ssa >> sa;
	ssb << b; ssb >> sb;
	letter = "select * from student where `sum` >= \'" + sa + "\' && sum <= \'" + sb + '\'';
	return letter;
}

MYSQL_RES* four_chaxun(MYSQL_RES* &res, int n, int judge)
{
	string message, letter;
	cout << "请输入想要查询的" << aim[1][n] << "：";
	cin >> message;
	letter = "select * from student where `" + aim[0][n] + "` like" + '\'' + message + "%\'";
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

void four_statistics(MYSQL_RES* &res)
{
	MYSQL_ROW row;
	string letter = "select * from student";
	const char* stl = letter.c_str();
	mysql_query(&mysql, stl);
	res = mysql_store_result(&mysql);
	float Spjnum = 0, math_pjnum = 0, C_program_pjnum = 0, English_pjnum = 0, dis_pjnum_num = 0, dis_C_program_num = 0, dis_math_num = 0, dis_English_num = 0;
	int len = 0;
	while (row = mysql_fetch_row(res))
	{
		stringstream ss4, ss5, ss6, ss8;
		float Ct, mt, Et, pjt;
		ss4 << row[4]; ss4 >> Ct;
		ss5 << row[5]; ss5 >> mt;
		ss6 << row[6]; ss6 >> Et;
		ss8 << row[8]; ss8 >> pjt;
		C_program_pjnum += Ct;//打印C语言成绩
		if (Ct < 60)
			dis_C_program_num++;
		math_pjnum += mt;//打印高数成绩
		if (mt < 60)
			dis_math_num++;
		English_pjnum += Et;//打印英语成绩
		if (Et < 60)
			dis_English_num++;
		if (pjt < 60)
			dis_pjnum_num++;
		len++;
	}
	C_program_pjnum /= len;
	math_pjnum /= len;
	English_pjnum /= len;
	Spjnum = (C_program_pjnum + math_pjnum + English_pjnum) / 3;
	cout << endl << "全校平均分\t\t" << Spjnum << endl;
	cout << "C语言平均分\t\t" << C_program_pjnum << endl;
	cout << "高数平均分\t\t" << math_pjnum << endl;
	cout << "英语平均分\t\t" << English_pjnum << endl << endl;
	cout << "平均分不及格总人数\t" << dis_pjnum_num << endl;
	cout << "C语言不及格人数\t\t" << dis_C_program_num << endl;
	cout << "高数不及格人数\t\t" << dis_math_num << endl;
	cout << "英语不及格人数\t\t" << dis_English_num << endl << endl;
	system("pause");
	return;
}

void three_adjust_sum_pjnum(MYSQL_ROW row, int n)
{
	string letter;
	stringstream ss1, ss2, ss3;
	float sum, t;
	ss1 << row[n - 1]; ss1 >> sum;
	for (int begin = 5; begin <= 7; begin++)
	{
		if (begin == n)
			continue;
		stringstream ss[3];
		ss[begin - 5] << row[begin - 1]; ss[begin - 5] >> t;
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
	return;
}

void three_empty_data(void)
{
	int n;
	cout << "是否清空数据？(1/0)" << endl;
	cin >> n;
	if (n)
	{
		string letter = "DELETE FROM student";
		const char* stl = letter.c_str();
		mysql_query(&mysql, stl);
	}
	return;
}

void five(void)//功能五：更改排序方式
{
	system("cls");
	cout << "+-------------------------------+\n";
	cout << "|         更改排序方式          |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 1 学号排序    | 2 班级排序    |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 3 C语言排序   | 4 高数排序    |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 5 英语排序    | 6 总分排序    |\n";
	cout << "+---------------+---------------+\n";
	cout << "| 7 升降序改变  | 0 返回        |\n";
	cout << "+---------------+---------------+\n";
	int n;
	cout << "功能：";
	cin >> n;
	if (!n) return;
	if (n == 7)
	{
		if (od)
			od = 0;
		else
			od = 1;
		ordertemp = order[od];
		return;
	}
	if (n > 1)
		n += 2;
	order_main = aim[0][n];
	return;
}

MYSQL_RES* four(int judge)//功能四：查询学生信息
{
	system("cls");
	MYSQL_RES* res;
	cout << endl;
	four_menu(judge);
	cout << "功能：";
	int n;
	cin >> n;
	string letter;
	switch (n)
	{
	case 0: 
	{
		if (judge)
			three();
		return 0;
	}
	case 3: n++; break;
	case 4: letter = four_fenduan(); break;
	case 1: case 2: break;
	case 5:
	{
		if (judge) { cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); four(judge); break; }
		four_statistics(res);
		mysql_free_result(res);
	}return 0;
	default: cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); four(judge);
	}
	res = four_chaxun(res, n, judge);
	if (!judge)
		system("pause");
	return res;
}

void three(void)//功能三：修改学生信息
{
	system("cls");
	cout << endl;
	cout << "+---------------------------------+\n";
	cout << "|          修改学生信息           |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 1 修改单行数据 | 2 清空所有数据 |\n";
	cout << "+----------------+----------------+\n";
	cout << "|                | 0 返回         |\n";
	cout << "+----------------+----------------+\n";
	cout << "功能：";
	int n;
	cin >> n;
	switch (n)
	{
	case 0: return;
	case 1:
	{
	end2:;
		MYSQL_RES* res = four(1);
		if (res == 0) return;
		MYSQL_ROW row;
		cout << endl;
		cout << "+---------------------------------+\n";
		cout << "|          修改单行数据           |\n";
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
		end1:;
			cout << "对" << row[0] << "进行修改" << endl;
			cout << "功能：";
			cin >> n;
			string letter;
			switch (n)
			{
			case 0: { mysql_free_result(res); goto end2;}
			case 9: continue;
			case 8:
			{
				letter = "DELETE FROM student WHERE id = ";
				letter += row[0];
			} continue;
			case 1:case 2:case 3:case 4:case 5:case 6:case 7:
			{
				cout << "请输入学号为" << row[0] << "的" << aim[1][n] << "新数据：";
				cin >> Newmessage;
				if (5 <= n && n <= 7)
					three_adjust_sum_pjnum(row, n);
				letter = "update student set " + aim[0][n] + " = \'" + Newmessage + "\' where id = " + row[0];
			} break;
			default: cout << "该程序没有这个功能！请重新输入" << endl; goto end1;
			}
			const char* stl = letter.c_str();
			sql_execute(mysql, stl);
			cout << endl;
		}
		mysql_free_result(res);
		two();
	} break;
	case 2: three_empty_data(); break;
	default: cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); three(); return;
	}
	system("pause");
	return;
}

void two()//功能二：输出学生信息
{
	MYSQL_RES* res;
	string letter = "select * from student ORDER BY `" + order_main + "` " + ordertemp;
	const char* stl = letter.c_str();
	mysql_query(&mysql, stl);
	res = mysql_store_result(&mysql);
	sql_printf(res);
	mysql_free_result(res);
	system("pause");
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
		case 5: five(); break;
		default: cout << "该程序没有这个功能！\n"; system("pause");
		}
		system("cls");
	}


	mysql_close(&mysql);
	return 0;
}