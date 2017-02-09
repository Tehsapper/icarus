#pragma once

#ifndef __cplusplus
#  error detector.hpp is a C++ header
#endif

#include "sentry.hpp"
#include "video.hpp"
#include "imageframe.hpp"

class Sentry;

class Detector
{
	protected:
		Sentry* parent;
	public:
		Detector(Sentry* ptr) { parent = ptr; }
		virtual void setup() = 0;
		virtual void process(ImageFrame& img) = 0;
		virtual void analyze(const ImageFrame& diff, ImageFrame& original) = 0;
		virtual ~Detector() {}
};

class DifferenceDetector : public Detector
{
	protected:
		int threshold, marked_frames_count, marked_frames_threshold, watch_count;
		float area_threshold;
	public:
		DifferenceDetector(Sentry* ptr);
		virtual void analyze(const ImageFrame& diff, ImageFrame& original);
		virtual void markFrame(ImageFrame& diff);
		virtual void unmarkFrame(ImageFrame& diff);
		virtual ~DifferenceDetector() {}
};