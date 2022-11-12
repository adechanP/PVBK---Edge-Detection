// PVBKConApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <cmath>

#include <opencv2/opencv.hpp>

#pragma comment(lib, "opencv_world460")

using namespace cv;
using namespace std;		

bool checkImageProperties(Mat& img1, Mat& img2)
{
	if (img1.empty())
	{
		printf("img1 is empty");
		return false;
	}

	if (img2.empty())
	{
		printf("img2 is empty");
		return false;
	}

	if (img1.cols != img2.cols)
	{
		printf("widths are different\n");
		return false;
	}

	if (img1.rows != img2.rows)
	{
		printf("heights are different\n");
		return false;
	}

	if (img1.channels() != img2.channels())
	{
		printf("Channels are different\n");
		return false;
	}

	return true;
}


///////////// Pixel Operation /////////////
/////// This is the basic template ////////
///////////// Don't change it /////////////
int Grayscale(Mat& img_src, Mat& img_dst)
{
	Vec3b bgr_src;
	uchar gray_dst;

	for (int y = 0; y < img_src.rows; y++)
	{
		for (int x = 0; x < img_src.cols; x++)
		{
			bgr_src = img_src.at<Vec3b>(y, x);
			unsigned char b = bgr_src[0];
			unsigned char g = bgr_src[1];
			unsigned char r = bgr_src[2];

			uchar gr = (uchar)((float)(b + g + r) / 3);
			gray_dst = gr;

			img_dst.at<uchar>(y, x) = gray_dst;
		}
	}
	return 0;
}


int EdgeDetection(Mat& img_src1, Mat& img_dst1)
{
	if (!checkImageProperties(img_src1, img_dst1))
	{
		printf("Error: pixelsOperation function, in checkImageProperties\n");
		return -1;
	}
	uchar gray_src1 = 0;
	uchar edge_dst1 = 0;
	uchar data[9];
	Mat ver_result = Mat::zeros(img_src1.size(), CV_8UC1);
	Mat hor_result = Mat::zeros(img_src1.size(), CV_8UC1);
	
	Mat ver_sobel = (Mat_<char>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	Mat hor_sobel = (Mat_<char>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
	for (int y = 1; y < img_src1.rows - 1; y++)
	{
		for (int x = 1; x < img_src1.cols - 1; x++)
		{
			gray_src1 = img_src1.at<uchar>(y, x);
			data[0] = img_src1.at<uchar>(y - 1, x - 1);
			data[1] = img_src1.at<uchar>(y - 1, x);
			data[2] = img_src1.at<uchar>(y - 1, x + 1);
			data[3] = img_src1.at<uchar>(y, x - 1);
			data[4] = img_src1.at<uchar>(y, x);
			data[5] = img_src1.at<uchar>(y, x + 1);
			data[6] = img_src1.at<uchar>(y + 1, x - 1);
			data[7] = img_src1.at<uchar>(y + 1, x);
			data[8] = img_src1.at<uchar>(y + 1, x + 1);
			
			int16_t	ver_sum = 0;
			for (int i = 0; i < 9; i++)
			{
				ver_sum += data[i] * ver_sobel.at<char>(i / 3, i % 3);
				ver_result.at<uchar>(y, x) = saturate_cast<uchar>(ver_sum);
			}
			int16_t	hor_sum = 0;
			for (int i = 0; i < 9; i++)
			{
				hor_sum += data[i] * hor_sobel.at<char>(i / 3, i % 3);
				hor_result.at<uchar>(y, x) = saturate_cast<uchar>(hor_sum);
			}

			edge_dst1 = saturate_cast<uchar>(sqrt(pow(ver_sum,2) + pow(hor_sum,2)));
			if (edge_dst1 < 254)
				edge_dst1 = 0;
			img_dst1.at<uchar>(y, x) = edge_dst1;
		}
	}
	return 0;
}

void mergeImage(Mat& img1, Mat& img2, Mat& img_dst2)
{
	Vec3b img1_src, img2_src, gray_dst;
	for (int y = 0; y < img1.rows; y++)
	{
		for (int x = 0; x < img1.cols; x++)
		{
			img1_src = img1.at<Vec3b>(y, x);
			img2_src = img2.at<Vec3b>(y, x);
			uchar gr_dt1 = img1_src[0];
			uchar gr_dt2 = img2_src[0];
			if (gr_dt1 > 192 || gr_dt2 > 192)
			{
				gray_dst[0] = 255;
				gray_dst[1] = 255;
				gray_dst[2] = 255;
			}
			else
			{
				gray_dst[0] = 0;
				gray_dst[1] = 0;
				gray_dst[2] = 0;
			}
			img_dst2.at<Vec3b>(y, x) = gray_dst;
		}
	}
}

int main()
{
	// Read the file
	Mat inputImg_1 = imread("Edge_Detection_1.jpg", IMREAD_GRAYSCALE);
	Mat inputImg_2 = imread("Edge_Detection_2.jpg", IMREAD_GRAYSCALE);

	// Check for invalid input
	if (inputImg_1.empty() || inputImg_2.empty())
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	// Create new image
	Mat GRoutputImg_1 = Mat::zeros(inputImg_1.size(), CV_8UC1);
	Mat GRoutputImg_2 = Mat::zeros(inputImg_2.size(), CV_8UC1);
	Mat EDGEoutputImg_1 = Mat::zeros(inputImg_1.size(), CV_8UC1);
	Mat EDGEoutputImg_2 = Mat::zeros(inputImg_2.size(), CV_8UC1);
	
	// Invoke RGB2GRAY function here
	Grayscale(inputImg_1, GRoutputImg_1);
	Grayscale(inputImg_2, GRoutputImg_2);
	EdgeDetection(inputImg_1, EDGEoutputImg_1);
	EdgeDetection(inputImg_2, EDGEoutputImg_2);
	
	//Confirmation Code - Using imgproc.h
	/*cvtColor(inputImg, outputImg, COLOR_RGB2HSV);*/
	//cvtColor(inputImg, outputImg, COLOR_BGR2HSV);

	// Create a window for display.
	namedWindow("Input_1", WINDOW_AUTOSIZE);
	namedWindow("OutputGR_1", WINDOW_AUTOSIZE);
	namedWindow("Input_2", WINDOW_AUTOSIZE);
	namedWindow("OutputGR_2", WINDOW_AUTOSIZE);
	

	// Show our image inside it.
	imshow("Input_1", inputImg_1);
	imshow("OutputGR_1",EDGEoutputImg_1);
	imshow("Input_2", inputImg_2);
	imshow("OutputGR_2", EDGEoutputImg_2);
	

	
	// Wait for a keystroke in the window
	waitKey(0);

	destroyAllWindows();

	return 0;
}


//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
