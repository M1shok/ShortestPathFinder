#include "roomelement.h"

RoomElement::RoomElement()
{
    type = FreeSpace;
}

RoomElement::RoomElement(ElementType newType)
{
    type = newType;
}

RoomElement::ElementType RoomElement::getType() const
{
    return type;
}

void RoomElement::setType(ElementType newType)
{
    type = newType;
}
