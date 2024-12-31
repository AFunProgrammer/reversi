#include "creversigame.h"

#include <QDebug>

// Global Instance
CReversiGame* CReversiGame::getGlobalInstance(){
    static CReversiGame g_ReversiGame;
    return &g_ReversiGame;
}


CReversiGame::CReversiGame(QSize BoardSize, QObject *parent)
    : QObject{parent}
{
    m_BoardSize = BoardSize;

    addPlayer("White", ePlayerType::Human);
    addPlayer("Black", ePlayerType::Human);
}

// return true if player added, otherwise false
bool CReversiGame::addPlayer(QString PlayerName, ePlayerType PlayerType)
{
    if ( m_Players.count() >= 2 ){
        return false;
    }

    // Create and assign new player data
    ReversiPlayer addPlayer;
    addPlayer.m_PlayerColor = (eColor)(m_Players.count());
    addPlayer.m_PlayerName = PlayerName;
    addPlayer.m_PlayerType = PlayerType;

    // add player to list of players
    m_Players.push_back(addPlayer);

    return true;
}

eColor m_PlayerColor = eColor::None;
QString m_PlayerName = "";
ePlayerType m_PlayerType = ePlayerType::Observer;
// get the player information to be adjusted
ReversiPlayer CReversiGame::getPlayer(eColor PlayerColor)
{
    for ( ReversiPlayer player: m_Players ){
        if ( player.m_PlayerColor == PlayerColor ){
            return player;
        }
    }

    // if the player wasn't found then either there's an error in the game or the
    //   color provided isn't correct - so return a player with the same color as "Not Found"
    ReversiPlayer playerNotFound = {PlayerColor, "Not Found", ePlayerType::Observer};
    return playerNotFound;
}

// set the player information that was needed to be adjusted (Color selects which
//   player is to be adjusted [either white or black]
// RETURNS true if set, otherwise false
bool CReversiGame::setPlayerInfo(eColor PlayerColor, QString PlayerName, ePlayerType PlayerType)
{
    for ( int Player = 0; Player < m_Players.count(); Player++ ){
        if (m_Players[Player].m_PlayerColor == PlayerColor ){
            m_Players[Player].m_PlayerName = PlayerName;
            m_Players[Player].m_PlayerType = PlayerType;

            // also check to see if the current player's turn
            if ( m_PlayerTurn.m_PlayerColor == PlayerColor ){
                m_PlayerTurn.m_PlayerName = PlayerName;
                m_PlayerTurn.m_PlayerType = PlayerType;
            }

            return true;
        }
    }

    return false;
}


// return true if a valid move, otherwise false
bool CReversiGame::makeMove(QPoint Spot)
{
    if ( m_Moves.count() >= (m_BoardSize.width() * m_BoardSize.height()) ){
        return false;
    }

    ReversiSpot move;
    move.m_SpotColor = m_PlayerTurn.m_PlayerColor;
    move.m_Spot = Spot;
    move.m_ChangedOnMove = m_Moves.count() + 1; // 1 based instead of 0 for moves

    if ( !isValidMove(move) ){
        return false;
    }

    // add move to list of moves
    m_Moves.push_back(move);

    // update the game board (flip any pieces that need to be flipped)
    updateBoard();

    // change the player to the next player (should only be 2 players)
    if ( m_PlayerTurn.m_PlayerColor == m_Players[0].m_PlayerColor ){
        m_PlayerTurn = m_Players[1];
    } else {
        m_PlayerTurn = m_Players[0];
    }

    return true;
}

// who's turn is it
ReversiPlayer CReversiGame::getPlayerTurn()
{
    return m_PlayerTurn;
}

