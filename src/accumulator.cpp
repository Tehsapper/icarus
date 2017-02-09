#include "accumulator.hpp"

Accumulator::Accumulator(ImageFrame& initial)
{
	// assumes that we're using a single-channel cv::Mat (a grayscale image, for example)
	initial.getMatrix().convertTo(matrix, CV_32F);
}

void Accumulator::accumulate(const ImageFrame& who, double alpha)
{
	cv::accumulateWeighted(who.getMatrix(), matrix, alpha);
	#ifdef DEBUG
	cv::Mat res;
	normalize( matrix, res, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
	cv::imshow("background", res);
	#endif
}

ImageFrame Accumulator::getFrame() const
{
	cv::Mat other;
	matrix.convertTo(other, CV_8U);
	return ImageFrame(other);
}