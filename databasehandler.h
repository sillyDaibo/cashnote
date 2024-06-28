#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QPair>
#include <QDate>

class DatabaseHandler : public QObject {
  Q_OBJECT
public:
  explicit DatabaseHandler(QObject *parent = nullptr);
  ~DatabaseHandler();
  QSqlDatabase db;
  int getThisMonthTotalSpent();
  int getThisMonthTotalEarned();
  int getThisMonthCount();
  QList<QPair<QString, int>> getTotalSpentGroupByDate();
  QList<QPair<QString, int>> getSpentGroupByCategory(const QString &date);
  QList<QPair<QString, int>> getSpentGroupByCategoryForMonth(const QString &month);

signals:
};

namespace queries {

const QString createCategoryTableStr = "\
CREATE TABLE IF NOT EXISTS category (\
cid INTEGER PRIMARY KEY AUTOINCREMENT,\
cname CHAR(200) NOT NULL);\
";

const QString createRecordTableStr = "\
CREATE TABLE IF NOT EXISTS record (\
id INTEGER PRIMARY KEY AUTOINCREMENT,\
d DATE DEFAULT CURRENT_DATE,\
t TIME DEFAULT CURRENT_TIME,\
val INTEGER NOT NULL,\
cid INTEGER,\
description TEXT,\
FOREIGN KEY(cid) REFERENCES category(cid)\
);\
";

const QString selectThisMonthTotalEarnedStr = "\
SELECT ifnull(sum(val), 0) AS result FROM record \
WHERE d BETWEEN date('now', 'start of month') \
    AND date('now', 'start of month', '+1 month', '-1 day') \
  AND val > 0;";

const QString selectThisMonthTotalSpentStr = "\
SELECT ifnull(sum(val), 0) AS result FROM record \
WHERE d BETWEEN date('now', 'start of month') \
    AND date('now', 'start of month', '+1 month', '-1 day') \
  AND val < 0;";

const QString selectThisMonthTableStr = "\
SELECT id, d, t, cname, description, val \
FROM record NATURAL JOIN category \
WHERE d BETWEEN date('now', 'start of month') \
    AND date('now', 'start of month', '+1 month', '-1 day');\
";

const QString selectThisMonthCountStr = "\
SELECT count(*) \
FROM record NATURAL JOIN category \
WHERE d BETWEEN date('now', 'start of month') \
  AND date ('now', 'start of month', '+1 month', '-1 day');\
";

const QString totalSpentGroupByDateStr = "\
SELECT d, sum(val) AS total_spent \
FROM record \
WHERE d BETWEEN date('now', '-6 days') AND date('now', 'localtime') \
GROUP BY d \
ORDER BY d DESC LIMIT 7;\
";

const QString selectSpentGroupByCategoryStr = "\
SELECT c.cname, sum(r.val) AS total_spent \
FROM record r \
JOIN category c ON r.cid = c.cid \
WHERE r.d = :date \
GROUP BY r.d, r.cid \
ORDER BY r.d, c.cname;\
";

const QString selectSpentGroupByCategoryForMonthStr = "\
SELECT cname, sum(val) AS total_spent \
FROM record NATURAL JOIN category \
WHERE strftime('%Y-%m', d) = ? \
GROUP BY cid;";
}

// namespace queries

#endif // DATABASEHANDLER_H
