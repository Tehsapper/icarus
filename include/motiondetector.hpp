#pragma once

#ifndef __cplusplus
#  error motiondetector.hpp is a C++ header
#endif

#include "sentry.hpp"
#include "video.hpp"
#include "imageframe.hpp"

class Sentry;

class MotionDetector
{
	protected:
		Sentry* parent;
	public:
		MotionDetector(Sentry* ptr);
		virtual void setup(Video& src) = 0;
		virtual void process(ImageFrame& img) = 0;
		virtual ~MotionDetector() {}
};

class DifferenceMotionDetector : public MotionDetector
{
	private:
		ImageFrame prev, curr, next;
		int threshold, marked_frames_count, marked_frames_threshold;
		float motion_area_threshold;
	public:
		DifferenceMotionDetector(Sentry* ptr);
		virtual void setup(Video& src);
		virtual void process(ImageFrame& img);
		virtual void markFrame(ImageFrame& img);
		virtual void unmarkFrame(ImageFrame& img);
		virtual ~DifferenceMotionDetector() {}
};