// generated by Fast Light User Interface Designer (fluid) version 1.0400

#ifndef FunctionGraphWindow_h
#define FunctionGraphWindow_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

class FunctionGraphWindow: public Fl_Double_Window
{
public:
  FunctionGraphWindow(double (*f)(double));
  void draw();
  int handle(int e);
  struct Pixel
  {
    int x, y;
  };
private:
  double xLeft = -1;
  double xRight = 1;
  double yLow = -1;
  double yHigh = 1;
  double (*f)(double);
  void drawXyAxis();
  void drawXUnitInterval();
  void drawYUnitInterval();
  Pixel getOrigin();
  inline bool isXAxisInUpperHalf();
  inline bool isYAxisInLeftHalf();
  void drawFunction();
  void drawLines();
};
#endif
