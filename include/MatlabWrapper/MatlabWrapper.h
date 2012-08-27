/*
MatlabWrapper

Andrej Karpathy
August 27, 2012
BSD Licence
*/

#ifndef MATLAB_WRAPPER_H__
#define MATLAB_WRAPPER_H__

#include <iostream>
#include <string>
#include <map>
#include <Eigen/Eigen>
#include "engine.h"

#define MATLAB_BUF_SIZE 1024

using namespace std;

/*
Lets you use Matlab from C++ effortlessly
Example usage:

Eigen::MatrixXf X = Eigen::MatrixXf::Random(3,4);
cout << X << endl;

// create wrapper and visualize tiny 3x4 random matrix
MatlabWrapper mw;
mw.sendMat("X", X);
mw.exec("imshow(X, [])", true); //true indicates that we want matlab to pause for keypress

// enter interactive session! Enter 'qq' to return 
mw.interact();

// receive matrix X back and print it to make sure it's the same
Eigen::MatrixXf mat;
mw.receiveMat("X", mat);
cout << mat << endl;
*/

class MatlabWrapper {
  
  public:
    MatlabWrapper();
    ~MatlabWrapper();
    
    // start the engine. Returns false on error.
    bool init(); 
    
    // send a matrix to matlab engine
    void sendMat(const string name, const Eigen::MatrixXf &mat);  
    
    // receive a matrix back from matlab engine
    void receiveMat(const string name, Eigen::MatrixXf &mat) const;  
    
    // execute a command in matlab
    void exec(const string cmd, const bool waitForKey = false) const;
    
    // starts interactive mode where you can type anything and get response back from MATLAB  
    // enter qq to exit interactive mode
    void interact(); 
    
    bool verbose_;
    
  protected:
    
    Engine *ep_;
    bool initted_;
    char buffer_[MATLAB_BUF_SIZE]; // output buffer for matlab to talk back to us
};

#endif
