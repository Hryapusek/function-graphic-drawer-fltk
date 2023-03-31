CXX			= $(shell fltk-config --cxx)
DEBUG		= -g
CXXFLAGS = $(shell fltk-config --use-gl --use-images --cxxflags ) -I.
LDFLAGS	= $(shell fltk-config --use-gl --use-images --ldflags )
LDSTATIC = $(shell fltk-config --use-gl --use-images --ldstaticflags )
LINK		 = $(CXX)
	
TARGET = drawFunc
OBJS = FunctionGraphWindow.o main.o
 
.SUFFIXES:            # Delete the default suffixes
.SUFFIXES: .cpp .cxx .hpp
 
all: $(TARGET)
				$(LINK) -o $(TARGET) $(OBJS) $(LDSTATIC)

start: clean all
				./$(TARGET)

$(TARGET): $(OBJS)
FunctionGraphWindow.o: FunctionGraphWindow.cxx FunctionGraphWindow.hpp
				$(CXX) $(CXXFLAGS) $(DEBUG) -c $^
main.o: main.cpp
				$(CXX) $(CXXFLAGS) $(DEBUG) -c $<
 
clean:
				rm -f *.o *.gch 2> /dev/null
				rm -f $(TARGET) 2> /dev/null