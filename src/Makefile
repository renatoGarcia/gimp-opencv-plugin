
CXX=g++
GIMPTOOL=gimptool-2.0

GIMP_FLAGS=`$(GIMPTOOL) --cflags`
GIMP_LIBS=`$(GIMPTOOL) --libs`

CV_FLAGS=`pkg-config --cflags opencv`
CV_LIBS=`pkg-config --libs opencv`

CXX_FLAGS=$(GIMP_FLAGS) $(CV_FLAGS) -I$(CURDIR) -g

RM=rm -f

VPATH=imgproc utility widget

SOURCES=opencv.cpp imgproc.cpp conversions.cpp cvt_color.cpp bilateral_filter.cpp blur.cpp enums.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)

all: opencv

opencv: $(OBJECTS)
	$(CXX) $(GIMP_LIBS) $(CV_LIBS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

install: opencv
	$(GIMPTOOL) --install-bin opencv

clean:
	$(RM) *.o opencv *~
