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
#ifndef _UTILITY_LAYERS_IPP_
#define _UTILITY_LAYERS_IPP_

#include <libgimp/gimp.h>
#include <opencv2/core/core.hpp>

#include <boost/foreach.hpp>
#include <cstddef>
#include <iostream>

template <typename SinglePassRange>
void Layers::insertLayersGroup(std::string const& layerGroupName, SinglePassRange const& range)
{
    gint32 const layerGroupId = gimp_layer_group_new(Layers::imageId);
    if (!gimp_item_set_name(layerGroupId, layerGroupName.c_str()))
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to set layer group name\n";
        return;
    }

    gint nImageLayers;
    if (gimp_image_get_layers(Layers::imageId, &nImageLayers) == NULL)
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to get number of image layers\n";
        return;
    }

    if (!gimp_image_insert_layer(Layers::imageId, layerGroupId, 0, nImageLayers))
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to insert layer group on image\n";
        return;
    }

    int counter = 0;
    BOOST_FOREACH (cv::Mat const& mat, range)
    {
        if (mat.type() != CV_8UC3)
        {
            // TODO: Handle error properly
            std::cerr << "opencv-plugin: cv::Mat's to insert as layer must be CV_8UC3\n";
            return;
        }

        GdkPixbuf* const tmpPixbuf =
            GDK_PIXBUF(gdk_pixbuf_new_from_data(mat.data,
                                                GDK_COLORSPACE_RGB,
                                                FALSE,
                                                8,
                                                mat.cols,
                                                mat.rows,
                                                mat.step.p[0],
                                                NULL,
                                                NULL));

        GdkPixbuf* const pixbuf = gdk_pixbuf_copy(tmpPixbuf);

        gint32 const layerId = gimp_layer_new_from_pixbuf(Layers::imageId, "", pixbuf, 100.0, GIMP_NORMAL_MODE, 1.0, 1.0);

        if (!gimp_image_insert_layer(Layers::imageId, layerId, layerGroupId, counter++))
        {
            // TODO: Handle error properly
            std::cerr << "opencv-plugin: Failed to insert layer on image\n";
            return;
        }

        g_object_unref(tmpPixbuf);
        g_object_unref(pixbuf);
    }
}

#endif /* _UTILITY_LAYERS_IPP_ */
