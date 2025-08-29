/*
 * This file is part of reversi.
 *
 * Copyright (C) 2024-2025 InspiringFamily.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

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
