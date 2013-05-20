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
#ifndef _WIDGET_TUPLE_WIDGET_HPP_
#define _WIDGET_TUPLE_WIDGET_HPP_

#include "utility/interface.hpp"
#include "utility/traits.hpp"

#include <gtk/gtk.h>

#include <boost/array.hpp>
#include <stdexcept>

namespace tuple_widget_detail
{
    template <typename T>
    T const& get(cv::Point_<T> const& point, int index)
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("");

        return index == 0 ? point.x : point.y;
    }

    template <typename T>
    T const& get(cv::Size_<T> const& size, int index)
    {
        if (index < 0 || index > 1)
            throw std::out_of_range("");

        return index == 0 ? size.width : size.height;
    }

    template <typename T, int N>
    T const& get(cv::Vec<T, N> const& vec, int index)
    {
        if (index < 0 || index >= N)
            throw std::out_of_range("");

        return vec[index];
    }
}

template <typename TupleType>
class TupleWidget
{
    static int const LENGTH = cv::DataType<TupleType>::channels;
    typedef typename cv::DataType<TupleType>::channel_type ElementType;

public:
    TupleWidget(TupleType const& defaultTuple)
        : gtkHBox(GTK_HBOX(gtk_hbox_new(FALSE, 0)))
        , gtkSpinButtons()
    {
        this->addLabel("(");
        for (int i = 0; i < TupleWidget::LENGTH; ++i)
        {
            this->gtkSpinButtons.at(i) =
                GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<ElementType>::min(),
                                                               SpinButtonTraits<ElementType>::max(),
                                                               SpinButtonTraits<ElementType>::step()));
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButtons.at(i)),
                                      tuple_widget_detail::get(defaultTuple, i));
            gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(this->gtkSpinButtons.at(i)));

            if (i + 1 != TupleWidget::LENGTH)
                this->addLabel(", ");
        }
        this->addLabel(")");
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkHBox);
    }

    operator TupleType()
    {
        return toCvTuple<TupleType>()(this->gtkSpinButtons);
    }

private:
    GtkHBox* gtkHBox;
    boost::array<GtkSpinButton*, TupleWidget::LENGTH> gtkSpinButtons;

    void addLabel(char const* const label)
    {
        GtkLabel* gtkLabel = GTK_LABEL(gtk_label_new(label));
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(gtkLabel));
    }
};


#endif /* _WIDGET_TUPLE_WIDGET_HPP_ */
