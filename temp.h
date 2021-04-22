#ifndef TEMP_H
#define TEMP_H

#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ntb_util.h"
using namespace std;

#include "matrix.h"
extern slain::Matrix <double> W1,W2,X,H,Y,Y_HAT,ERR,dJdB2,dJdW2,dJdB1,dJdW1;
void init(size_t inputNeuron, size_t hiddenNeuron, size_t outputNeuron, double rate);
void learn(const std::vector<double> &expectedOutput);
double stepFunction(double x);
slain::Matrix<double> &computeOutput(const std::vector<double> input);
void loadTraining(const char *filename, std::vector<std::vector<double> > &input, std::vector<std::vector<double> > &output);
#else
#include "matrix_bad.h"
void loadTraining(const char *filename, vector<vector<double> > &input, vector<vector<double> > &output);
void init(int inputNeuron, int hiddenNeuron, int outputNeuron, double rate);
Matrix computeOutput(vector<double> input);
void learn(vector<double> expectedOutput);
double stepFunction(double x);

#endif // TEMP_H
