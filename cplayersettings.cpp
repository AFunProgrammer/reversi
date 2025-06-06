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
    ui->cboPlayerType->addItem(tr("Computer"),  QVariant::fromValue(static_cast<int>(ePlayerType::Computer)));
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
            QPixmap playerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", color, QSize(40,40)));
            ui->btnColor->setIcon(QIcon(playerIcon));
            pSettings->setPlayerColor(m_EditPlayer.m_PlayerColor, color);
        }
    });

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Close), &QPushButton::clicked, [this](){applyChanges();});
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
    QPixmap playerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(m_EditPlayer.m_PlayerColor), QSize(40,40)));
    ui->btnColor->setIcon(QIcon(playerIcon));
}


void CPlayerSettings::applyChanges()
{
    // Get the settings to update them
    CSettings* pSettings = CSettings::getGlobalInstance();

    QString updateName = ui->txtName->text();
    ePlayerType playerType = (ePlayerType)(ui->cboPlayerType->currentData().toInt());

    // only update the settings and call game settings update after this dialog closes
    pSettings->setPlayerName(m_EditPlayer.m_PlayerColor, updateName);
    pSettings->setPlayerType(m_EditPlayer.m_PlayerColor, playerType);

    qDebug() << "Player Changes Applied for Player:" << (int)m_EditPlayer.m_PlayerColor << " Name: " << updateName << " Type: " << (int)playerType;

    this->accept();
}


CPlayerSettings::~CPlayerSettings()
{
    delete ui;
}
