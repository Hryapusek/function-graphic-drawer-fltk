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
  drawUnitInterval();
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
    if (origin.y < h()/2)
      fl_draw(xAxisText, w() - xStrWidth - 5, origin.y + 5);
    else
      fl_draw(xAxisText, w() - xStrWidth - 5, origin.y - 5);
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

void FunctionGraphWindow::drawUnitInterval()
{

  //int nUnits = (w() / 2 - 10) / unitWidthInPixels;
  int nUnits = 10;
  int unitWidthInPixels = (w() / 2 - 20) / nUnits;
  int lineWidth = 2;
  int lineStyle = FL_SOLID;
  Fl_Color lineColor = FL_WHITE;
  int lineLengthHalf = 3;
  int yUpperBound = h() / 2 - lineLengthHalf;
  int yLowerBound = h() / 2 + lineLengthHalf;
  for (int i = 1; i <= nUnits; ++i)
  {
    int posUnitOffset = w() / 2 + i * unitWidthInPixels;
    int negUnitOffset = w() / 2 - i * unitWidthInPixels;
    fl_line_style(lineStyle, lineWidth);
    fl_color(lineColor);
    fl_yxline(posUnitOffset, yUpperBound, yLowerBound);

    fl_line_style(lineStyle, lineWidth);
    fl_color(lineColor);
    fl_yxline(negUnitOffset, yUpperBound, yLowerBound);

    {
      Fl_Font font = FL_TIMES;
      int fontSize = 13;
      Fl_Color fontColor = FL_WHITE;
      fl_font(font, fontSize);
      fl_color(fontColor);
      int strWidth, strHeight;
      std::string posUnitValue = std::to_string(round((1. / nUnits * i) * 100.) / 100.);
      posUnitValue.erase ( posUnitValue.find_last_not_of('0') + 1, std::string::npos );
      posUnitValue.erase ( posUnitValue.find_last_not_of('.') + 1, std::string::npos );
      fl_measure(posUnitValue.c_str(), strWidth, strHeight);
      fl_draw(posUnitValue.c_str(), w() / 2 + i * unitWidthInPixels - strWidth / 2., yLowerBound + strHeight);
    }

    {
      Fl_Font font = FL_TIMES;
      int fontSize = 13;
      Fl_Color fontColor = FL_WHITE;
      fl_font(font, fontSize);
      fl_color(fontColor);
      int strWidth, strHeight;
      std::string negUnitValue = std::to_string(-(round((1. / nUnits * i) * 100.) / 100.));
      negUnitValue.erase(negUnitValue.find_last_not_of('0') + 1, std::string::npos );
      negUnitValue.erase(negUnitValue.find_last_not_of('.') + 1, std::string::npos );
      fl_measure(negUnitValue.c_str(), strWidth, strHeight);
      fl_draw(negUnitValue.c_str(), w() / 2 - i * unitWidthInPixels - strWidth / 2., yLowerBound + strHeight);
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

/*void FunctionGraphWindow::drawFunction()
   { }*/
