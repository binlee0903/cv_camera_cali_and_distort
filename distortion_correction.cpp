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
	cv::VideoCapture camVideoCapture("./checker.png");
	cv::Matx33d kValues(441.987937630417, 0, 483.8420546929668, 0, 444.9334411107657, 289.2140595890675, 0, 0, 1);
	std::vector<double> distanceCoefficient = { -0.3217870173551936, 0.1501651599582398, -1.985534104724499e-05, -0.00541502074269745, -0.0439181875233754 };

	int key = 0;
	bool ret = 0;
	cv::Mat buffer1, buffer2;

	if (camVideoCapture.isOpened() != true)
	{
		__debugbreak();
	}

	camVideoCapture >> buffer;

	if (buffer1.empty() || buffer2.empty())
	{
		cv::initUndistortRectifyMap(kValues, distanceCoefficient, cv::Mat(), cv::Mat(), buffer.size(), CV_32FC1, buffer1, buffer2);
	}

	cv::remap(buffer, buffer, buffer1, buffer2, cv::InterpolationFlags::INTER_LINEAR);

	cv::imshow("record", buffer);
	cv::waitKey();
	cv::destroyAllWindows();

	return 0;
}
