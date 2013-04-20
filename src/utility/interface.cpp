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
#include "interface.hpp"

cv::Mat drawableToMat(GimpDrawable* drawable)
{
    gint x1, y1, x2, y2;
    gimp_drawable_mask_bounds(drawable->drawable_id,
                              &x1, &y1,
                              &x2, &y2);

    GimpPixelRgn rgnIn;
    gimp_pixel_rgn_init(&rgnIn,
                        drawable,
                        x1, y1,
                        x2 - x1, y2 - y1,
                        FALSE, FALSE);

    cv::Mat mat(y2 - y1, x2 - x1, CV_8UC3);
    gimp_pixel_rgn_get_rect(&rgnIn,
                            mat.data,
                            x1, y1,
                            x2 - x1, y2 - y1);

    return mat;
}

void setMatToDrawable(cv::Mat& mat, GimpDrawable* drawable)
{
    gint x1, y1, x2, y2;
    gimp_drawable_mask_bounds(drawable->drawable_id,
                              &x1, &y1,
                              &x2, &y2);

    GimpPixelRgn rgn;
    gimp_pixel_rgn_init(&rgn,
                        drawable,
                        x1, y1,
                        x2 - x1, y2 - y1,
                        TRUE, TRUE);

    gimp_pixel_rgn_set_rect(&rgn,
                            mat.data,
                            x1, y1,
                            x2 - x1, y2 - y1);

    gimp_drawable_flush(drawable);
    gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
    gimp_drawable_update(drawable->drawable_id,
                         x1, y1,
                         x2 - x1, y2 - y1);
}
