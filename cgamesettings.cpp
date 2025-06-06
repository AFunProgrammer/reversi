#include "cgamesettings.h"
#include "csettings.h"
#include "ui_gamesettings.h"

#include <QColorDialog>
#include <QMetaEnum>
#include <QtWidgets/qpushbutton.h>

#include "ceventfilter.h"
#include "utility.h"


CGameSettings::CGameSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CGameSettings)
{
    // Create a color dialog
    QColorDialog colorPicker(this);
    colorPicker.setOption(QColorDialog::NoButtons);         // Hide OK/Cancel buttons
    colorPicker.setOption(QColorDialog::DontUseNativeDialog);
    CSettings* pSettings = CSettings::getGlobalInstance();

    ui->setupUi(this);

    // setup the Board Color control, including having a mouse click / touch filter
    ui->ctlBoardColor->setStyleSheet(QString("background-color: %1").arg(pSettings->boardColor().name()));
    CEventFilter *clickFilter = new CEventFilter(ui->ctlBoardColor);
    ui->ctlBoardColor->installEventFilter(clickFilter);

    // Connect signal to a slot
    QObject::connect(clickFilter, &CEventFilter::clicked, [this,pSettings]() {
        // Show the dialog and get selected color
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");

        if(color.isValid()) {
            qDebug() << "selected color: " << color;
            QString colorName = color.name();
            QString backgroundStyle = QString("background-color: %1;").arg(colorName);
            ui->ctlBoardColor->setStyleSheet(backgroundStyle);
            pSettings->setBoardColor(color);
        }
    });

    // setup the Player Color control
    QPixmap firstPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(eColor::White), QSize(40,40)));
    ui->btnFirstPlayerColor->setIcon(QIcon(firstPlayerIcon));

    // Connect signal to a slot
    ui->btnFirstPlayerColor->connect(ui->btnFirstPlayerColor, &QPushButton::clicked, [this,pSettings]() {
        // Show the dialog and get selected color
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");

        if(color.isValid()) {
            qDebug() << "selected color: " << color;
            QPixmap firstPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", color, QSize(40,40)));
            ui->btnFirstPlayerColor->setIcon(QIcon(firstPlayerIcon));
            pSettings->setPlayerColor(eColor::White, color);
        }
    });

    // setup the Player Color control
    QPixmap secondPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(eColor::Black), QSize(40,40)));
    ui->btnSecondPlayerColor->setIcon(QIcon(secondPlayerIcon));

    // Connect signal to a slot
    ui->btnSecondPlayerColor->connect(ui->btnSecondPlayerColor, &QPushButton::clicked, [this,pSettings]() {
        // Show the dialog and get selected color
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");

        if(color.isValid()) {
            qDebug() << "selected color: " << color;
            QPixmap secondPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", color, QSize(40,40)));
            ui->btnSecondPlayerColor->setIcon(QIcon(secondPlayerIcon));
            pSettings->setPlayerColor(eColor::Black, color);
        }
    });

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, [this,pSettings]()
    {
        pSettings->resetSettings();
        this->accept();
    });

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Close), &QPushButton::clicked, [this](){applyChanges();});

}


void CGameSettings::showEvent(QShowEvent *event) {
    CSettings* pSettings = CSettings::getGlobalInstance();

    // setup the Board Color control, including having a mouse click / touch filter
    ui->ctlBoardColor->setStyleSheet(QString("background-color: %1").arg(pSettings->boardColor().name()));

    // setup the Player Color control
    QPixmap firstPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(eColor::White), QSize(40,40)));
    ui->btnFirstPlayerColor->setIcon(QIcon(firstPlayerIcon));

    // setup the Player Color control
    QPixmap secondPlayerIcon = QPixmap::fromImage(createColoredSvgButton(":/images/glassbutton", pSettings->playerColor(eColor::Black), QSize(40,40)));
    ui->btnSecondPlayerColor->setIcon(QIcon(secondPlayerIcon));
}


void CGameSettings::applyChanges(){
    qDebug() << "Apply Changes Clicked";

    this->accept();
}

CGameSettings::~CGameSettings()
{
    delete ui;
}
