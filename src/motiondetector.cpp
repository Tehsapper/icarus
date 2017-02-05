#include "motiondetector.hpp"
#include "sentry.hpp"
#include <string>
#include <stdio.h>

MotionDetector::MotionDetector(Sentry* ptr)
{
	parent = ptr;
}

DifferenceMotionDetector::DifferenceMotionDetector(Sentry* ptr) : MotionDetector(ptr)
{
	threshold = stoi(parent->getConfig()["detector.threshold_value"]);
	marked_frames_count = 0;
	motion_area_threshold = stof(parent->getConfig()["detector.motion_area_alert_threshold"]);
}	

void DifferenceMotionDetector::setup(Video& src)
{
	marked_frames_threshold = (int)(stof(parent->getConfig()["detector.alert_time"]) * src.getFPS());
	if(marked_frames_threshold < 1) marked_frames_threshold = 1;

	if(src.read(prev) && src.read(curr))
	{
		prev.crop(parent->getDetectionRegion());
		curr.crop(parent->getDetectionRegion());
		prev.grayscale();
		curr.grayscale();
	} else throw std::runtime_error(std::string("failed to setup DifferenceMotionDetector: couldn't read 2 frames from video source"));
}

void DifferenceMotionDetector::process(ImageFrame& img)
{
	next = img;
	next.grayscale();

	ImageFrame diff = (next - curr) & (curr - prev);
	diff.threshold(threshold);
	diff.closeMorph();

	int nz = countNonZero(diff.getMatrix()), all = diff.getMatrix().total();
	float motion_area = ((float)nz / (float)all) * 100.0f;
	
	if (motion_area > motion_area_threshold) markFrame(diff);
 	else unmarkFrame(diff);

	prev = curr;
	curr = next;
}

void DifferenceMotionDetector::markFrame(ImageFrame& img)
{
	if(marked_frames_count > marked_frames_threshold)
	{
		parent->raise();
	} else marked_frames_count++;
}

void DifferenceMotionDetector::unmarkFrame(ImageFrame& img)
{
	if(marked_frames_count > 0) marked_frames_count--; else parent->lower();
}