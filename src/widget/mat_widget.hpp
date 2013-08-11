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
#include "utility/interface.hpp"
#include "utility/layers.hpp"

#include <gtk/gtk.h>

#include <boost/container/list.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>
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
    : private boost::noncopyable
{
private:
    typedef MatType ElementType;
    typedef typename mat_widget_detail::SelectWidget<ElementType>::type ElementWidget;
    // Boost multi_array doesn't work with movable-only types, neither have an emplace inserter...
    typedef boost::container::list<boost::container::list<ElementWidget> > MatrixType;

public:
    explicit MatWidget(ElementType const& defaultElement)
        : gtkTable(GTK_TABLE(gtk_table_new(1, 1, FALSE)))
        , gtkHBox_rowButons(this->createButtons(G_CALLBACK(stAddRow), G_CALLBACK(stRemoveRow)))
        , gtkHBox_colButons(this->createButtons(G_CALLBACK(stAddColumn), G_CALLBACK(stRemoveColumn)))
        , rowHeader()
        , columnHeader()
        , matrix()
        , nRows(0)
        , nColumns(0)
        , defaultElement(defaultElement)
    {
        g_object_ref_sink(G_OBJECT(this->gtkTable));
        g_object_ref_sink(G_OBJECT(this->gtkHBox_rowButons));
        g_object_ref_sink(G_OBJECT(this->gtkHBox_colButons));

        //-----------------------------------------------------------
        GtkButton* const gtkButtonLoad = GTK_BUTTON(gtk_button_new());
        GtkImage* const gtkImageLoad = GTK_IMAGE(gtk_image_new_from_stock(GTK_STOCK_INDEX, GTK_ICON_SIZE_SMALL_TOOLBAR));
        gtk_button_set_image(gtkButtonLoad, GTK_WIDGET(gtkImageLoad));

        g_signal_connect(gtkButtonLoad, "clicked", G_CALLBACK(stLoadMat), this);
        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(gtkButtonLoad),
                         0, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
        //----------------------------------------------------------------

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, 1, 2, GTK_SHRINK, GTK_SHRINK, 0, 0);
        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         1, 2, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    ~MatWidget()
    {
        while (!this->rowHeader.empty())
        {
            g_object_unref(G_OBJECT(this->rowHeader.top()));
            this->rowHeader.pop();
        }

        while (!this->columnHeader.empty())
        {
            g_object_unref(G_OBJECT(this->columnHeader.top()));
            this->columnHeader.pop();
        }

        g_object_unref(G_OBJECT(this->gtkTable));
        g_object_unref(G_OBJECT(this->gtkHBox_rowButons));
        g_object_unref(G_OBJECT(this->gtkHBox_colButons));
    }

    template <typename T>
    MatWidget& operator=(cv::Mat_<T> const& value)
    {
        while (this->nRows > value.rows)
            this->removeRow();

        while (this->nRows < value.rows)
            this->addRow();

        while (this->nColumns > value.cols)
            this->removeColumn();

        while (this->nColumns < value.cols)
            this->addColumn();

        int iRow = 0;
        for (typename MatrixType::iterator rowIt = this->matrix.begin();
             rowIt != this->matrix.end();
             ++rowIt, ++iRow)
        {
            int iColumn = 0;
            for (typename MatrixType::value_type::iterator elementIt = rowIt->begin();
                 elementIt != rowIt->end();
                 ++elementIt, ++iColumn)
            {
                *elementIt = value(iRow, iColumn);
            }
        }

        return *this;
    }

    operator GtkWidget*() const
    {
        return GTK_WIDGET(this->gtkTable);
    }

    operator typename cv::Mat_<MatType>() const
    {
        cv::Mat_<MatType> mat(this->nRows, this->nColumns);

        int iRow = 0;
        for (typename MatrixType::const_iterator rowIt = this->matrix.begin();
            rowIt != this->matrix.end();
             ++rowIt, ++iRow)
        {
            int iColumn = 0;
            for (typename MatrixType::value_type::const_iterator elementIt = rowIt->begin();
                 elementIt != rowIt->end();
                 ++elementIt, ++iColumn)
            {
                mat(iRow, iColumn) = *elementIt;
            }
        }

        return mat;
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

    static void stLoadMat(GtkWidget* /*widget*/, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->loadMat();
    }
    void loadMat()
    {
        // boost::optional<int> selectedLayer = Layers::selectLayer();
        // if (selectedLayer)
        // {
        //     cv::Mat_<MatType> image = drawableToMat(gimp_drawable_get(*selectedLayer));
        cv::Mat_<MatType> image(3, 3);
        *this = image;
        // }
    }

    static void stAddRow(GtkWidget* /*widget*/, gpointer data)
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

        this->matrix.push_back(boost::container::list<ElementWidget>());
        for (typename MatrixType::size_type col = 0; col < this->nColumns; ++col)
        {
            this->matrix.back().emplace_back(this->defaultElement);
            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), (GtkWidget*)this->matrix.back().back(),
                                      col + 1, col + 2, this->nRows, this->nRows + 1);
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, this->nRows + 2, this->nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveRow(GtkWidget* /*widget*/, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeRow();
    }
    void removeRow()
    {
        if (this->nRows == 0) return;
        --this->nRows;

        BOOST_FOREACH(typename MatrixType::value_type::value_type const& element, this->matrix.back())
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), (GtkWidget*)element);
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->rowHeader.top()));
        g_object_unref(G_OBJECT(this->rowHeader.top()));

        this->rowHeader.pop();
        this->matrix.pop_back();
        gtk_table_resize(this->gtkTable, this->nRows + 2, this->nColumns + 2);

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_rowButons),
                         0, 1, this->nRows + 2, this->nRows + 3, GTK_SHRINK, GTK_SHRINK, 0, 0);
    }

    static void stAddColumn(GtkWidget* /*widget*/, gpointer data)
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

        int iRow = 0;
        BOOST_FOREACH(typename MatrixType::value_type& row, this->matrix)
        {
            row.emplace_back(this->defaultElement);
            gtk_table_attach_defaults(GTK_TABLE(this->gtkTable), (GtkWidget*)row.back(),
                                      this->nColumns, this->nColumns + 1, iRow + 1, iRow + 2);
            ++iRow;
        }

        gtk_table_attach(GTK_TABLE(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons),
                         this->nColumns + 2, this->nColumns + 3, 0, 1, GTK_SHRINK, GTK_SHRINK, 0, 0);

        gtk_widget_show_all(GTK_WIDGET(this->gtkTable));
    }

    static void stRemoveColumn(GtkWidget* /*widget*/, gpointer data)
    {
        MatWidget* self = reinterpret_cast<MatWidget*>(data);
        self->removeColumn();
    }
    void removeColumn()
    {
        if (this->nColumns == 0) return;
        --this->nColumns;

        BOOST_FOREACH(typename MatrixType::value_type& row, this->matrix)
        {
            gtk_container_remove(GTK_CONTAINER(this->gtkTable), (GtkWidget*)row.back());
            row.pop_back();
        }

        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->gtkHBox_colButons));
        gtk_container_remove(GTK_CONTAINER(this->gtkTable), GTK_WIDGET(this->columnHeader.top()));
        g_object_unref(G_OBJECT(this->columnHeader.top()));

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
