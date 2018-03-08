#ifndef AROYA_BPNN
#define AROYA_BPNN
#include<iostream>
#include<fstream>
#include<cmath>
#include<ctime>
#include<sstream>
using namespace std;

#define Bpnn_Init 0

//展示实验结果用
//#define ShowAllNode

double defaultActive(const double&t);
double defaultActiveD(const double&t);
//first use BPNN to init
//second use setInputData to init inputArray
//then use setLayerNodes to init LayerArray
//last use setAnsNodes to init AnsArray
class BPNN {
private:
	//first index point to  
	//hidden_layout[layer0....layerN]
	//second index point to the Node
	//third index point to the W_i
	double ***layerW;
	//record nodes of every layer
	int *layerNodes;
	//record count of layer
	int layers;
	//for caculate Y=WX
	//also the X_i for next layer
	double **layerData;
	//after activation function
	double **activation;
	//bias
	double **bias;
	//write 0 to layerData
	void clearData();
	//record expected value
	double **expected;
	//expect fixed value of W
	double ***fixW;
	//expect fixed value of bias
	double **fixBias;
	//after update learn from the data
	void learn(const int&groups);
	//double forward;

	//统计
	double loss;
	double dynamicRate();
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
	void setExpectData(double*Data,double(*)(const double&));
	//run with group data
	void runGroup(double**groupData, double**flag, const int&groups,
		double(*)(const double&) = defaultActive, double(*)(const double&) = defaultActiveD,
		int writeFileTest=1);
};



#endif