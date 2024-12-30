#ifndef CREVERSIGAME_H
#define CREVERSIGAME_H

#pragma once
#include <QList>
#include <QObject>
#include <QPoint>
#include <QSet>
#include <QSize>

enum class eGameType{
    Local = 0x0,
    Online
};

enum class ePlayerType{
    Computer = 0x0,
    Human,
    Observer
};

enum class eColor{
    White = 0x0,
    Black,
    None
};

typedef struct S_ReversiPlayer{
    eColor m_PlayerColor = eColor::None;
    QString m_PlayerName = "";
    ePlayerType m_PlayerType = ePlayerType::Observer;
}ReversiPlayer,*PReversiPlayer;

typedef struct S_ReversiSpot{
    eColor m_SpotColor = eColor::None;
    QPoint m_Spot = QPoint(-1,-1);
    int m_ChangedOnMove = -1;
}ReversiSpot,*PReversiSpot;

class CReversiGame : public QObject
{
    Q_OBJECT
public:
    explicit CReversiGame(QSize BoardSize = QSize(8,8), QObject *parent = nullptr);

    // return true if player added, otherwise false
    bool addPlayer(QString PlayerName, ePlayerType PlayerType);
    // return true if a valid move, otherwise false
    bool makeMove(QPoint Spot);

    // who's turn is it
    ReversiPlayer getPlayerTurn();

    // get a list of valid movies
    QList<QPoint> getValidMoves();

    // get the last move in the game
    ReversiSpot getLastMove();
    // get the last move that a player made
    ReversiSpot getLastMove(eColor Color);

    // get all moves made
    QList<ReversiSpot> getMoves();
    // get all moves made by a player
    QList<ReversiSpot> getMoves(eColor Color);

    // get game board
    QList<QList<ReversiSpot>> getGameBoard();

    // get the current score for a player
    int getPlayerScore(eColor Color);

    // get list of players
    QList<ReversiPlayer> getPlayers();

    // this randomly selects a player to go first and clears all moves (if any)
    void startGame();

    // have a single global instance
    static CReversiGame* getGlobalInstance();
private:
    QList<ReversiPlayer> m_Players;
    ReversiPlayer m_PlayerTurn;

    QList<ReversiSpot> m_Moves;

    QSize m_BoardSize;
    QList<QList<ReversiSpot>> m_Board;


    // constructs a board with specific number of spots
    void createBoard();
    // update board after a valid move has been made
    void updateBoard();

    // get connecting colors index to selected spot from same line
    QList<int> getNearestSameColor(ReversiSpot Spot, QList<ReversiSpot*> Line);

    // check to see if a move is valid
    bool isValidMove(ReversiSpot Spot);

    // get all adjacent spots from spot
    QList<ReversiSpot*> getPrincipalWinds(ReversiSpot Spot);

    // get all directional lines from spot
    QList<QList<ReversiSpot*>> getPrincipalWindLines(ReversiSpot Spot);
    QList<ReversiSpot*> getForwardDiagnol(ReversiSpot Spot);
    QList<ReversiSpot*> getBackwardDiagnol(ReversiSpot Spot);
    QList<ReversiSpot*> getColumn(ReversiSpot Spot);
    QList<ReversiSpot*> getRow(ReversiSpot Spot);

signals:
};

#endif // CREVERSIGAME_H
