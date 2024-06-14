#ifndef MAINDATAWIDGET_H
#define MAINDATAWIDGET_H

#include "databasehandler.h"
#include "qboxlayout.h"
#include <QBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QWidget>

class MainDataWidget : public QWidget {
  Q_OBJECT
public:
  explicit MainDataWidget(DatabaseHandler *dbHandler_,
                          QWidget *parent = nullptr);

signals:

private:
  QVBoxLayout *vLayout;
  QHBoxLayout *hLayoutAbove;
  QVBoxLayout *totalWidgetLayout;
  QVBoxLayout *totalNumWidgetLayout;
  QHBoxLayout *belowWidgetLayout;
  QLabel *earnedBar;
  QLabel *spentBar;
  QLabel *earnedLabel;
  QLabel *spentLabel;

public:
  QWidget *aboveWidget;
  QWidget *belowWidget;
  QWidget *logoWidget;
  QWidget *totalWidget;
  QWidget *totalNumWidget;
  QTableWidget *tableWidget;
  QWidget *editWidget;
  DatabaseHandler *dbHandler;
  void setThisMonthWidget();
  void showTableWidget();
};

#endif // MAINDATAWIDGET_H
