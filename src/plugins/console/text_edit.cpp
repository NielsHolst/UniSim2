#include <QKeyEvent>
#include "text_edit.h"

namespace console {

TextEdit::TextEdit(QWidget *parent)
    : QTextEdit(parent)
{
}

void TextEdit::keyPressEvent(QKeyEvent *event) {
       switch (event->key()) {
       case Qt::Key_Return:
       case Qt::Key_Enter:
       case Qt::Key_Left:
       case Qt::Key_Right:
       case Qt::Key_Up:
       case Qt::Key_Down:
            event->ignore();
            break;
       default:
            QTextEdit::keyPressEvent(event);
            break;
       }
    }

}
