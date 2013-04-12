#include "cvt_color.hpp"

#include "enums.hpp"
#include "utility/conversions.hpp"
#include "widget/enum_widget.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <memory>
#include <boost/optional/optional.hpp>
#include <boost/tuple/tuple.hpp>

namespace
{
    typedef boost::tuple<int> Arguments;

    boost::optional<Arguments> presentDialog()
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

        EnumWidget enumWidget(TYPE_CVT_COLOR_ENUM, cv::COLOR_BGR2BGRA);

        GtkHBox* gtkHBox_1 = GTK_HBOX(gtk_hbox_new(FALSE, 0));
        GtkLabel* gtkLabel_1 = GTK_LABEL(gtk_label_new("code:"));
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), GTK_WIDGET(gtkLabel_1));
        gtk_container_add(GTK_CONTAINER(gtkHBox_1), enumWidget);

        gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                          GTK_WIDGET(gtkHBox_1));

        gtk_widget_show_all(GTK_WIDGET(dialog));

        boost::optional<Arguments> arguments;
        if (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK)
        {
            arguments = Arguments(enumWidget);
        }

        gtk_widget_destroy(GTK_WIDGET(dialog));

        return arguments;
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

void imgproc::cvtColor::run(GimpDrawable* drawable)
{
    boost::optional<Arguments> arguments = presentDialog();
    if (!arguments)
    {
        return;
    }

    cv::Mat src = drawableToMat(drawable);
    cv::Mat dst;
    cv::cvtColor(src, dst, boost::get<0>(*arguments));
    setMatToDrawable(dst, drawable);
}
