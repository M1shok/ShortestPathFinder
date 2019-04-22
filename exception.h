#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QString>
class Exception
{
public:
    Exception();
    virtual QString show() const = 0;
};

#endif // EXCEPTION_H
