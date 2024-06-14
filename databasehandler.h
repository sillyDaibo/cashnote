#ifndef DATABASEHANDLER_H
#define DATABASEHANDLER_H

#include <QObject>
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

} // namespace queries

#endif // DATABASEHANDLER_H
