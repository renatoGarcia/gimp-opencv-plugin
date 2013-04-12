#ifndef _WIDGET_ENUM_WIDGET_HPP_
#define _WIDGET_ENUM_WIDGET_HPP_

#include <glib-object.h>
#include <libgimp/gimpui.h>

class EnumWidget
{
public:
    EnumWidget(GType enumType, gint defaultItem)
        : gimpEnumComboBox(GIMP_ENUM_COMBO_BOX(gimp_enum_combo_box_new(enumType)))
    {
        gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(this->gimpEnumComboBox), defaultItem);
    }

    operator GtkWidget*()
    {
        return GTK_WIDGET(this->gimpEnumComboBox);
    }

    operator int() const
    {
        gint value;
        gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(this->gimpEnumComboBox), &value);

        return value;
    }

private:
    GimpEnumComboBox* gimpEnumComboBox;
};

#endif /* _WIDGET_ENUM_WIDGET_HPP_ */
