#include "globals.hpp"
#include "math.hpp"

#include "drawing_window.hpp"

#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <QApplication>


int main(int argc, char** argv) {
  QApplication app = QApplication(argc, argv);
    
  DrawingWindow window;
  window.setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowDoesNotAcceptFocus | Qt::BypassWindowManagerHint | Qt::WindowTransparentForInput);
  window.setFixedSize(W_WIDTH, W_HEIGHT);
  //window.move(QCursor::pos());  
  window.hide_from_taskbar();
  window.setAttribute(Qt::WA_TranslucentBackground, true);
  window.setAttribute(Qt::WA_NoSystemBackground, true);
  window.setAttribute(Qt::WA_ShowWithoutActivating, true);
  window.setAttribute(Qt::WA_KeyboardFocusChange, false);
  window.setAttribute(Qt::WA_X11DoNotAcceptFocus, true);
  window.setFocusPolicy(Qt::NoFocus);
  window.show();
  g_window = &window;
  
  return app.exec();
}
