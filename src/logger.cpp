#include "logger.hpp"
#include <cstdio>

Logger::Logger(const char* filename)
{
	stream.open(filename);
	if(!stream) throw std::runtime_error(std::string("failed to open log file ") + filename);
}

struct tm* getLocalTime()
{
	static time_t raw_time;
	time(&raw_time);
	return localtime(&raw_time);
}

void Logger::log(const std::string msg)
{
	static char buffer[64];
	
	strftime(buffer, 64, "%F %T ", getLocalTime());
	stream << buffer;
	stream << msg;
	stream << std::endl;
}

Logger::~Logger()
{
	stream.close();
}

VideoLogger::VideoLogger(double p_fps, cv::Size p_size)
{
	count = 0;
	fps = p_fps; frame_size = p_size; save_flag = true;
}

void VideoLogger::start()
{
	count++;
	stop();
	save_flag = false;
	strftime(name_buffer, 255, "%F-%H%M%S.mkv", getLocalTime());
	if(!writer.open(name_buffer, CV_FOURCC('H','2','6','4'), fps, frame_size))
		throw std::runtime_error(std::string("failed to open video log file ") + name_buffer);
}

void VideoLogger::save()
{
	save_flag = true;
}

void VideoLogger::stop()
{
	writer.release();
	if(!save_flag) remove(name_buffer);
}

void VideoLogger::log( ImageFrame& frame)
{
	if( writer.isOpened() ) writer << frame;
}

const char* VideoLogger::getName() const
{
	return name_buffer;
}

VideoLogger::~VideoLogger()
{
	stop();
}