#include "databasehandler.h"
#include <QStandardPaths>

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject{parent} {
  db = QSqlDatabase::addDatabase("QSQLITE");
  QString location =
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  db.setDatabaseName(location + QString("/cashnote1.db"));
  if (!db.open()) {
    qDebug() << "cannot connect to database";
  } else {
    qDebug() << "open database success!";
  }
  QSqlQuery query;
  if (!query.exec(queries::createCategoryTableStr)) {
    qDebug() << "query create failed :" << query.lastError();
  }
  if (!query.exec(queries::createRecordTableStr)) {
    qDebug() << "query create failed :" << query.lastError();
  }
  if (!query.exec(queries::selectAllCategorieStr)) {
    qDebug() << "query select all category failed :" << query.lastError();
  }
  if (!query.next()) {
    if (!query.exec(queries::insertBasicCategoryStr)) {
      qDebug() << "query insert record failed :" << query.lastError();
    } else {
      qDebug() << "Category initialized";
    }
  }
}

DatabaseHandler::~DatabaseHandler() { db.close(); }

int DatabaseHandler::getThisMonthTotalSpent() {
  QSqlQuery query;
  if (!query.exec(queries::selectThisMonthTotalSpentStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    int result = query.value(0).toInt();
    return result;
  }
  qDebug() << "query failed";
  return 0;
}

int DatabaseHandler::getThisMonthTotalEarned() {
  QSqlQuery query;
  if (!query.exec(queries::selectThisMonthTotalEarnedStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    int result = query.value(0).toInt();
    return result;
  }
  qDebug() << "query failed";
  return 0;
}

int DatabaseHandler::getThisMonthCount() {
  QSqlQuery query;
  if (!query.exec(queries::selectThisMonthCountStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    int result = query.value(0).toInt();
    return result;
  }
  qDebug() << "query failed";
  return 0;
}

QList<QPair<QString, int>> DatabaseHandler::getTotalSpentGroupByDate() {
  QList<QPair<QString, int>> result;
  QSqlQuery query;
  if (!query.exec(queries::totalSpentGroupByDateStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    QString date = query.value(0).toDate().toString("yyyy-MM-dd");
    int totalSpent = query.value(1).toInt();
    result.append(qMakePair(date, totalSpent));
  }

  QDate currentDate = QDate::currentDate();
  for (int i = 0; i < 7; ++i) {
    QString dateStr = currentDate.addDays(-i).toString("yyyy-MM-dd");
    bool found = false;
    for (const auto &pair : result) {
      if (pair.first == dateStr) {
        found = true;
        break;
      }
    }
    if (!found) {
      result.append(qMakePair(dateStr, 0));
    }
  }

  std::sort(result.begin(), result.end(),
            [](const QPair<QString, int> &a, const QPair<QString, int> &b) {
              return a.first < b.first;
            });

  return result;
}

QList<QPair<QString, int>>
DatabaseHandler::getSpentGroupByCategory(const QString &date) {
  QList<QPair<QString, int>> result;
  QSqlQuery query;
  query.prepare(queries::selectSpentGroupByCategoryStr);
  query.bindValue(":date", date);
  if (!query.exec()) {
    qDebug() << "query failed :" << query.lastError();
    return result;
  }

  while (query.next()) {
    QString category = query.value(0).toString();
    int totalSpent = query.value(1).toInt();
    result.append(qMakePair(category, totalSpent));
  }

  return result;
}

QList<QPair<QString, int>>
DatabaseHandler::getSpentGroupByCategoryForMonth(const QString &month) {
  QList<QPair<QString, int>> result;
  QSqlQuery query;
  query.prepare(queries::selectSpentGroupByCategoryForMonthStr);
  query.addBindValue(month);
  if (!query.exec()) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    QString category = query.value(0).toString();
    int totalSpent = query.value(1).toInt();
    result.append(qMakePair(category, totalSpent));
  }
  return result;
}

QStringList
DatabaseHandler::getCategorySpentPercentageForMonth(const QString &month) {
  QList<QPair<QString, int>> data = getSpentGroupByCategoryForMonth(month);
  int sum = 0;
  for (const auto &pair : data) {
    sum += pair.second;
  }

  QStringList result;
  for (const auto &pair : data) {
    double percentage = (static_cast<double>(pair.second) / sum) * 100;
    QString formattedString =
        QString("%1: %2%").arg(pair.first).arg(percentage, 0, 'f', 2);
    result.append(formattedString);
  }

  return result;
}

int DatabaseHandler::insertRecord(const QDate &date, int spent, int category,
                                  const QString &description) {
  QSqlQuery query;
  QString queryStr = queries::insertRecordStr.arg(date.toString("yyyy-MM-dd"))
                         .arg(spent)
                         .arg(category)
                         .arg(description);
  qDebug() << queryStr;
  if (!query.exec(queryStr)) {
    qDebug() << "insert failed :" << query.lastError();
    return -1;
  }
  qDebug() << "insert success";
  return 0;
}

int DatabaseHandler::deleteRecord(int id) {
  QSqlQuery query;
  QString queryStr = queries::deleteRecordStr.arg(id);
  if (!query.exec(queryStr)) {
    qDebug() << "delete failed :" << query.lastError();
    return -1;
  }
  qDebug() << "delete success";
  return 0;
}

QList<QPair<QString, int>> DatabaseHandler::getAllCategories() {
  QList<QPair<QString, int>> result;
  QSqlQuery query;
  query.prepare(queries::selectAllCategorieStr);
  if (!query.exec()) {
    qDebug() << "query failed :" << query.lastError();
  }
  while (query.next()) {
    int cid = query.value(0).toInt();
    QString category = query.value(1).toString();
    result.append(qMakePair(category, cid));
  }
  return result;
}

int DatabaseHandler::updateRecord(int id, const QDate &date, int spent,
                                  int category, const QString &description) {
  if (id <= 0)
    return -1;
  QSqlQuery query;
  QString queryStr = queries::updateRecordStr.arg(date.toString("yyyy-MM-dd"))
                         .arg(spent)
                         .arg(category)
                         .arg(description)
                         .arg(id);
  if (!query.exec(queryStr)) {
    qDebug() << "update failed :" << query.lastError();
    return -1;
  }
  qDebug() << "update success";
  return 0;
}
