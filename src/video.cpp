#include "video.hpp"

Video::Video(const int index)
{
	captor.open(index);
	fps = captor.get(CV_CAP_PROP_FPS);
	frame_size = cv::Size((int)captor.get(CV_CAP_PROP_FRAME_WIDTH), (int)captor.get(CV_CAP_PROP_FRAME_HEIGHT));
}

Video::Video(const char* filename)
{
	captor.open(filename);
	fps = captor.get(CV_CAP_PROP_FPS);
	frame_size = cv::Size((int)captor.get(CV_CAP_PROP_FRAME_WIDTH), (int)captor.get(CV_CAP_PROP_FRAME_HEIGHT));
}

const double Video::getFPS( void ) const 
{
	return fps;
}

const cv::Size& Video::getFrameSize( void ) const
{
	return frame_size;
}

bool Video::read(ImageFrame& frame)
{
	return frame.readFrom(captor);
}