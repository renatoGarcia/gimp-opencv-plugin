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
#include "imgproc.hpp"

#include "imgproc/bilateral_filter.hpp"
#include "imgproc/blur.hpp"
#include "imgproc/box_filter.hpp"
#include "imgproc/build_pyramid.hpp"
#include "imgproc/cvt_color.hpp"

void imgproc::install()
{
    bilateralFilter::install();
    gimp_plugin_menu_register("bilateralFilter",
                              "<Image>/_OpenCV/_imgproc/");

    blur::install();
    gimp_plugin_menu_register("blur",
                              "<Image>/_OpenCV/_imgproc/");

    boxFilter::install();
    gimp_plugin_menu_register("boxFilter",
                              "<Image>/_OpenCV/_imgproc/");

    buildPyramid::install();
    gimp_plugin_menu_register("buildPyramid",
                              "<Image>/_OpenCV/_imgproc/");

    cvtColor::install();
    gimp_plugin_menu_register("cvtColor",
                              "<Image>/_OpenCV/_imgproc/");

}

void imgproc::registerNames(std::map<std::string, void(*)(GimpRunMode, gint32, gint32)>& runFunctions)
{
    bilateralFilter::registerName(runFunctions);
    blur::registerName(runFunctions);
    boxFilter::registerName(runFunctions);
    buildPyramid::registerName(runFunctions);
    cvtColor::registerName(runFunctions);
}
