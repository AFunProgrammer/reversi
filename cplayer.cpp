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

#include "cplayer.h"

CPlayer::CPlayer(QObject *parent)
    : QObject{parent}
{}


void CPlayer::setPlayerType(ePlayerType Type){
    m_Type = Type;
}

ePlayerType CPlayer::playerType(){
    return m_Type;
}


void CPlayer::setPlayerName(QString Name){
    m_Name = Name;
}

QString CPlayer::playerName(){
    return m_Name;
}


void CPlayer::setPlayerColor(QColor Color){
    m_Color = Color;
}

QColor CPlayer::playerColor(){
    return m_Color;
}


void CPlayer::setPlayerConnection(ePlayerConnection Connection){
    m_Connection = Connection;
}

ePlayerConnection CPlayer::playerConnection(){
    return m_Connection;
}
