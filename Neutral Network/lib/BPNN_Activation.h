#ifndef BPNN_ACTIVATION
#define BPNN_ACTIVATION
#include<cmath>
using namespace std;
double sigmoid(const double&t);
double sigmoidD(const double&t);
double softmax(const double&wx);
double softmaxD(const double&wx);
double ReLinear(const double&wx);
double ReLinearD(const double&wx);
#endif // !BPNN_ACTIVATION