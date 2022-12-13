#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;
int main()
{
	Mat image(200,300,CV_8U,Scalar(255));
	namedWindow("key event",WINDOW_AUTOSIZE);
	imshow("key event",image);

	while(1)
	{
		int key = waitKey(100);
		if(key ==27) break;
		//printf("%x",key);
		//cout <<hex<<key<<endl;
		switch(key)
		{
			case 'a': cout <<"a input"<<endl; break;
			case 'b': cout <<"b input"<<endl; break;
			case 0x41: cout <<"A input"<<endl; break;
			case 66 : cout <<"B input"<<endl; break;

			case 0x51: cout<<"left key"<<endl; break;
			case 0x52: cout<<"over key"<<endl; break;
			case 0x53: cout<<"right key"<<endl; break;
			case 0x54: cout<<"under key"<<endl; break;
		}
	}
	return 0;
}
