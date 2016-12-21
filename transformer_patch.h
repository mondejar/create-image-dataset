#ifndef TRANSFORMER_PATCH_H
#define TRANSFORMER_PATCH_H

#include <cv.h>
#include <highgui.h>

class Transformer_patch
{

public:
	void setInputPatch(cv::Mat);
	void blur();
	void non_uniform_light();
	void dynamic_range_compression();
	void affine_transformation();
	void dilate();

	cv::Mat getPatch();

private:
	cv::Mat _patch;
	cv::Size _sizePatch;
};

#endif