#include "widget.h"
#include "maindatawidget.h"
#include <QLabel>

Widget::Widget(QWidget *parent) : QWidget(parent) {
  dbHandler = new DatabaseHandler(this);

  hlayout = new QHBoxLayout(this);
  hlayout->setSpacing(0);
  hlayout->setContentsMargins(0, 0, 0, 0);

  mySidebar = new SideBar(this);
  mySidebar->setObjectName(QString("mySideBar"));

  hlayout->addWidget(mySidebar);

  dataWidget = new QStackedWidget(this);
  dataWidget->setObjectName("dataWidget");
  hlayout->addWidget(dataWidget);

  QWidget *widget2 = new QWidget(this);
  widget2->setObjectName("Widget2");
  QLabel *label2 = new QLabel(widget2);
  label2->setText("hello2");
  label2->setObjectName("Label2");

  widget1 = new MainDataWidget(dbHandler, this);
  widget1->setObjectName("Widget1");

  widget3 = new QWidget(this);
  widget3->setObjectName("Widget3");
  QLabel *label1 = new QLabel(widget3);
  label1->setText("hello3");

  dataWidget->addWidget(widget1);
  dataWidget->addWidget(widget2);
  dataWidget->addWidget(widget3);

  connect(mySidebar->buttonGroup, &QButtonGroup::idClicked, dataWidget,
          &QStackedWidget::setCurrentIndex);
}

Widget::~Widget() {}
