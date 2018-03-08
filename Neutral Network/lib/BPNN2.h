#ifndef AROYA_BPNN
#define AROYA_BPNN

#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<ctime>
using namespace std;

#include<Eigen/Dense>
using namespace Eigen;
//#define ShowAllNodes
#define LastLayerLinear
#define MinAccurary 1
double defaultActive(const double&t);
double defaultActiveD(const double&t);

enum Regularization { none, L1, L2 };

class BPNN {
public:
	//contain input,hidden and output
	BPNN(const int&CountOfLayers);
	//set input array size
	void setInputNodes(const int&Nodes);
	//copy input data to first index of private data
	void setInputData(double* NodeDataArray, double(*)(const double&));
	//set layer array size(not contain input)
	void setLayerNodes(int* Nodes);
	//Get Ans
	void updateLayers(double(*)(const double&) = defaultActive);
	//Update W and bias
	void updateParameter(double(*)(const double&) = defaultActiveD);
	//input expect value(output node)
	void setExpectData(double*Data, double(*)(const double&));
	//run with group data
	void runGroup(double**groupData, double**flag, const int&groups,
		double(*)(const double&) = defaultActive, double(*)(const double&) = defaultActiveD,
		int writeFileTest = 1);
	//正则化测试
	void printW();
	//正则化设置
	void setRegularization(const Regularization&);
	Regularization getRegularization();
private:
	//存储每一层的W矩阵
	vector<MatrixXd> W;
	//存储每一层的计算结果
	vector<VectorXd> X_Origin;
	//存储每一层的输出结果
	vector<VectorXd> X;
	//存储每一层的Bias
	vector<VectorXd> Bias;
	//每层的偏差
	vector<VectorXd> fixY;
	//每层的Bias偏移
	vector<VectorXd> fixBias;
	//每层的Bias偏移（单个样例）
	vector<VectorXd> diffBias;
	//每层的激活值求导
	vector<VectorXd> Diff;
	//每层的W偏移值
	vector<MatrixXd> fixW;

	MatrixXd emptyMatrix;
	VectorXd emptyVector;
	int layers;
	int *layerNodes;

	//正则化设置
	Regularization regularization;

	double loss;
	double dynamic();
	void learn(const int&groups);
	void clearFix();
	double computeRegularization();
};
#endif