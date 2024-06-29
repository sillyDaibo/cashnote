#include "ChartWidget.h"
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLegend>

ChartWidget::ChartWidget(DatabaseHandler *dbHandler_, QWidget *parent) : QWidget(parent), dbHandler(dbHandler_) {
    QHBoxLayout *layout = new QHBoxLayout(this);

    leftWidget = new QWidget(this);
    rightWidget = new QWidget(this);

    // 设置左侧 widget
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    barChartView = new QChartView(this);
    leftLayout->addWidget(barChartView);
    leftWidget->setLayout(leftLayout);

    // 设置右侧 widget
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    dateInputLabel = new QLabel("按日&按月查询消费类别占比(yyyy-MM-dd & yyyy-MM)", rightWidget);
    rightLayout->addWidget(dateInputLabel);

    dateInput = new QLineEdit(rightWidget);
    rightLayout->addWidget(dateInput);

    submitButton = new QPushButton("生成饼状图", rightWidget);
    rightLayout->addWidget(submitButton);

    pieChartView = new QChartView(rightWidget);
    rightLayout->addWidget(pieChartView);

    rightWidget->setLayout(rightLayout);

    // 将左右两个 widget 添加到水平布局中
    layout->addWidget(leftWidget,1);
    layout->addWidget(rightWidget,1);

    setLayout(layout);

    // 连接按钮点击信号和槽函数
    connect(submitButton, &QPushButton::clicked, this, &ChartWidget::handleQuery);

    // 初始化柱状图
    QList<QPair<QString, int>> data = dbHandler->getTotalSpentGroupByDate();
    drawBarChart(data);
}

void ChartWidget::drawBarChart(const QList<QPair<QString, int>> &data) {
    QBarSeries *series = new QBarSeries();
    QBarSet *barSet = new QBarSet("Total Spent");
    QStringList categories;

    for (const auto &pair : data) {
        QString date = pair.first.mid(5, 5);
        int totalSpent = pair.second;
        *barSet << totalSpent;
        categories << date;
        series->append(barSet);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("近7日净开销");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    barChartView->setChart(chart);
}

void ChartWidget::handleQuery() {
    QString input = dateInput->text().trimmed();
    QList<QPair<QString, int>> data;

    if (input.length() == 7 && input[4] == '-') {
        // 输入为 yyyy-MM 格式，显示当月消费类别饼状图
        data = dbHandler->getSpentGroupByCategoryForMonth(input);
    } else if (input.length() == 10 && input[4] == '-' && input[7] == '-') {
        // 输入为 yyyy-MM-dd 格式，显示当日消费类别饼状图
        data = dbHandler->getSpentGroupByCategory(input);
    } else {
        qDebug() << "输入格式错误，请输入 yyyy-MM 或 yyyy-MM-dd 格式的日期。";
        return;
    }

    drawPieChart(data);
}


void ChartWidget::drawPieChart(const QList<QPair<QString, int>> &data) {
    QPieSeries *series = new QPieSeries();

    for (const auto &pair : data) {
        QString category = pair.first;
        int totalSpent = pair.second;
        QPieSlice *slice = series->append(category, totalSpent);
        slice->setLabel(category + ": " + QString::number(totalSpent));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("按类别分析消费");
    chart->setAnimationOptions(QChart::AllAnimations);

    pieChartView->setChart(chart);
}
