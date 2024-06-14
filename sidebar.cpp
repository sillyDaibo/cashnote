#include "sidebar.h"

SideBar::SideBar(QWidget *parent) : QWidget{parent} {
  setAttribute(Qt::WA_StyledBackground);
  vLayout = new QVBoxLayout(this);
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->setSpacing(0);

  buttonGroup = new QButtonGroup(this);

  for (int i = 0; i < BUTTONS; ++i) {
    buttons[i] = new QToolButton(this);
    buttons[i]->setIcon(QIcon(QString(":/icons/icon%1").arg(i)));
    buttons[i]->setCheckable(true);
    vLayout->addWidget(buttons[i]);
    buttonGroup->addButton(buttons[i], i);
  }

  buttonGroup->setExclusive(true);
  vLayout->addStretch();

  setLayout(vLayout);
}
