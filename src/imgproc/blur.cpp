#include "blur.hpp"

#include <memory>

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include "../utils.hpp"

namespace
{
    struct Options
    {
        cv::Size ksize;
        cv::Point anchor;
        int borderType;
    };

    std::auto_ptr<Options> presentDialog()
    {
        gimp_ui_init("opencv", FALSE);

        GimpDialog* dialog =
            GIMP_DIALOG(gimp_dialog_new("blur",
                                        "blur_role",
                                        NULL,
                                        GTK_DIALOG_MODAL,
                                        gimp_standard_help_func,
                                        "opencv_gimp",

                                        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                        GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                        NULL));

        GtkTable* gtkTable_1 = GTK_TABLE(gtk_table_new(3, 2, FALSE));
        //----- ksize
        GtkLabel* gtkLabel_1 = GTK_LABEL(gtk_label_new("ksize:"));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkLabel_1), 0, 1, 0, 1);
        GtkHBox* gtkHBox_1 = GTK_HBOX(gtk_hbox_new(FALSE, 0));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkHBox_1), 1, 2, 0, 1);
        GtkLabel* gtkLabel_2 = GTK_LABEL(gtk_label_new("("));
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkLabel_2));
        GtkSpinButton* gtkSpinButton_1 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<int>::min(),
                                                                                        SpinButtonTraits<int>::max(),
                                                                                        SpinButtonTraits<int>::step()));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_1), 0);
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkSpinButton_1));
        GtkLabel* gtkLabel_3 = GTK_LABEL(gtk_label_new(", "));
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkLabel_3));
        GtkSpinButton* gtkSpinButton_2 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<int>::min(),
                                                                                        SpinButtonTraits<int>::max(),
                                                                                        SpinButtonTraits<int>::step()));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_2), 0);
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkSpinButton_2));
        GtkLabel* gtkLabel_4 = GTK_LABEL(gtk_label_new(")"));
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkLabel_4));
        //----- anchor
        GtkLabel* gtkLabel_5 = GTK_LABEL(gtk_label_new("anchor:"));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkLabel_5), 0, 1, 1, 2);
        GtkHBox* gtkHBox_2 = GTK_HBOX(gtk_hbox_new(FALSE, 0));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkHBox_2), 1, 2, 1, 2);
        GtkLabel* gtkLabel_6 = GTK_LABEL(gtk_label_new("("));
        gtk_container_add(GTK_CONTAINER(gtkHBox_2), GTK_WIDGET(gtkLabel_6));
        GtkSpinButton* gtkSpinButton_3 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<int>::min(),
                                                                                        SpinButtonTraits<int>::max(),
                                                                                        SpinButtonTraits<int>::step()));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_3), 0);
        gtk_container_add(GTK_CONTAINER(gtkHBox_2), GTK_WIDGET(gtkSpinButton_3));
        GtkLabel* gtkLabel_7 = GTK_LABEL(gtk_label_new(", "));
        gtk_container_add(GTK_CONTAINER(gtkHBox_2), GTK_WIDGET(gtkLabel_7));
        GtkSpinButton* gtkSpinButton_4 = GTK_SPIN_BUTTON(gtk_spin_button_new_with_range(SpinButtonTraits<int>::min(),
                                                                                        SpinButtonTraits<int>::max(),
                                                                                        SpinButtonTraits<int>::step()));
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(gtkSpinButton_4), 0);
        gtk_container_add(GTK_CONTAINER(gtkHBox_2), GTK_WIDGET(gtkSpinButton_4));
        GtkLabel* gtkLabel_8 = GTK_LABEL(gtk_label_new(")"));
        gtk_container_add(GTK_CONTAINER(gtkHBox_2), GTK_WIDGET(gtkLabel_8));



        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(gtkTable_1));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        std::auto_ptr<Options> options;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            options.reset(new Options());
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return options;
    }

} // namespace

void imgproc::blur::install()
{
    static GimpParamDef args[] =
    {
        {GIMP_PDB_INT32,    (gchar*)"run-mode", (gchar*)"Run mode"},
        {GIMP_PDB_IMAGE,    (gchar*)"image",    (gchar*)"Input image"},
        {GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable"}
    };

    gimp_install_procedure("blur",
        "Blurs an image using the normalized box filter",
        "Blurs an image using the normalized box filter",
        "Renato Florentino Garcia",
        "Renato Florentino Garcia",
        "2013",
        "_blur",
        "RGB*, GRAY*",
        GIMP_PLUGIN,
        G_N_ELEMENTS(args), 0,
        args, NULL);
}

void imgproc::blur::run(GimpDrawable *drawable)
{
    std::auto_ptr<Options> options = presentDialog();
    if (options.get() == NULL)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::blur(src, dst, cv::Size(3,3));
    setMatToDrawable(dst, drawable);
}
