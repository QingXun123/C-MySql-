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

bool sql_connect(void)//sql连接
{
	mysql_init(&mysql);
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (mysql_real_connect(&mysql, "localhost", "root", "sz2003gd78439851", "test", 3306, NULL, 0) == NULL)
	{
		cout << "数据库连接失败，请检查数据库服务是否开启！" << endl;
		return 1;
	}
	else
	{
		system("cls");
		cout << "数据库连接成功！" << endl;
	}
	return 0;
}

void sql_login()//登录注册账号
{
	cout << "+---------------------------------+\n";
	cout << "|           学生管理系统          |\n";
	cout << "+----------------+----------------+\n";
	cout << "| 1 登录账号     | 2 注册账号     |\n";
	cout << "+----------------+----------------+\n";
	string username, password;
	char letter[1024];
	bool judge = 1;
	end:;
	cout << "功能：";
	int n;
	cin >> n;
	switch (n)
	{
	case 1:
		while (judge)
		{
			MYSQL_ROW row;
			MYSQL_RES* res;
			cout << "请输入账号：";
			cin >> username;
			cout << "请输入密码：";
			cin >> password;
			sprintf(letter, "SELECT * FROM `user` WHERE `username` = '%s';", username.c_str());
			const char* stl = letter;
			mysql_query(&mysql, stl);
			res = mysql_store_result(&mysql);
			row = mysql_fetch_row(res);
			if (row == NULL)
			{
				mysql_free_result(res);
				cout << "账号不存在！" << endl;
				continue;
			}
			if (row[1] != password)
				cout << "密码错误" << endl;
			else
			{
				system("cls");
				cout << "登录成功！" << endl;
				judge = 0;
			}
			mysql_free_result(res);
		} break;
	case 2:
		while (judge)
		{
			cout << "请输入账号：";
			cin >> username;
			cout << "请输入密码：";
			cin >> password;
			sprintf(letter, "INSERT INTO `user` (username, `password`) VALUES ('%s', '%s');", username.c_str(), password.c_str());
			const char* stl = letter;
			if (mysql_query(&mysql, stl))
				cout << "注册失败，账号名字已经被使用！" << endl << endl;
			else
			{
				system("cls");
				cout << "注册成功！" << endl;
				judge = 0;
			}
		} break;
	default: cout << "该程序没有这个功能！请重新输入" << endl << endl; goto end;
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

void four_menu(bool judge)//功能四菜单
{
	cout << "+---------------------------------+\n";
	cout << "|          ";
	if (judge)//根据功能的不同来选择输出内容的不同
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

MYSQL_RES* four_refer_deassign(MYSQL_RES* res, const char* stl, bool judge)//输出查询结果并且重新给res赋值
{
	if (!judge)
		sql_printf(res);
	else
	{
		system("cls");
		sql_printf(res);
		mysql_query(&mysql, stl);
		res = mysql_store_result(&mysql);
	}
	return res;
}

MYSQL_RES* four_refer_section(bool judge)//分段查找
{
	MYSQL_RES* res;
	char letter[1024];
	float a, b;
	cout << "请输入两个分段" << endl << "第一个分段：";
	cin >> a;
	cout << "第二个分段：";
	cin >> b;
	if (a > b)
		swap(a, b);
	sprintf(letter, "SELECT * FROM student WHERE `sum` >= \' %.2f \' && sum <= \' %.2f \'", a, b);
	const char* stl = letter;
	mysql_query(&mysql, stl);
	res = mysql_store_result(&mysql);
	return four_refer_deassign(res, stl, judge);
}

MYSQL_RES* four_refer(MYSQL_RES* &res, int n, int judge)//查询学生信息
{
	string message;
	char letter[1024];
	cout << "请输入想要查询的" << aim[1][n] << "：";
	cin >> message;
	sprintf(letter, "SELECT * FROM student WHERE `%s` like '%s';", aim[0][n].c_str(), message.c_str());//模糊查找
	const char* stl = letter;
	mysql_query(&mysql, stl);
	res = mysql_store_result(&mysql);
	return four_refer_deassign(res, stl, judge);
}

int four_statistics_row_num(MYSQL_RES* &res)//返回表内学生信息数量
{
	MYSQL_ROW row;
	mysql_query(&mysql, "SELECT COUNT(1) FROM student");
	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);
	stringstream ssum;
	int row_num;
	ssum << row[0]; ssum >> row_num;
	return row_num;
}

void four_statistics(MYSQL_RES* &res)//统计学生信息
{
	MYSQL_ROW row;
	int row_num = four_statistics_row_num(res);
	mysql_query(&mysql, "SELECT * FROM student");
	res = mysql_store_result(&mysql);
	float Spjnum = 0, math_pjnum = 0, C_program_pjnum = 0, English_pjnum = 0, dis_pjnum_num = 0, dis_C_program_num = 0, dis_math_num = 0, dis_English_num = 0;
	while (row = mysql_fetch_row(res))
	{
		stringstream ss4, ss5, ss6, ss8;
		float Ct, mt, Et, pjt;
		ss4 << row[4]; ss4 >> Ct;
		ss5 << row[5]; ss5 >> mt;
		ss6 << row[6]; ss6 >> Et;
		ss8 << row[8]; ss8 >> pjt;
		C_program_pjnum += Ct / row_num;//打印C语言成绩
		if (Ct < 60)
			dis_C_program_num++;
		math_pjnum += mt / row_num;//打印高数成绩
		if (mt < 60)
			dis_math_num++;
		English_pjnum += Et / row_num;//打印英语成绩
		if (Et < 60)//记录不及格人数
			dis_English_num++;
		if (pjt < 60)
			dis_pjnum_num++;
	}
	Spjnum = (C_program_pjnum + math_pjnum + English_pjnum) / 3;
	cout << "------------------------------------";
	cout << endl << "全校平均分\t\t" << Spjnum << endl;
	cout << "C语言平均分\t\t" << C_program_pjnum << endl;
	cout << "高数平均分\t\t" << math_pjnum << endl;
	cout << "英语平均分\t\t" << English_pjnum << endl << endl;
	cout << "平均分不及格总人数\t" << dis_pjnum_num << endl;
	cout << "C语言不及格人数\t\t" << dis_C_program_num << endl;
	cout << "高数不及格人数\t\t" << dis_math_num << endl;
	cout << "英语不及格人数\t\t" << dis_English_num << endl;
	cout << "------------------------------------" << endl;
	system("pause");
	return;
}

void three_adjust_sum_pjnum(MYSQL_ROW row, int n)//计算总分和平均分并存储
{
	char letter1[1024], letter2[1024];
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
	sprintf(letter1, "UPDATE student SET %s = \'%.2f\' WHERE id = %s", aim[0][8].c_str(), sum, row[0]);
	const char* stl = letter1;
	sql_execute(mysql, stl);
	sprintf(letter2, "UPDATE student SET %s = \'%.2f\' WHERE id = %s", aim[0][9].c_str(), sum / 3, row[0]);
	stl = letter2;
	sql_execute(mysql, stl);
	return;
}

void three_empty_data(void)//清空表数据
{
	int n;
	cout << "是否清空数据？(1/0)" << endl;//防止误按
	cin >> n;
	if (n)
		mysql_query(&mysql, "DELETE FROM student");
	return;
}

void five(void)//功能五：更改排序方式
{
	while (1)
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
		switch (n)
		{
		case 0: return;
		case 1: order_main = aim[0][n]; break;
		case 2: case 3: case 4: case 5: case 6: n += 2; order_main = aim[0][n]; break;
		case 7:
			od = 1 - od;
			ordertemp = order[od];
			break;
		default: cout << "该程序没有这个功能！\n"; system("pause");
		}
		two();
	}
	return;
}

MYSQL_RES* four(bool judge)//功能四：查询学生信息，根据judge值的不同而不同，与功能三共用
{
	MYSQL_RES* res;
	while (1)
	{
		system("cls");
		cout << endl;
		four_menu(judge);
		cout << "功能：";
		int n;
		cin >> n;
		string letter;
		switch (n)
		{
		case 0:
			if (judge)
				three();
			return 0;
		case 3: n++; break;
		case 4:
			if (judge) return four_refer_section(judge);//在功能三中返回res值并对其进行修改操作
			four_refer_section(judge);//返回分段查询到的信息并输出
			if (!judge)
				system("pause");
			break;
		case 1: case 2: break;
		case 5:
			if (judge) { cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); four(judge); break; }//功能三中没有这个功能
			four_statistics(res);
			mysql_free_result(res);
			four(judge);
			return 0;
		default: cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); four(judge); return 0;
		}
		res = four_refer(res, n, judge);
		if (!judge)
			system("pause");
		else
			break;
	}
	return res;
}

