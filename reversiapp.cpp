#include "reversiapp.h"
#include "ui_reversiapp.h"

#include "creversigame.h"
#include "utility.h"

#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QRandomGenerator>
#include <QREsource>

void ReversiApp::updateGameSettings()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();
    CReversiWidget* pGameBoard = ui->oglReversi;
    CSettings* pSettings = CSettings::getGlobalInstance();

    for (int i=0; i<pSettings->numberOfPlayers(); i++){
        pGame->setPlayerInfo(ePlayer(i), pSettings->playerName(ePlayer(i)), pSettings->playerType(ePlayer(i)));
    }

    pGameBoard->setBoardColor(pSettings->boardColor());

    pGameBoard->setPlayerColor(0,pSettings->playerColor(ePlayer::First));
    pGameBoard->setPlayerColor(1,pSettings->playerColor(ePlayer::Second));

    QPixmap firstPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(ePlayer::First), QSize(16,16)));
    QPixmap secondPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(ePlayer::Second), QSize(16,16)));

    // Set player colored icons
    ui->btnWhite->setIcon(QIcon(firstPlayerIcon));
    ui->btnBlack->setIcon(QIcon(secondPlayerIcon));

    // Set the initial players for startup of Player,Computer
    ui->btnWhite->setText(pSettings->playerName(ePlayer::First));
    ui->btnBlack->setText(pSettings->playerName(ePlayer::Second));
}

void ReversiApp::showGameSettings()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    m_gameOptionsDlg.setModal(true);

#if defined(Q_OS_ANDROID)
    // Adjust the width to the screen width (or center in the screen)
    //fileDialog.setWindowState(Qt::WindowMaximized);
#endif
    m_gameOptionsDlg.exec();

    // get the resulting data
    updateGameSettings();
}

void ReversiApp::showPlayerSettings(ePlayer PlayerColor)
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    m_playerSettingsDlg.editPlayer(pGame->getPlayer(PlayerColor));
    m_playerSettingsDlg.setModal(true);

#if defined(Q_OS_ANDROID)
    // Adjust the width to the screen width (or center in the screen)
    //fileDialog.setWindowState(Qt::WindowMaximized);
#endif
    m_playerSettingsDlg.exec();

    updateGameSettings();
}

void announceWinner(QWidget* parent)
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();
    ReversiPlayer whitePlayer = pGame->getPlayer(ePlayer::First);
    ReversiPlayer blackPlayer = pGame->getPlayer(ePlayer::Second);
    QString winnerScore = "";
    QString loserScore = "";

    int whiteScore = pGame->getPlayerScore(ePlayer::First);
    int blackScore = pGame->getPlayerScore(ePlayer::Second);

    if ( whiteScore > blackScore ){
        winnerScore = QString::number(whiteScore,10).rightJustified(2,'0');
        loserScore = QString::number(blackScore,10).rightJustified(2,'0');
    } else {
        winnerScore = QString::number(blackScore,10).rightJustified(2,'0');
        loserScore = QString::number(whiteScore,10).rightJustified(2,'0');
    }

    QString winner = (whiteScore > blackScore ? whitePlayer.m_PlayerName : blackPlayer.m_PlayerName);
    QString winnerMsg = (whiteScore != blackScore ? QString(winner + " is the winner!") : QString("Tie Game, Wow!"));
    QString scoreComparison = QString( "Score:  " + winnerScore + " : " + loserScore );

    QString displayMessage = QString(winnerMsg + "\r\n" + scoreComparison);

    // Always display this message, this only runs after checking that no more moves exist
    QMessageBox::information(parent, "Game Over", displayMessage, QMessageBox::Ok);
}

