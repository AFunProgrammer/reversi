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

    void setPlayerColor(eColor playerColor, QColor Color);
    QColor playerColor(eColor playerColor);

    void setPlayerName(eColor playerColor, QString playerName);
    QString playerName(eColor playerColor);

    void setPlayerType(eColor playerColor, ePlayerType playerType);
    ePlayerType playerType(eColor playerColor);

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
    QList<ePlayerType> m_playerType = QList<ePlayerType>({ePlayerType::Human,ePlayerType::Computer});

    int m_difficulty = 1; //place holder for something more constructive later

    int m_numberOfPlayers = 2; // this probably will never change but useful to have...

signals:
};

#endif // CSETTINGS_H