// attempt to change move to next player because there aren't any valid moves found
bool CReversiGame::setNextTurnFromNoValidMoves()
{
    if ( getValidMoves(m_PlayerTurn.m_PlayerColor).count() == 0 ){
        // change the player to the next player (should only be 2 players)
        if ( m_PlayerTurn.m_PlayerColor == m_Players[0].m_PlayerColor ){
            m_PlayerTurn = m_Players[1];
        } else {
            m_PlayerTurn = m_Players[0];
        }

        qDebug() << "No Valid Moves Request For Change In Turn - New Player Turn: " << m_PlayerTurn.m_PlayerName;
    } else {
        return false; // there are valid moves
    }

    return true;
}


// get a list of valid movies
QList<QPoint> CReversiGame::getValidMoves(eColor PlayerColor)
{
    QList<QPoint> validMoves;
    QSet<ReversiSpot*> Locations;
    // looking for the color of the opposite player to get spots to move in to
    eColor lookForColor = (PlayerColor == eColor::Black ? eColor::White : eColor::Black);

    // cycle through the entire board
    for ( QList<ReversiSpot> row: m_Board ){
        for ( ReversiSpot col: row ){
            // check for any opposite color piece
            if ( col.m_SpotColor == lookForColor ){
                QList<ReversiSpot*> adjacent = getPrincipalWinds(col);

                // now check for any open spots next to that piece
                for ( ReversiSpot* spot: adjacent ){
                    if ( spot->m_SpotColor == eColor::None )
                        Locations.insert(spot);
                }
            }
        }
    }

    // check move for current player to see if a valid move
    for ( ReversiSpot* checkSpot: Locations ){
        ReversiSpot validateSpot = {PlayerColor, checkSpot->m_Spot, -1};

        if ( isValidMove(validateSpot) ){
            validMoves.append(checkSpot->m_Spot);
        }
    }

    // return list of spots for a valid move
    return validMoves;
}

// check to see if a move is valid
bool CReversiGame::isValidMove(ReversiSpot Spot)
{
    ReversiSpot DesiredSpot = m_Board[Spot.m_Spot.y()][Spot.m_Spot.x()];
    // check to see if spot is used
    if ( DesiredSpot.m_SpotColor != eColor::None ){
        return false;
    }

    // get all surrounding spots to check for opposite color
    QList<ReversiSpot*> adjacentSpots = getPrincipalWinds(Spot);
    bool bOppositeNotFound = true;
    for ( ReversiSpot* spot: adjacentSpots ){
        if ( spot->m_SpotColor != eColor::None && spot->m_SpotColor != Spot.m_SpotColor ){
            bOppositeNotFound = false;
        }
    }
    if ( bOppositeNotFound ){
        return false;
    }

    // check that there is a connecting element that can be used as an anchor to change colors
    QList<QList<ReversiSpot*>> lines = getPrincipalWindLines(Spot);
    bOppositeNotFound = true;
    for ( QList<ReversiSpot*> line: lines ){
        QList<int> idxList = getNearestSameColor(Spot, line);
        if ( idxList.count() > 0 ){
            bOppositeNotFound = false;
        }
    }
    if ( bOppositeNotFound ){
        return false;
    }

    return true;
}


// get the last move in the game
ReversiSpot CReversiGame::getLastMove()
{
    if ( m_Moves.count() == 0 ){
        return {eColor::None,QPoint(-1,-1),-1};
    }

    return m_Moves[m_Moves.count()-1];
}

// get the last move that a player made
ReversiSpot CReversiGame::getLastMove(eColor Color)
{
    if ( m_Moves.count() == 0 ){
        return {eColor::None,QPoint(-1,-1),-1};
    }

    if ( m_Moves.count() == 1 && m_Moves[m_Moves.count()-1].m_SpotColor != Color ){
        return {eColor::None,QPoint(-1,-1),-1};
    }

    int lastMove = m_Moves.count() - 1;
    while ( lastMove >= 0 ){
        if ( m_Moves[lastMove].m_SpotColor == Color ){
            return m_Moves[lastMove];
        }
        lastMove--;
    }

    return {eColor::None,QPoint(-1,-1),-1};
}

// get all moves made
QList<ReversiSpot> CReversiGame::getMoves()
{
    return m_Moves;
}

