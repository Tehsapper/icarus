#pragma once

#ifndef __cplusplus
#  error tamperingdetector.hpp is a C++ header
#endif

#include "detector.hpp"
#include "accumulator.hpp"
#include <opencv2/video.hpp>

class TamperingDifferenceDetector : public DifferenceDetector
{
	private:
		Accumulator* acc;
		double adaption_rate;
	public:
		TamperingDifferenceDetector(Sentry* ptr);
		virtual void setup();
		virtual void process(ImageFrame& img);
		virtual void unmarkFrame(ImageFrame& img);
		virtual ~TamperingDifferenceDetector();
};