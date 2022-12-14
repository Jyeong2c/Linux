#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void put_string(Mat &frame, string text,Point pt, int value)
{
	text += to_string(value);
	Point shade = pt +Point(2,2);
	int font = FONT_HERSHEY_SIMPLEX;
	putText(frame, text, shade, font,0.7,Scalar(0,0,0),2);
	putText(frame, text, pt, font,0.7,Scalar(120,200,90),2);
}
VideoCapture capture;

void zoom_bar(int value, void*)
{
	capture.set(CAP_PROP_ZOOM, value);
}
void focus_bar(int value, void*)
{
	capture.set(CAP_PROP_FOCUS,value);
}

int main()
{

	VideoCapture capture(0);
	CV_Assert(capture.isOpened());

	if(!capture.isOpened())
	{
		cout<<"don't connect" <<endl;
		exit(1);
	}

	cout<<"width"<<capture.get(CAP_PROP_FRAME_WIDTH)<<endl;
	cout<<"height"<<capture.get(CAP_PROP_FRAME_HEIGHT)<<endl;
	cout<<"exposure"<<capture.get(CAP_PROP_EXPOSURE)<<endl;
	cout<<"bright"<<capture.get(CAP_PROP_BRIGHTNESS)<<endl;

//	capture.set(CAP_PROP_FRAME_WIDTH,400);
//	capture.set(CAP_PROP_FRAME_HEIGHT,300);
//	capture.set(CAP_PROP_AUTOFOCUS,0);
//	capture.set(CAP_PROP_BRIGHTNESS,150);

	int zoom = capture.get(CAP_PROP_ZOOM);
	int focus = capture.get(CAP_PROP_FOCUS);

	String title = "camera";
	namedWindow(title);
	createTrackbar("zoom",title,&zoom,10,zoom_bar);
	createTrackbar("focus",title,&focus,40,focus_bar);

	for(;;){
		Mat frame;
		capture >>frame;
		put_string(frame,"zoom :", Point(10,240),zoom);
		put_string(frame,"focus :", Point(10,270),focus);

		imshow(title, frame);
		if(waitKey(30)>=0) break;
	}
	return 0;
}


