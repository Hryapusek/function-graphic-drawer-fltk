#include "FunctionGraphWindow.hpp"
#include <FL/fl_draw.H>
#include <FL/Fl_Box.H>
#include <string>
#include <cmath>
#include <iostream>
#include <functional>

FunctionGraphWindow::FunctionGraphWindow(double (*f)(double))
  : Fl_Double_Window(600, 600), f(f) {
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
  drawYUnitInterval();
  //drawFunction();
}

int FunctionGraphWindow::handle(int e)
{
  int ret = Fl_Double_Window::handle(e);
  static int x;
  static int y;
  switch (e)
  {
  case FL_PUSH:
    x = Fl::event_x();
    y = Fl::event_y();
    break;
  case FL_DRAG:
    double dx = (Fl::event_x() - x) / (w() / 3.);
    double dy = (Fl::event_y() - y) / (h() / 3.);
    xLeft -= dx;
    xRight -= dx;
    yLow += dy;
    yHigh += dy;
    x = Fl::event_x();
    y = Fl::event_y();
    redraw();
    break;
  }
  return ret;
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
    if (origin.x <= w() / 2)
      fl_draw(yAxisText, origin.x + 5, yStrHeight);
    else
      fl_draw(yAxisText, origin.x - yStrWidth - 5, yStrHeight);
  }

}

void FunctionGraphWindow::drawXUnitInterval()
{
  Pixel origin = getOrigin();
  const int stepWidth = 60;
  const double valueOnPixel = (xRight - xLeft) / w();
  const double step = stepWidth * valueOnPixel;
  int lineWidth = 2;
  int lineStyle = FL_SOLID;
  Fl_Color lineColor = FL_WHITE;
  int lineLengthHalf = 6;
  int yUpperBound = origin.y - lineLengthHalf;
  int yLowerBound = origin.y + lineLengthHalf;
  if (xRight > 0)
  { //positive half
    int unitOffset;
    double unitValue;
    if (xLeft <= 0)
    {
      unitOffset = origin.x + stepWidth;
      unitValue = step;
    }
    else
    {
      unitOffset = (1 - std::fmod(xLeft, step) / step) * stepWidth;
      unitValue = xLeft + (1 - std::fmod(xLeft, step) / step) * step;
    }
    for (; unitOffset < w(); unitOffset += stepWidth)
    {
      fl_line_style(lineStyle, lineWidth);
      fl_color(lineColor);
      fl_yxline(unitOffset, yUpperBound, yLowerBound);
      {
        Fl_Font font = FL_TIMES;
        int fontSize = 16;
        Fl_Color fontColor = FL_WHITE;
        fl_font(font, fontSize);
        fl_color(fontColor);
        int strWidth, strHeight;
        std::string posUnitValue = std::to_string(round((unitValue) * 100.) / 100.);
        posUnitValue.erase(posUnitValue.find_last_not_of('0') + 1, std::string::npos );
        posUnitValue.erase(posUnitValue.find_last_not_of('.') + 1, std::string::npos );
        fl_measure(posUnitValue.c_str(), strWidth, strHeight);
        if (isXAxisInUpperHalf())
          fl_draw(posUnitValue.c_str(), unitOffset - strWidth / 2., yLowerBound + strHeight + 5);
        else
          fl_draw(posUnitValue.c_str(), unitOffset - strWidth / 2., yUpperBound - 5);
      }
      unitValue += step;
    }
  }
  if (xLeft < 0)
  { //negative half
    int unitOffset;
    double unitValue;
    if (xRight >= 0)
    {
      unitOffset = origin.x - stepWidth;
      unitValue = -step;
    }
    else
    {
      unitOffset = w() + (-step - std::fmod(xRight, step)) / step * stepWidth;
      unitValue = xRight - step - std::fmod(xRight, step);
    }
    for (; unitOffset > 0; unitOffset -= stepWidth)
    {
      fl_line_style(lineStyle, lineWidth);
      fl_color(lineColor);
      fl_yxline(unitOffset, yUpperBound, yLowerBound);
      {
        Fl_Font font = FL_TIMES;
        int fontSize = 16;
        Fl_Color fontColor = FL_WHITE;
        fl_font(font, fontSize);
        fl_color(fontColor);
        int strWidth, strHeight;
        std::string strUnitValue = std::to_string(round((unitValue) * 100.) / 100.);
        strUnitValue.erase(strUnitValue.find_last_not_of('0') + 1, std::string::npos );
        strUnitValue.erase(strUnitValue.find_last_not_of('.') + 1, std::string::npos );
        fl_measure(strUnitValue.c_str(), strWidth, strHeight);
        if (isXAxisInUpperHalf())
          fl_draw(strUnitValue.c_str(), unitOffset - strWidth / 2., yLowerBound + strHeight);
        else
          fl_draw(strUnitValue.c_str(), unitOffset - strWidth / 2., yUpperBound - 5);
      }
      unitValue -= step;
    }
  }
}

