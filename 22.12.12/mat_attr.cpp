#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
	Mat m1(4,3,CV_32SC3);
	cout<<"demasion number =" <<m1.dims<<endl;
	cout<<"row  number =" <<m1.rows<<endl;
	cout<<"col number =" <<m1.cols<<endl;
	cout<<"m1 size =" <<m1.size()<<endl<<endl;

	cout<< "m1 total = " <<m1.total()<<endl;
	cout<< "m1 elemSize = " <<m1.elemSize()<<endl;
	cout<< "m1 elemSize1 = " <<m1.elemSize1()<<endl<<endl;

	
	cout<< "m1 type = " <<m1.type()<<endl;
	cout<< "m1 channels|depth = " <<((m1.channels() - 1) << 3)+ m1.depth() <<endl;
	cout<< "m1 depth = " <<m1.depth()<<endl;
	cout<< "m1 channels = " <<m1.channels()<<endl<<endl;


	cout<< "step = " <<m1.step<<endl;
	cout<< "step1() = " <<m1.step1()<<endl;
	return 0;
	}

