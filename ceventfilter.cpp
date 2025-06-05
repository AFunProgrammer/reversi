#include "ceventfilter.h"

CEventFilter::CEventFilter(QObject *parent)
    : QObject{parent}
{}

bool CEventFilter::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        emit clicked();  // Emit signal when clicked
        return true;  // Event handled
    }
    return QObject::eventFilter(obj, event);
}
