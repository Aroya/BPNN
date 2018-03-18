#include<iostream>
using namespace std;
#include"lib\BPNN2.h"//BPNN中使用到Eigen库
#include"lib\Divider.h"//划分文件
#include"lib\Reader.h"//读取文件
#include"lib\ReaderHelper.h"//处理Reader中的生数据
#include"lib\BPNN_Activation.h"//激活函数
//#define Test//每次迭代都测试
			//当大于BPNN2.h中的MinAccurary阈值时
			//不输出
//#define validate
int main() {
	AroyaReader train;
	train.read("examples/xor.csv");

	//使用helper选取数据
	AroyaReaderHelper trainSet;
	trainSet.insert(train, "x");
	trainSet.insert(train, "y");
	AroyaReaderHelper trainFlag;
	trainFlag.insert(train,"cnt");

	//设置隐藏层+输出层数量
	int l[] = { 10,1 };
	int layers = sizeof(l) / sizeof(int) + 1;
	//初始化BPNN总层数
	BPNN bpnn(layers);
	//设置正则化
	bpnn.setRegularization(none);
	//设置earlystopping
	bpnn.setEarlyStoppiing(true,100);
	//设置输入节点数
	bpnn.setInputNodes(trainSet.getColumns());
	//设置输入层之后的每层节点数
	bpnn.setLayerNodes(l);
	//使用helper接口获取bpnn需要的数据结构
	bpnn.setTrainingSet(trainSet.getDataPointer(), trainFlag.getDataPointer(), trainSet.getRows());
	bpnn.setActiveFunction(sigmoid, sigmoidD);
	bpnn.train(100000);

	system("pause");
}