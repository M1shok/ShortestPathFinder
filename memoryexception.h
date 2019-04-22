#ifndef MEMORYEXCEPTION_H
#define MEMORYEXCEPTION_H
#include "exception.h"

class MemoryException: public Exception
{
public:
    enum ErrorCode{ForbiddenSizeError,
                   OutOfRangeIndexError,
                   UnknownError};
private:
    ErrorCode errorCode;

public:
    MemoryException(ErrorCode errorCode = UnknownError);
    virtual QString show() const;
};

#endif // MEMORYEXCEPTION_H
