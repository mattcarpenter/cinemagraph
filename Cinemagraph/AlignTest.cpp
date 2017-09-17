#include "AlignTest.h"

using namespace std;
using namespace cv;

AlignTest::AlignTest()
{
}

AlignTest::~AlignTest()
{
}

void AlignTest::Test()
{
	Mat still = imread("C:\\sample\\still.png");
	Mat video = imread("C:\\sample\\video.png");

	Mat still_gray, video_gray;

	cvtColor(still, still_gray, COLOR_BGR2GRAY);
	cvtColor(video, video_gray, COLOR_BGR2GRAY);

	Mat warp_matrix = Mat::eye(2, 3, CV_32F);
	findTransformECC(video_gray, still_gray, warp_matrix, MOTION_AFFINE);

	Mat dest;
	warpAffine(still, dest, warp_matrix, video.size(), INTER_LANCZOS4 + WARP_INVERSE_MAP);

	imshow("still", still);
	imshow("video", video);
	imshow("dest", dest);
	waitKey(0);
}
