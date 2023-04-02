#include "FunctionGraphWindow.hpp"

double f(double x)
{
  return x*x*x;
}

int main()
{
  FunctionGraphWindow *wind = new FunctionGraphWindow(f);
  return Fl::run();
}