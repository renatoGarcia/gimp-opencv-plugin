#ifndef _UTILITY_CONVERSIONS_HPP_
#define _UTILITY_CONVERSIONS_HPP_

#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>

cv::Mat drawableToMat(GimpDrawable* drawable);

void setMatToDrawable(cv::Mat& mat, GimpDrawable* drawable);

#endif /* _UTILITY_CONVERSIONS_HPP_ */
