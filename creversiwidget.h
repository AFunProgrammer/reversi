/*
 * This file is part of reversi.
 *
 * Copyright (C) 2024-2025 InspiringFamily.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see http://www.gnu.org/licenses/.
 */

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
