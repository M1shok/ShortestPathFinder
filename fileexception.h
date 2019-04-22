#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H
#include "exception.h"

class FileException : public Exception
{
public:
    enum ErrorCode {FileNotFoundError,
                    WrongNameError,
                    DictionaryError,
                    BadQueryResultError,
                    RecordNotFoundInDBError,
                    DBContainsSuchNameError,
                    DBNotOpenedError,
                    BadDataError,
                    UnknownError};
private:
    ErrorCode errorCode;

public:
    FileException(ErrorCode errorCode = UnknownError);
    virtual QString show() const;
};

#endif // FILEEXCEPTION_H
