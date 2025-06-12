#ifndef REVERSIAPP_H
#define REVERSIAPP_H

#pragma once
#include <QMainWindow>
#include <QTimer>

#include "csettings.h"
#include "cgamesettings.h"
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

    CSettings m_gameSettings;
    CPlayerSettings m_playerSettingsDlg;
    CGameSettings m_gameOptionsDlg;


    void selectNextMoveForComputer();
    void updateGameSettings();
    void showPlayerSettings(ePlayer PlayerColor);
    void showGameSettings();
    void updateUiForGameProgress();

    bool m_gameStart = false;
};
#endif // REVERSIAPP_H
