#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <Eigen/Eigen>
#include <MatlabWrapper/MatlabWrapper.h>

using namespace std;
int main()
{
	
	Eigen::MatrixXf X = Eigen::MatrixXf::Random(10, 10);
	cout << "generated random matrix. It is " << X << endl;
	
	MatlabWrapper mw;
	mw.sendMat("X", X); 	// move matrix X to Matlab Engine under variable name X
	mw.exec("imshow(X, [])", true); // display it and wait for user to hit Enter
	mw.exec("close"); // close the figure
	
	// start an interactive session. Use Matlab as normal!
	// feel free to modify the matrix X we sent over. We will retrieve it back
	mw.interact();
	
	Eigen::MatrixXf mat;
  mw.receiveMat("X", mat);
	cout << "retrieving matrix X back. It is " << mat << endl;
	
	return 0;
}


