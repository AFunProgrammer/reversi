#ifndef CGAMEOPTIONS_H
#define CGAMEOPTIONS_H

#pragma once

#include <QDialog>
#include <QtWidgets/qlistwidget.h>

namespace Ui {
class CGameOptions;
}

class CGameOptions : public QDialog
{
    Q_OBJECT

public:
    explicit CGameOptions(QWidget *parent = nullptr);
    ~CGameOptions();

private:
    Ui::CGameOptions *ui;
    QList<QListWidgetItem> m_colorList;
};

#endif // CGAMEOPTIONS_H
