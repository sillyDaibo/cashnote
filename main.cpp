#include "widget.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Read qss
  QFile qss(QString(":/style.qss"));
  if (qss.open(QFile::ReadOnly)) {
    qDebug() << "open qss success";
    QString style = QLatin1String(qss.readAll());
    a.setStyleSheet(style);
    qss.close();
  } else {
    qDebug() << "Open qss failed";
  }

  Widget w;
  w.setObjectName(QString("MainWindow"));
  w.show();

  return a.exec();
}
