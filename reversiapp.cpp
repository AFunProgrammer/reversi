#include "reversiapp.h"
#include "ui_reversiapp.h"

#include <QTimer>
#include "creversigame.h"
#include <QRandomGenerator>

ReversiApp::ReversiApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ReversiApp)
{
    ui->setupUi(this);

    ui->btnClose->connect(ui->btnClose, &QPushButton::clicked, qApp, &QCoreApplication::quit);

    CReversiGame* pGame = CReversiGame::getGlobalInstance();

    pGame->addPlayer("White",ePlayerType::Human);
    pGame->addPlayer("Black",ePlayerType::Human);

    pGame->startGame();


    // Set up the timer to execute the lambda every 1000ms (1 second)
    gameTimer.setInterval(250);

    // Connect the timer's timeout signal to the lambda
    gameTimer.connect(&gameTimer, &QTimer::timeout, [this]() {
        // just randomly move until the game ends
        static int lackOfMoves = 0;

        CReversiGame* pGame = CReversiGame::getGlobalInstance();
        ReversiPlayer player = pGame->getPlayerTurn();

        QList<QPoint> validMoves = pGame->getValidMoves();

        if ( lackOfMoves >= 10 ){
            lackOfMoves = 0;
            //reset the game
            pGame->startGame();
            return;
        }

        if ( validMoves.count() > 0 ){
            int randomIndex = QRandomGenerator::global()->generate() % validMoves.size();
            QPoint move = validMoves.at(randomIndex);
            qDebug() << "chose Random move at: " << move << " For Player: " << (player.m_PlayerColor == eColor::Black? "Black" : "White");
            pGame->makeMove(move);

            int WhiteScore = pGame->getPlayerScore(eColor::White);
            int BlackScore = pGame->getPlayerScore(eColor::Black);

            QString formattedWhiteScore = QString::number(WhiteScore,10).rightJustified(2,'0');
            QString formattedBlackScore = QString::number(BlackScore,10).rightJustified(2,'0');

            ui->lblWhiteScore->setText(formattedWhiteScore);
            ui->lblBlackScore->setText(formattedBlackScore);
        } else {
            lackOfMoves++;
        }

        ui->oglReversi->update();
    });

    gameTimer.start(1000);
}

ReversiApp::~ReversiApp()
{
    delete ui;
}
