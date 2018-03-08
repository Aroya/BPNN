#include"BPNN_Activation.h"


double sigmoid(const double&t) { return 1.0 / (1.0 + exp(-t)); }
double sigmoidD(const double&t) {
	return sigmoid(t)*(1 - sigmoid(t));
}
double softmax(const double&wx) {
	return log(1 + exp(wx));
}
double softmaxD(const double&wx) {
	return 1.0 / (1 + exp(-wx));
}

double ReLinear(const double&wx) {
	return wx > 0 ? wx : 0;
}
double ReLinearD(const double&wx) {
	return wx > 0 ? 1 : 0;
}
