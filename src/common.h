#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QApplication>
#include <QFile>
#include <QDebug>

QString loadFileToString(QString nameFile);
int saveStringToFile(QString string, QString nameFile);

#endif // COMMON_H
