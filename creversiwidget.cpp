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
                QRectF drawRect = QRectF(QPointF(row * m_CellSize.width(), col * m_CellSize.height()), m_CellSize);
                if ( Col.m_SpotColor == eColor::White ){
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_WhitePiece);
                } else {
                    paintGameBoard.drawPixmap(drawRect.toRect(), m_BlackPiece);
                }
            }
        }
    }

    // Draw Next Valid Move For Current Player
    QList<QPoint> validMoves = pGame->getValidMoves();
    eColor currentPlayer = pGame->getPlayerTurn().m_PlayerColor;
    for( QPoint pos: validMoves ){
        int row = pos.y();
        int col = pos.x();
        QRectF drawRect = QRectF(QPointF(row * m_CellSize.width(), col * m_CellSize.height()), m_CellSize);
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

    return drawRect;
}

void CReversiWidget::setClearColor(QColor Color)
{
    m_ClearColor = Color;
}

void CReversiWidget::resizeEvent(QResizeEvent *event)
{
    QOpenGLWidget::resizeEvent(event);
    // create a new square board every time the widget changes size
    createBoard(QSize(8,8));
}

void CReversiWidget::mousePressEvent(QMouseEvent* /*event*/)
{

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

    drawGameOnBoard();
    //Painter.drawPixmap(getBoardDrawRect(event->rect()),m_Board);
    Painter.drawPixmap(getBoardDrawRect(event->rect()),m_GameBoard);

    Painter.end();
}
