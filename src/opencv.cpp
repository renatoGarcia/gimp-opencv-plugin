/* Gimp OpenCV Plugin
 *
 * Copyright (c) 2013 see AUTHORS file.
 *
 * Gimp OpenCV Plugin is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * Gimp OpenCV Plugin is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Gimp OpenCV Plugin. If not, see <http://www.gnu.org/licenses/>.
 */
#include "imgproc.hpp"

#include <libgimp/gimp.h>

#include <map>
#include <string>

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

    std::map<std::string, void(*)(GimpDrawable *drawable)> runFunctions;
    imgproc::registerNames(runFunctions);

    /* Getting run_mode - we won't display a dialog if
     * we are in NONINTERACTIVE mode
     */
    run_mode = (GimpRunMode)params[0].data.d_int32;

    /*  Get the specified drawable  */
    drawable = gimp_drawable_get(params[2].data.d_drawable);

    runFunctions[name](drawable);

    gimp_displays_flush();
    gimp_drawable_detach(drawable);
}

}
