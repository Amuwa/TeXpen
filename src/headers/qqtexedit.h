#ifndef QQTEXEDIT_H
#define QQTEXEDIT_H

#include "qtexedit.h"

class QmlQTeXEditInternalWidget : public QTeXEdit
{
    friend class QQTeXEdit;
    Q_OBJECT
public:
    explicit QmlQTeXEditInternalWidget(QQuickPaintedItem *parent) {
        m_qquickContainer = parent;
        setFocusPolicy(Qt::WheelFocus);
        setAttribute(Qt::WA_WState_Created, true);// don't create window on setVisible(true)
        setVisible(true);// if not visible some operations will not done.
    }

public slots:
protected:
    QPaintEngine * paintEngine() const {
        return NULL;
    }
    void paintEvent(QPaintEvent * event) {
        QTeXEdit::paintEvent(event);
        m_qquickContainer->update(event->rect());
    }

private:
    QQuickPaintedItem *m_qquickContainer;

};


class QQTeXEdit: public QQuickPaintedItem {
    Q_OBJECT
public:
    QQTeXEdit(QQuickItem *parent = NULL);
    ~QQTeXEdit() {delete editor;}
    void paint(QPainter *painter) override {
        if (this->size().toSize() != editor->size()) {
            editor->resize(this->size().toSize());
        }
        return editor->render(painter);
    }
//    void keyPressEvent(QKeyEvent * e) override {
//        qDebug() << "key press\n";
//        return editor->keyPressEvent(e);
//    }
//    void keyReleaseEvent(QKeyEvent * e) override {
//        qDebug() << "key press\n";
//        return editor->keyReleaseEvent(e);
//    }
//    void inputMethodEvent(QInputMethodEvent *e) override {
//        qDebug() << "key press\n";
//        return editor->inputMethodEvent(e);
//    }
    void mousePressEvent(QMouseEvent *e) override {
        qDebug() << "mouse press\n";
        return editor->mousePressEvent(e);
    }
    void mouseMoveEvent(QMouseEvent *e) override {
        qDebug() << "mouse press\n";
        return editor->mousePressEvent(e);
    }
    void mouseReleaseEvent(QMouseEvent *e) override {
        qDebug() << "mouse release\n";
        return editor->mousePressEvent(e);
    }
    void contextMenuEvent(QContextMenuEvent *e) {
        return editor->contextMenuEvent(e);
    }
//    void resizeEvent(QResizeEvent *e) {
//        qDebug() << "resize QTeXEdit: "
//                 << this->width() << " "
//                 << this->height() << "\n";
//        return editor->resizeEvent(e);
//    }
//    void SetSize(const QSizeF &s) {
//        qDebug() << "resize QTeXEdit\n";
//        QResizeEvent e(s.toSize(), size().toSize());
//        resizeEvent(&e);
//    }
    void updateGeometry() {
        QPointF newPos(0, 0);
        newPos = mapToItem(0, newPos);
        QRectF absRect(newPos, contentsBoundingRect().size());
        editor->setGeometry(absRect.toRect());
    }
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) {
        if(newGeometry == oldGeometry)
            return;
        QQuickItem::geometryChanged(newGeometry, oldGeometry);
        updateGeometry();
    }

    void setFocus(bool arg)  {
        QQuickPaintedItem::setFocus(arg);
        if(arg) {
            //editor->setFocus();
            forceActiveFocus();
        } else {
            editor->clearFocus();
        }
        setActiveFocusOnTab(arg);
    }
    bool event(QEvent *e) override {
        qDebug() << ":QQTeXEdit:event" << e->type();

        if(e->type() == QEvent::HoverEnter ||
           e->type() == QEvent::HoverMove ||
           e->type() == QEvent::HoverLeave ){

            //handleHoverMoveEvent((QHoverEvent*)e);
            return true;
        }

        if( e->type() == QEvent::MouseButtonPress ||
            e->type() == QEvent::MouseButtonDblClick )
            setFocus(true);

        if(editor->event(e))
            return true;
        return QQuickPaintedItem::event(e);
    }
private:
    QmlQTeXEditInternalWidget *editor;
};

#endif // QQTEXEDIT_H
