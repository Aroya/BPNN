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

	//���ü����
	void setActiveFunction(double(*active)(const double&), double(*activeD)(const double&));
	//����ѵ����
	void setTrainingSet(double**set, double**flag, const int&count);
	//������֤��
	void setValidationSet(double**set, double**flag, const int&count);
	//������Լ�
	void setTestSet(double**set, const int&count);
	//ѵ��,����֤���Զ�ʹ��ѵ����
	void train(const int&times);

	//���򻯲���
	void printW();
	//��������
	void setRegularization(const Regularization&);
	Regularization getRegularization();
	//��ǰ��ֹ
	void setEarlyStoppiing(const bool&,const int&);
private:
	//�洢ÿһ���W����
	vector<MatrixXd> W;
	vector<MatrixXd> W_best;
	//�洢ÿһ��ļ�����
	vector<VectorXd> X_Origin;
	//�洢ÿһ���������
	vector<VectorXd> X;
	//�洢ÿһ���Bias
	vector<VectorXd> Bias;
	//ÿ���ƫ��
	vector<VectorXd> fixY;
	//ÿ���Biasƫ��
	vector<VectorXd> fixBias;
	//ÿ���Biasƫ�ƣ�����������
	vector<VectorXd> diffBias;
	//ÿ��ļ���ֵ��
	vector<VectorXd> Diff;
	//ÿ���Wƫ��ֵ
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

	//�����ָ��
	double (*activeFunction)(const double&);
	double(*activeFunctionD)(const double&);

	//ѵ����
	double**trainingSet;
	double**trainingFlag;
	int trainingCount;
	//��֤��
	bool validation;
	double**validationSet;
	double**validationFlag;
	int validationCount;
	//���Լ�
	bool test;
	double**testSet;
	int testCount;

	//��������
	Regularization regularization;
	double regularizationFactor;//��W��size����
	//����-��ǰ��ֹ
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