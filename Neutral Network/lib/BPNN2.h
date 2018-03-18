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

	//set layer array size(not contain input)
	void setLayerNodes(int* Nodes);

	//run with group data
	void runGroup(double**groupData, double**flag, const int&groups,
		double(*)(const double&) = defaultActive, double(*)(const double&) = defaultActiveD,
		int writeFileTest = 1);

	//设置激活函数
	void setActiveFunction(double(*active)(const double&), double(*activeD)(const double&));
	//输入训练集
	void setTrainingSet(double**set, double**flag, const int&count);
	//输入验证集
	void setValidationSet(double**set, double**flag, const int&count);
	//输入测试集
	void setTestSet(double**set, const int&count);
	//训练,无验证集自动使用训练集
	void train(const int&times);

	//正则化测试
	void printW();
	//正则化设置
	void setRegularization(const Regularization&);
	Regularization getRegularization();
	//提前终止
	void setEarlyStoppiing(const bool&,const int&);
private:
	//存储每一层的W矩阵
	vector<MatrixXd> W;
	vector<MatrixXd> W_best;
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

	//copy input data to first index of private data
	void setInputData(double* NodeDataArray);
	//Get Ans
	void updateLayers();
	//Update W and bias
	void updateParameter();
	//input expect value(output node)
	void setExpectData(double*Data);

	//激活函数指针
	double (*activeFunction)(const double&);
	double(*activeFunctionD)(const double&);

	//训练集
	double**trainingSet;
	double**trainingFlag;
	int trainingCount;
	//验证集
	bool validation;
	double**validationSet;
	double**validationFlag;
	int validationCount;
	//测试集
	bool test;
	double**testSet;
	int testCount;

	//正则化设置
	Regularization regularization;
	double regularizationFactor;//由W的size决定
	//正则化-提前终止
	bool earlyStopping;

	int patient_earlyStopping;
	int worse_earlyStopping;
	void setPatient_EarlyStopping(const int&);


	double loss;
	double loss_before;
	double dynamic();
	void learn(const int&groups);
	void clearFix();

	double computeRegularization();
	MatrixXd derivateRegularization(const MatrixXd&);
};
#endif