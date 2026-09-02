#include "common.h"

QString loadFileToString(QString nameFile)
{
    QString result = "";

    QFile file(nameFile);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        result = stream.readAll();
        file.close();
    }

    return result;
}

int saveStringToFile(QString string, QString nameFile)
{
    QFile file(nameFile);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        stream << string;

        file.close();
        return 1;
    }
    else
    {
        return 0;
    }
}
