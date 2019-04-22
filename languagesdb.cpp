#include "languagesdb.h"

LanguagesDB::LanguagesDB(languages lg)
{
    currentLanguage = lg;
    QString lang = getFilename();
    dictionary = new QJsonDocument(loadDictionary(lang));
}

QString LanguagesDB::getValueByKey(const QString &keyword)
{
    if(dictionary == NULL)
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    QString value = dictionary->object().value(keyword).toString();

    if(value == "")
        throw FileException(FileException::ErrorCode::DictionaryError);

    return value;
}

void LanguagesDB::changeLanguage(languages lg)
{
    currentLanguage = lg;
    QString lang = getFilename();
    delete dictionary;
    dictionary = new QJsonDocument(loadDictionary(lang));
}

QJsonDocument LanguagesDB::loadDictionary(const QString &filename)
{
    QFile file(filename);
    if(!file.exists() || !file.open(QFile::ReadOnly))
        throw FileException(FileException::ErrorCode::FileNotFoundError);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    return doc;
}

QString LanguagesDB::getFilename()
{
    QString filename;

    switch (currentLanguage)
    {
    case English:
        filename = ":/languages/english.json";
        break;
    case Ukrainian:
        filename = ":/languages/ukrainian.json";
        break;
    case Russian:
        filename = ":/languages/russian.json";
        break;
    }

    return filename;
}
