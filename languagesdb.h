#ifndef LANGUAGESDB_H
#define LANGUAGESDB_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include "fileexception.h"

class LanguagesDB
{
public:
    enum languages {English, Ukrainian, Russian};
    LanguagesDB(languages lg = English);
    QString getValueByKey(const QString &key);
    void changeLanguage(languages lg);

private:
    QJsonDocument *dictionary;
    languages currentLanguage;

    QJsonDocument loadDictionary(const QString &filename);
    QString getFilename();
};

#endif // LANGUAGESDB_H
