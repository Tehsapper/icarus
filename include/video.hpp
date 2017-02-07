#pragma once

#ifndef __cplusplus
#  error video.hpp is a C++ header
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "imageframe.hpp"

class Video
{
	private:
		cv::VideoCapture captor;
		double fps;
		cv::Size frame_size;
	public:
		Video(const int index);
		Video(const char* filename);
		bool read(ImageFrame& frame);

		const double getFPS() const ;
		const cv::Size& getFrameSize() const ;
};