// get all moves made by a player
QList<ReversiSpot> CReversiGame::getMoves(eColor Color)
{
    QList<ReversiSpot> moves;

    for( ReversiSpot move: m_Moves ){
        if ( move.m_SpotColor == Color ){
            moves.append(move);
        }
    }

    return moves;
}

// get game board
QList<QList<ReversiSpot>> CReversiGame::getGameBoard()
{
    return m_Board;
}



// get the current score for a player
int CReversiGame::getPlayerScore(eColor Color)
{
    int score = 0;
    // cycle through the entire board
    for ( QList<ReversiSpot> row: m_Board ){
        for ( ReversiSpot col: row ){
            // check for color piece
            if ( col.m_SpotColor == Color ){
                score++;
            }
        }
    }

    return score;
}

// get list of players
QList<ReversiPlayer> CReversiGame::getPlayers()
{
    return m_Players;
}

// this randomly selects a player to go first and clears all moves (if any)
void CReversiGame::startGame()
{
    // Can't play if we got no players
    if ( m_Players.count() < 2 ){
        return;
    }

    // clear old moves
    m_Moves.clear();

    // Generate new board
    createBoard();

    // TODO: Make this random for selecting a player to start
    m_PlayerTurn = m_Players[0];
}


// get connecting colors index to selected spot from same line
QList<int> CReversiGame::getNearestSameColor(ReversiSpot Spot, QList<ReversiSpot*> Line)
{
    QList<int> nearestStartEnd;
    int pos = -1;
    int nearBehind = -1;
    int nearForward = -1;

    for ( int i = 0; i < Line.count(); i++ ){
        if ( Line[i]->m_Spot == Spot.m_Spot ){
            nearBehind = nearForward = pos = i;
            break;
        }
    }

    // Check from position backward
    for ( int i = pos-1; i >= 0; i-- ){
        if ( i >= Line.count() )
            break;
        if(Line[i]->m_SpotColor == eColor::None){
            break;
        } else if (Line[i]->m_SpotColor == Spot.m_SpotColor || Line[i]->m_SpotColor == eColor::None){
            nearBehind = i+1;
            break;
        }
    }

    // Check from position forward
    for ( int i = pos+1; i <= Line.count(); i++ ){
        if ( i >= Line.count() )
            break;
        if(Line[i]->m_SpotColor == eColor::None){
            break;
        } else if (Line[i]->m_SpotColor == Spot.m_SpotColor){
            nearForward = i-1;
            break;
        }
    }

    if ( nearBehind == nearForward && nearBehind == pos ){
        return nearestStartEnd; //Return empty list as there is no near same color
    }

    nearestStartEnd.append(nearBehind);
    nearestStartEnd.append(nearForward);

    return nearestStartEnd;
}

// get all adjacent spots from spot
QList<ReversiSpot*> CReversiGame::getPrincipalWinds(ReversiSpot Spot) {
    QList<ReversiSpot*> principalWind;

    int row = Spot.m_Spot.y();
    int col = Spot.m_Spot.x();

    for (int i = row - 1; i <= row + 1; ++i) {
        for (int j = col - 1; j <= col + 1; ++j) {
            // check for bounds, and don't include self
            if (i >= 0 && i < m_BoardSize.height() && j >= 0 && j < m_BoardSize.width() && !(i == row && j == col)) {
                principalWind.append(&m_Board[i][j]); //m_Board[row][col]
            }
        }
    }

    return principalWind;
}

// get all directional lines from spot
QList<QList<ReversiSpot*>> CReversiGame::getPrincipalWindLines(ReversiSpot Spot)
{
    QList<QList<ReversiSpot*>> principalWinds;

    principalWinds.append(getForwardDiagnol(Spot));
    principalWinds.append(getBackwardDiagnol(Spot));
    principalWinds.append(getColumn(Spot));
    principalWinds.append(getRow(Spot));

    return principalWinds;
}

