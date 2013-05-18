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
#ifndef _WIDGET_MAT_WIDGET_HPP_
#define _WIDGET_MAT_WIDGET_HPP_

#include <gtk/gtk.h>

#include <boost/lexical_cast.hpp>
#include <boost/multi_array.hpp>
#include <stack>
#include <string>

template <typename MatType>
class MatWidget
{
public:
    MatWidget()
        : gtkTable(GTK_TABLE(gtk_table_new(1, 1, FALSE)))
        , gtkHBox_rowButons(this->createButtons(G_CALLBACK(stAddRow), G_CALLBACK(stRemoveRow)))
        , gtkHBox_colButons(this->createButtons(G_CALLBACK(stAddColumn), G_CALLBACK(stRemoveColumn)))
        , matrix(boost::extents[0][0])
    {
        g_object_ref_sink(G_OBJECT(this->gtkTable));
        g_object_ref_sink(G_OBJECT(this->gtkHBox_rowButons));
        g_object_ref_sink(G_OBJECT(this->gtkHBox_colButons));

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         1, 2, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    ~MatWidget()
    {
        g_object_unref(G_OBJECT(this->gtkTable));
        g_object_unref(G_OBJECT(this->gtkHBox_rowButons));
        g_object_unref(G_OBJECT(this->gtkHBox_colButons));
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkTable);
    }

    operator typename cv::Mat_<MatType>()
    {
        return cv::Mat_<MatType>();
    }

private:
    typedef boost::multi_array<GtkSpinButton*, 2> MatrixType;

    GtkTable* const gtkTable;
    GtkHBox* const gtkHBox_rowButons;
    GtkHBox* const gtkHBox_colButons;
    std::stack<GtkLabel*> rowHeader;
    std::stack<GtkLabel*> columnHeader;
    MatrixType matrix;

    static void stAddRow(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->addRow();
    }
    void addRow()
    {
        MatrixType::size_type const nRows = this->matrix.shape()[0] + 1;
        MatrixType::size_type const nColumns = this->matrix.shape()[1];

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons));

        this->matrix.resize(boost::extents[nRows][nColumns]);
        gtk_table_resize(this->gtkTable, nRows + 2, nColumns + 2);

        GtkLabel* const gtkLabel = GTK_LABEL(gtk_label_new(boost::lexical_cast<std::string>(nRows - 1).c_str()));
        g_object_ref_sink(G_OBJECT(gtkLabel));
        this->rowHeader.push(gtkLabel);
        gtk_table_attach_defaults(this->gtkTable, GTK_WIDGET(gtkLabel), 0, 1, nRows, nRows + 1);

        for (MatrixType::size_type col = 0; col < nColumns; ++col)
        {
            GtkSpinButton* const gtkSpinButton = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(0, 10, 1));
            g_object_ref_sink(G_OBJECT(gtkSpinButton));

            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), GTK_WIDGET(gtkSpinButton),
                                      col + 1, col + 2, nRows, nRows + 1);
            this->matrix[nRows - 1][col] = gtkSpinButton;
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, nRows + 2, nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveRow(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeRow();
    }
    void removeRow()
    {
        MatrixType::size_type nRows = this->matrix.shape()[0];
        MatrixType::size_type const nColumns = this->matrix.shape()[1];
        if (nRows == 0) return;
        --nRows;

        for (MatrixType::size_type col = 0; col < nColumns; ++col)
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->matrix[nRows][col]));
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->rowHeader.top()));

        this->rowHeader.pop();
        this->matrix.resize(boost::extents[nRows][nColumns]);
        gtk_table_resize(this->gtkTable, nRows + 2, nColumns + 2);

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, nRows + 2, nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    static void stAddColumn(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->addColumn();
    }
    void addColumn()
    {
        MatrixType::size_type const nRows = this->matrix.shape()[0];
        MatrixType::size_type const nColumns = this->matrix.shape()[1] + 1;

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons));

        this->matrix.resize(boost::extents[nRows][nColumns]);
        gtk_table_resize(this->gtkTable, nRows + 2, nColumns + 2);

        GtkLabel* const gtkLabel = GTK_LABEL(gtk_label_new(boost::lexical_cast<std::string>(nColumns - 1).c_str()));
        g_object_ref_sink(G_OBJECT(gtkLabel));
        this->columnHeader.push(gtkLabel);
        gtk_table_attach_defaults(this->gtkTable, GTK_WIDGET(gtkLabel), nColumns, nColumns + 1, 0, 1);

        for (MatrixType::size_type row = 0; row < nRows; ++row)
        {
            GtkSpinButton* const gtkSpinButton = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(0, 10, 1));
            g_object_ref_sink(G_OBJECT(gtkSpinButton));

            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), GTK_WIDGET(gtkSpinButton),
                                      nColumns, nColumns + 1, row + 1, row + 2);
            this->matrix[row][nColumns - 1] = gtkSpinButton;
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         nColumns + 2, nColumns + 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveColumn(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeColumn();
    }
    void removeColumn()
    {
        MatrixType::size_type const nRows = this->matrix.shape()[0];
        MatrixType::size_type nColumns = this->matrix.shape()[1];
        if (nColumns == 0) return;
        --nColumns;

        for (MatrixType::size_type row = 0; row < nRows; ++row)
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->matrix[row][nColumns]));
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->columnHeader.top()));

        this->columnHeader.pop();
        this->matrix.resize(boost::extents[nRows][nColumns]);
        gtk_table_resize(this->gtkTable, nRows + 2, nColumns + 2);

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         nColumns + 2, nColumns + 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    GtkHBox* createButtons(GCallback addCallback, GCallback removeCallback)
    {
        GtkHBox* const gtkHBox = GTK_HBOX(gtk_hbox_new(TRUE, 3));

        GtkButton* const gtkButtonAdd = GTK_BUTTON(gtk_button_new());
        GtkImage* const gtkImageAdd = GTK_IMAGE(gtk_image_new_from_stock(GTK_STOCK_ADD, GTK_ICON_SIZE_SMALL_TOOLBAR));
        gtk_button_set_image(gtkButtonAdd, GTK_WIDGET(gtkImageAdd));

        GtkButton* const gtkButtonRemove = GTK_BUTTON(gtk_button_new());
        GtkImage* const gtkImageRemove = GTK_IMAGE(gtk_image_new_from_stock(GTK_STOCK_REMOVE, GTK_ICON_SIZE_SMALL_TOOLBAR));
        gtk_button_set_image(gtkButtonRemove, GTK_WIDGET(gtkImageRemove));

        g_signal_connect(gtkButtonAdd, "clicked", addCallback, this);
        g_signal_connect(gtkButtonRemove, "clicked", removeCallback, this);

        gtk_container_add(GTK_CONTAINER(gtkHBox), GTK_WIDGET(gtkButtonAdd));
        gtk_container_add(GTK_CONTAINER(gtkHBox), GTK_WIDGET(gtkButtonRemove));

        return gtkHBox;
    }
};

#endif /* _WIDGET_MAT_WIDGET_HPP_ */
