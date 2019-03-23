#include "qqtexedit.h"

QQTeXEdit:: QQTeXEdit(QQuickItem *parent): QQuickPaintedItem(parent) {
    setOpaquePainting(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    setFlag(QQuickItem::ItemAcceptsDrops, true);
    setFlag(QQuickItem::ItemHasContents, true);

    editor = new QmlQTeXEditInternalWidget(this);
    editor->setFocus();
    editor->insertPlainText("");
    qDebug() << "new QTeXEdit: "
             << this->width() << " "
             << this->height() << "\n";
    connect(editor, &QmlQTeXEditInternalWidget::updateRequest, this, &QQTeXEdit::update);
}