bool areThereAnyValidMovesLeft(ReversiPlayer* Player, QList<QPoint>* ValidMoves){
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    // check for any valid moves, if not see if the next player has moves
    if ( ValidMoves->count() == 0 ){
        ePlayer checkColorForMoves = ePlayer::None;
        // change the player to the next player (should only be 2 players)
        if ( Player->m_Player == pGame->getPlayers()[0].m_Player ){
            checkColorForMoves = pGame->getPlayers()[1].m_Player;
        } else {
            checkColorForMoves = pGame->getPlayers()[0].m_Player;
        }

        // check to see if the other player has any moves
        if ( pGame->getValidMoves(checkColorForMoves).count() > 0 ){
            // change player
            pGame->setNextTurnFromNoValidMoves();
            // get data to be used for movements
            *Player = pGame->getPlayerTurn();
            *ValidMoves = pGame->getValidMoves(Player->m_Player);
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

    // check to see if there is a corner in the moves, if so then take the corner
    for ( QPoint move: validMoves ){
        if ( ( move.x() == 0 && move.y() == 0 ) ||
           ( move.x() == 7 && move.y() == 0 ) ||
           ( move.x() == 0 && move.y() == 7 ) ||
           ( move.x() == 7 && move.y() == 7 ) ) {
            qDebug() << "chose corner move at: " << move << " For Player: " << (player.m_Player == ePlayer::Second? "Black" : "White");
            pGame->makeMove(move);
            return;
        }
    }

    int randomIndex = QRandomGenerator::global()->generate() % validMoves.size();
    QPoint move = validMoves.at(randomIndex);
    qDebug() << "chose Random move at: " << move << " For Player: " << (player.m_Player == ePlayer::Second? "Black" : "White");
    pGame->makeMove(move);
}

void ReversiApp::selectNextMoveForComputer()
{
    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    ReversiPlayer player = pGame->getPlayerTurn();
    QList<QPoint> validMoves = pGame->getValidMoves(player.m_Player);

    // Check to see if all moves have been done, if so then don't try to move anymore
    if ( pGame->getMoves().count() <= 60 && m_gameStart ){
        validMoves = pGame->getValidMoves(player.m_Player);

        if ( areThereAnyValidMovesLeft(&player, &validMoves) == false ){
            announceWinner(this); // no moves left so there must be a winner
            m_gameStart = false;
        }

        if ( player.m_PlayerType == ePlayerType::Ai && m_gameStart == true){
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

    int WhiteScore = pGame->getPlayerScore(ePlayer::First);
    int BlackScore = pGame->getPlayerScore(ePlayer::Second);

    QString formattedWhiteScore = QString::number(WhiteScore,10).rightJustified(2,'0');
    QString formattedBlackScore = QString::number(BlackScore,10).rightJustified(2,'0');

    ui->lblWhiteScore->setText(formattedWhiteScore);
    ui->lblBlackScore->setText(formattedBlackScore);

    ui->oglReversi->update();
}

ReversiApp::ReversiApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ReversiApp)
{
    ui->setupUi(this);

    qDebug() << "Resource valid?" << QResource(":/images/whitemove").isValid();

    // match the clear of the reversi widget to the window color
    ui->oglReversi->setClearColor(ui->centralwidget->palette().color(QPalette::Window));

    // Setup Buttons
    ui->btnClose->connect(ui->btnClose, &QPushButton::clicked, qApp, &QCoreApplication::quit);
    ui->btnWhite->connect(ui->btnWhite, &QPushButton::clicked, [this](){showPlayerSettings(ePlayer::First);});
    ui->btnBlack->connect(ui->btnBlack, &QPushButton::clicked, [this](){showPlayerSettings(ePlayer::Second);});

    ui->btnStart->connect(ui->btnStart, &QPushButton::clicked, [this](){
        // Setup Game
        CReversiGame* pGame = CReversiGame::getGlobalInstance();
        pGame->startGame();
        m_gameStart = true;
    });

    ui->btnSettings->connect(ui->btnSettings, &QPushButton::clicked, [this](){showGameSettings();});


    // make sure all settings are updated to start initially
    updateGameSettings();

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
