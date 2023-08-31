#include "stdafx.h"
#include <Windows.h>
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

int main(int argc, char* argv[])
{
	cv::VideoCapture capture;
	if (argc <= 1)
		capture.open("sample.mp4");
	else
		capture.open(argv[1]);
	if (!capture.isOpened())
		return -1;

	cv::Mat frame;
	capture >> frame;
	bool flip = false;
	cv::Size dst = { frame.cols,frame.rows };
	if (!frame.empty()) {
		int fourcc = (int)capture.get(cv::CAP_PROP_FOURCC);
		float scale = frame.rows / 640.0f;
		if (scale > 1) dst = { (int)(dst.width / scale), (int)(dst.height / scale) };
		if (capture.get(cv::CAP_PROP_ORIENTATION_AUTO)) {
			flip = capture.get(cv::CAP_PROP_ORIENTATION_META)>0;
		}
	}

	int n = 1;
	char msg[MAX_PATH] = { 0 };
	bool paused = false;

	while (!frame.empty() || ++n < argc)
	{
		if (frame.empty() && capture.open(argv[n])) {
			capture >> frame;
			if (frame.empty())
				break;
		}

		long fps = capture.get(cv::CAP_PROP_POS_FRAMES);
		long msec = capture.get(cv::CAP_PROP_POS_MSEC);
		sprintf(msg, "fps: %ld, msec: %ld", fps, msec);

		cv::resize(frame, frame, dst);
		if(flip) cv::flip(frame, frame, -1);  // Flip both horizontally and vertically

		cv::putText(frame, msg, cv::Point(frame.cols / 3, frame.rows / 3), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(128, 0, 256), 2);
		cv::imshow("cvPlayer", frame);

		int key = cv::waitKey(paused ? 0 : 1);
		if (key == 27)  // ESC¼ü
		{
			break;
		}
		else if (key == 32)  // ¿Õ¸ñ¼ü
		{
			paused = !paused;  // ÔÝÍ£/¼ÌÐø
		}
		else if (key == 'f')  // ×ó·½Ïò¼ü
		{
			capture.set(cv::CAP_PROP_POS_FRAMES, fps - 2);
		} else if(key == 'x') {
			flip = !flip;
		}
		capture >> frame;
	}

	capture.release();
	return 0;
}