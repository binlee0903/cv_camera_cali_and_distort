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

	int key = 0;
	bool ret = 0;
	//camVideoCapture.set(cv::CAP_PROP_FRAME_WIDTH, DESIRED_WIDTH);
	//camVideoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, DESIRED_HEIGHT);

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
			images.push_back(buffer);
		}
	}

	cv::destroyAllWindows();

	std::vector<std::vector<cv::Point2f>> _2dPoints;
	std::vector<cv::Point2f> pointBuffer;

	for (auto& x : images)
	{
		if (cv::findChessboardCorners(x, { BOARD_WIDTH, BOARD_HEIGHT }, pointBuffer) == true)
		{
			_2dPoints.push_back(pointBuffer);
		}

		pointBuffer.clear();
	}

	if (_2dPoints.empty() == true)
	{
		__debugbreak();
	}

	std::vector<std::vector<cv::Point3f>> _3dPoints;
	_3dPoints.push_back(std::vector<cv::Point3f>());

	for (size_t i = 0; i < BOARD_HEIGHT; i++)
	{
		for (size_t j = 0; j < BOARD_WIDTH; j++)
		{
			_3dPoints[0].push_back({ BOARD_CELL_SIZE * j, BOARD_CELL_SIZE * i, 0 });
		}
	}

	_3dPoints.resize(images.size(), _3dPoints[0]);

	cv::Mat view = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distanceCoefficient = cv::Mat::zeros(4, 1, CV_64F);
	std::vector<cv::Mat> rvecs, tvecs;

	double rms = cv::calibrateCamera(_3dPoints, _2dPoints, images[0].size(), view, distanceCoefficient, rvecs, tvecs);

	std::cout << "RMS ERROR: " << rms << std::endl;
	std::cout << "* Camera matrix (K) = " << std::endl << "  " << view.row(0) << view.row(1) << view.row(2) << std::endl;
	std::cout << "Distortion coefficient (k1, k2, p1, p2, k3 ...): " << distanceCoefficient.t() << std::endl;

	return 0;
}
