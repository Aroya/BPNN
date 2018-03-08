#ifndef	Aroya_READER
#define Aroya_READER

#include<string>
#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;
#include"Date.h"

class AroyaReader {
private:
	vector<vector<string>>data;		//ȫ����string�ݴ�
	stringstream internalSst;		//ת��ʹ��
public:
	AroyaReader();
	void read(const char*fileName);
	string getStringData(const int&rows, const int&columns);
	double getDoubleData(const int&rows, const int&columns);
	int findTable(const char*tableName);
	int getRows();
	int getColumns();
	void setTableName(const char*origin, const char*new_);
	void deleteColumn(const int&col);

	//��ֵ����:ɾ����ֵ����ɾ���к���
	void deleteRow(const int&row);
	//����yyyy:mm:dd��table mm,weekday
	//Ĭ��ɾ��ԭ������
	void dispartTime(const char*tableName, const bool&deleteOrigin = true);
	//���������֡���ɢtable
	void discrete(const int&column);
	//�Զ����������У��Զ�ת���з�����������Ϊ��ɢ����
	void discrete();
	//����Ƿ�����
	void deleteInstantZero();
	//����������ɢ
	void PercisionDown(const int&column);
	//ɾ���ض�����
	void deleteRows(const char*TableName, const char*DataType);
};

#endif