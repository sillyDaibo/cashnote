#include "maindatawidget.h"
#include "databasehandler.h"
#include "qtablewidget.h"
#include <QtSql/QSqlRecord>

MainDataWidget::MainDataWidget(DatabaseHandler *dbHandler_, QWidget *parent)
    : QWidget{parent} {

  dbHandler = dbHandler_;

  // Set AboveWidget
  aboveWidget = new QWidget(this);
  aboveWidget->setObjectName("MainDataWidget-aboveWidget");

  logoWidget = new QWidget(aboveWidget);
  logoWidget->setObjectName("MainDataWidget-logoWidget");

  totalWidget = new QWidget(aboveWidget);
  totalWidget->setObjectName("MainDataWidget-totalWidget");
  earnedBar = new QLabel(totalWidget);
  earnedBar->setObjectName("MainDataWidget-earnedBar");
  spentBar = new QLabel(totalWidget);
  spentBar->setObjectName("MainDataWidget-spentBar");
  totalWidgetLayout = new QVBoxLayout(totalWidget);
  totalWidgetLayout->addWidget(earnedBar);
  totalWidgetLayout->addWidget(spentBar);
  totalWidget->setLayout(totalWidgetLayout);

  totalNumWidget = new QWidget(aboveWidget);
  totalNumWidget->setObjectName("MainDataWidget-totalNumWidget");
  spentLabel = new QLabel(totalNumWidget);
  earnedLabel = new QLabel(totalNumWidget);

  totalNumWidgetLayout = new QVBoxLayout(totalNumWidget);
  totalNumWidgetLayout->addWidget(earnedLabel);
  totalNumWidgetLayout->addWidget(spentLabel);
  totalNumWidget->setLayout(totalNumWidgetLayout);

  hLayoutAbove = new QHBoxLayout(aboveWidget);
  hLayoutAbove->setContentsMargins(0, 0, 0, 0);
  hLayoutAbove->addWidget(logoWidget);
  hLayoutAbove->addStretch();
  hLayoutAbove->addWidget(totalWidget);
  hLayoutAbove->addWidget(totalNumWidget);

  aboveWidget->setLayout(hLayoutAbove);

  // Set BelowWidget
  belowWidget = new QWidget(this);
  belowWidget->setObjectName("MainDataWidget-belowWidget");

  tableWidget = new QTableWidget(belowWidget);
  tableWidget->setObjectName("MainDataWidget-tableWidget");
  editWidget = new QWidget(belowWidget);
  editWidget->setObjectName("MainDataWidget-editWidget");

  belowWidgetLayout = new QHBoxLayout(belowWidget);
  belowWidgetLayout->setSpacing(0);
  belowWidgetLayout->setContentsMargins(0, 0, 0, 0);
  belowWidgetLayout->addWidget(tableWidget);
  belowWidgetLayout->addWidget(editWidget);
  belowWidget->setLayout(belowWidgetLayout);

  // Set Global Layout
  vLayout = new QVBoxLayout(this);
  vLayout->setSpacing(0);
  vLayout->setContentsMargins(0, 0, 0, 0);

  vLayout->addWidget(aboveWidget);
  vLayout->addWidget(belowWidget);

  setLayout(vLayout);

  setThisMonthWidget();
  showTableWidget();
}

void MainDataWidget::setThisMonthWidget() {
  const int MIN_WIDTH = 20;
  int earned = dbHandler->getThisMonthTotalEarned();
  int spent = -dbHandler->getThisMonthTotalSpent();
  int width = totalWidget->size().width();
  earnedLabel->setText(QString("本月收入: %1").arg(earned));
  spentLabel->setText(QString("本月支出: %1").arg(spent));
  if (earned == 0 && spent == 0) {
    earnedBar->setMaximumWidth(MIN_WIDTH);
    spentBar->setMaximumWidth(MIN_WIDTH);
    return;
  }
  if (earned >= spent) {
    spentBar->setMaximumWidth(std::max(spent * 300 / earned, MIN_WIDTH));
    return;
  }
  if (earned < spent) {
    earnedBar->setMaximumWidth(std::max(earned * 300 / spent, MIN_WIDTH));
    return;
  }
}

void MainDataWidget::showTableWidget() {
  QSqlQuery query;

  if (!query.exec(queries::selectThisMonthTableStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  int i = 0;
  tableWidget->setColumnCount(query.record().count());
  tableWidget->setRowCount(dbHandler->getThisMonthCount());
  while (query.next()) {
    for (int j = 0; j < query.record().count(); ++j) {
      qDebug() << query.value(j);
      tableWidget->setItem(i, j,
                           new QTableWidgetItem(query.value(j).toString()));
    }
    ++i;
  }
}
