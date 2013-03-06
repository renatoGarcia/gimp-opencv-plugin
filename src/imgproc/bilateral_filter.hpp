#ifndef _IMGPROC_BILATERAL_FILTER_HPP_
#define _IMGPROC_BILATERAL_FILTER_HPP_

#include <libgimp/gimp.h>

namespace imgproc
{
    namespace bilateralFilter
    {
        void install();

        void run(GimpDrawable *drawable);
    }
}

#endif /* _IMGPROC_BILATERAL_FILTER_HPP_ */
