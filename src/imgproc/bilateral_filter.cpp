#include "bilateral_filter.hpp"

#include "imgproc/enums.hpp"
#include "utility/conversions.hpp"
#include "widget/numeric_widget.hpp"
#include "widget/enum_widget.hpp"

#include <gtk/gtk.h>
#include <libgimp/gimpui.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>

namespace
{
    typedef boost::tuple<int, double, double, int> Arguments;

    boost::optional<Arguments> presentDialog()
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

        NumericWidget<int> dWidget(0);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), dWidget, 1, 2, 0, 1);

        //----- sigmaColor
        GtkLabel* gtkLabel_2 = GTK_LABEL(gtk_label_new("sigmaColor:"));
        gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_2), 0, 1, 1, 2);

        NumericWidget<double> sigmaColorWidget(0.0);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), sigmaColorWidget, 1, 2, 1, 2);

        //----- sigmaSpace
        GtkLabel* gtkLabel_3 = GTK_LABEL(gtk_label_new("sigmaSpace:"));
        gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_3), 0, 1, 2, 3);

        NumericWidget<double> sigmaSpaceWidget(0.0);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), sigmaSpaceWidget, 1, 2, 2, 3);

        //----- borderType
        GtkLabel* gtkLabel_4 = GTK_LABEL(gtk_label_new("borderType:"));
        gtk_table_attach_defaults(gtkTable_1, GTK_WIDGET(gtkLabel_4), 0, 1, 3, 4);

        EnumWidget borderTypeWidget(TYPE_BORDER_ENUM, cv::BORDER_DEFAULT);
        gtk_table_attach_defaults(GTK_TABLE(gtkTable_1), borderTypeWidget, 1, 2, 3, 4);


        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(gtkTable_1));
        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(dWidget, sigmaColorWidget, sigmaSpaceWidget, borderTypeWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
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

void imgproc::bilateralFilter::registerName(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions)
{
    runFunctions["bilateralFilter"] = imgproc::bilateralFilter::run;
}

void imgproc::bilateralFilter::run(GimpDrawable* drawable)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::bilateralFilter(src, dst, boost::get<0>(*arguments), boost::get<1>(*arguments), boost::get<2>(*arguments), boost::get<3>(*arguments));
    setMatToDrawable(dst, drawable);
}
