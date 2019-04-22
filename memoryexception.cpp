#include "memoryexception.h"

MemoryException::MemoryException(ErrorCode errorCode)
{
    this->errorCode = errorCode;
}

QString MemoryException::show() const
{
    QString message;
    switch (errorCode)
    {
    case OutOfRangeIndexError:
        message = "Index is out of range!";
        break;
    case ForbiddenSizeError:
        message = "Forbidden size value!";
        break;
    case UnknownError:
        message = "An unknown error occured!";
        break;
    }
    return message;
}
