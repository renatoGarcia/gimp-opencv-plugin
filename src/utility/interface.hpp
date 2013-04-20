/* Gimp OpenCV Plugin
 *
 * Copyright (c) 2013 see AUTHORS file.
 *
 * Gimp OpenCV Plugin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Gimp OpenCV Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gimp OpenCV Plugin. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _UTILITY_INTERFACE_HPP_
#define _UTILITY_INTERFACE_HPP_

#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>

cv::Mat drawableToMat(GimpDrawable* drawable);

void setMatToDrawable(cv::Mat& mat, GimpDrawable* drawable);

#endif /* _UTILITY_INTERFACE_HPP_ */
