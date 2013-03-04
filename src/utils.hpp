#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <glib.h>
#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>

cv::Mat drawableToMat(GimpDrawable* drawable);

void setMatToDrawable(cv::Mat& mat, GimpDrawable* drawable);

#endif /* _UTILS_HPP_ */
