#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QWidget>
#include <QtCharts/QChart>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include "databasehandler.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(DatabaseHandler *dbHandler, QWidget *parent = nullptr);
    void drawBarChart(const QList<QPair<QString, int>> &data);
    void drawPieChart(const QList<QPair<QString, int>> &data);
    void handleQuery();

private:
    QChartView *barChartView;
    QChartView *pieChartView;
    QWidget *leftWidget;
    QWidget *rightWidget;
    DatabaseHandler *dbHandler;
    QLabel *dateInputLabel;
    QLineEdit *dateInput;
    QPushButton *submitButton;
};

#endif // CHARTWIDGET_H
