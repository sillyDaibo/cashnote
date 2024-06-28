#ifndef WIDGET_H
#define WIDGET_H

#include "databasehandler.h"
#include "maindatawidget.h"
#include "sidebar.h"
#include "callgpt.h"
#include <QBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QToolButton>
#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include "ChartWidget.h"

class Widget : public QWidget {
  Q_OBJECT

public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

private slots:
  void onCallNetwork();


private:
    QHBoxLayout *hlayout;
    QStackedWidget *dataWidget;
    SideBar *mySidebar;
    QWidget *widget3;
    MainDataWidget *widget1;
    ChartWidget *widget2;
    DatabaseHandler *dbHandler;
    QTextEdit *textEdit; // 用于窗口3显示文本的标签

    NetWork *network;  // 声明 Network 类的实例
    QChartView *chartView;
};
#endif // WIDGET_H
