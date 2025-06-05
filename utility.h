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
