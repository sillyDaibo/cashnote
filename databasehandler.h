#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QDate>
#include <QObject>
#include <QPair>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

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
  QList<QPair<QString, int>>
  getSpentGroupByCategoryForMonth(const QString &month);
  int insertRecord(const QDate &date, int spent, int category,
                   const QString &description = "");
  int deleteRecord(int id);
  QList<QPair<QString, int>> getAllCategories();
  int updateRecord(int id, const QDate &date, int spent, int category,
                   const QString &description);

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

const QString insertRecordStr = "\
INSERT INTO record(d, val, cid, description) \
VALUES (strftime('%Y-%m-%d', '%1'), %2, %3, '%4');\
";

const QString deleteRecordStr = "\
DELETE FROM record \
WHERE id = %1;\
";

const QString selectAllCategorieStr = "\
SELECT cid, cname \
FROM category;\
";

const QString selectRecordStr = "\
SELECT id, cid, val, d, description, cname \
FROM record NATURAL JOIN category \
WHERE d BETWEEN date('now', 'start of month') \
    AND date('now', 'start of month', '+1 month', '-1 day') \
";

const QString updateRecordStr = "\
UPDATE record \
SET \
  d=strftime('%Y-%m-%d', '%1'), \
  val=%2, \
  cid=%3, \
  description='%4'  \
WHERE id=%5; \
";

} // namespace queries

// namespace queries

#endif // DATABASEHANDLER_H
