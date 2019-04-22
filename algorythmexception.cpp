#include "algorythmexception.h"

AlgorythmException::AlgorythmException(AlgorythmException::ErrorCode errorCode)
{
    this->errorCode = errorCode;
}

QString AlgorythmException::show() const
{
    QString message;
    switch (errorCode)
    {
    case StartingPointNotFoundError:
        message = "Starting point is not found!";
        break;
    case FinishingPointNotFoundError:
        message = "Finishing point is not found!";
        break;
    case StartingPointDuplicateError:
        message = "Duplicate of starting point found!";
        break;
    case FinishingPointDuplicateError:
        message = "Duplicate of finishing point found!";
        break;
    case UnknownError:
        message = "An unknown error occured!";
        break;
    }
    return message;
}
