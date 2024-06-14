#include "databasehandler.h"

#include <QStandardPaths>

DatabaseHandler::DatabaseHandler(QObject *parent) : QObject{parent} {
  db = QSqlDatabase::addDatabase("QSQLITE");
  QString location =
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
  db.setDatabaseName(location + QString("/cashnote.db"));
  if (!db.open()) {
    qDebug() << "cannot connect to database";
  } else {
    qDebug() << "open database success!";
  }
  QSqlQuery query;
  if (!query.exec(queries::createCategoryTableStr)) {
    qDebug() << "query failed :" << query.lastError();
  }
  if (!query.exec(queries::createRecordTableStr)) {
    qDebug() << "query failed :" << query.lastError();
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
