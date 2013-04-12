#include "blur.hpp"

#include "enums.hpp"
#include "widget/pair_widget.hpp"
#include "widget/enum_widget.hpp"
#include "utility/conversions.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <string>

namespace
{
    typedef boost::tuple<cv::Size, cv::Point, int> Arguments;

    boost::optional<Arguments> presentDialog()
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

        PairWidget<cv::Size_<int> > sizeWidget(0, 0);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), sizeWidget, 1, 2, 0, 1);

        //----- anchor
        GtkLabel* gtkLabel_2 = GTK_LABEL(gtk_label_new("anchor:"));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkLabel_2), 0, 1, 1, 2);

        PairWidget<cv::Point_<int> > anchorWidget(-1, -1);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), anchorWidget, 1, 2, 1, 2);

        //----- borderType
        GtkLabel* gtkLabel_3 = GTK_LABEL(gtk_label_new("borderType:"));
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), GTK_WIDGET(gtkLabel_3), 0, 1, 2, 3);

        EnumWidget borderTypeWidget(TYPE_BORDER_ENUM, cv::BORDER_DEFAULT);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), borderTypeWidget, 1, 2, 2, 3);


        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(gtkTable_1));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(sizeWidget, anchorWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
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
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::blur(src, dst, boost::get<0>(*arguments), boost::get<1>(*arguments), boost::get<2>(*arguments));
    setMatToDrawable(dst, drawable);
}
