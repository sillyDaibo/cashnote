#ifndef WIDGET_H
#define WIDGET_H

#include "databasehandler.h"
#include "maindatawidget.h"
#include "sidebar.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QToolButton>
#include <QWidget>

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private:
  QHBoxLayout *hlayout;
  QStackedWidget *dataWidget;
  SideBar *mySidebar;
  QWidget *widget3;
  MainDataWidget *widget1;
  DatabaseHandler *dbHandler;
};
#endif // WIDGET_H
