#ifndef DATAITEM_H
#define DATAITEM_H

#include <QBoxLayout>
#include <QDate>
#include <QLabel>
#include <QListWidgetItem>
#include <QPushButton>
#include <QWidget>

class DataItemWidget : public QWidget {
  Q_OBJECT
public:
  explicit DataItemWidget(int id, int cid, int price, const QDate &date,
                          const QString &description,
                          const QString &categoryName,
                          QWidget *parent = nullptr);
  ~DataItemWidget();

  int id;
  int cid;
  QString categoryName;
  int price;
  QDate date;
  QString description;

  QLabel *categoryLabel;
  QLabel *priceLabel;
  QLabel *dateLabel;
  QPushButton *deleteButtom;
  QHBoxLayout *layout;
};

#endif // DATAITEM_H
