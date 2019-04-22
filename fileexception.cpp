#include "fileexception.h"

FileException::FileException(ErrorCode errorCode)
{
    this->errorCode = errorCode;
}

QString FileException::show() const
{
    QString message;
    switch (errorCode)
    {
    case FileNotFoundError:
        message = "File wasn`t found!";
        break;
    case WrongNameError:
        message = "Wrong filename!";
        break;
    case DictionaryError:
        message = "The dictionary error occured!";
        break;
    case BadQueryResultError:
        message = "Bad query result error occured!";
        break;
    case RecordNotFoundInDBError:
        message = "Record wasn`t found in database!";
        break;
    case DBContainsSuchNameError:
        message = "Database contains such name!";
        break;
    case DBNotOpenedError:
        message = "Database wasn`t opened!";
        break;
    case BadDataError:
        message = "Bad data found!";
        break;
    case UnknownError:
        message = "An unknown error occured!";
        break;
    }
    return message;
}
