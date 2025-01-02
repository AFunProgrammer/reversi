#include "creversiwidget.h"

#include <QMouseEvent>
#include <QPaintEvent>
#include <QResizeEvent>

#include <QPainter>
#include <QPixmap>

#include <QDebug>
#include <QRandomGenerator>
#include <QTimer>

#include "creversigame.h"

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
    drawBoard.setBrush(QBrush(Qt::darkGreen));

    for( int row = 0; row < BoardSize.height(); row++ ){
        for( int col = 0; col < BoardSize.width(); col++ ){
            QRectF drawRect = QRectF(QPointF(row * cellSize.width(), col * cellSize.height()), cellSize);
            drawBoard.drawRect(drawRect);
        }
    }

    drawBoard.end();

    m_Board = board;

    // Setup game pieces
    m_WhiteMove  = QPixmap(":/images/whitemove");
    m_WhitePiece = QPixmap(":/images/whitepiece");
    m_BlackMove  = QPixmap(":/images/blackmove");
    m_BlackPiece = QPixmap(":/images/blackpiece");

    m_WhiteMove  = m_WhiteMove.scaled(m_CellSize.toSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    m_WhitePiece = m_WhitePiece.scaled(m_CellSize.toSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    m_BlackMove  = m_BlackMove.scaled(m_CellSize.toSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    m_BlackPiece = m_BlackPiece.scaled(m_CellSize.toSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);

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
            if ( Col.m_SpotColor != eColor::None ){
                int row = Col.m_Spot.y();
                int col = Col.m_Spot.x();
                QRectF drawRect = QRectF(QPointF(col * m_CellSize.width(), row * m_CellSize.height()), m_CellSize);
                if ( Col.m_SpotColor == eColor::White ){
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_WhitePiece);
                } else {
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_BlackPiece);
                }
            }
        }
    }

    // Draw Next Valid Move For Current Player
    eColor currentPlayer = pGame->getPlayerTurn().m_PlayerColor;
    QList<QPoint> validMoves = pGame->getValidMoves(currentPlayer);

    for( QPoint pos: validMoves ){
        int row = pos.y();
        int col = pos.x();
        float offsetX = m_CellSize.width() / 2 - (m_CellSize.width() / 4);
        float offsetY = m_CellSize.height() / 2 - (m_CellSize.height() / 4);
        QPointF origin = QPointF( (col * m_CellSize.width() + offsetX), (row * m_CellSize.height() + offsetY) );

        QRectF drawRect = QRectF(origin, m_CellSize / 2);
        if ( currentPlayer == eColor::White ){
            paintGameBoard.drawPixmap(drawRect.toRect(), m_WhiteMove);
        } else {
            paintGameBoard.drawPixmap(drawRect.toRect(), m_BlackMove);
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
}

void CReversiWidget::mousePressEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();
    QPoint cellOrigin;

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
    QColor lastMoveColor = (lastMove.m_SpotColor == eColor::White ? Qt::white : Qt::black);

    static float opacity = 0.3f;
    static bool increase = true;
    static eColor lastColor = eColor::White;

    if ( lastColor == lastMove.m_SpotColor ){
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
        lastColor = lastMove.m_SpotColor;
    }

    /* show where the last move occured */
    lastMoveColor.setAlphaF(opacity);
    Painter->setPen(QPen(lastMoveColor, 4));
    Painter->setBrush(QBrush(lastMoveColor));

    QRect lastMoveRect = convertCellToRect(lastMove.m_Spot);
    Painter->drawRect(lastMoveRect);
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