void three(void)//功能三：修改学生信息
{
	while (1)
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
				char letter[1024];
				switch (n)
				{
				case 0: { mysql_free_result(res); goto end2; }
				case 9: continue;
				case 8: sprintf(letter, "DELETE FROM student WHERE id = %s", row[0]); break;
				case 1:case 2:case 3:case 4:case 5:case 6:case 7:
					cout << "请输入学号为" << row[0] << "的" << aim[1][n] << "新数据：";
					cin >> Newmessage;
					if (5 <= n && n <= 7)
						three_adjust_sum_pjnum(row, n);
					sprintf(letter, "UPDATE student SET %s = '%s' WHERE id = %s", aim[0][n].c_str(), Newmessage.c_str(), row[0]);
					break;
				default: cout << "该程序没有这个功能！请重新输入" << endl; goto end1;
				}
				const char* stl = letter;
				sql_execute(mysql, stl);
				if (5 <= n && n <= 7)
				{
					float rw, t;
					stringstream ss1, ss2;
					ss1 << row[7]; ss1 >> rw;
					ss2 << row[n-1]; ss2 >> t;
					cout << rw << ' ' << t << endl;
					rw -= t;
					float sum = rw + stoi(Newmessage);
					float pjnum = sum / 3;
					sprintf(letter, "UPDATE student SET sum = '%f' WHERE id = %s", sum, row[0]);
					stl = letter;
					sql_execute(mysql, stl);
					sprintf(letter, "UPDATE student SET pjnum = '%f' WHERE id = %s", pjnum, row[0]);
					stl = letter;
					sql_execute(mysql, stl);
				}
				cout << endl;
			}
			two();
			mysql_free_result(res);
		} break;
		case 2: three_empty_data(); break; system("pause");
		default: cout << "该程序没有这个功能！\n"; system("pause"); system("cls"); three(); return;
		}
	}
	return;
}

