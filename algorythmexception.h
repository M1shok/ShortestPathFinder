#ifndef ALGORYTHMEXCEPTION_H
#define ALGORYTHMEXCEPTION_H
#include "exception.h"

class AlgorythmException : public Exception
{
public:
    enum ErrorCode{StartingPointNotFoundError,
                   FinishingPointNotFoundError,
                   StartingPointDuplicateError,
                   FinishingPointDuplicateError,
                   UnknownError};

private:
    ErrorCode errorCode;

public:
    AlgorythmException(ErrorCode errorCode = UnknownError);
    virtual QString show() const;
};

#endif // ALGORYTHMEXCEPTION_H
