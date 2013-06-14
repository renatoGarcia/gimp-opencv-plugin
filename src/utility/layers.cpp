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
#include "layers.hpp"

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

gint32 Layers::imageId = -1;

namespace
{
    std::string pre(int n)
    {
        std::string result;
        for (int i = 0; i < n; ++i)
        {
            result += "\t";
        }

        return result;
    }

    void vai(gint32 itemId, int level)
    {
        if (!gimp_item_is_layer(itemId))
            std::cout << "Não é layer!!" << std::endl;

        std::cout << pre(level) << "- " << gimp_item_get_name(itemId) << std::endl;

        if (gimp_item_is_group(itemId))
        {
            gint nLayers;
            gint* layers = gimp_item_get_children(itemId, &nLayers);
            for (gint i = 0; i < nLayers; ++i)
            {
                vai(layers[i], level + 1);
            }

        }
    }
}

void Layers::setCurrentImageId(gint32 imageId)
{
    Layers::imageId = imageId;
}

void Layers::selectLayer()
{
    gint nLayers;
    gint* layers = gimp_image_get_layers(Layers::imageId, &nLayers);
    if (layers == NULL)
    {
        // TODO: Handle error properly
        std::cerr << "opencv-plugin: Failed to get number of image layers\n";
        return;
    }

    for (gint i = 0; i < nLayers; ++i)
    {
        vai(layers[i], 0);
    }

    GimpDialog* gimpDialog =
        GIMP_DIALOG(gimp_dialog_new("dialog",
                                    "dialog_role",
                                    NULL,
                                    GTK_DIALOG_MODAL,
                                    gimp_standard_help_func,
                                    "opencv_gimp",

                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                    NULL));

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(gimpDialog))),
                      GTK_WIDGET(gtk_label_new("oi")));
    gtk_widget_show_all(GTK_WIDGET(gimpDialog));

    if (gimp_dialog_run(gimpDialog) == GTK_RESPONSE_OK)
    {
    }

    gtk_widget_destroy(GTK_WIDGET(gimpDialog));
}
