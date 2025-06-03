#include "cgamesettings.h"
#include "csettings.h"
#include "ui_gamesettings.h"

#include <QColorDialog>
#include <QMetaEnum>

CGameSettings::CGameSettings(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CGameSettings)
{
    CSettings* pSettings = CSettings::getGlobalInstance();

    ui->setupUi(this);

    ui->ctlBoardColor->setStyleSheet(QString("background-color: %1").arg(pSettings->boardColor().name()));

    ui->btnBoardColor->connect(ui->btnBoardColor, &QPushButton::clicked, [this](){
        CSettings* pSettings = CSettings::getGlobalInstance();
        // Create a color dialog
        QColorDialog colorPicker(this);
        colorPicker.setOption(QColorDialog::NoButtons);         // Hide OK/Cancel buttons
        colorPicker.setOption(QColorDialog::DontUseNativeDialog);
        // Show the dialog and get selected color
        QColor color = QColorDialog::getColor(Qt::white, nullptr, "Select Color");

        if(color.isValid()) {
            qDebug() << "selected color: " << color;

            // convert colors to strings for using in the style sheets
            QString colorName = color.name();

            // Generate stylesheet string
            QString backgroundStyle = QString("background-color: %1;").arg(colorName);

            // Set Control Color
            ui->ctlBoardColor->setStyleSheet(backgroundStyle);
            pSettings->setBoardColor(color);
        }
    });

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Close), &QPushButton::clicked, [this](){applyChanges();});

}

void CGameSettings::applyChanges(){
    qDebug() << "Apply Changes Clicked";

    this->accept();
}

CGameSettings::~CGameSettings()
{
    delete ui;
}
