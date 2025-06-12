#ifndef CSETTINGS_H
#define CSETTINGS_H

#pragma once

#include "creversigame.h"
#include <QObject>
#include <QtGui/qcolor.h>

// All Settable Options for the game

class CSettings : public QObject
{
    Q_OBJECT
public:
    explicit CSettings(QObject *parent = nullptr);

    void setBoardColor(QColor Color);
    QColor boardColor();

    void setPlayerColor(ePlayer playerColor, QColor Color);
    QColor playerColor(ePlayer playerColor);

    void setPlayerName(ePlayer playerColor, QString playerName);
    QString playerName(ePlayer playerColor);

    void setPlayerType(ePlayer playerColor, ePlayerType playerType);
    ePlayerType playerType(ePlayer playerColor);

    void setDifficulty(int difficulty);
    int difficulty();

    void setNumberOfPlayers(int players);
    int numberOfPlayers();


    void resetSettings();
    // have a single global instance
    static CSettings* getGlobalInstance();
private:
    QColor m_boardColor = Qt::darkGreen;

    QList<QColor> m_playerColors = QList<QColor>({Qt::white,Qt::black});
    QList<QString> m_playerNames = QList<QString>({"Player","Computer"});
    QList<ePlayerType> m_playerType = QList<ePlayerType>({ePlayerType::Human,ePlayerType::Ai});

    int m_difficulty = 1; //place holder for something more constructive later

    int m_numberOfPlayers = 2; // this probably will never change but useful to have...

signals:
};

#endif // CSETTINGS_H
