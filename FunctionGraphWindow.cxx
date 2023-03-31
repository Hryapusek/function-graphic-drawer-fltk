#include "FunctionGraphWindow.hpp"
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <string>
#include <cmath>
#include <iostream>

FunctionGraphWindow::FunctionGraphWindow(double (*f)(double))
  : Fl_Double_Window(600, 600) {
  Fl_Double_Window::box(FL_BORDER_BOX);
  Fl_Double_Window::color(FL_BLACK);
  Fl_Double_Window::labelfont(1);
  Fl_Double_Window::user_data((void *)(this));
  Fl_Double_Window::size_range(300, 300);
  Fl_Double_Window::end();
  Fl_Double_Window::show();
  Fl_Double_Window::resizable(this);
}

void FunctionGraphWindow::draw()
{
  Fl_Double_Window::draw();
  drawXyAxis();
  drawXUnitInterval();
  //drawFunction();
}

void FunctionGraphWindow::drawXyAxis()
{
  Fl_Font xyFont = FL_TIMES | FL_BOLD;
  int fontSize = 14;
  Fl_Color fontColor = FL_WHITE;
  int lineWidth = 3;
  int lineStyle = FL_SOLID;
  Fl_Color lineColor = FL_WHITE;
  Pixel origin = getOrigin();
  { //drawing X axis
    fl_line_style(lineStyle, lineWidth);
    fl_color(lineColor);
    fl_xyline(0, origin.y, w());

    fl_font(xyFont, fontSize);
    fl_color(fontColor);
    int xStrWidth, xStrHeight;
    const char *xAxisText = "X";
    fl_measure(xAxisText, xStrWidth, xStrHeight);
    if (origin.y < h() / 2)
      fl_draw(xAxisText, w() - xStrWidth - 5, origin.y + xStrHeight + 30);
    else
      fl_draw(xAxisText, w() - xStrWidth - 5, origin.y - xStrHeight - 30);
  }

  //drawing Y axis
  {
    fl_line_style(lineStyle, lineWidth);
    fl_color(lineColor);
    fl_yxline(origin.x, 0, h());

    fl_font(xyFont, fontSize);
    fl_color(fontColor);
    int yStrWidth, yStrHeight;
    const char *yAxisText = "Y";
    fl_measure(yAxisText, yStrWidth, yStrHeight);
    if (origin.x < w() / 2)
      fl_draw(yAxisText, origin.x + yStrWidth + 5, yStrHeight);
    else
      fl_draw(yAxisText, origin.x - yStrWidth - 5, yStrHeight);
  }

}

void FunctionGraphWindow::drawXUnitInterval()
{
  Pixel origin = getOrigin();
  int nUnits = 12;
  int nNegUnits = (double)origin.x / w() * nUnits;
  int nPosUnits = nUnits - nNegUnits;
  int lineWidth = 2;
  int lineStyle = FL_SOLID;
  Fl_Color lineColor = FL_WHITE;
  int lineLengthHalf = 6;
  int yUpperBound = origin.y - lineLengthHalf;
  int yLowerBound = origin.y + lineLengthHalf;
  if (nPosUnits != 0) //place only positive unit intervals
  {
    //width in pixels of single unit interval;
    int stepInPixels = (w() - origin.x - 10) / nPosUnits;
    //value of single unit interval
    double stepValue = (xRight - std::max(0., xLeft)) / nPosUnits;
    int firstUnitOffset;
    double firstUnitVal;
    if (xLeft <= 0)
    {
      firstUnitOffset = origin.x + stepInPixels;
      firstUnitVal = stepValue;
    }
    else
    {
      firstUnitOffset = (1 - std::fmod(xLeft, stepValue) / stepValue) * stepInPixels;
      firstUnitVal = xLeft + (1 - std::fmod(xLeft, stepValue) / stepValue) * stepValue;
    }
    for (int i = 0; i < nPosUnits; ++i)
    {
      int curUnitOffset = firstUnitOffset + i * stepInPixels;
      fl_line_style(lineStyle, lineWidth);
      fl_color(lineColor);
      fl_yxline(curUnitOffset, yUpperBound, yLowerBound);
      {
        Fl_Font font = FL_TIMES;
        int fontSize = 16;
        Fl_Color fontColor = FL_WHITE;
        fl_font(font, fontSize);
        fl_color(fontColor);
        int strWidth, strHeight;
        std::string posUnitValue = std::to_string(round((firstUnitVal + stepValue * i) * 100.) / 100.);
        posUnitValue.erase(posUnitValue.find_last_not_of('0') + 1, std::string::npos );
        posUnitValue.erase(posUnitValue.find_last_not_of('.') + 1, std::string::npos );
        fl_measure(posUnitValue.c_str(), strWidth, strHeight);
        if (isYAxisInUpperHalf())
          fl_draw(posUnitValue.c_str(), curUnitOffset - strWidth / 2., yLowerBound + strHeight);
        else
          fl_draw(posUnitValue.c_str(), curUnitOffset - strWidth / 2., yUpperBound - strHeight);
      }
    }
  }
}

FunctionGraphWindow::Pixel FunctionGraphWindow::getOrigin()
{
  int y;
  if (yHigh <= 0)
    y = 0;
  else if (yLow > 0)
    y = w();
  else
    y = std::abs(yHigh / (yHigh - yLow) * h());
  int x;
  if (xLeft >= 0)
    x = 0;
  else if (xRight <= 0)
    x = w();
  else
    x = std::abs(xLeft / (xRight - xLeft) * w());
  return { x, y };
}

bool FunctionGraphWindow::isYAxisInUpperHalf()
{
  return getOrigin().y <= h() / 2;
}

/*void FunctionGraphWindow::drawFunction()
   { }*/
