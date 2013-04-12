#ifndef _CVT_COLOR_HPP_
#define _CVT_COLOR_HPP_

#include <libgimp/gimp.h>

namespace imgproc
{
    namespace cvtColor
    {
        void install();

        void run(GimpDrawable *drawable);
    }
}

#endif /* _CVTCOLOR_HPP_ */
