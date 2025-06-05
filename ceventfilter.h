#ifndef CEVENTFILTER_H
#define CEVENTFILTER_H

#pragma once
#include <QEvent>
#include <QObject>

class CEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CEventFilter(QObject *parent = nullptr);

signals:
    void clicked();  // Signal emitted when a mouse click occurs

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CEVENTFILTER_H
