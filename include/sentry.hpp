#pragma once

#ifndef __cplusplus
#  error sentry.hpp is a C++ header
#endif

#include "video.hpp"
#include "config.hpp"
#include "logger.hpp"

enum class ThreatLevel { ALL_CLEAR, WARNING, ALERT };

class Detector;

class Sentry
{
	protected:
		Video* source;
		VideoLogger* video_logger;
		Detector* detector;
		Config* config;
		Logger* logger;
		ThreatLevel level;
		float relaxed_delay;
		cv::Rect detectionRegion;
		cv::Scalar alertColor, clearColor, warningColor;
	public:
		Sentry();
		Sentry( const int camera_index );
		Sentry( const char* filename );

		void loop();
		bool provideFrame(ImageFrame& frame);
		void initVideoLogger();
		void initDetectionRegion();

		void raise();
		void lower();
		
		const Video& getVideo() const { return *source; }
		Config& getConfig() const { return *config; }
		const ThreatLevel getLevel() const { return level; }
		const cv::Rect& getDetectionRegion() const { return detectionRegion; }

		virtual ~Sentry();
};