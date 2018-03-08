#include"Date.h"

bool static PureNumber(const char&);

AroyaDate::AroyaDate(){
	//empty
}
void AroyaDate::input(const string&str) {
	string buffer;
	stringstream sst;
	char temp;
	int position = 0;
	int i, l = str.length();
	//读取年月日
	for (i = 0; i < l; i++) {
		if (PureNumber(temp = str[i])) {
			buffer += temp;
		}
		else {
			sst.str(buffer);
			sst.clear();
			switch (position)
			{
			case 0:
				sst >> year;
				break;
			case 1:
				sst >> month;
				break;
			default:
				break;
			}
			position++;
			buffer.clear();
		}
	}
	//跳出时最后的day存储在buffer中
	sst.str(buffer);
	sst.clear();
	sst >> day;

	input(year, month, day);
}
void AroyaDate::input(const int&yourYear, const int&yourMonth, const int&yourDay) {
	year = yourYear;
	month = yourMonth;
	day = yourDay;

	int i;
	//计算日期差instant和weekday
	instant = 0;
	//年
	for (i = StartYear; i < year; i++) {
		if (i % 100 && !(i % 4) || !(i % 400)) {
			instant += 366;
		}
		else instant += 365;
	}
	//月
	for (i = StartMonth; i < month; i++) {
		switch (i)
		{
		case 1:
			instant += January;
			break;
		case 2:
			//特殊情况
			if (year % 100 && !(year % 4) || !(year % 400)) {
				instant += 29;
			}
			else instant += 28;
			break;
		case 3:
			instant += March;
			break;
		case 4:
			instant += April;
			break;
		case 5:
			instant += May;
			break;
		case 6:
			instant += June;
			break;
		case 7:
			instant += July;
			break;
		case 8:
			instant += August;
			break;
		case 9:
			instant += September;
			break;
		case 10:
			instant += October;
			break;
		case 11:
			instant += November;
			break;
		case 12:
			instant += December;
			break;
		default:
			printf("Error month:%d\n", i);
			system("pause");
			break;
		}
	}
	//日
	instant += day - StartDay;
	//周
	{
		int temp = instant % 7;
		if (temp > 1) {
			weekday = temp - 1;
		}
		else {
			weekday = temp + 6;
		}
	}
}
bool static PureNumber(const char&t) {
	return t >= '0'&&t <= '9';
}
int AroyaDate::getWeekday() { return weekday; }
int AroyaDate::getDateInstant() { return instant; }
int AroyaDate::getMonth() { return month; }
int AroyaDate::getWorkingDay() {
	if (getHoliday() == 0
		&& getWeekday() > 0 && getWeekday() < 6
		)return 1;
	else return 0;
}
int AroyaDate::getHoliday() {
	switch (month)
	{
	case 1:
		switch (day)
		{
		case 17://Martin Luther King Jr. Day

			return 1;
		default:
			return 0;
		}
	case 2:
		switch (day)
		{
		case 21://Washington's Birthday

			return 1;
		default:
			return 0;
		}
	case 4:
		switch (day)
		{
		case 15://DC Emancipation Day
			return 1;
		default:
			return 0;
		}
	case 5:
		switch (day)
		{
		case 30://Memorial Day
			return 1;
		default:
			return 0;
		}
	case 6:
		switch (day)
		{
		case 4://Independence Day

			return 1;
		default:
			return 0;
		}
	case 9:
		switch (day)
		{
		case 5://Labor Day
			return 1;
		default:
			return 0;
		}
	case 10:
		switch (day)
		{
		case 10://Columbus Day
			return 1;
		default:
			return 0;
		}
	case 11:
		switch (day)
		{
		case 11://Veterans Day
		case 24://Thanksgiving Day
			return 1;
		default:
			return 0;
		}
	case 12:
		switch (day)
		{
		case 31://New Year's Day
		case 26://Christmas Day
			return 1;
		default:
			return 0;
		}
	default:
		return 0;
	}
	return 0;
}

int AroyaDate::getYear() {
	return year;
}