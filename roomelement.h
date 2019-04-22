#ifndef ROOMSQUAREELEMENT_H
#define ROOMSQUAREELEMENT_H


class RoomElement
{
public:
    enum ElementType {StartingPoint, FinishingPoint, Wall, FreeSpace};

private:
    ElementType type;

public:
    RoomElement();
    RoomElement(ElementType newType);
    void setType(ElementType newType);
    ElementType getType() const;
};

#endif // ROOMSQUAREELEMENT_H
