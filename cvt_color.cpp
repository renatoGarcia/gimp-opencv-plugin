#include "cvt_color.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstring>

namespace {

// G_MODULE_EXPORT
// void
// cb_ok_clicked(GtkButton *button, gpointer user_data)
// {
//     gtk_main_quit();
//     std::cout << "AHQUITTT" << std::endl;
// }

gboolean cvtColor_dialog_new(GimpDrawable *drawable)
{
    gimp_ui_init("opencv", FALSE);

    GimpDialog *dialog =
        GIMP_DIALOG(gimp_dialog_new("blbl_title", "bjlbl_role",
                                    NULL, (GtkDialogFlags)0,
                                    gimp_standard_help_func, "opencv_gimp",

                                    GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                    GTK_STOCK_OK,     GTK_RESPONSE_OK,

                                    NULL));

    // GimpEnumStore *cvtColorEnum =
    //     GIMP_ENUM_STORE(gimp_enum_store_new_with_values(GType enum_type,
    //                                                     gint n_values)

    GtkLabel *label = GTK_LABEL(gtk_label_new("Teste label"));
    gtk_container_add(GTK_CONTAINER(gtk_dialog_get_content_area(GTK_DIALOG(dialog))),
                      GTK_WIDGET(label));

    gtk_widget_show(GTK_WIDGET(label));
    gtk_widget_show(GTK_WIDGET(dialog));
    bool run = (gimp_dialog_run(GIMP_DIALOG(dialog)) == GTK_RESPONSE_OK);
    gtk_widget_destroy(GTK_WIDGET(dialog));

    return 0;
}

}

void cvtColor(GimpDrawable *drawable)
{
    if (!cvtColor_dialog_new(drawable)) {
        return;
    }

    gint channels;
    gint x1, y1, x2, y2;
    GimpPixelRgn rgn_in, rgn_out;

    /* Gets upper left and lower right coordinates,
     * and layers number in the image */
    gimp_drawable_mask_bounds(drawable->drawable_id,
                              &x1, &y1,
                              &x2, &y2);
    channels = gimp_drawable_bpp(drawable->drawable_id);

    /* Initialises two PixelRgns, one to read original data,
     * and the other to write output data. That second one will
     * be merged at the end by the call to
     * gimp_drawable_merge_shadow() */
    gimp_pixel_rgn_init(&rgn_in,
                        drawable,
                        x1, y1,
                        x2 - x1, y2 - y1,
                        FALSE, FALSE);
    gimp_pixel_rgn_init(&rgn_out,
                        drawable,
                        x1, y1,
                        x2 - x1, y2 - y1,
                        TRUE, TRUE);

    cv::Mat tmp;
    cv::Mat cvImage(y2 - y1, x2 - x1, CV_8UC3);
    gimp_pixel_rgn_get_rect(&rgn_in,
                            cvImage.data,
                            x1, y1,
                            x2 - x1, y2 - y1);

    cv::cvtColor(cvImage, tmp, CV_RGB2Lab);

    cvImage.setTo(cv::Scalar(0, 0, 255));

    gimp_pixel_rgn_set_rect(&rgn_out,
                            tmp.data,
                            x1, y1,
                            x2 - x1, y2 - y1);

    /* Update the modified region */
    gimp_drawable_flush(drawable);
    gimp_drawable_merge_shadow(drawable->drawable_id, TRUE);
    gimp_drawable_update(drawable->drawable_id,
                         x1, y1,
                         x2 - x1, y2 - y1);
}
