#include "comboboxadjusted.h"

ComboBoxAdjusted::ComboBoxAdjusted(QWidget *parent) :
    QComboBox(parent)
{
    //setAdjustedText();
}


void ComboBoxAdjusted::setAdjustedText()
{
    QFont font = this->font();
    QFont tempFont = font;
    QFontMetrics metr(font);

    QRect rectLbl = contentsRect();
    QRect textRect = metr.boundingRect(this->currentText());
    double size = font.pointSizeF();
    while(textRect.width() + 23 >= rectLbl.width() && size > 0)
    {
        size -= 0.25;
        font.setPointSizeF(size);
        QFontMetrics metr2(font);
        textRect = metr2.boundingRect(this->currentText());
    }

    font.setPointSizeF(size);
    setFont(font);
    for(int i = 0; i < count(); ++i)
        setItemData(i, tempFont, Qt::FontRole);
}
