#include"Reader.h"

//仅依据不包含数字、小数点来判断
bool static NotPureNumber(const string&);
bool static PureNumber(const char&);

AroyaReader::AroyaReader() {
	//nothing to do yet
}

void AroyaReader::read(const char*fileName) {
	ifstream fin;
	fin.open(fileName);
	if (!fin.is_open()) {
		cout << "AroyaReader:read() cannot open file!\n";
		system("pause");
	}
	char reading;
	string buffer;
	bool didntInit = true;
	vector<string>emptyVector;
	int nowPoint = 0;
	while (fin.good() && (reading = fin.get()) != -1) {
		//数据结束 处理数据
		if (reading == ',' || reading == '\n') {
			if (didntInit) {					//init this row
				data.push_back(emptyVector);
				didntInit = false;
			}
			data[nowPoint].push_back(buffer);	//put data to this row

			buffer.clear();						//clear buffer
			if (reading == '\n') {				//next row
				nowPoint++;
				didntInit = true;
			}
		}
		//无分隔符 记录数据
		else {
			buffer.push_back(reading);
		}
	}
	fin.close();
}

string AroyaReader::getStringData(const int&rows, const int&columns) {
	return data[rows][columns];
}

double AroyaReader::getDoubleData(const int&rows, const int&columns) {
	internalSst.str("");
	internalSst.clear();
	internalSst << data[rows][columns];
	double temp;
	internalSst >> temp;
	return temp;
}

int AroyaReader::findTable(const char*tableName) {
	int tables = data[0].size();
	for (int i = 0; i < tables; i++) {
		if (data[0][i] == tableName)return i;
	}
	cout << "Cannot find " << tableName << endl;
	return -1;
}
int AroyaReader::getRows() { return data.size(); }
int AroyaReader::getColumns() { return data[0].size(); }

void AroyaReader::discrete(const int&column) {
	int i, j, nums = 0;
	int rows = getRows();
	bool exists = false;
	//统计类型数量
	vector<string>tables;
	for (i = 1; i < rows; i++) {
		//是否在tables中已存在
		exists = false;
		for (j = 0; j < nums; j++) {
			if (data[i][column] == tables[j]) {
				exists = true;
				break;
			}
		}
		//不存在则加入
		if (!exists) {
			tables.push_back(data[i][column]);
			nums++;
		}
	}

	//添加数据
	//首行列名称 原table名+'/'+值
	for (i = 0; i < nums; i++)data[0].push_back(data[0][column] + "/" + tables[i]);
	//每行存在则0，不存在为1
	for (i = 1; i < rows; i++) {
		for (j = 0; j < nums; j++) {
			if (data[i][column] == tables[j])data[i].push_back("1");
			else data[i].push_back("0");
		}
	}
	//清除原数据列
	deleteColumn(column);
}

void AroyaReader::discrete() {
	bool toDiscrete;
	int columns = getColumns();
	for (int j = 1; j < columns; j++) {
		toDiscrete = false;
		//日期格式转日期
		//未完成
		//非连续数字转离散值
		if (NotPureNumber(data[1][j])) {
			discrete(j);
			j--;//j++ after this
		}
	}
}

bool static NotPureNumber(const string&str) {
	int i, l = str.length();
	char left = '0' - 1, right = '9' + 1;
	bool point = true;
	for (i = 0; i < l; i++) {
		if (point&& str[i] > left &&str[i] < right) {
			if (str[i] == ',')point = false;
		}
		else return true;
	}
	return false;
}
bool static PureNumber(const char&t) {
	return t >= '0'&&t <= '9';
}

void AroyaReader::setTableName(const char*origin, const char*new_) {
	data[0][findTable(origin)] = new_;
}
void AroyaReader::deleteColumn(const int&col) {
	int rows = getRows();
	for (int i = 0; i < rows; i++) {
		data[i].erase(data[i].begin() + col);
	}
}
void AroyaReader::deleteRow(const int&row) {
	data.erase(data.begin() + row);
}

void AroyaReader::dispartTime(const char*tableName, const bool&flag) {
	int column = findTable(tableName);
	AroyaDate date;
	AroyaDate Christmas;

	//table
	data[0].push_back("month");
	data[0].push_back("weekday");
	//holiday
	data[0].push_back("holiday");
	data[0].push_back("workingday");
	data[0].push_back("year");
	data[0].push_back("nearChristmas");
	//temp str
	string str;
	int j;
	for (int i = 1; i < getRows(); i++) {
		date.input(data[i][column]);

		//clear stringstream
		internalSst.str("");
		internalSst.clear();
		//get Month
		internalSst << date.getMonth();
		internalSst >> str;
		data[i].push_back(str);
		//clear stringstream
		internalSst.str("");
		internalSst.clear();
		//get Weekday
		internalSst << date.getWeekday();
		internalSst >> str;
		data[i].push_back(str);


		//get nearChristmas
		Christmas.input(date.getYear(), 12, 26);
		j = date.getDateInstant() - Christmas.getDateInstant();

		//get holiday
		//clear stringstream
		internalSst.str("");
		internalSst.clear();

		internalSst << date.getHoliday();
		internalSst >> str;
		data[i].push_back(str);

		//workingday
		internalSst.str("");
		internalSst.clear();
		internalSst << date.getWorkingDay();
		internalSst >> str;
		data[i].push_back(str);

		//get year
		//clear stringstream
		internalSst.str("");
		internalSst.clear();
		internalSst << date.getYear();
		internalSst >> str;
		data[i].push_back(str);


		//near Christmas
		//23~26~29 day
		if (j > 4 || j < -4) {
			data[i].push_back("0");
		}
		else {
			data[i].push_back("1");
		}
	}
	//删除原数据列
	if (flag)deleteColumn(column);
}

void AroyaReader::deleteInstantZero() {
	int i, j, k;
	j = findTable("instant");
	k = getRows();
	for (i = 0; i < k; i++) {
		if (data[i][j] == "0") {
			deleteRow(i);
			i--;
			k--;
		}
	}
}
void AroyaReader::PercisionDown(const int&column) {
	if (column < 0) return;
	int i, k = getRows();
	double doubleTemp, doubleTemp2;
	for (i = 1; i < k; i++) {
		//clear
		internalSst.clear();
		internalSst.str("");
		//read data
		internalSst << data[i][column];
		internalSst >> doubleTemp;
		//test
		doubleTemp2 = doubleTemp + 0.50;
		//clear
		internalSst.str("");
		internalSst.clear();
		//judge
		if (int(doubleTemp2) > int(doubleTemp)) {
			internalSst << int(doubleTemp2);
		}
		else {
			internalSst << int(doubleTemp);
		}
		internalSst >> data[i][column];
	}
}
void AroyaReader::deleteRows(const char*TableName, const char*DataType) {
	int column = findTable(TableName);
	int i, j = getRows();
	for (i = 1; i < j; i++) {
		if (data[i][column] == DataType) {
			deleteRow(i);
			i--;
			j = getRows();
		}
	}
}