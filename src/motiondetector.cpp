#include "motiondetector.hpp"
#include "sentry.hpp"
#include <string>

DifferenceMotionDetector::DifferenceMotionDetector(Sentry* ptr) : DifferenceDetector(ptr)
{
	threshold = stoi(parent->getConfig()["detector.threshold_value"]);
	area_threshold = stof(parent->getConfig()["motion.area_threshold"]);
}	

void DifferenceMotionDetector::setup()
{
	marked_frames_threshold = (int)(stof(parent->getConfig()["motion.alert_time"]) * parent->getVideo().getFPS());
	watch_count = (int)(stof(parent->getConfig()["motion.watch_time"]) * parent->getVideo().getFPS());

	if(marked_frames_threshold < 1) marked_frames_threshold = 1;

	if(parent->provideFrame(prev) && parent->provideFrame(curr))
	{
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

	analyze(diff, img);

	prev = curr;
	curr = next;
}