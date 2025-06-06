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
void CSettings::setPlayerColor(eColor playerColor, QColor Color) {
    m_playerColors[static_cast<int>(playerColor)] = Color;
}

QColor CSettings::playerColor(eColor playerColor) {
    return m_playerColors[static_cast<int>(playerColor)];
}


// Player Names
void CSettings::setPlayerName(eColor playerColor, QString playerName){
    m_playerNames[static_cast<int>(playerColor)] = playerName;
}

QString CSettings::playerName(eColor playerColor){
    return m_playerNames[static_cast<int>(playerColor)];
}

// Player Types
void CSettings::setPlayerType(eColor playerColor, ePlayerType playerType){
    m_playerType[static_cast<int>(playerColor)] = playerType;
}

ePlayerType CSettings::playerType(eColor playerColor){
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
    m_playerType = QList<ePlayerType>({ePlayerType::Human, ePlayerType::Computer});

    m_difficulty = 1; //place holder for something more constructive later

    m_numberOfPlayers = 2; // this probably will never change but useful to have...
}
