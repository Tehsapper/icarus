#include "detector.hpp"

DifferenceDetector::DifferenceDetector(Sentry* ptr) : Detector(ptr)
{
	marked_frames_count = 0;
}

void DifferenceDetector::analyze(const ImageFrame& diff, ImageFrame& original)
{
	int nz = countNonZero(diff.getMatrix()), all = diff.getMatrix().total();
	float area = ((float)nz / (float)all) * 100.0f;
	
	if (area > area_threshold) markFrame(original);
	else unmarkFrame(original);

	#ifdef DEBUG
	cv::imshow("diff", diff.getMatrix());
 	printf("area: %f (%d/%d), marked frame count: %d out of %d [%d]\n", area, nz, all, marked_frames_count, marked_frames_threshold, watch_count);
 	#endif
}

void DifferenceDetector::markFrame(ImageFrame& img)
{
	if(marked_frames_count == 1 && parent->getLevel() == ThreatLevel::ALL_CLEAR) parent->raise();
	if(marked_frames_count < marked_frames_threshold) marked_frames_count++;
	if(marked_frames_count == marked_frames_threshold) parent->raise();
}

void DifferenceDetector::unmarkFrame(ImageFrame& img)
{
	if(marked_frames_count > watch_count) marked_frames_count = watch_count;
	if(marked_frames_count > 0) marked_frames_count--; else parent->lower();
}