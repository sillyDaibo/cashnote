#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QBoxLayout>
#include <QButtonGroup>
#include <QToolButton>
#include <QWidget>

class SideBar : public QWidget {
  Q_OBJECT
public:
  explicit SideBar(QWidget *parent = nullptr);
  static const int BUTTONS = 3;
  QButtonGroup *buttonGroup;

signals:

private:
  QVBoxLayout *vLayout;
  QToolButton *buttons[BUTTONS];
};

#endif // SIDEBAR_H
