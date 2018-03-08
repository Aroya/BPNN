#include<iostream>
using namespace std;
#include"lib\BPNN2.h"//BPNN��ʹ�õ�Eigen��
#include"lib\Divider.h"//�����ļ�
#include"lib\Reader.h"//��ȡ�ļ�
#include"lib\ReaderHelper.h"//����Reader�е�������
#include"lib\BPNN_Activation.h"//�����
//#define Test//ÿ�ε���������
			//������BPNN2.h�е�MinAccurary��ֵʱ
			//�����
//#define validate
int main() {
	AroyaReader train;
	train.read("examples/xor.csv");

	//ʹ��helperѡȡ����
	AroyaReaderHelper trainSet;
	trainSet.insert(train, "x");
	trainSet.insert(train, "y");
	AroyaReaderHelper trainFlag;
	trainFlag.insert(train,"cnt");

	//�������ز�+���������
	int l[] = { 10,1 };
	int layers = sizeof(l) / sizeof(int) + 1;
	//��ʼ��BPNN�ܲ���
	BPNN bpnn(layers);
	//��������
	bpnn.setRegularization(L1);
	//��������ڵ���
	bpnn.setInputNodes(trainSet.getColumns());
	//���������֮���ÿ��ڵ���
	bpnn.setLayerNodes(l);
	//ʹ��helper�ӿڻ�ȡbpnn��Ҫ�����ݽṹ
	double **db = trainSet.getDataPointer();
	double **fdb = trainFlag.getDataPointer();
	int dr = trainSet.getRows();


	double **tdb = fdb;
	int tdr = dr;
	
	/*runGroup����
	void runGroup(double**���ݾ���(���ݰ�), double**���������(CNT����), const int&��������,
	double(*�����)(const double&) = ����, double(*�������)(const double&) = ������,
	int writeFileTest = ��д�ļ�ֻѵ��);
	*/
	for (int i = 0; i < 999999; i++) {
		bpnn.runGroup(db, fdb, dr, sigmoid, sigmoidD);
		//if (i % 100000 == 0) {
		//	bpnn.printW();
		//	system("pause");
		//}
	}
#ifndef Test//���û��ifndef�����ڵ�����ɺ�������ļ�
	bpnn.printW();
	bpnn.runGroup(tdb, nullptr, tdr, sigmoid, sigmoidD, 0);

#endif // !Test Output after all iteration
	system("pause");
}