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

#include "cplayersettings.h"
#include "csettings.h"
#include "ui_playersettings.h"

#include "utility.h"

#include <QDebug>
#include <QVariant>
#include <QPushButton>
#include <QColorDialog>

CPlayerSettings::CPlayerSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CPlayerSettings)
{
    ui->setupUi(this);
    CSettings* pSettings = CSettings::getGlobalInstance();

    // Add known player types (not clean but effective)
    ui->cboPlayerType->addItem(tr("Computer"),  QVariant::fromValue(static_cast<int>(ePlayerType::Ai)));
    ui->cboPlayerType->addItem(tr("Human"),     QVariant::fromValue(static_cast<int>(ePlayerType::Human)));

    // Setup the control for player color changing
    ui->btnColor->connect(ui->btnColor, &QPushButton::clicked, [this,pSettings]() {
        // Create a color dialog
        QColorDialog colorPicker(this);
        colorPicker.setOption(QColorDialog::NoButtons);         // Hide OK/Cancel buttons
        colorPicker.setOption(QColorDialog::DontUseNativeDialog);
        // Show the dialog and get selected color
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");

        if(color.isValid()) {
            qDebug() << "selected color: " << color;
            QString colorName = color.name();
            QPixmap playerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", color, QSize(36,36)));
            ui->btnColor->setIcon(QIcon(playerIcon));
            pSettings->setPlayerColor(m_EditPlayer.m_Player, color);
        }
    });

    ui->btnClose->connect(ui->btnClose, &QPushButton::clicked, [this](){applyChanges();});
}

void CPlayerSettings::editPlayer(ReversiPlayer EditPlayer)
{
    CSettings* pSettings = CSettings::getGlobalInstance();

    m_EditPlayer = EditPlayer;

    // set the cboPlayerType to current text
    int index = ui->cboPlayerType->findData(static_cast<int>(EditPlayer.m_PlayerType));
    ui->cboPlayerType->setCurrentIndex(index);

    // set the player name for the specific color
    ui->txtName->setText(m_EditPlayer.m_PlayerName);

    // set the player color
    QPixmap playerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(m_EditPlayer.m_Player), QSize(40,40)));
    ui->btnColor->setIcon(QIcon(playerIcon));
}


void CPlayerSettings::applyChanges()
{
    // Get the settings to update them
    CSettings* pSettings = CSettings::getGlobalInstance();

    QString updateName = ui->txtName->text();
    ePlayerType playerType = (ePlayerType)(ui->cboPlayerType->currentData().toInt());

    // only update the settings and call game settings update after this dialog closes
    pSettings->setPlayerName(m_EditPlayer.m_Player, updateName);
    pSettings->setPlayerType(m_EditPlayer.m_Player, playerType);

    qDebug() << "Player Changes Applied for Player:" << (int)m_EditPlayer.m_Player << " Name: " << updateName << " Type: " << (int)playerType;

    this->accept();
}


CPlayerSettings::~CPlayerSettings()
{
    delete ui;
}
