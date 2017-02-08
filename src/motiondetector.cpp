#include "motiondetector.hpp"
#include "sentry.hpp"
#include <string>
#include <stdio.h>

Detector::Detector(Sentry* ptr)
{
	parent = ptr;
}

DifferenceMotionDetector::DifferenceMotionDetector(Sentry* ptr) : Detector(ptr)
{
	threshold = stoi(parent->getConfig()["detector.threshold_value"]);
	marked_frames_count = 0;
	motion_area_threshold = stof(parent->getConfig()["detector.motion_area_alert_threshold"]);
}	

void DifferenceMotionDetector::setup(Video& src)
{
	marked_frames_threshold = (int)(stof(parent->getConfig()["detector.alert_time"]) * src.getFPS());
	watch_count = (int)(stof(parent->getConfig()["detector.watch_time"]) * src.getFPS());

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

 	#ifdef DEBUG
 	cv::imshow("diff", diff.getMatrix());
 	printf("motion area: %f (%d/%d), marked frame count: %d out of %d\n", motion_area, nz, all, marked_frames_count, marked_frames_threshold);
 	#endif

	prev = curr;
	curr = next;
}

void DifferenceMotionDetector::markFrame(ImageFrame& img)
{
	if(marked_frames_count == 1 && parent->getLevel() == ThreatLevel::ALL_CLEAR) parent->raise();
	if(marked_frames_count < marked_frames_threshold) marked_frames_count++;
	if(marked_frames_count == marked_frames_threshold)
	{
		parent->raise();
		marked_frames_count = watch_count;
	}
}

void DifferenceMotionDetector::unmarkFrame(ImageFrame& img)
{
	if(marked_frames_count > 0) marked_frames_count--; else parent->lower();
}

TamperingDetector::TamperingDetector(Sentry* ptr) : Detector(ptr)
{
	printf("Created tampering detector!\n");
	pbgSub = cv::createBackgroundSubtractorMOG2(1000, 32, false);
}

#define DEBUG

void TamperingDetector::setup(Video& src)
{
	marked_frames_threshold = (int)(stof(parent->getConfig()["detector.alert_time"]) * src.getFPS());
	watch_count = (int)(stof(parent->getConfig()["detector.watch_time"]) * src.getFPS());

	if(marked_frames_threshold < 1) marked_frames_threshold = 1;

	if(src.read(frame))
	{
		pbgSub->apply(frame.getMatrix(), fgMask);
	} else throw std::runtime_error(std::string("failed to setup TamperingDetector: couldn't read a frame from video source"));
}

void TamperingDetector::process(ImageFrame& img)
{
	pbgSub->apply(img.getMatrix(), fgMask);
	ImageFrame fgFrame(fgMask);

	fgFrame.closeMorph();

	int nz = countNonZero(fgFrame.getMatrix()), all = fgFrame.getMatrix().total();
	float area = ((float)nz / (float)all) * 100.0f;
	
	if (area > area_threshold) markFrame(img);
 	else unmarkFrame(img);

 	#ifdef DEBUG
 	cv::imshow("fgMask", fgFrame.getMatrix());
 	printf("area: %f (%d/%d), marked frame count: %d out of %d\n", area, nz, all, marked_frames_count, marked_frames_threshold);
 	#endif
}

void TamperingDetector::markFrame(ImageFrame& img)
{
	if(marked_frames_count == 1 && parent->getLevel() == ThreatLevel::ALL_CLEAR) parent->raise();
	if(marked_frames_count < marked_frames_threshold) marked_frames_count++;
	if(marked_frames_count == marked_frames_threshold)
	{
		parent->raise();
		marked_frames_count = watch_count;
	}
}

void TamperingDetector::unmarkFrame(ImageFrame& img)
{
	if(marked_frames_count > 0) marked_frames_count--; else parent->lower();
}

TamperingDetector::~TamperingDetector()
{
}