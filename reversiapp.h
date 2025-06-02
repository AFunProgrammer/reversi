#ifndef REVERSIAPP_H
#define REVERSIAPP_H

#pragma once
#include <QMainWindow>
#include <QTimer>

#include "cgameoptions.h"
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
    CGameOptions m_gameOptionsDlg;


    void selectNextMoveForComputer();
    void updatePlayerSettings(eColor PlayerColor);
    void updateGameOptions();
    void updateUiForGameProgress();

    bool m_gameStart = false;
};
#endif // REVERSIAPP_H
