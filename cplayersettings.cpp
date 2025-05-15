#include "cplayersettings.h"
#include "ui_playersettings.h"

#include <QDebug>
#include <QVariant>
#include <QPushButton>

CPlayerSettings::CPlayerSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CPlayerSettings)
{
    ui->setupUi(this);

    // Add known player types (not clean but effective)
    ui->cboPlayerType->addItem(tr("Computer"),  QVariant::fromValue(static_cast<int>(ePlayerType::Computer)));
    ui->cboPlayerType->addItem(tr("Human"),     QVariant::fromValue(static_cast<int>(ePlayerType::Human)));

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, [this](){applyChanges();});
    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Close), &QPushButton::clicked, [this](){cancelChangers();});
}

void CPlayerSettings::editPlayer(ReversiPlayer EditPlayer)
{
    m_EditPlayer = EditPlayer;

    // set the cboPlayerType to current text
    int index = ui->cboPlayerType->findData(static_cast<int>(EditPlayer.m_PlayerType));
    ui->cboPlayerType->setCurrentIndex(index);

    // set the player name for the specific color
    ui->txtName->setText(m_EditPlayer.m_PlayerName);
}


void CPlayerSettings::applyChanges()
{
    // Get the game to update the player data
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    QString updateName = ui->txtName->text();
    ePlayerType playerType = (ePlayerType)(ui->cboPlayerType->currentData().toInt());

    pGame->setPlayerInfo(m_EditPlayer.m_PlayerColor, updateName, playerType);

    qDebug() << "Player Changes Applied for Player:" << (int)m_EditPlayer.m_PlayerColor << " Name: " << updateName << " Type: " << (int)playerType;

    this->accept();
}

void CPlayerSettings::cancelChangers()
{
    qDebug() << "Cancel Changes Clicked";

    this->reject();
}


CPlayerSettings::~CPlayerSettings()
{
    delete ui;
}
