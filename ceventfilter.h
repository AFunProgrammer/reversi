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

#ifndef CEVENTFILTER_H
#define CEVENTFILTER_H

#pragma once
#include <QEvent>
#include <QObject>

class CEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit CEventFilter(QObject *parent = nullptr);

signals:
    void clicked();  // Signal emitted when a mouse click occurs

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // CEVENTFILTER_H
