#pragma once

#ifndef __cplusplus
#  error accumulator.hpp is a C++ header
#endif

#include "imageframe.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Accumulator
{
	private:
		cv::Mat matrix;
	public:
		Accumulator(ImageFrame& initial);
		void accumulate(const ImageFrame& who, double alpha = 0.5);
		ImageFrame getFrame() const;
		virtual ~Accumulator() {}
};