#ifndef _WIDGET_PAIR_WIDGET_HPP_
#define _WIDGET_PAIR_WIDGET_HPP_

#include "utility/conversions.hpp"
#include "utility/meta.hpp"
#include "utility/traits.hpp"

#include <gtk/gtk.h>

template <typename PairType>
class PairWidget
{
    typedef typename element<0, PairType>::type Element0;
    typedef typename element<1, PairType>::type Element1;

public:
    PairWidget(Element0 const& element0, Element1 const& element1)
        : gtkHBox(GTK_HBOX(gtk_hbox_new(FALSE, 0)))
        , gtkSpinButton_0(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<Element0>::min(),
                                                                         SpinButtonTraits<Element0>::max(),
                                                                         SpinButtonTraits<Element0>::step())))
        , gtkSpinButton_1(GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<Element1>::min(),
                                                                         SpinButtonTraits<Element1>::max(),
                                                                         SpinButtonTraits<Element1>::step())))
    {
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButton_0), element0);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(this->gtkSpinButton_1), element1);

        this->addLabel("(");
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(this->gtkSpinButton_0));
        this->addLabel(", ");
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(this->gtkSpinButton_1));
        this->addLabel(")");
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gtkHBox);
    }

    operator PairType()
    {
        return PairType(gtk_spin_button_get_value(this->gtkSpinButton_0),
                        gtk_spin_button_get_value(this->gtkSpinButton_1));
    }

private:
    GtkHBox* gtkHBox;
    GtkSpinButton* gtkSpinButton_0;
    GtkSpinButton* gtkSpinButton_1;

    void addLabel(char const* const label)
    {
        GtkLabel* gtkLabel = GTK_LABEL(gtk_label_new(label));
        gtk_container_add(GTK_CONTAINER(this->gtkHBox), GTK_WIDGET(gtkLabel));
    }
};


#endif /* _WIDGET_PAIR_WIDGET_HPP_ */
