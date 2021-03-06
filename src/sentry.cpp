#include "sentry.hpp"
#include "tamperingdetector.hpp"
#include <ctime>
#include <assert.h>
#include <iostream>

#ifndef CONFIG_FILE_PATH
#define CONFIG_FILE_PATH "./config.ini"
#endif

Sentry::Sentry()
{
	source = NULL;
	config = new Config( CONFIG_FILE_PATH );
	logger = new Logger( "log.txt" );

	level = ThreatLevel::ALL_CLEAR;

	relaxed_delay = 0;
	detector = new TamperingDifferenceDetector( this );

	// B G R
	alertColor = cv::Scalar(0, 0, 255);
	warningColor = cv::Scalar(0, 255, 255);
	clearColor = cv::Scalar(0, 0, 0);

	cv::namedWindow("Icarus");
}

Sentry::Sentry( const int camera_index) : Sentry()
{
	source = new Video( camera_index );
	relaxed_delay = (1.0f / (float)source->getFPS());
	initDetectionRegion();
	initVideoLogger();
}

Sentry::Sentry( const char* filename ) : Sentry()
{
	source = new Video( filename );
	relaxed_delay = (1.0f / (float)source->getFPS());
	initDetectionRegion();
	initVideoLogger();
}

void Sentry::initDetectionRegion()
{
	detectionRegion = cv::Rect(cv::Point(stoi((*config)["detector.region_x1"]), stoi((*config)["detector.region_y1"])),
							   cv::Point(stoi((*config)["detector.region_x2"]), stoi((*config)["detector.region_y2"])));
	const cv::Size& fs = source->getFrameSize();
	if(detectionRegion.tl().x > fs.width || detectionRegion.tl().y > fs.height)
		throw std::runtime_error(std::string("icarus: detection region does not cover frame area!"));
	detectionRegion &= cv::Rect(cv::Point(0,0), source->getFrameSize());
	std::cout << "Detection region: " << detectionRegion << std::endl;
}

void Sentry::initVideoLogger()
{
	assert(source);

	if((*config)["logger.log_video"] == "true") video_logger = new VideoLogger(source->getFPS(), source->getFrameSize());
	else video_logger = NULL;
}

void Sentry::loop()
{
	detector->setup();
	ImageFrame frame;
	int wait = (int)(relaxed_delay * 1000.f);

	while(source->read(frame))
	{
		ImageFrame frame_region((frame.getMatrix())(detectionRegion));
		detector->process(frame_region);

		switch(level)
		{
			case ThreatLevel::ALL_CLEAR: frame.drawRectangle( detectionRegion, clearColor, 1); break;
			case ThreatLevel::WARNING:
				frame.drawRectangle( detectionRegion, warningColor, 1);
				if(video_logger) video_logger->log(frame);
				break;
			case ThreatLevel::ALERT:
				frame.drawRectangle( detectionRegion, alertColor, 1);
				if(video_logger) video_logger->log(frame);
				break;
		}

		cv::imshow("Icarus", frame.getMatrix() );

		if(wait > 0) cv::waitKey(wait);
	}
}

bool Sentry::provideFrame(ImageFrame& frame)
{
	if(source->read(frame))
	{
		frame.crop(detectionRegion);
		return true;
	}
	return false;
}

void Sentry::raise()
{
	switch(level)
	{
		case ThreatLevel::ALL_CLEAR:
			level = ThreatLevel::WARNING;
			if(video_logger) video_logger->start();
			break;
		case ThreatLevel::WARNING:
			level = ThreatLevel::ALERT;
			logger->log("Tampering detected!");
			if(video_logger)
			{
				video_logger->save();
				logger->log(std::string("Writing video output to ") + video_logger->getName());
			}
			break;
	}
}

void Sentry::lower()
{
	switch(level)
	{
		case ThreatLevel::WARNING:
			level = ThreatLevel::ALL_CLEAR;
			if(video_logger) video_logger->stop();
			break;
		case ThreatLevel::ALERT:
			level = ThreatLevel::WARNING;
			logger->log("Tampering stopped.");
			break;
	}
}

Sentry::~Sentry()
{
	delete source;
	delete config;
	delete logger;
	delete video_logger;
}