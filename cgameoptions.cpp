#include "cgameoptions.h"
#include "ui_options.h"

#include <QColorDialog>
#include <QMetaEnum>

CGameOptions::CGameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CGameOptions)
{

    ui->setupUi(this);

    ui->ctlBoardColor->setStyleSheet(QString("background-color: %1").arg(m_BoardColor.name()));

    ui->btnBoardColor->connect(ui->btnBoardColor, &QPushButton::clicked, [this](){
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
            m_BoardColor = color;
        }
    });

    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, [this](){applyChanges();});
    ui->btnDialogBox->connect(ui->btnDialogBox->button(QDialogButtonBox::Close), &QPushButton::clicked, [this](){cancelChanges();});

}

void CGameOptions::applyChanges(){
    qDebug() << "Cancel Changes Clicked";

    this->reject();
}

void CGameOptions::cancelChanges(){
    qDebug() << "Cancel Changes Clicked";

    this->reject();
}

CGameOptions::~CGameOptions()
{
    delete ui;
}