void FunctionGraphWindow::drawYUnitInterval()
{
  Pixel origin = getOrigin();
  const int stepWidth = 60;
  const double valueOnPixel = (yHigh - yLow) / h();
  const double step = stepWidth * valueOnPixel;
  int lineWidth = 2;
  int lineStyle = FL_SOLID;
  Fl_Color lineColor = FL_WHITE;
  int lineLengthHalf = 6;
  int xLeftBound = origin.x - lineLengthHalf;
  int xRightBound = origin.x + lineLengthHalf;
  if (yHigh > 0)
  { //positive half
    int unitOffset;
    double unitValue;
    if (yLow <= 0)
    {
      unitOffset = origin.y - stepWidth;
      unitValue = step;
    }
    else
    {
      unitOffset = h() - (1 - std::fmod(yLow, step) / step) * stepWidth;
      unitValue = yLow + (1 - std::fmod(yLow, step) / step) * step;
    }
    for (; unitOffset > 0; unitOffset -= stepWidth)
    {
      fl_line_style(lineStyle, lineWidth);
      fl_color(lineColor);
      fl_xyline(xLeftBound, unitOffset, xRightBound);
      {
        Fl_Font font = FL_TIMES;
        int fontSize = 16;
        Fl_Color fontColor = FL_WHITE;
        fl_font(font, fontSize);
        fl_color(fontColor);
        int strWidth, strHeight;
        std::string strUnitValue = std::to_string(round((unitValue) * 100.) / 100.);
        strUnitValue.erase(strUnitValue.find_last_not_of('0') + 1, std::string::npos );
        strUnitValue.erase(strUnitValue.find_last_not_of('.') + 1, std::string::npos );
        fl_measure(strUnitValue.c_str(), strWidth, strHeight);
        if (isYAxisInLeftHalf())
          fl_draw(strUnitValue.c_str(), xRightBound + 5, unitOffset + strHeight / 4.);
        else
          fl_draw(strUnitValue.c_str(), xLeftBound - strWidth - 5, unitOffset + strHeight / 4.);
      }
      unitValue += step;
    }
  }
  if (yLow < 0)
  { //negative half
    int unitOffset;
    double unitValue;
    if (yHigh >= 0)
    {
      unitOffset = origin.y + stepWidth;
      unitValue = -step;
    }
    else
    {
      unitOffset = (step + std::fmod(yHigh, step)) / step * stepWidth;
      unitValue = yHigh - step - std::fmod(yHigh, step);
    }
    for (; unitOffset < h(); unitOffset += stepWidth)
    {
      fl_line_style(lineStyle, lineWidth);
      fl_color(lineColor);
      fl_xyline(xLeftBound, unitOffset, xRightBound);
      {
        Fl_Font font = FL_TIMES;
        int fontSize = 16;
        Fl_Color fontColor = FL_WHITE;
        fl_font(font, fontSize);
        fl_color(fontColor);
        int strWidth, strHeight;
        std::string strUnitValue = std::to_string(round((unitValue) * 100.) / 100.);
        strUnitValue.erase(strUnitValue.find_last_not_of('0') + 1, std::string::npos );
        strUnitValue.erase(strUnitValue.find_last_not_of('.') + 1, std::string::npos );
        fl_measure(strUnitValue.c_str(), strWidth, strHeight);
        if (isYAxisInLeftHalf())
          fl_draw(strUnitValue.c_str(), xRightBound + 5, unitOffset + strHeight / 4.);
        else
          fl_draw(strUnitValue.c_str(), xLeftBound - strWidth - 5, unitOffset + strHeight / 4.);
      }
      unitValue -= step;
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

inline bool FunctionGraphWindow::isXAxisInUpperHalf()
{
  return getOrigin().y <= h() / 2;
}

inline bool FunctionGraphWindow::isYAxisInLeftHalf()
{
  return getOrigin().x <= w() / 2;;
}

/*void FunctionGraphWindow::drawFunction()
   { }*/
