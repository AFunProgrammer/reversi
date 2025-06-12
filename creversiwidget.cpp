#include "creversiwidget.h"

#include <QApplication>

#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>

#include <QPainter>
#include <QPixmap>

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>

#include <QColor>

#include "creversigame.h"
#include "utility.h"

CReversiWidget::CReversiWidget(QWidget* parent) : QOpenGLWidget{parent}
{
    setAutoFillBackground(false);
}


void CReversiWidget::createBoard(QSize BoardSize)
{
    QSize wSize = geometry().size();
    int maxBoardSize = (wSize.height() >= wSize.width() ? wSize.width() : wSize.height());
    QSizeF cellSize = QSizeF((float)maxBoardSize / (float)BoardSize.width(), (float)maxBoardSize / (float)BoardSize.height());

    // set globals to be used when adding drawing pieces
    m_BoardSize = QSize(maxBoardSize,maxBoardSize);
    m_CellSize = cellSize;

    QPixmap board = QPixmap(maxBoardSize,maxBoardSize);
    board.fill(Qt::transparent);

    QPainter drawBoard = QPainter(&board);
    QPen boardPen = QPen(Qt::white);
    boardPen.setWidth(2);
    drawBoard.setPen(boardPen);
    drawBoard.setBrush(QBrush(m_BoardColor));

    for( int row = 0; row < BoardSize.height(); row++ ){
        for( int col = 0; col < BoardSize.width(); col++ ){
            QRectF drawRect = QRectF(QPointF(row * cellSize.width(), col * cellSize.height()), cellSize);
            drawBoard.drawRect(drawRect);
        }
    }

    drawBoard.end();

    m_Board = board;
}

void CReversiWidget::createPieces(){
    // Setup game pieces
    m_firstPlayerPiece = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", m_firstPlayerColor, QSize(128,128)));
    m_secondPlayerPiece = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", m_secondPlayerColor, QSize(128,128)));
    m_firstPlayerMove = QPixmap::fromImage(createColoredMovePiece(":/images/flatmove", m_firstPlayerColor, QSize(128,128)));
    m_secondPlayerMove = QPixmap::fromImage(createColoredMovePiece(":/images/flatmove", m_secondPlayerColor, QSize(128,128)));
}

void CReversiWidget::drawGameOnBoard()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();
    QList<QList<ReversiSpot>> gameBoard = pGame->getGameBoard();

    if ( m_Board.rect().size().width() <= 0 )
        return; //it's tool soon

    m_GameBoard = QPixmap(m_Board.rect().size());
    QPainter paintGameBoard = QPainter(&m_GameBoard);

    paintGameBoard.drawPixmap(0,0,m_Board);

    // Draw Each Cell That Isn't eColor::None
    for ( QList<ReversiSpot> Row: gameBoard ){
        for ( ReversiSpot Col: Row ){
            if ( Col.m_CapturedBy != ePlayer::None ){
                int row = Col.m_Spot.y();
                int col = Col.m_Spot.x();
                QRectF drawRect = QRectF(QPointF(col * m_CellSize.width(), row * m_CellSize.height()), m_CellSize);
                if ( Col.m_CapturedBy == ePlayer::First ){
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_firstPlayerPiece);
                } else {
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_secondPlayerPiece);
                }
            }
        }
    }

    // Draw Next Valid Move For Current Player
    ePlayer currentPlayer = pGame->getPlayerTurn().m_Player;
    QList<QPoint> validMoves = pGame->getValidMoves(currentPlayer);

    for( QPoint pos: validMoves ){
        int row = pos.y();
        int col = pos.x();
        float offsetX = m_CellSize.width() / 2 - (m_CellSize.width() / 4);
        float offsetY = m_CellSize.height() / 2 - (m_CellSize.height() / 4);
        QPointF origin = QPointF( (col * m_CellSize.width() + offsetX), (row * m_CellSize.height() + offsetY) );

        QRectF drawRect = QRectF(origin, m_CellSize / 2);
        if ( currentPlayer == ePlayer::First ){
            paintGameBoard.drawPixmap(drawRect.toRect(), m_firstPlayerMove);
        } else {
            paintGameBoard.drawPixmap(drawRect.toRect(), m_secondPlayerMove);
        }
    }

    paintGameBoard.end();
}


QRect CReversiWidget::getBoardDrawRect(QRect PaintRect)
{
    QRect drawRect = QRect(QPoint(0,0),m_Board.size());
    drawRect.moveCenter(PaintRect.center());

    m_LastDrawOrigin = drawRect.topLeft();

    return drawRect;
}

void CReversiWidget::setClearColor(QColor Color)
{
    m_ClearColor = Color;
}

void CReversiWidget::setBoardColor(QColor Color){
    qDebug() << "Color is Valid: " << Color.isValid() << " Color Value: " << Color.rgb() << " Board Color: " << m_BoardColor.rgb();

    if (Color.isValid() && Color.rgb() != m_BoardColor.rgb()){
        m_BoardColor = Color;
        // create a new square board after color update
        createBoard(QSize(8,8));
    }
}

