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
#ifndef _IMGPROC_DILATE_HPP_
#define _IMGPROC_DILATE_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    namespace dilate
    {
        void install();

        void registerName(std::map<std::string, void(*)(GimpRunMode, gint32, gint32)>& runFunctions);

        void run(GimpRunMode, gint32, gint32 drawableId);
    }
}


#endif /* _IMGPROC_DILATE_HPP_ */
