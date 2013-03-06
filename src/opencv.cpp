#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cstring>

#include "imgproc.hpp"

extern "C" {

static void query(void);
static void run(const gchar *name,
                gint nparams, const GimpParam *param,
                gint *nreturn_vals, GimpParam **return_vals);

GimpPlugInInfo PLUG_IN_INFO =
{
    NULL,
    NULL,
    query,
    run
};

MAIN()

static void query(void)
{
    imgproc::install();
}

static void run(gchar const* name,
                gint nParams,
                GimpParam const* params,
                gint* nReturnVals,
                GimpParam** returnVals)
{
    static GimpParam  values[1];
    GimpPDBStatusType status = GIMP_PDB_SUCCESS;
    GimpRunMode       run_mode;
    GimpDrawable     *drawable;

    /* Setting mandatory output values */
    *nReturnVals = 1;
    *returnVals  = values;

    values[0].type = GIMP_PDB_STATUS;
    values[0].data.d_status = status;

    /* Getting run_mode - we won't display a dialog if
     * we are in NONINTERACTIVE mode
     */
    run_mode = (GimpRunMode)params[0].data.d_int32;

    /*  Get the specified drawable  */
    drawable = gimp_drawable_get (params[2].data.d_drawable);

    if (std::strcmp(name, "cvtColor") == 0)
    {
        imgproc::cvtColor::run(drawable);
    }
    else if (std::strcmp(name, "bilateralFilter") == 0)
    {
        imgproc::bilateralFilter::run(drawable);
    }

    gimp_displays_flush();
    gimp_drawable_detach(drawable);
}

}
