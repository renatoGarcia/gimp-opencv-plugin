#ifndef _IMGPROC_BLUR_HPP_
#define _IMGPROC_BLUR_HPP_

#include <libgimp/gimp.h>

namespace imgproc
{
    namespace blur
    {
        void install();

        void run(GimpDrawable *drawable);
    }
}


#endif /* _IMGPROC_BLUR_HPP_ */
