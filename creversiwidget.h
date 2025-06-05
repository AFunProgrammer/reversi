#ifndef CREVERSIWIDGET_H
#define CREVERSIWIDGET_H

#pragma once

#include <QOpenGLWidget>

QT_BEGIN_NAMESPACE
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QPixmap;
QT_END_NAMESPACE

class CReversiWidget : public QOpenGLWidget
{
public:
    CReversiWidget(QWidget* parent);

    void setClearColor(QColor Color);

    void setBoardColor(QColor Color);
    QColor boardColor();

    void setPlayerColor(int Player, QColor Color);
    QColor playerColor(int Player);

    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    QPoint lastClickedCell();

private:
    QColor m_ClearColor = Qt::black;
    QColor m_BoardColor = Qt::darkGreen;

    QSize m_BoardSize;
    QSizeF m_CellSize;
    QPixmap m_Board;

    QRect m_LastClickRect = QRect(-1,-1,-1,-1);
    QPoint m_LastDrawOrigin = QPoint(-1,-1);
    QPoint m_LastClickCell = QPoint(-1,-1);

    QPixmap m_GameBoard;
    QPixmap m_firstPlayerMove;
    QPixmap m_firstPlayerPiece;
    QPixmap m_secondPlayerMove;
    QPixmap m_secondPlayerPiece;

    QColor m_firstPlayerColor = Qt::white;
    QColor m_secondPlayerColor = Qt::black;

    void createBoard(QSize BoardSize /* typically 8x8 */);
    void createPieces();

    void drawGameOnBoard();

    QRect getBoardDrawRect(QRect PaintRect);

    QRect convertCellToRect(QPoint Cell);

    void drawLastMoveIndicator(QPainter* Painter);
};

#endif // CREVERSIWIDGET_H
