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
