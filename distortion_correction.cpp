#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <iostream>

bool g_isRecorded = false;
bool g_isFlipped = false;

constexpr int SPACE_ASCII = 32;
constexpr int ESC_ASCII = 27;
constexpr int F_ASCII = 'F';
constexpr int f_ASCII = 'f';

constexpr int BOARD_WIDTH = 10;
constexpr int BOARD_HEIGHT = 7;
constexpr float BOARD_CELL_SIZE = 0.025f;

int main(int argc, char** argv)
{
	std::vector<cv::Mat> images;
	cv::Mat buffer;
	cv::VideoCapture camVideoCapture("./chessboard-720.mp4");
	cv::Matx33d kValues(988.0130764328934, 0, 420.9784087272643, 0, 1001.272688293928, 204.1115496365296, 0, 0, 1);
	std::vector<double> distanceCoefficient = { 0.2319312814294019, -4.133235939041602, 0.00254585949211152, 0.007092331493671674, 21.51409719800601 };

	int key = 0;
	bool ret = 0;
	cv::Mat buffer1, buffer2;

	if (camVideoCapture.isOpened() != true)
	{
		__debugbreak();
	}

	while (camVideoCapture.isOpened() == true)
	{
		ret = camVideoCapture.read(buffer);

		if (ret == false)
		{
			break;
		}
		else
		{
			cv::initUndistortRectifyMap(kValues, distanceCoefficient, cv::Mat(), cv::Mat(), buffer.size(), CV_32FC1, buffer1, buffer2);
			cv::remap(buffer, buffer, buffer1, buffer2, cv::InterpolationFlags::INTER_LINEAR);
			cv::imshow("record", buffer);
			cv::waitKey();
		}
		
		cv::waitKey(1);
	}
	
	cv::destroyAllWindows();
	camVideoCapture.release();

	return 0;
}
