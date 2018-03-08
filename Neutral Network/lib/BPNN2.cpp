#include"BPNN2.h"

static short printCount = 1;
static short printMax = 10000;
static int allCnt = 1;
static stringstream sst;
static string outputFileName;
static string sstOut;
static string sstOut2;
double defaultActive(const double&t) { return t; }
double defaultActiveD(const double&t) { return 1.0; }
template<class T>
T sign(const T&);
double BPNN::dynamic() {
	//单层
	//return 0.001;
	//5隐藏层
	//return 0.000005;
	//2隐藏层
	//if(loss>10000)return 0.0005;
	//else return 0.0001;
	//3隐藏层
	//return 0.00001;
	//4隐藏层
	//最终结果使用静态
	return 0.1;
}

BPNN::BPNN(const int&CountOfLayers) {
	layers = CountOfLayers;
	layerNodes = new int[CountOfLayers];
	regularization = none;
	regularizationFactor = 0.01;
}

void BPNN::setInputNodes(const int&Nodes) {
	layerNodes[0] = Nodes;
	W.push_back(emptyMatrix);
	X_Origin.push_back(emptyVector);
	X.push_back(emptyVector);
	X[0].resize(Nodes);
	X[0].setZero();
	Bias.push_back(emptyVector);

	fixY.push_back(emptyVector);
	fixBias.push_back(emptyVector);
	diffBias.push_back(emptyVector);
	Diff.push_back(emptyVector);
	fixW.push_back(emptyMatrix);
}
//copy input data to first index of private data
void BPNN::setInputData(double* NodeDataArray, double(*active)(const double&)) {
	int i, j = layerNodes[0];
	for (i = 0; i < j; i++) {
		X[0](i) = NodeDataArray[i];
	}
}
//set layer array size(not contain input)
void BPNN::setLayerNodes(int* Nodes) {
	int i, j, k, sum;
	sum = 0;
	for (i = 1, j = 0; i < layers; i++, j++) {
		//sum += Nodes[j];
		k = Nodes[j];
		layerNodes[i] = k;

		X_Origin.push_back(emptyVector);
		X_Origin[i].resize(k);
		X_Origin[i].setZero();

		X.push_back(emptyVector);
		X[i].resize(k);
		X[i].setZero();
		W.push_back(emptyMatrix);
		//一行为一个节点的W
		//列数为上一层的节点数
		W[i].resize(k, layerNodes[j]);
		//W[i].setZero();
		W[i].setRandom();

		//Bias
		Bias.push_back(emptyVector);
		Bias[i].resize(k);
		//Bias[i].setZero();
		Bias[i].setRandom();

		fixY.push_back(emptyVector);
		fixY[i].resize(k);
		fixY[i].setZero();
		fixBias.push_back(emptyVector);
		fixBias[i].resize(k);
		fixBias[i].setZero();
		diffBias.push_back(emptyVector);
		diffBias[i].resize(k);
		diffBias[i].setZero();
		Diff.push_back(emptyVector);
		Diff[i].resize(k);
		Diff[i].setZero();
		fixW.push_back(emptyMatrix);
		fixW[i].resize(k, layerNodes[j]);
		fixW[i].setZero();
	}
	//regularizationFactor = 1 / double(sum);
}
//Get Ans
void BPNN::updateLayers(double(*active)(const double&)) {
	int k, l;
	for (int i = 1, j = 0; i < layers; i++, j++) {
//#ifdef ShowAllNodes
//		cout << "X[" << j << "]:\n";
//		cout << X[j] << endl;
//		cout << "W[" << i << "]:\n";
//		cout << W[i] << endl;
//#endif // ShowAllNodes


		//h=WX
		X_Origin[i] = W[i] * X[j] + Bias[i];

		//激活
		l = layerNodes[i];
#ifndef LastLayerLinear
		for (k = 0; k < l; k++) {
			X[i](k) = active(X_Origin[i](k));
		}
#endif
#ifdef LastLayerLinear
		if (i == layers - 1) {
			X[i] = X_Origin[i];
		}
		else for (k = 0; k < l; k++) {
			X[i](k) = active(X_Origin[i](k));
		}
#endif

	}
}
//Update W and bias
void BPNN::updateParameter(double(*activeD)(const double&)) {
	int i, j, k, l;
	for (i = layers - 1, j = layers - 2; i > 0; i--, j--) {
		l = layerNodes[i];
		for (k = 0; k < l; k++) {
#ifdef LastLayerLinear
			if (i == layers - 1) {
				Diff[i](k) = 1;
			}
			else {
				Diff[i](k) = activeD(X_Origin[i](k));
				
			}
#endif
#ifndef LastLayerLinear
			Diff[i](k) = activeD(X_Origin[i](k));
#endif
			diffBias[i](k) = fixY[i](k)*Diff[i](k);
		}
		//cout << "layers:\t\t" << i << endl;
		//fixBias[i] += diffBias[i];
		//cout << "DiffBias\n";
		//cout << diffBias[i] << endl;
		//cout << "X.trans\n";
		//cout << X[j].transpose() << endl;
		//system("pause");

		//累计当前批梯度的偏移
		fixBias[i] += diffBias[i];
		//w偏移=Bias偏移*对应Xi
		fixW[i] += diffBias[i] * X[j].transpose();
		//前向传递
		fixY[j] = W[i].transpose()*diffBias[i];
	}
}
//input expect value(output node)
void BPNN::setExpectData(double*Data, double(*active)(const double&)) {
	VectorXd temp;
	int last = layers - 1;
	int size = layerNodes[last];
	temp.resize(size);
	for (int i = 0; i < size; i++) {
		fixY[last](i) = Data[i] - X[last](i);
	}
}
void BPNN::learn(const int&groups) {
	int i;
	for (i = layers - 1; i > 0; i--) {
		Bias[i] = Bias[i] + dynamic()*(fixBias[i] - derivateRegularization(Bias[i]))/ groups;
		W[i] = W[i] + dynamic()*(fixW[i] - derivateRegularization(W[i])) / groups;
	}
}
void BPNN::clearFix() {
	for (int i = 0; i < layers; i++) {
		fixW[i].setZero();
		fixBias[i].setZero();
	}
}
//run with group data
void BPNN::runGroup(double**group, double**flag, const int&groups,
	double(*active)(const double&), double(*activeD)(const double&),
	int writeFile) {
	if (writeFile) {
		//清空偏移量
		clearFix();
		loss = 0;
		for (int i = 0; i < groups; i++) {
			setInputData(group[i], active);
			//后向传递
			updateLayers(active);
			setExpectData(flag[i], active);

			//前向传递
			if (writeFile > 0)updateParameter(activeD);
			int lastLayer = layers - 1;
			for (int j = 0; j < layerNodes[lastLayer]; j++) {
				loss += pow(fixY[lastLayer](j), 2);
			}
		}
		if (writeFile > 0) {//>0训练
			if (printCount == printMax) {
				//正则化
				cout << allCnt << "\t\tLoss:\t\t" << (loss /= double(groups)) + computeRegularization() << endl;
				printCount = 1;
			}
			else printCount++;
			
			//学习前向传递的内容
			learn(groups);
		}
		else {//<0验证
			cout << allCnt << "\t\tValiLoss:\t" << (loss /= double(groups)) + computeRegularization() << endl;
		}
	}
	else {//==0测试
		cout << allCnt << "\t\t";
		sst.str("");
		sst.clear();
		sst << loss;
		sst >> sstOut2;
		if (loss>MinAccurary) {
			cout << "Test error:loss too high:\t" << loss << endl;
			return;
		}
		if (sstOut2 == "nan" || sstOut2 == "-nan(ind)") {
			cout << "Error:\t" << loss << endl;
			system("pause");
			exit(0);
		}

		outputFileName = "out/result_";
		sst.str("");
		sst.clear();
		sst << clock();
		sst >> sstOut;

		outputFileName = outputFileName + sstOut + "_" + sstOut2 + ".csv";

		ofstream fout;
		fout.open(outputFileName);

		for (int i = 0; i < groups; i++) {
			setInputData(group[i], active);
			updateLayers(active);
			for (int i = 0; i < layerNodes[layers - 1]; i++) {
				//fout << max(int(X[layers - 1](i)),0) << endl;
				fout << X[layers - 1](i) << endl;
			}
		}
		fout.close();
		cout << "Output\n";
	}
#ifdef ShowAllNodes
	for (int i = 0; i < layers; i++) {
		cout << "*************************************\n";
		cout << "Layer\t\t" << i << endl;
		cout << "X_Origin\t" << endl;
		cout << X_Origin[i] << endl;
		cout << "Diff\t\t" << endl;
		cout << Diff[i] << endl;
		cout << "X\t\t" << endl;
		cout << X[i] << endl;
		cout << "fixY\t\t" << endl;
		cout << fixY[i] << endl;
		cout << "fixBias\t\t" << endl;
		cout << fixBias[i] << endl;
		cout << "Bias\t\t" << endl;
		cout << Bias[i] << endl;
		cout << "fixW\t\t" << endl;
		cout << fixW[i] << endl;
		cout << "W\t" << endl;
		cout << W[i] << endl;
		cout << "*************************************\n";
	}
	system("pause");
#endif // ShowAllNodes
	allCnt++;
}

