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
#include "exception.hpp"

#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

gint32 Layers::imageId = -1;

void Layers::populateTree(gint32 layerId, GtkTreeStore* store, GtkTreeIter* parent)
{
    if (!gimp_item_is_layer(layerId))
    {
        throw std::runtime_error("Gimp item is not a layer.");
    }

    GtkTreeIter iterator;
    gtk_tree_store_append(store, &iterator, parent);
    gtk_tree_store_set(store, &iterator,
                       Layers::ID_COLUMN, layerId,
                       Layers::IMAGE_COLUMN, gimp_drawable_get_thumbnail(layerId, 35, 25, GIMP_PIXBUF_KEEP_ALPHA),
                       Layers::NAME_COLUMN, gimp_item_get_name(layerId),
                       -1);

    if (gimp_item_is_group(layerId))
    {
        gint nLayers;
        gint* layers = gimp_item_get_children(layerId, &nLayers);
        for (gint i = 0; i < nLayers; ++i)
        {
            Layers::populateTree(layers[i], store, &iterator);
        }
    }
}

GtkTreeView* Layers::buildTreeView(GtkTreeStore* store)
{
    GtkTreeView* tree;
    GtkTreeViewColumn* column;
    GtkCellRenderer* renderer;

    tree = GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(store)));
    gtk_tree_view_set_headers_visible(tree, FALSE);
    gtk_tree_view_set_enable_tree_lines(tree, TRUE);

    renderer = gtk_cell_renderer_pixbuf_new();
    column = gtk_tree_view_column_new();
    gtk_tree_view_column_pack_start(column, renderer, FALSE);
    gtk_tree_view_column_set_attributes(column, renderer,
                                        "pixbuf", Layers::IMAGE_COLUMN,
                                        NULL);
    gtk_tree_view_append_column(tree, column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new();
    gtk_tree_view_column_pack_start(column, renderer, TRUE);
    gtk_tree_view_column_set_attributes(column, renderer,
                                        "text", Layers::NAME_COLUMN,
                                        NULL);
    gtk_tree_view_append_column(tree, column);

    return tree;
}

void Layers::setCurrentImageId(gint32 imageId)
{
    Layers::imageId = imageId;
}

boost::optional<int> Layers::selectLayer()
{
    gint nLayers;
    gint* layers = gimp_image_get_layers(Layers::imageId, &nLayers);
    if (layers == NULL)
    {
        throw std::runtime_error("Cannot get layers list on current image.");
    }

    GtkTreeStore* const store =
        GTK_TREE_STORE(gtk_tree_store_new(Layers::N_COLUMNS,
                                          G_TYPE_INT,
                                          GDK_TYPE_PIXBUF,
                                          G_TYPE_STRING));

    for (gint i = 0; i < nLayers; ++i)
    {
        Layers::populateTree(layers[i], store, NULL);
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

    GtkTreeView* tree = Layers::buildTreeView(store);

    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(gimpDialog))),
                      GTK_WIDGET(tree));

    gtk_widget_show_all(GTK_WIDGET(gimpDialog));

    boost::optional<int> layerId;
    if (gimp_dialog_run(gimpDialog) == GTK_RESPONSE_OK)
    {
        GtkTreeSelection* selection = gtk_tree_view_get_selection(tree);
        GtkTreeIter iter;
        GtkTreeModel* model;
        gint32 tmpId;

        if (gtk_tree_selection_get_selected(selection, &model, &iter))
        {
            gtk_tree_model_get(model, &iter, Layers::ID_COLUMN, &tmpId, -1);
            layerId = tmpId;
        }
    }

    g_object_unref(G_OBJECT(store));
    g_object_unref(G_OBJECT(tree));
    gtk_widget_destroy(GTK_WIDGET(gimpDialog));

    return layerId;
}
