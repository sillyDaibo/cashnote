#ifndef MAINDATAWIDGET_H
#define MAINDATAWIDGET_H

#include "databasehandler.h"
#include "qboxlayout.h"
#include "qdatetimeedit.h"
#include "qlistwidget.h"
#include "qtmetamacros.h"
#include <QBoxLayout>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QTextEdit>
#include <QWidget>

class MainDataWidget : public QWidget {
  Q_OBJECT
public:
  explicit MainDataWidget(DatabaseHandler *dbHandler_,
                          QWidget *parent = nullptr);

signals:

private:
  int currentId;
  QVBoxLayout *vLayout;
  QHBoxLayout *hLayoutAbove;
  QVBoxLayout *totalWidgetLayout;
  QVBoxLayout *totalNumWidgetLayout;
  QHBoxLayout *belowWidgetLayout;
  QVBoxLayout *editWidgetLayout;
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
  QListWidget *listWidget;
  QWidget *editWidget;
  DatabaseHandler *dbHandler;
  QComboBox *categoryComboBox;
  QSpinBox *priceSpinBox;
  QDateEdit *dateEdit;
  QTextEdit *infoTextEdit;
  QPushButton *savePushButton;
  QPushButton *updatePushButton;

  void drawListWidget();
  void deleteItem(QListWidgetItem *item);
  void setThisMonthWidget();
  void addRecord();
  void refresh();

public slots:
  void onSaveButtonClicked();
  void onUpdateButtonClicked();
  void onItemSelectionChanged();
};

#endif // MAINDATAWIDGET_H
