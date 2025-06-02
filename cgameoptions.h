#ifndef CGAMEOPTIONS_H
#define CGAMEOPTIONS_H

#pragma once

#include <QDialog>

namespace Ui {
class CGameOptions;
}

class CGameOptions : public QDialog
{
    Q_OBJECT

public:
    explicit CGameOptions(QWidget *parent = nullptr);
    ~CGameOptions();

    QColor m_BoardColor = Qt::darkGreen;
private:
    Ui::CGameOptions *ui;

    void applyChanges();
    void cancelChanges();
};

#endif // CGAMEOPTIONS_H