void BPNN::printW() {
	for (auto i : W) {
		cout << i << endl;
	}
}
void BPNN::setRegularization(const Regularization&t) {
	regularization = t;
}
Regularization BPNN::getRegularization() {
	return regularization;
}
double BPNN::computeRegularization() {
	double sum = 0;
	switch (regularization) {
	case L1:
		for (auto i : W) {
			sum += i.lpNorm<1>();
		}
		return sum*regularizationFactor;
	case L2:
		for (auto i : W) {
			//sum += i.squaredNorm();
			sum += i.lpNorm<2>();
		}
		return sum / 2 * regularizationFactor;
	}
	return 0;
}
MatrixXd BPNN::derivateRegularization(const MatrixXd&t) {
	//cout << "t" << endl;
	//cout << t << endl;
	MatrixXd p = t;
	//cout << "p" << endl;
	//cout << p << endl;
	//cout << p.rows() << "\t" << p.cols() << endl;
	switch(regularization) {
	case L1:
		for (int i = 0; i < p.rows();i++) {
			for (int j = 0; j < p.cols(); j++) {
				p(i, j) = sign(p(i, j));
			}
		}
		p *= regularizationFactor / 2;
		break;
	case L2:
		p *= regularizationFactor;
		break;
	default:
		p.setZero();
	}
	//cout << "p" << endl;
	//cout << p << endl;
	//cout << "\n\n";
	return p;
}

template<class T>
T sign(const T&t) {
	if (t < 0)return -1;
	return 1;
}