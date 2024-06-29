#include "dataitem.h"

DataItemWidget::DataItemWidget(int id, int cid, int price, const QDate &date,
                               const QString &description,
                               const QString &categoryName, QWidget *parent)
    : id(id), cid(cid), categoryName(categoryName), price(price), date(date),
      description(description), QWidget{parent} {
  layout = new QHBoxLayout(this);

  dateLabel = new QLabel(date.toString("yyyy-MM-dd"), this);
  layout->addWidget(dateLabel);

  categoryLabel = new QLabel(categoryName, this);
  layout->addWidget(categoryLabel);

  layout->addStretch();

  priceLabel = new QLabel(QString::number(price), this);
  layout->addWidget(priceLabel);

  deleteButtom = new QPushButton(QString("删除"), this);
  layout->addWidget(deleteButtom);

  setLayout(layout);
}

DataItemWidget::~DataItemWidget() {
  delete layout;
  delete categoryLabel;
  delete priceLabel;
  delete dateLabel;
}
