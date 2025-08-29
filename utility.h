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

#ifndef UTILITY_H
#define UTILITY_H

#pragma once

#include <QDomDocument>

#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>

#include <QDebug>

#include <QFile>
#include <QColor>

// Helper function to get shades for the main gradient
void getGradientColors(const QColor& baseColor, QColor& lighter, QColor& darker);

// Helper function to get colors for the highlight gradient
void getHighlightColors(const QColor& baseColor, QColor& highlightStart, QColor& highlightEnd);

QImage createColoredSvgButton(const QString& resourcePath, const QColor& userColor, const QSize& targetSize);

// Function to recolor the SVG icon
QImage createColoredMovePiece(const QString& resourcePath, const QColor& userColor, const QSize& targetSize);


#endif // UTILITY_H
