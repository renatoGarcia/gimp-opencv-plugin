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

#include "numeric_widget.hpp"

#include <gtk/gtk.h>

#include <boost/container/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <stack>
#include <string>

namespace mat_widget_detail
{
    template <typename MatType>
    struct SelectWidget
    {};

    template <>
    struct SelectWidget<double>
    {
        typedef NumericWidget<double> type;
    };
}

template <typename MatType>
class MatWidget
{
private:
    typedef MatType ElementType;
    typedef typename mat_widget_detail::SelectWidget<ElementType>::type ElementWidget;
    // Boost multi_array doesn't work with movable-only types...
    typedef boost::container::vector<boost::container::vector<ElementWidget> > MatrixType;

public:
    explicit MatWidget(ElementType const& defaultElement)
        : gtkTable(GTK_TABLE(gtk_table_new(1, 1, FALSE)))
        , gtkHBox_rowButons(this->createButtons(G_CALLBACK(stAddRow), G_CALLBACK(stRemoveRow)))
        , gtkHBox_colButons(this->createButtons(G_CALLBACK(stAddColumn), G_CALLBACK(stRemoveColumn)))
        , matrix()
        , nRows(0)
        , nColumns(0)
        , defaultElement(defaultElement)
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
    GtkTable* const gtkTable;
    GtkHBox* const gtkHBox_rowButons;
    GtkHBox* const gtkHBox_colButons;
    std::stack<GtkLabel*> rowHeader;
    std::stack<GtkLabel*> columnHeader;
    MatrixType matrix;
    typename MatrixType::size_type nRows;
    typename MatrixType::size_type nColumns;
    ElementType const defaultElement;

    static void stAddRow(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->addRow();
    }
    void addRow()
    {
        ++this->nRows;

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons));

        gtk_table_resize(this->gtkTable, this->nRows + 2, this->nColumns + 2);

        GtkLabel* const gtkLabel = GTK_LABEL(gtk_label_new(boost::lexical_cast<std::string>(this->nRows - 1).c_str()));
        g_object_ref_sink(G_OBJECT(gtkLabel));
        this->rowHeader.push(gtkLabel);
        gtk_table_attach_defaults(this->gtkTable, GTK_WIDGET(gtkLabel), 0, 1, this->nRows, this->nRows + 1);

        this->matrix.push_back(boost::container::vector<ElementWidget>());
        for (typename MatrixType::size_type col = 0; col < this->nColumns; ++col)
        {
            ElementWidget elementWidget(this->defaultElement);
            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), (GtkWidget*)elementWidget,
                                      col + 1, col + 2, this->nRows, this->nRows + 1);
            this->matrix.back().push_back(boost::move(elementWidget));
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, this->nRows + 2, this->nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveRow(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeRow();
    }
    void removeRow()
    {
        if (this->nRows == 0) return;
        --this->nRows;

        for (typename MatrixType::size_type col = 0; col < this->nColumns; ++col)
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), (GtkWidget*)this->matrix.back().at(col));
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->rowHeader.top()));

        this->rowHeader.pop();
        this->matrix.pop_back();
        gtk_table_resize(this->gtkTable, this->nRows + 2, this->nColumns + 2);

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, this->nRows + 2, this->nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    static void stAddColumn(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->addColumn();
    }
    void addColumn()
    {
        ++this->nColumns;

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons));

        gtk_table_resize(this->gtkTable, this->nRows + 2, this->nColumns + 2);

        GtkLabel* const gtkLabel = GTK_LABEL(gtk_label_new(boost::lexical_cast<std::string>(this->nColumns - 1).c_str()));
        g_object_ref_sink(G_OBJECT(gtkLabel));
        this->columnHeader.push(gtkLabel);
        gtk_table_attach_defaults(this->gtkTable, GTK_WIDGET(gtkLabel), this->nColumns, this->nColumns + 1, 0, 1);

        for (typename MatrixType::size_type row = 0; row < this->nRows; ++row)
        {
            ElementWidget elementWidget(this->defaultElement);
            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), (GtkWidget*)elementWidget,
                                      this->nColumns, this->nColumns + 1, row + 1, row + 2);

            this->matrix.at(row).push_back(boost::move(elementWidget));
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         this->nColumns + 2, this->nColumns + 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveColumn(GtkWidget* widget, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeColumn();
    }
    void removeColumn()
    {
        if (this->nColumns == 0) return;
        --this->nColumns;

        for (typename MatrixType::size_type row = 0; row < nRows; ++row)
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), (GtkWidget*)this->matrix.at(row).back());
            this->matrix.at(row).pop_back();
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->columnHeader.top()));

        this->columnHeader.pop();
        gtk_table_resize(this->gtkTable, this->nRows + 2, this->nColumns + 2);

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         this->nColumns + 2, this->nColumns + 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
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
