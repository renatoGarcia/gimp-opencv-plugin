#include "bilateral_filter.hpp"

#include <limits>
#include <memory>

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "enums.hpp"
#include "utility/conversions.hpp"
#include "utility/traits.hpp"

namespace
{

struct Options
{
    int d;
    double sigmaColor;
    double sigmaSpace;
    int borderType;
};

std::auto_ptr<Options> presentDialog()
{
    gimp_ui_init("opencv", FALSE);

    GimpDialog* dialog =
        GIMP_DIALOG(gimp_dialog_new("bilateralFilter",
                                    "bilateralFilter_role",
                                    NULL,
                                    GTK_DIALOG_MODAL,
                                    gimp_standard_help_func,
                                    "opencv_gimp",

                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                    NULL));

    GtkTable* gtkTable_1 = GTK_TABLE(gtk_table_new(4, 2, FALSE));
    //----- d
    GtkLabel* gtkLabel_1 = GTK_LABEL(gtk_label_new("d:"));
    gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkLabel_1), 0, 1, 0, 1);
    GtkSpinButton* gtkSpinButton_1 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<int>::min(),
                                                                                    SpinButtonTraits<int>::max(),
                                                                                    SpinButtonTraits<int>::step()));
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_1), 0);
    gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkSpinButton_1), 1, 2, 0, 1);
    //----- sigmaColor
    GtkLabel* gtkLabel_2 = GTK_LABEL(gtk_label_new("sigmaColor:"));
    gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_2), 0, 1, 1, 2);
    GtkSpinButton* gtkSpinButton_2 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<double>::min(),
                                                                                    SpinButtonTraits<double>::max(),
                                                                                    SpinButtonTraits<double>::step()));
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_2), 0);
    gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkSpinButton_2), 1, 2, 1, 2);
    //----- sigmaSpace
    GtkLabel* gtkLabel_3 = GTK_LABEL(gtk_label_new("sigmaSpace:"));
    gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_3), 0, 1, 2, 3);
    GtkSpinButton* gtkSpinButton_3 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<double>::min(),
                                                                                    SpinButtonTraits<double>::max(),
                                                                                    SpinButtonTraits<double>::step()));
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_3), 0);
    gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkSpinButton_3), 1, 2, 2, 3);
    //----- borderType
    GtkLabel* gtkLabel_4 = GTK_LABEL(gtk_label_new("borderType:"));
    gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_4), 0, 1, 3, 4);
    GimpEnumComboBox* gimpEnumComboBox_1 =
        GIMP_ENUM_COMBO_BOX(gimp_enum_combo_box_new(TYPE_BORDER_ENUM));
    gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(gimpEnumComboBox_1), cv::BORDER_DEFAULT);
    gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gimpEnumComboBox_1), 1, 2, 3, 4);


    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                      GTK_WIDGET(gtkTable_1));
    gtk_widget_show_all(GTK_WIDGET(dialog));

    std::auto_ptr<Options> options;
    if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        options.reset(new Options());

        options->d = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gtkSpinButton_1));
        options->sigmaColor = gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtkSpinButton_2));
        options->sigmaSpace = gtk_spin_button_get_value(GTK_SPIN_BUTTON(gtkSpinButton_3));
        gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(gimpEnumComboBox_1), &options->borderType);
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));

    return options;
}

} // namespace

void imgproc::bilateralFilter::install()
{
    static GimpParamDef args[] =
    {
        {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
        {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
        {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
    };

    gimp_install_procedure("bilateralFilter",
        "OpenCV color converter",
        "OpenCV color converter",
        "Renato Florentino Garcia",
        "Renato Florentino Garcia",
        "2013",
        "_bilateralFilter",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::bilateralFilter::run(GimpDrawable *drawable)
{
    std::auto_ptr<Options> options = presentDialog();
    if (options.get() == NULL)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::bilateralFilter(src, dst, options->d, options->sigmaColor, options->sigmaSpace, options->borderType);
    setMatToDrawable(dst, drawable);
}
