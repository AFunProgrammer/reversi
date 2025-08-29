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

#include "csettings.h"


// Global Instance
CSettings* CSettings::getGlobalInstance(){
    static CSettings g_globalSettings;
    return &g_globalSettings;
}

CSettings::CSettings(QObject *parent)
    : QObject{parent}
{}

// Board Color
void CSettings::setBoardColor(QColor Color) {
    m_boardColor = (Color.isValid() ? Color : Qt::darkGreen);
}

QColor CSettings::boardColor(){
    return m_boardColor;
}


// Player Colors
void CSettings::setPlayerColor(ePlayer playerColor, QColor Color) {
    m_playerColors[static_cast<int>(playerColor)] = Color;
}

QColor CSettings::playerColor(ePlayer playerColor) {
    return m_playerColors[static_cast<int>(playerColor)];
}


// Player Names
void CSettings::setPlayerName(ePlayer playerColor, QString playerName){
    m_playerNames[static_cast<int>(playerColor)] = playerName;
}

QString CSettings::playerName(ePlayer playerColor){
    return m_playerNames[static_cast<int>(playerColor)];
}

// Player Types
void CSettings::setPlayerType(ePlayer playerColor, ePlayerType playerType){
    m_playerType[static_cast<int>(playerColor)] = playerType;
}

ePlayerType CSettings::playerType(ePlayer playerColor){
    return m_playerType[static_cast<int>(playerColor)];
}

// Difficulty Level
void CSettings::setDifficulty(int difficulty){
    m_difficulty = (difficulty < 1 || difficulty > 9 ? 1 : difficulty);
}

int CSettings::difficulty(){
    return m_difficulty;
}

// Number of players
void CSettings::setNumberOfPlayers(int players){
    m_numberOfPlayers = 2;
}

int CSettings::numberOfPlayers(){
    return m_numberOfPlayers;
}

// Reset the settings
void CSettings::resetSettings(){
    m_boardColor = Qt::darkGreen;

    m_playerColors = QList<QColor>({Qt::white, Qt::black});
    m_playerNames = QList<QString>({"Player", "Computer"});
    m_playerType = QList<ePlayerType>({ePlayerType::Human, ePlayerType::Ai});

    m_difficulty = 1; //place holder for something more constructive later

    m_numberOfPlayers = 2; // this probably will never change but useful to have...
}
