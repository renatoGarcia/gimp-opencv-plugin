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
#ifndef _IMGPROC_ENUMS_HPP_
#define _IMGPROC_ENUMS_HPP_

#include <glib-object.h>

#define TYPE_BORDER_ENUM (BorderEnum_get_type())
GType BorderEnum_get_type();

#define TYPE_CVT_COLOR_ENUM (CvtColorEnum_get_type())
GType CvtColorEnum_get_type();

#endif /* _IMGPROC_ENUMS_HPP_ */
