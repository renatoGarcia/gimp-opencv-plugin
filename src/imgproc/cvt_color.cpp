#include "cvt_color.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cstring>
#include <memory>

#include "enums.hpp"
#include "utility/conversions.hpp"

namespace
{
struct Options
{
    int code;
};

std::auto_ptr<Options> cvtColor_dialog()
{
    gimp_ui_init("opencv", FALSE);

    GimpDialog* dialog =
        GIMP_DIALOG(gimp_dialog_new("cvtColor",
                                    "cvtColor_role",
                                    NULL,
                                    GTK_DIALOG_MODAL,
                                    gimp_standard_help_func,
                                    "opencv_gimp",

                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                    NULL));

    GtkHBox* gtkHBox_1 = GTK_HBOX(gtk_hbox_new(FALSE, 0));

    GimpEnumComboBox* gimpEnumComboBox_1 =
        GIMP_ENUM_COMBO_BOX(gimp_enum_combo_box_new(TYPE_CVT_COLOR_ENUM));
    gimp_int_combo_box_set_active(GIMP_INT_COMBO_BOX(gimpEnumComboBox_1), cv::COLOR_BGR2BGRA);

    GtkLabel* gtkLabel_1 = GTK_LABEL(gtk_label_new("code:"));

    gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkLabel_1));
    gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gimpEnumComboBox_1));
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                      GTK_WIDGET(gtkHBox_1));

    gtk_widget_show_all(GTK_WIDGET(dialog));

    std::auto_ptr<Options> options;
    if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
    {
        options.reset(new Options());

        gint value;
        gimp_int_combo_box_get_active(GIMP_INT_COMBO_BOX(gimpEnumComboBox_1), &value);
        options->code = value;
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));

    return options;
}

} // namespace

void imgproc::cvtColor::install()
{
    static GimpParamDef args[] =
    {
        {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
        {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
        {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
    };

    gimp_install_procedure("cvtColor",
        "OpenCV color converter",
        "OpenCV color converter",
        "Renato Florentino Garcia",
        "Renato Florentino Garcia",
        "2013",
        "_cvtColor",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::cvtColor::run(GimpDrawable *drawable)
{
    std::auto_ptr<Options> options = cvtColor_dialog();
    if (options.get() == NULL)
    {
        return;
    }
    cv::Mat cvImage = drawableToMat(drawable);
    cv::Mat tmp;
    cv::cvtColor(cvImage, tmp, options->code);
    setMatToDrawable(tmp, drawable);
}
