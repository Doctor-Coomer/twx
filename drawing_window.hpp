#ifndef DRAWING_WINDOW_HPP
#define DRAWING_WINDOW_HPP

#include "globals.hpp"
#include "math.hpp"

#include "tux.hpp"

#include <QSystemTrayIcon>
#include <QPainter>
#include <QWidget>
#include <QTimer>
#include <QIcon>
#include <QMenu>

#if TARGET_OS_OSX
#import <AppKit/AppKit.h>
#endif

class DrawingWindow : public QWidget {
private:
  QTimer* timer = nullptr;
  QSystemTrayIcon* tray = nullptr;
  QMenu* tray_menu = nullptr;
  
  QPixmap* image_tux = nullptr;

public:
  QAction* follow_action = nullptr;
  QAction* circle_action = nullptr;


  static void follow_loop(void) {
    g_window->circle_action->setChecked(false);
    if (!g_window->follow_action->isChecked()) {
       g_window->follow_action->setChecked(true);
    }

    QPoint cursor_position = QCursor::pos();
    cursor_position.setX(cursor_position.x()-W_WIDTH/2);
    cursor_position.setY(cursor_position.y()-W_HEIGHT/2);
    
    QPoint final_position = linear_interp(g_window->pos(), cursor_position, 0.025);
    g_window->move(final_position);
  }
  
  static void circle_loop(void) {
    g_window->follow_action->setChecked(false);
    if (!g_window->circle_action->isChecked()) {
       g_window->circle_action->setChecked(true);
    }
    
    static float f = 0.0;
    QScreen* screen = QGuiApplication::primaryScreen();
    int width = screen->availableGeometry().width();
    int height = screen->availableGeometry().height();
    int hyp = height/4.0;
    if (width < height) {
      hyp = width/4.0;
    }

    QPoint final_position = QPoint((cos(f*X_COS_FRAC)*hyp)+width/2.0-W_WIDTH/2.0, (sin(f*Y_SIN_FRAC)*hyp)+height/2.0-W_HEIGHT/2.0);
    g_window->move(final_position);
    f+=0.01;
  }
  
  DrawingWindow(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) {     
    image_tux = new QPixmap();
    image_tux->loadFromData(tux_png, tux_png_len);

    tray = new QSystemTrayIcon(QIcon(*image_tux));
    
    tray_menu = new QMenu("Menu");

    follow_action = tray_menu->addAction("Follow");
    follow_action->setCheckable(true);

    circle_action = tray_menu->addAction("Circle");
    circle_action->setCheckable(true);
    circle_action->setChecked(true);

    tray_menu->addSeparator();

    QAction* settings_action = tray_menu->addAction("Settings");
    
    QObject::connect(follow_action, &QAction::toggled, [&](bool checked) {
      if (checked == true) {
	timer->stop();
	delete timer;

	timer = new QTimer();      
	timer->setInterval(5);
	timer->callOnTimeout(follow_loop);
	timer->start();
      }
    });

    QObject::connect(circle_action, &QAction::toggled, [&](bool checked) {
      if (checked == true) {
	timer->stop();
	delete timer;

	timer = new QTimer();
	timer->setInterval(5);
	timer->callOnTimeout(circle_loop);
	timer->start();
      }
    });

    QObject::connect(settings_action, &QAction::triggered, [&](bool checked = false) {
      
    });

    
    tray->setToolTip("Twx");
    
    tray->setContextMenu(tray_menu);
    tray->show();

    QObject::connect(tray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason) {

      //printf("a\n");
      //tray_menu->popup(QCursor::pos());
      
    });
    
    timer = new QTimer();
    timer->setInterval(5);
    timer->callOnTimeout(circle_loop);
    timer->start();

    QWidget(parent, f);
  }
  
protected:
  void paintEvent(QPaintEvent* event) override {
    QPainter painter = QPainter(this);

    QRect target_rect = QRect(0, 0, W_WIDTH, W_HEIGHT);
    painter.drawPixmap(target_rect, *image_tux);
  }
  
public:
  void hide_from_taskbar(void) {
#if TARGET_OS_OSX
    {
      NSApplication* ns_app = [NSApplication sharedApplication];
      [ns_app setActivationPolicy:NSApplicationActivationPolicyAccessory];
    }
#endif
  }
};

#endif
