#ifndef _INTERFACE_IPP_
#define _INTERFACE_IPP_

#include <gdk-pixbuf/gdk-pixbuf.h>

#include <boost/foreach.hpp>
#include <cstddef>
#include <iostream>

template <typename SinglePassRange>
void insertLayersGroup(gint32 imageId, std::string const& layerGroupName, SinglePassRange const& range)
{
    gint32 const layerGroupId = gimp_layer_group_new(imageId);
    if (!gimp_item_set_name(layerGroupId, layerGroupName.c_str()))
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to set layer group name\n";
        return;
    }

    gint nImageLayers;
    if (gimp_image_get_layers(imageId, &nImageLayers) == NULL)
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to get number of image layers\n";
        return;
    }

    if (!gimp_image_insert_layer(imageId, layerGroupId, 0, nImageLayers))
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

        gint32 const layerId = gimp_layer_new_from_pixbuf(imageId, "", pixbuf, 100.0, GIMP_NORMAL_MODE, 1.0, 1.0);

        if (!gimp_image_insert_layer(imageId, layerId, layerGroupId, counter++))
        {
            // TODO: Handle error properly
            std::cerr << "opencv-plugin: Failed to insert layer on image\n";
            return;
        }

        g_object_unref(tmpPixbuf);
        g_object_unref(pixbuf);
    }
}

template <typename T>
struct toCvTuple<cv::Size_<T> >
{
    cv::Size_<T> operator()(boost::array<GtkSpinButton*, 2> gtkSpinButtons)
    {
        return cv::Size_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                            gtk_spin_button_get_value(gtkSpinButtons.at(1)));
    }
};

template <typename T>
struct toCvTuple<cv::Point_<T> >
{
    cv::Point_<T> operator()(boost::array<GtkSpinButton*, 2> gtkSpinButtons)
    {
        return cv::Point_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                             gtk_spin_button_get_value(gtkSpinButtons.at(1)));
    }
};

template <typename T>
struct toCvTuple<cv::Scalar_<T> >
{
    cv::Scalar_<T> operator()(boost::array<GtkSpinButton*, 4> gtkSpinButtons)
    {
        return cv::Scalar_<T>(gtk_spin_button_get_value(gtkSpinButtons.at(0)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(1)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(2)),
                              gtk_spin_button_get_value(gtkSpinButtons.at(3)));
    }
};

#endif /* _INTERFACE_IPP_ */
