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

#ifndef CPLAYER_H
#define CPLAYER_H

#pragma once
#include <QObject>
#include <QColor>

// Type of player is Human controlled, Ai (makes moves automagically), Observer to watch
enum class ePlayerType{
    Human = 0x0,
    Ai,
    Observer
};

// Where is the player connected from
enum class ePlayerConnection{
    Local = 0x0,
    Network
};

class CPlayer : public QObject
{
    Q_OBJECT
public:
    explicit CPlayer(QObject *parent = nullptr);


private:
    QColor m_Color = Qt::gray;
    QString m_Name = "Player";
    ePlayerType m_Type = ePlayerType::Human;
    ePlayerConnection m_Connection = ePlayerConnection::Local;


public:
    void setPlayerType(ePlayerType Type);
    ePlayerType playerType();

    void setPlayerName(QString Name);
    QString playerName();

    void setPlayerColor(QColor Color);
    QColor playerColor();

    void setPlayerConnection(ePlayerConnection Connection);
    ePlayerConnection playerConnection();


    // makeMove() || getMove()

    // QList moveHistory
    // wait for move timeout
    //   if timed out then switch to Ai player and doesn't count game in stats?
    //   if disconnect or left then forfeit and automatic win?


signals:
};

#endif // CPLAYER_H
