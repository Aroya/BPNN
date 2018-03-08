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
	bpnn.setRegularization(L1);
	//设置输入节点数
	bpnn.setInputNodes(trainSet.getColumns());
	//设置输入层之后的每层节点数
	bpnn.setLayerNodes(l);
	//使用helper接口获取bpnn需要的数据结构
	double **db = trainSet.getDataPointer();
	double **fdb = trainFlag.getDataPointer();
	int dr = trainSet.getRows();


	double **tdb = fdb;
	int tdr = dr;
	
	/*runGroup参数
	void runGroup(double**数据矩阵(数据包), double**期望的输出(CNT数据), const int&数据组数,
	double(*激活函数)(const double&) = 线性, double(*激活函数求导)(const double&) = 线性求导,
	int writeFileTest = 不写文件只训练);
	*/
	for (int i = 0; i < 999999; i++) {
		bpnn.runGroup(db, fdb, dr, sigmoid, sigmoidD);
		//if (i % 100000 == 0) {
		//	bpnn.printW();
		//	system("pause");
		//}
	}
#ifndef Test//如果没有ifndef将会在迭代完成后再输出文件
	bpnn.printW();
	bpnn.runGroup(tdb, nullptr, tdr, sigmoid, sigmoidD, 0);

#endif // !Test Output after all iteration
	system("pause");
}