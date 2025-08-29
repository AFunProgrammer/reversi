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

#ifndef CPLAYERSETTINGS_H
#define CPLAYERSETTINGS_H

#pragma once
#include <QDialog>
#include "creversigame.h"

namespace Ui {
class CPlayerSettings;
}

class CPlayerSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayerSettings(QWidget *parent = nullptr);
    ~CPlayerSettings();

    void editPlayer(ReversiPlayer EditPlayer = {ePlayer::None, "Not Found", ePlayerType::Observer});

private:
    Ui::CPlayerSettings *ui;
    ReversiPlayer m_EditPlayer;

    void applyChanges();
};

#endif // CPLAYERSETTINGS_H
