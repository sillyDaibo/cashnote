#include "widget.h"
#include "maindatawidget.h"
#include <QLabel>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>

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

  widget1 = new MainDataWidget(dbHandler, this);
  widget1->setObjectName("Widget1");

  widget2 = new ChartWidget(dbHandler,this);
  widget2->setObjectName("Widget2");

  widget3 = new QWidget(this);
  widget3->setObjectName("Widget3");
  QVBoxLayout *vLayout = new QVBoxLayout(widget3);


  // 新增 QLabel 作为介绍文本框
  QLabel *introLabel = new QLabel("这是一个分析页面，请点击按钮获取分析结果。", widget3);
  introLabel->setAlignment(Qt::AlignCenter); // 居中对齐
  introLabel->setStyleSheet(
      "QLabel {"
      "    font-size: 18px;"
      "    color: #333333;"
      "    background-color: #f0f0f0;"
      "    border: 1px solid #cccccc;"
      "    border-radius: 8px;"
      "    padding: 10px;"
      "    margin-bottom: 10px;"
      "}"
      );
  vLayout->addWidget(introLabel);

  // 创建 QHBoxLayout 来容纳两个文本框
  QHBoxLayout *hLayout = new QHBoxLayout();

  // 左侧展示文本框
  QTextEdit *leftTextEdit = new QTextEdit(widget3);
  leftTextEdit->setReadOnly(true);
  leftTextEdit->setText("这是原始数据，展示给用户看的...\n测试用数据：\n本月的支出：\n饮食10%，\n娱乐70%，\n交通10%，\n其余10%。");
  leftTextEdit->setStyleSheet(
      "QTextEdit {"
      "    font-size: 16px;"
      "    color: #333333;"
      "    background-color: #f9f9f9;"
      "    border: 1px solid #cccccc;"
      "    border-radius: 8px;"
      "    padding: 10px;"
      "    margin-right: 10px;"
      "}"
      );
  hLayout->addWidget(leftTextEdit);

  // 右侧原来的 textEdit
  textEdit = new QTextEdit(widget3);
  textEdit->setReadOnly(true); // 设置为只读
  textEdit->setText("这里显示分析结果..."); // 设置初始文本值
  textEdit->setStyleSheet(
      "QTextEdit {"
      "    font-size: 16px;"
      "    color: blue;"
      "    background-color: #f9f9f9;"
      "    border: 1px solid #cccccc;"
      "    border-radius: 8px;"
      "    padding: 10px;"
      "}"
      );
  hLayout->addWidget(textEdit);

  // 将 QHBoxLayout 添加到 vLayout
  vLayout->addLayout(hLayout);


  // 初始化 Network 实例
  network = new NetWork();

  // 添加按钮并连接到槽函数
  QPushButton *callNetworkButton = new QPushButton("Click to get analysis.", widget3);
  vLayout->addWidget(callNetworkButton);
  connect(callNetworkButton, &QPushButton::clicked, this, &Widget::onCallNetwork);

  // 设置按钮样式和字体
  QFont buttonFont("Arial", 16);
  buttonFont.setBold(true);
  buttonFont.setItalic(false);
  buttonFont.setUnderline(false);
  callNetworkButton->setFont(buttonFont);

  // 设置按钮样式
  callNetworkButton->setStyleSheet(
      "QPushButton {"
      "    background-color: #4CAF50;"
      "    color: white;"
      "    border: none;"
      "    padding: 15px 32px;"
      "    text-align: center;"
      "    text-decoration: none;"
      "    display: inline-block;"
      "    font-size: 16px;"
      "    margin: 4px 2px;"
      "    transition-duration: 0.4s;"
      "    cursor: pointer;"
      "}"
      "QPushButton:hover {"
      "    background-color: white;"
      "    color: black;"
      "    border: 2px solid #4CAF50;"
      "}"
      );

  // 设置 widget3 的样式
  widget3->setStyleSheet("QWidget { background-color: white; }");

  dataWidget->addWidget(widget1);
  dataWidget->addWidget(widget2);
  dataWidget->addWidget(widget3);

  connect(mySidebar->buttonGroup, &QButtonGroup::idClicked, dataWidget, &QStackedWidget::setCurrentIndex);
  connect(mySidebar->buttonGroup, &QButtonGroup::idClicked, dataWidget, [this](int index){
      if (index != 1)
          return;
      QList<QPair<QString, int>> data = dbHandler->getTotalSpentGroupByDate();
      widget2->drawBarChart(data);
  });
}

// 槽函数，用于处理按钮点击事件
void Widget::onCallNetwork()
{
    qDebug() << "CALLING>>>";
    textEdit->setText("分析中，请稍等。。。");
    // 调用 Network 类中的方法
    QString result = network->handle_SetEnquireText("下方是某人本月的支出：饮食10%，娱乐70%，交通10%，其余10%。对他给出简短的消费建议和生活指南。注意直接给出纯文本，不要带markdown格式。");
    qDebug() << "RECEIVE<<<";
    // 显示结果到 label3
    textEdit->setText(result);

}

Widget::~Widget() {}

