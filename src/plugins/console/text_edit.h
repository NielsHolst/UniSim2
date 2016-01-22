#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H
#include <QTextEdit>

namespace console {

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = 0);
protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
};

}

#endif
