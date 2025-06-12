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
