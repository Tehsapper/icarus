#pragma once

#ifndef __cplusplus
#  error motiondetector.hpp is a C++ header
#endif

#include "sentry.hpp"
#include "video.hpp"
#include "imageframe.hpp"
#include <opencv2/video.hpp>
#include <opencv2/core/core.hpp>

class Sentry;

class Detector
{
	protected:
		Sentry* parent;
	public:
		Detector(Sentry* ptr);
		virtual void setup(Video& src) = 0;
		virtual void process(ImageFrame& img) = 0;
		virtual ~Detector() {}
};

class DifferenceMotionDetector : public Detector
{
	private:
		ImageFrame prev, curr, next;
		int threshold, marked_frames_count, marked_frames_threshold, watch_count;
		float motion_area_threshold;
	public:
		DifferenceMotionDetector(Sentry* ptr);
		virtual void setup(Video& src);
		virtual void process(ImageFrame& img);
		virtual void markFrame(ImageFrame& img);
		virtual void unmarkFrame(ImageFrame& img);
		virtual ~DifferenceMotionDetector() {}
};

class TamperingDetector : public Detector
{
	private:
		cv::Mat fgMask;

		cv::Ptr<cv::BackgroundSubtractor> pbgSub;
		ImageFrame frame;
		int threshold, marked_frames_count, marked_frames_threshold, watch_count;
		float area_threshold;
	public:
		TamperingDetector(Sentry* ptr);
		virtual void setup(Video& src);
		virtual void process(ImageFrame& img);
		virtual void markFrame(ImageFrame& img);
		virtual void unmarkFrame(ImageFrame& img);
		virtual ~TamperingDetector();
};