QColor CReversiWidget::boardColor(){
    return m_BoardColor;
}

void CReversiWidget::setPlayerColor(int Player, QColor Color){
    bool colorChanged = false;
    if (Player == 0){
        if ( Color.rgb() != m_firstPlayerColor.rgb() )
            colorChanged = true;

        m_firstPlayerColor = (Color.isValid() ? Color : Qt::white);
    }
    else if (Player == 1) {
        if ( Color.rgb() != m_secondPlayerColor.rgb() )
            colorChanged = true;

        m_secondPlayerColor = (Color.isValid() ? Color : Qt::black);
    }


    if (colorChanged) {
        createPieces();
    }
}

QColor CReversiWidget::playerColor(int Player){
    if (Player == 0)
        return m_firstPlayerColor;
    else
        return m_secondPlayerColor;

}


QPoint CReversiWidget::lastClickedCell()
{
    QPoint returnLastClickCell = m_LastClickCell;

    // clear all previous clicked information
    m_LastClickCell = QPoint(-1,-1);
    m_LastClickRect = QRect(-1,-1,-1,-1);

    return returnLastClickCell;
}

void CReversiWidget::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    // create a new square board every time the widget changes size
    createBoard(QSize(8,8));
    createPieces();
}

void CReversiWidget::mousePressEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();

    clickPos.setY(clickPos.y() - m_LastDrawOrigin.y());
    clickPos.setX(clickPos.x() - m_LastDrawOrigin.x());

    if ( clickPos.x() > m_Board.width() || clickPos.y() > m_Board.height() ){
        return; // did not click on the board
    }

    int col = clickPos.x() / (int)m_CellSize.width();
    int row = clickPos.y() / (int)m_CellSize.height();

    m_LastClickCell = QPoint(col,row);
    m_LastClickRect = convertCellToRect(m_LastClickCell);

    // if it's a human player's turn, then make a move
    if ( CReversiGame::getGlobalInstance()->getPlayerTurn().m_PlayerType == ePlayerType::Human){
        CReversiGame::getGlobalInstance()->makeMove(m_LastClickCell);
        this->update();
    }
}

QRect CReversiWidget::convertCellToRect(QPoint Cell)
{
    QPoint cellOrigin;
    QRect cellRect;

    cellOrigin = QPointF(Cell.x() * m_CellSize.width(), Cell.y() * m_CellSize.height()).toPoint();
    cellOrigin.setX(cellOrigin.x() + m_LastDrawOrigin.x());
    cellOrigin.setY(cellOrigin.y() + m_LastDrawOrigin.y());

    cellRect = QRect(cellOrigin, m_CellSize.toSize());

    return cellRect;
}

void CReversiWidget::drawLastMoveIndicator(QPainter* Painter)
{
    ReversiSpot lastMove = CReversiGame::getGlobalInstance()->getLastMove();
    QColor lastMoveColor = (lastMove.m_CapturedBy == ePlayer::First ? m_firstPlayerColor : m_secondPlayerColor);

    static float opacity = 0.3f;
    static bool increase = true;
    static ePlayer lastColor = ePlayer::First;

    if ( lastColor == lastMove.m_CapturedBy ){
        if ( increase ){
            if ( opacity <= 0.8f ){
                opacity += 0.05f;
            }
            else{
                increase = false;
            }
        } else {
            if ( opacity >= 0.3f ){
                opacity -= 0.05f;
            } else {
                increase = true;
            }
        }
    } else {
        opacity = 0.3f;
        increase = true;
        lastColor = lastMove.m_CapturedBy;
    }

    /* show where the last move occured */
    lastMoveColor.setAlphaF(opacity);
    Painter->setPen(QPen(lastMoveColor, 4));
    Painter->setBrush(QBrush(lastMoveColor));

    if ( lastMove.m_Spot != QPoint(-1,-1))
    {
        QRect lastMoveRect = convertCellToRect(lastMove.m_Spot);
        Painter->drawRect(lastMoveRect);
    }
}


void CReversiWidget::paintEvent(QPaintEvent *event)
{
    QPainter Painter(this);
    QBrush brushBackground = QBrush(m_ClearColor);

    //Painter.begin(this);
    //glEnable(GL_MULTISAMPLE);
    //glEnable(GL_LINE_SMOOTH);
    Painter.setRenderHint(QPainter::Antialiasing);
    Painter.setRenderHint(QPainter::SmoothPixmapTransform);
    Painter.fillRect(event->rect(),brushBackground);

    // Draw current game setup with moves on the game board
    drawGameOnBoard();

    //Painter.drawPixmap(getBoardDrawRect(event->rect()),m_Board);
    Painter.drawPixmap(getBoardDrawRect(event->rect()),m_GameBoard);

    /* making sure input is correct - only in debug */
    // Painter.setPen(QPen(Qt::red,4));
    // Painter.drawRect(m_LastClickRect);
    drawLastMoveIndicator(&Painter);


    Painter.end();
}
