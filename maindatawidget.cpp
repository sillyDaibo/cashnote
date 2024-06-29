#include "maindatawidget.h"
#include "databasehandler.h"
#include "dataitem.h"
#include "qlistwidget.h"
#include "qobject.h"
#include "qpushbutton.h"
#include "qtextedit.h"
#include <QLabel>
#include <QtSql/QSqlRecord>

MainDataWidget::MainDataWidget(DatabaseHandler *dbHandler_, QWidget *parent)
    : QWidget{parent} {

  currentId = 0;
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

  listWidget = new QListWidget(belowWidget);
  listWidget->setObjectName("MainDataWidget-listWidget");

  editWidget = new QWidget(belowWidget);
  editWidget->setObjectName("MainDataWidget-editWidget");

  categoryComboBox = new QComboBox(editWidget);
  categoryComboBox->setObjectName("MainDataWidget-categoryComboBox");
  QList<QPair<QString, int>> categories = dbHandler->getAllCategories();
  for (const QPair<QString, int> &pair : categories) {
    categoryComboBox->addItem(pair.first);
  }

  dateEdit = new QDateEdit(editWidget);
  dateEdit->setDate(QDate::currentDate());

  priceSpinBox = new QSpinBox(editWidget);
  priceSpinBox->setRange(-1000000, 1000000);

  infoTextEdit = new QTextEdit(editWidget);
  infoTextEdit->setObjectName("MainDataWidget-infoTextEdit");

  savePushButton = new QPushButton("新增", editWidget);

  updatePushButton = new QPushButton("修改", editWidget);

  editWidgetLayout = new QVBoxLayout(editWidget);
  editWidgetLayout->addWidget(new QLabel("类别"));
  editWidgetLayout->addWidget(categoryComboBox);
  editWidgetLayout->addWidget(new QLabel("日期"));
  editWidgetLayout->addWidget(dateEdit);
  editWidgetLayout->addWidget(new QLabel("收支"));
  editWidgetLayout->addWidget(priceSpinBox);
  editWidgetLayout->addWidget(new QLabel("备注"));
  editWidgetLayout->addWidget(infoTextEdit);
  editWidgetLayout->addWidget(savePushButton);
  editWidgetLayout->addWidget(updatePushButton);
  editWidgetLayout->addStretch();
  editWidget->setLayout(editWidgetLayout);

  belowWidgetLayout = new QHBoxLayout(belowWidget);
  belowWidgetLayout->setSpacing(0);
  belowWidgetLayout->setContentsMargins(0, 0, 0, 0);
  belowWidgetLayout->addWidget(listWidget);
  belowWidgetLayout->addWidget(editWidget);
  belowWidget->setLayout(belowWidgetLayout);

  // Set Global Layout
  vLayout = new QVBoxLayout(this);
  vLayout->setSpacing(0);
  vLayout->setContentsMargins(0, 0, 0, 0);

  vLayout->addWidget(aboveWidget);
  vLayout->addWidget(belowWidget);

  setLayout(vLayout);

  refresh();

  connect(savePushButton, &QPushButton::clicked, this,
          &MainDataWidget::onSaveButtonClicked);
  connect(listWidget, &QListWidget::itemSelectionChanged, this,
          &MainDataWidget::onItemSelectionChanged);
  connect(updatePushButton, &QPushButton::clicked, this,
          &MainDataWidget::onUpdateButtonClicked);
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

void MainDataWidget::addRecord() {
  int returnVal = dbHandler->insertRecord(
      dateEdit->date(), priceSpinBox->value(),
      categoryComboBox->currentIndex() + 1, infoTextEdit->toPlainText());
  if (returnVal != 0) {
    qDebug() << "add record failed";
  }
}

void MainDataWidget::drawListWidget() {
  QSqlQuery query;
  if (!query.exec(queries::selectRecordStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  listWidget->clear();
  while (query.next()) {
    int id = query.value(0).toInt();
    qDebug() << QString::number(id);
    int cid = query.value(1).toInt();
    int price = query.value(2).toInt();
    QDate date = QDate::fromString(query.value(3).toString(), "yyyy-MM-dd");
    QString description = query.value(4).toString();
    QString categoryName = query.value(5).toString();
    DataItemWidget *widget = new DataItemWidget(
        id, cid, price, date, description, categoryName, listWidget);
    QListWidgetItem *newItem = new QListWidgetItem(listWidget);
    newItem->setSizeHint(widget->sizeHint());

    listWidget->setItemWidget(newItem, widget);

    connect(widget->deleteButtom, &QPushButton::clicked, this,
            [this, newItem]() { deleteItem(newItem); });
  }
}

void MainDataWidget::deleteItem(QListWidgetItem *item) {
  int row = listWidget->row(item);
  dbHandler->deleteRecord(
      qobject_cast<DataItemWidget *>(listWidget->itemWidget(item))->id);
  listWidget->takeItem(row);
  delete item;
  setThisMonthWidget();
}

void MainDataWidget::onSaveButtonClicked() {
  addRecord();
  refresh();
}

void MainDataWidget::onUpdateButtonClicked() {
  int currentRow = listWidget->currentRow();
  int returnVal = dbHandler->updateRecord(
      currentId, dateEdit->date(), priceSpinBox->value(),
      categoryComboBox->currentIndex() + 1, infoTextEdit->toPlainText());
  if (returnVal != 0) {
    qDebug() << "update failed";
    return;
  }
  refresh();
  if (listWidget->count() != 0)
    listWidget->setCurrentRow(currentRow);
}

inline void MainDataWidget::refresh() {
  setThisMonthWidget();
  drawListWidget();
}

void MainDataWidget::onItemSelectionChanged() {
  if (listWidget->count() == 0)
    return;
  QListWidgetItem *item = listWidget->currentItem();
  DataItemWidget *widget =
      qobject_cast<DataItemWidget *>(listWidget->itemWidget(item));
  categoryComboBox->setCurrentIndex(widget->cid - 1);
  dateEdit->setDate(widget->date);
  priceSpinBox->setValue(widget->price);
  infoTextEdit->setText(widget->description);
  currentId = widget->id;
}