QList<ReversiSpot*> CReversiGame::getForwardDiagnol(ReversiSpot Spot)
{
    QList<ReversiSpot*> boardLine;
    int row = Spot.m_Spot.y();
    int col = Spot.m_Spot.x();
    int width = m_BoardSize.width();
    int height = m_BoardSize.height();

    // Find the starting point of the diagonal
    while (row > 0 && col < width - 1) {
        row--;
        col++;
    }

    // Traverse the diagonal and add items to the list
    //  BUG BUG: check line after removing all parenthesis to see if it compiles (minus function())
    while ( row < height && col >= 0 ) {
        boardLine.append(&m_Board[row][col]); // Use append() to ensure order
        row++;
        col--;
    }

    return boardLine;
}

QList<ReversiSpot*> CReversiGame::getBackwardDiagnol(ReversiSpot Spot)
{
    QList<ReversiSpot*> boardLine;
    int row = Spot.m_Spot.y();
    int col = Spot.m_Spot.x();
    int width = m_BoardSize.width();
    int height = m_BoardSize.height();

    // Find the starting point of the diagonal
    while ( row > 0 && col > 0 ) {
        row--;
        col--;
    }

    // Traverse the diagonal and add items to the list
    while ( row < height && col < width ) {
        boardLine.append(&m_Board[row][col]); // Use append() to ensure order
        row++;
        col++;
    }

    return boardLine;
}

QList<ReversiSpot*> CReversiGame::getColumn(ReversiSpot Spot)
{
    QList<ReversiSpot*> boardLine = QList<ReversiSpot*>();
    int sRow = 0, eRow = m_BoardSize.height();

    while ( sRow < eRow ){
        boardLine.append(&m_Board[sRow][Spot.m_Spot.x()]);
        sRow++;
    }

    return boardLine;
}

QList<ReversiSpot*> CReversiGame::getRow(ReversiSpot Spot)
{
    QList<ReversiSpot*> boardLine = QList<ReversiSpot*>();
    int sCol = 0, eCol = m_BoardSize.width();

    while ( sCol < eCol ){
        boardLine.append(&m_Board[Spot.m_Spot.y()][sCol]);
        sCol++;
    }

    return boardLine;
}


// constructs a board with specific number of spots
void CReversiGame::createBoard()
{
    // clear lists first
    for ( auto &row: m_Board ){
        row.clear();
    }
    m_Board.clear();

    // create new empty board
    for ( int row = 0; row < m_BoardSize.height(); row++ ){
        m_Board.push_back(QList<ReversiSpot>());
        for ( int col = 0; col < m_BoardSize.width(); col++ ){
            ReversiSpot spot = {eColor::None, QPoint(col,row), -1};
            m_Board[row].push_back(spot);
        }
    }

    // add initial setup pieces (2 white, 2 black)
    // white, black
    // black, white
    int cRow = m_BoardSize.height() / 2;
    int cCol = m_BoardSize.width() / 2;
    m_Board[cRow-1][cCol-1].m_SpotColor = eColor::White;
    m_Board[cRow-1][cCol].m_SpotColor = eColor::Black;
    m_Board[cRow][cCol-1].m_SpotColor = eColor::Black;
    m_Board[cRow][cCol].m_SpotColor = eColor::White;

}

void CReversiGame::updateBoard()
{
    ReversiSpot lastMove = getLastMove();
    QList<QList<ReversiSpot*>> lines = getPrincipalWindLines(lastMove);

    for ( QList<ReversiSpot*> line: lines ){
        int moveNumber = lastMove.m_ChangedOnMove;
        QList<int> idxList = getNearestSameColor(lastMove, line);

        if ( idxList.count() > 0 ){
            for ( int i = idxList[0]; i <= idxList[1]; i++ ){
                line[i]->m_SpotColor = lastMove.m_SpotColor;
                line[i]->m_ChangedOnMove = moveNumber;
            }
        }
    }
}




