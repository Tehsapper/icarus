#pragma once

#ifndef __cplusplus
#  error logger.hpp is a C++ header
#endif

#include "imageframe.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <string>
#include <time.h>

struct tm* getLocalTime();

class Logger
{
	private:
		std::ofstream stream;
	public:
		Logger(const char* filename);
		virtual void log(const std::string msg);
		virtual ~Logger();
};

class VideoLogger
{
	private:
		size_t count;
		char name_buffer[255];
		double fps;
		cv::Size frame_size;
		cv::VideoWriter writer;
	public:
		VideoLogger(double p_fps, cv::Size p_size);
		virtual void start();
		virtual void log( ImageFrame& frame );

		const char* getName();

		virtual ~VideoLogger();
};