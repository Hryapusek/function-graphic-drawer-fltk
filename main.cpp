#include "FunctionGraphWindow.hpp"

double f(double x)
{
  return 1/x;
}

int main()
{
  FunctionGraphWindow *wind = new FunctionGraphWindow(f);
  return Fl::run();
}