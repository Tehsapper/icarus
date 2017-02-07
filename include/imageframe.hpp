#pragma once

#ifndef __cplusplus
#  error imageframe.hpp is a C++ header
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>

class ImageFrame
{
	private:
		cv::Mat matrix;
	public:
		ImageFrame();
		ImageFrame( const ImageFrame& img);
		ImageFrame( const cv::Mat& m);

		ImageFrame& operator=( const ImageFrame& img);
		ImageFrame& operator&=( const ImageFrame& img);
		friend ImageFrame operator& (ImageFrame left, const ImageFrame& right)
		{
			left &= right;
			return left;
		}

		ImageFrame& operator-=( const ImageFrame& img);
		friend ImageFrame operator- (ImageFrame left, const ImageFrame& right)
		{
			left -= right;
			return left;
		}

		friend cv::VideoWriter& operator<< (cv::VideoWriter& left, const ImageFrame& right)
		{
			return left << right.matrix;
		}

		friend cv::VideoCapture& operator>> (cv::VideoCapture& left, ImageFrame& right)
		{
			return left >> right.matrix;
		}

		bool readFrom(cv::VideoCapture& video);

		const cv::Mat& getMatrix() const { return matrix; }
		void grayscale();
		void threshold( const int value );
		void closeMorph();
		void drawRectangle(const cv::Rect& rect, const cv::Scalar& color, int thickness);
		ImageFrame& crop(const cv::Rect& rect);
};