#pragma once

#ifndef __cplusplus
#  error motiondetector.hpp is a C++ header
#endif

#include "detector.hpp"
#include <opencv2/core/core.hpp>

class DifferenceMotionDetector : public DifferenceDetector
{
	private:
		ImageFrame prev, curr, next;
	public:
		DifferenceMotionDetector(Sentry* ptr);
		virtual void setup();
		virtual void process(ImageFrame& img);
		virtual ~DifferenceMotionDetector() {}
};