void two()//功能二：输出学生信息
{
	MYSQL_RES* res;
	char letter[1024];
	sprintf(letter, "SELECT * FROM student ORDER BY `%s` %s", order_main.c_str(), ordertemp.c_str());
	const char* stl = letter;
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
		string id, name, sex, grade;
		float C_program, math, English, sum, pjnum;
		cout << "请输入第" << begin << "行数据" << endl;
		cout << "学号："; cin >> id;
		cout << "姓名："; cin >> name;
		cout << "性别："; cin >> sex;
		cout << "班级："; cin >> grade;
		cout << "C语言成绩："; cin >> C_program;
		cout << "高等数学成绩："; cin >> math;
		cout << "英语成绩："; cin >> English;
		sum = C_program + math + English;
		pjnum = sum / 3;
		char letter[1024];
		sprintf(letter, "INSERT INTO student (id, `name`, sex, grade, C_program, math, English, `sum`, pjnum) VALUES (%s, \'%s\', \'%s\', %s, %.2f, %.2f, %.2f, %.2f, %.2f)" , id.c_str(), name.c_str(), sex.c_str(), grade.c_str(), C_program, math, English, sum, pjnum);
		const char* sql1 = letter;
		sql_execute(mysql, sql1);
		cout << endl;
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
	if (sql_connect()) return 0;
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
		case 5: five();  break;
		default: cout << "该程序没有这个功能！\n"; system("pause");
		}
		system("cls");
	}


	mysql_close(&mysql);
	return 0;
}