/*
MatlabWrapper

Andrej Karpathy
August 27, 2012
BSD Licence
*/

#include <MatlabWrapper/MatlabWrapper.h>
#include <stdio.h>

using namespace std;

MatlabWrapper::MatlabWrapper() {
  initted_ = false; 
  verbose_ = true;
}

MatlabWrapper::~MatlabWrapper() {
  engClose(ep_); // close down the engine
}

bool MatlabWrapper::init() {
  
  if (!(ep_ = engOpen(""))) {
		fprintf(stderr, "\nCan't start MATLAB engine.\n");
		return false;
	}
	
	// connect Matlab engine to a char buffer for output
	buffer_[MATLAB_BUF_SIZE - 1] = '\0';
	engOutputBuffer(ep_, buffer_, MATLAB_BUF_SIZE - 1);
	
	initted_ = true;
	return true;
}

void MatlabWrapper::sendMat(const string name, const Eigen::MatrixXf &mat) {
  
  int N = mat.rows();
  int D = mat.cols();
  if(N <= 0 || D <= 0) {
    fprintf(stderr, "\nWow, weird matrix is of size %dx%d. Ignoring sendMat request.\n", N, D);
    return;
  }
  if(!initted_) { init(); }
  
  mxArray *X = mxCreateNumericMatrix(N, D, mxSINGLE_CLASS, mxREAL);
  memcpy((float *)mxGetData(X), mat.data(), sizeof(float)*N*D);
	engPutVariable(ep_, name.c_str(), X);
	mxDestroyArray(X); // the engine takes ownership of a copy
}

void MatlabWrapper::receiveMat(const string name, Eigen::MatrixXf &mat) const {
  
  mxArray *result = NULL;
  if ((result = engGetVariable(ep_, name.c_str())) == NULL) {
    printf("Variable %s is not defined! Ignoring request.\n", name.c_str());
    return;
  }
  
  int number_of_dims; 
  number_of_dims = mxGetNumberOfDimensions(result);
  if(number_of_dims != 2) {
    printf("Can only receive 2D matrix back! Exitting.\n");
    mxDestroyArray(result);
    return;
  }
  
  const int *dim_array = mxGetDimensions(result);
  int N= dim_array[0];
  int D= dim_array[1];
  
  mxClassID classType= mxGetClassID(result);
  if(classType == mxSINGLE_CLASS) { 
    mat.resize(N, D);
    memcpy(mat.data(), (float *)mxGetData(result), sizeof(float)*N*D);
    
  } else if(classType == mxDOUBLE_CLASS) {
    if(verbose_) printf("Oi! Variable %s is double, converting to single!\n", name.c_str());
    Eigen::MatrixXd proxy(N, D);
    memcpy(proxy.data(), (double *)mxGetData(result), sizeof(double)*N*D);
    mat = proxy.cast<float>();
    
  } else {
    printf("Error! Variable %s is not single/double. Ignoring.\n", name.c_str());
  }
  
  mxDestroyArray(result); // we own this newly allocated mxArray. Destroy it
}

void MatlabWrapper::exec(const string cmd, const bool waitForKey) const {

  engEvalString(ep_, cmd.c_str());
  if(waitForKey) {
    printf("Hit return to continue\n"); 
    fgetc(stdin);
  }
}

void MatlabWrapper::interact() {
  
  printf("Started interactive MATLAB session! Enter 'qq' to exit.\n");
  char str[MATLAB_BUF_SIZE + 1];
  while(true) {
    
    printf(">> ");
    fgets(str, MATLAB_BUF_SIZE, stdin);
    if(strcmp(str, "qq\n") == 0) break;
    
    engEvalString(ep_, str);
    printf("%s\n", buffer_);
  }
}



