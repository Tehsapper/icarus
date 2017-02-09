#include "tamperingdetector.hpp"
#include <iostream>

TamperingDifferenceDetector::TamperingDifferenceDetector(Sentry* ptr) : DifferenceDetector(ptr)
{
	threshold = stoi(parent->getConfig()["detector.threshold_value"]);
	area_threshold = stof(parent->getConfig()["tampering.area_threshold"]);
	adaption_rate = stod(parent->getConfig()["tampering.adaption_rate"]);
}

void TamperingDifferenceDetector::setup()
{
	ImageFrame frame;

	marked_frames_threshold = (int)(stof(parent->getConfig()["tampering.alert_time"]) * parent->getVideo().getFPS());
	watch_count = (int)(stof(parent->getConfig()["tampering.watch_time"]) * parent->getVideo().getFPS());

	if(marked_frames_threshold < 1) marked_frames_threshold = 1;

	int accumulate_frames = (int)(stof(parent->getConfig()["tampering.accumulate_time"]) * parent->getVideo().getFPS());
	std::cout << "accumulating " << accumulate_frames << " frames" << std::endl;

	if(parent->provideFrame(frame))
	{
		frame.grayscale();
		acc = new Accumulator(frame);
		for(size_t i = 0; i < accumulate_frames; i++) if(parent->provideFrame(frame))
		{
			frame.grayscale();
			acc->accumulate(frame, 0.5);
		}
	} else throw std::runtime_error(std::string("failed to setup TamperingDifferenceDetector: couldn't read background frame from video source"));
}

void TamperingDifferenceDetector::process(ImageFrame& img)
{
	assert(acc);
	img.grayscale();

	ImageFrame diff = img - acc->getFrame();
	diff.threshold(threshold);
	diff.closeMorph();

	analyze(diff, img);
}

void TamperingDifferenceDetector::unmarkFrame(ImageFrame& img)
{
	acc->accumulate(img, adaption_rate);
	DifferenceDetector::unmarkFrame(img);
}

TamperingDifferenceDetector::~TamperingDifferenceDetector() {}