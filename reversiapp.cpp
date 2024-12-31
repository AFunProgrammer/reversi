#include "reversiapp.h"
#include "ui_reversiapp.h"

#include "creversigame.h"

#include <QDebug>
#include <QTimer>
#include <QRandomGenerator>

bool areThereAnyValidMovesLeft(ReversiPlayer* Player, QList<QPoint>* ValidMoves){
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    // check for any valid moves, if not see if the next player has moves
    if ( ValidMoves->count() == 0 ){
        eColor checkColorForMoves = eColor::None;
        // change the player to the next player (should only be 2 players)
        if ( Player->m_PlayerColor == pGame->getPlayers()[0].m_PlayerColor ){
            checkColorForMoves = pGame->getPlayers()[1].m_PlayerColor;
        } else {
            checkColorForMoves = pGame->getPlayers()[0].m_PlayerColor;
        }

        // check to see if the other player has any moves
        if ( pGame->getValidMoves(checkColorForMoves).count() > 0 ){
            // change player
            pGame->setNextTurnFromNoValidMoves();
            // get data to be used for movements
            *Player = pGame->getPlayerTurn();
            *ValidMoves = pGame->getValidMoves(Player->m_PlayerColor);
        } else {
            //nobody has any moves, end the game
            return false;
        }
    }

    return true;
}

void makeComputerMove(QList<QPoint> validMoves)
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();
    ReversiPlayer player = pGame->getPlayerTurn();

    int randomIndex = QRandomGenerator::global()->generate() % validMoves.size();
    QPoint move = validMoves.at(randomIndex);
    qDebug() << "chose Random move at: " << move << " For Player: " << (player.m_PlayerColor == eColor::Black? "Black" : "White");
    pGame->makeMove(move);
}

void ReversiApp::selectNextMoveForComputer()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    ReversiPlayer player = pGame->getPlayerTurn();
    QList<QPoint> validMoves = pGame->getValidMoves(player.m_PlayerColor);

    // Check to see if all moves have been done, if so then don't try to move anymore
    if ( pGame->getMoves().count() <= 60 && m_gameStart ){
        validMoves = pGame->getValidMoves(player.m_PlayerColor);

        if ( areThereAnyValidMovesLeft(&player, &validMoves) == false ){
            m_gameStart = false;
        }

        if ( player.m_PlayerType == ePlayerType::Computer && m_gameStart == true){
            makeComputerMove(validMoves);
        } else if (player.m_PlayerType == ePlayerType::Human && m_gameStart == true){
            /* Human moves are done on the widget, not in the timer */
        }
    } else {
        m_gameStart = false;
    }

    updateUiForGameProgress();
}

void ReversiApp::updateUiForGameProgress()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    int WhiteScore = pGame->getPlayerScore(eColor::White);
    int BlackScore = pGame->getPlayerScore(eColor::Black);

    QString formattedWhiteScore = QString::number(WhiteScore,10).rightJustified(2,'0');
    QString formattedBlackScore = QString::number(BlackScore,10).rightJustified(2,'0');

    ui->lblWhiteScore->setText(formattedWhiteScore);
    ui->lblBlackScore->setText(formattedBlackScore);

    ui->oglReversi->update();
}

void ReversiApp::updatePlayerSettings(eColor PlayerColor)
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();
    ReversiPlayer editPlayer = pGame->getPlayer(PlayerColor);

    m_playerSettingsDlg.editPlayer(editPlayer);

    m_playerSettingsDlg.setModal(true);

#if defined(Q_OS_ANDROID)
    // Adjust the width to the screen width (or center in the screen)
    //fileDialog.setWindowState(Qt::WindowMaximized);
#endif
    m_playerSettingsDlg.exec();

    int result = m_playerSettingsDlg.result();

    // Update the game dialog if the player name was updated
    if ( result == QDialog::Accepted ){
        QPushButton* pCtrl = nullptr;
        editPlayer = pGame->getPlayer(PlayerColor);

        switch(PlayerColor){
            case eColor::White:
                pCtrl = ui->btnWhite;
                break;
            case eColor::Black:
                pCtrl = ui->btnBlack;
                break;
            default:
                qDebug() << "Updated Uknown Player Color: " << (int)PlayerColor;
                return;
        }

        pCtrl->setText(editPlayer.m_PlayerName);
    }
}


ReversiApp::ReversiApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ReversiApp)
{
    ui->setupUi(this);


    // Setup Buttons
    ui->btnClose->connect(ui->btnClose, &QPushButton::clicked, qApp, &QCoreApplication::quit);
    ui->btnWhite->connect(ui->btnWhite, &QPushButton::clicked, [this](){updatePlayerSettings(eColor::White);});
    ui->btnBlack->connect(ui->btnBlack, &QPushButton::clicked, [this](){updatePlayerSettings(eColor::Black);});

    ui->btnStart->connect(ui->btnStart, &QPushButton::clicked, [this](){
        // Setup Game
        CReversiGame* pGame = CReversiGame::getGlobalInstance();
        pGame->startGame();
        m_gameStart = true;
    });

    // Setup Timer to function as "Computer Player
    // Set up the timer to execute the lambda every 1000ms (1 second)
    m_gameTimer.setInterval(66);
    // Connect the timer's timeout signal to the lambda
    m_gameTimer.connect(&m_gameTimer, &QTimer::timeout, [this]() { selectNextMoveForComputer(); });
    m_gameTimer.start(66);

}

ReversiApp::~ReversiApp()
{
    delete ui;
}
