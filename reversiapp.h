#ifndef REVERSIAPP_H
#define REVERSIAPP_H

#pragma once
#include <QMainWindow>
#include <QTimer>

#include "cplayersettings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ReversiApp;
}
QT_END_NAMESPACE

class ReversiApp : public QMainWindow
{
    Q_OBJECT

public:
    ReversiApp(QWidget *parent = nullptr);
    ~ReversiApp();

private:
    Ui::ReversiApp *ui;
    QTimer m_gameTimer;
    CPlayerSettings m_playerSettingsDlg;

    void selectNextMoveForComputer();
    void updatePlayerSettings(eColor PlayerColor);
    void updateUiForGameProgress();

    bool m_gameStart = false;
};
#endif // REVERSIAPP_H
