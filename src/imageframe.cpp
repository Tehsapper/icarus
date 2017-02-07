#include "imageframe.hpp"

ImageFrame::ImageFrame() {}

ImageFrame::ImageFrame( const ImageFrame& img)
{
	img.matrix.copyTo(matrix);
}

ImageFrame::ImageFrame( const cv::Mat& mat)
{
	matrix = mat;
}

ImageFrame& ImageFrame::operator=( const ImageFrame& img)
{
	img.matrix.copyTo(matrix);
	return *this;
}

ImageFrame& ImageFrame::operator&=( const ImageFrame& img)
{
	cv::bitwise_and(matrix, img.matrix, matrix);
	return *this;
}

ImageFrame& ImageFrame::operator-=( const ImageFrame& img)
{
	cv::absdiff(matrix, img.matrix, matrix);
	return *this;
}

void ImageFrame::drawRectangle(const cv::Rect& rect, const cv::Scalar& color, int thickness)
{
	cv::rectangle( matrix, rect, color, thickness );
}

void ImageFrame::grayscale()
{
	cv::cvtColor( matrix, matrix, CV_BGR2GRAY );
}

void ImageFrame::threshold( const int value )
{
	cv::threshold( matrix, matrix, value, 255, 0 );
}

void ImageFrame::closeMorph()
{
	static int kernel_size = 5;
	static cv::Mat kernel = getStructuringElement( 2, cv::Size( 2*kernel_size + 1, 2*kernel_size+1 ) );
	cv::morphologyEx( matrix, matrix, cv::MORPH_CLOSE, kernel );
}

ImageFrame& ImageFrame::crop(const cv::Rect& rect)
{
	matrix = matrix(rect);
	return *this;
}

bool ImageFrame::readFrom(cv::VideoCapture& video)
{
	return video.read(matrix);
}