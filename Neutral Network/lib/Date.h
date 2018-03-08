#ifndef AROYA_DATE
#define AROYA_DATE

#include<sstream>
#include<string>
using namespace std;

class AroyaDate {
public:
	AroyaDate();
	/*date format
	YYYY?MM?DD
	?是任何非数字字符
	*/
	void input(const string&dateStr);
	void input(const int&yourYear, const int&yourMonth, const int&yourDay);
	//获取星期
	int getWeekday();
	//获取与标准时间的日期差
	int getDateInstant();
	//获取月份
	int getMonth();
	//判断节假日
	int getHoliday();
	int getWorkingDay();
	int getYear();
private:
	int year, month, day, weekday, instant;
	const static int StartYear = 2000;
	const static int StartMonth = 1;
	const static int StartDay = 1;
	const static int StartWeekday = 6;

	const static int January = 31;
	const static int February = 28;
	const static int FebruaryE = 29;
	const static int March = 31;
	const static int April = 30;
	const static int May = 31;
	const static int June = 30;
	const static int July = 31;
	const static int August = 31;
	const static int September = 30;
	const static int October = 31;
	const static int November = 30;
	const static int December = 31;
};
#endif // !AROYA_DATE
