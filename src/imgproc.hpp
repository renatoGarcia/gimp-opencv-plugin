#ifndef _SRC_IMGPROC_HPP_
#define _SRC_IMGPROC_HPP_

#include <libgimp/gimp.h>

#include <map>
#include <string>

namespace imgproc
{
    void install();

    void registerNames(std::map<std::string, void(*)(GimpDrawable *drawable)>& runFunctions);
}

#endif /* _SRC_IMGPROC_HPP_ */
