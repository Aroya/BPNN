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
	bpnn.setRegularization(none);
	//����earlystopping
	bpnn.setEarlyStoppiing(true,100);
	//��������ڵ���
	bpnn.setInputNodes(trainSet.getColumns());
	//���������֮���ÿ��ڵ���
	bpnn.setLayerNodes(l);
	//ʹ��helper�ӿڻ�ȡbpnn��Ҫ�����ݽṹ
	bpnn.setTrainingSet(trainSet.getDataPointer(), trainFlag.getDataPointer(), trainSet.getRows());
	bpnn.setActiveFunction(sigmoid, sigmoidD);
	bpnn.train(100000);

	system("pause");
}