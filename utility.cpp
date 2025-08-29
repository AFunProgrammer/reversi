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

#include "utility.h"



// Helper function to get shades for the main gradient
void getGradientColors(const QColor& baseColor, QColor& lighter, QColor& darker) {
    int h, s, l, a;
    baseColor.getHsl(&h, &s, &l, &a);

    // Darken for the start of the gradient
    // Adjust these factors based on how much darker/lighter you want the stops to be.
    // Ensure 'l' stays within 0-255 range.
    darker.setHsl(h, s, qBound(0, l - 50, 255), a); // Adjust '-50' for desired darkness
    lighter.setHsl(h, s, qBound(0, l + 50, 255), a); // Adjust '+50' for desired lightness
}

// Helper function to get colors for the highlight gradient
void getHighlightColors(const QColor& baseColor, QColor& highlightStart, QColor& highlightEnd) {
    int h, s, l, a;
    baseColor.getHsl(&h, &s, &l, &a);

    // Highlight start (brighter, less saturated version of base)
    // Maybe closer to white or a very light version of the base color.
    highlightStart.setHsl(h, qBound(0, s - 50, 255), qBound(0, l + 80, 255), a); // More saturated, brighter
    // Highlight end (fades out, so it can be a neutral gray or even a darker version of the base color
    // with alpha 0. For a "glass" effect, white fading to transparent is common.
    highlightEnd.setHsl(h, qBound(0, s - 100, 255), qBound(0, l + 20, 255), 0); // Fades to transparent, slightly desaturated
}

QImage createColoredSvgButton(const QString& resourcePath, const QColor& userColor, const QSize& targetSize) {
    // Load SVG from Qt resource system
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open SVG resource: %s", qPrintable(resourcePath));
        return QImage();
    }
    QByteArray svgData = file.readAll();
    file.close();

    // Parse SVG content
    QDomDocument document;
    if (!document.setContent(svgData)) {
        qWarning("Failed to parse SVG content.");
        return QImage();
    }

    // Prepare color variations outside the loop
    QColor mainStop1Color, mainStop2Color;
    getGradientColors(userColor, mainStop1Color, mainStop2Color);

    QColor highlightStartColor, highlightEndColor;
    getHighlightColors(userColor, highlightStartColor, highlightEndColor);

    // Modify gradients by iterating through all linearGradient elements
    QDomNodeList gradients = document.elementsByTagName("linearGradient");
    for (int i = 0; i < gradients.size(); ++i) {
        QDomElement gradient = gradients.at(i).toElement();
        QString gradientId = gradient.attribute("id");

        QDomNodeList stops = gradient.elementsByTagName("stop");
        if (stops.size() >= 2) {
            if (gradientId == "linearGradient3601") {
                // Main body gradient
                stops.at(0).toElement().setAttribute("style", "stop-color:" + mainStop1Color.name() + ";stop-opacity:1;");
                stops.at(1).toElement().setAttribute("style", "stop-color:" + mainStop2Color.name() + ";stop-opacity:1;");
            } else if (gradientId == "linearGradient3614") {
                // Highlight/Gloss gradient
                stops.at(0).toElement().setAttribute("style", "stop-color:" + highlightStartColor.name() + ";stop-opacity:1;");
                // The second stop of the highlight gradient should be transparent for the fade effect
                stops.at(1).toElement().setAttribute("style", "stop-color:" + highlightEndColor.name() + ";stop-opacity:0;");
            }
        }
    }

    // Convert modified SVG back to QByteArray
    QByteArray modifiedSvgData = document.toByteArray();

    // Render the modified SVG to a QImage
    QSvgRenderer renderer(modifiedSvgData);
    QImage image(targetSize, QImage::Format_ARGB32); // Use ARGB32 for alpha channel
    image.fill(Qt::transparent); // Ensure background is transparent

    QPainter painter(&image);
    // If the renderer is invalid, it won't draw anything. You might want to check isValid()
    if (renderer.isValid()) {
        renderer.render(&painter);
    } else {
        qWarning("Modified SVG content is invalid for rendering.");
    }

    return image;
}

// Function to recolor the SVG icon
QImage createColoredMovePiece(const QString& resourcePath, const QColor& userColor, const QSize& targetSize) {
    // Load SVG from Qt resource system
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Failed to open SVG resource: %s", qPrintable(resourcePath));
        return QImage();
    }
    QByteArray svgData = file.readAll();
    file.close();

    // Parse SVG content
    QDomDocument document;
    if (!document.setContent(svgData)) {
        qWarning("Failed to parse SVG content.");
        return QImage();
    }

    // Get the root SVG element
    QDomElement svgRoot = document.documentElement();

    // Check if the root element is valid and is indeed an "svg" tag
    if (svgRoot.isNull() || svgRoot.tagName() != "svg") {
        qWarning("Invalid SVG root element found.");
        return QImage();
    }

    // Set the fill attribute of the root SVG element to the user's color.
    // QColor::name(QColor::HexRgb) returns the color in "#RRGGBB" format.
    svgRoot.setAttribute("fill", userColor.name(QColor::HexRgb));

    // Convert modified SVG back to QByteArray
    QByteArray modifiedSvgData = document.toByteArray();

    // Render the modified SVG to a QImage
    QSvgRenderer renderer(modifiedSvgData);
    QImage image(targetSize, QImage::Format_ARGB32); // Use ARGB32 for alpha channel
    image.fill(Qt::transparent); // Ensure background is transparent

    QPainter painter(&image);
    // It's good practice to check if the renderer is valid before rendering
    if (renderer.isValid()) {
        renderer.render(&painter);
    } else {
        qWarning("Modified SVG content is invalid for rendering.");
    }

    return image;
}
