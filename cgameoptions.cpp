#include "cgameoptions.h"
#include "ui_options.h"

#include <QList>
#include <QPair>

using qStringColor = QPair<QString,QColor>;

QList<qStringColor> g_colorList = QList<qStringColor>({
    qMakePair("red",Qt::red),
    qMakePair("darkred",Qt::darkRed),
    qMakePair("orange",QColor( 0xFF, 0xA0, 0x00 )),
    qMakePair("darkorange",QColor( 0xFF, 0xA0, 0x00 ).darker(150)),
    qMakePair("yellow",Qt::yellow),
    qMakePair("darkyellow",Qt::darkYellow),
    qMakePair("green",Qt::green),
    qMakePair("darkgreen",Qt::darkGreen),
    qMakePair("blue",Qt::blue),
    qMakePair("darkblue",Qt::darkBlue),
    qMakePair("purple",Qt::magenta),
    qMakePair("darkpurple",Qt::darkMagenta)
});


CGameOptions::CGameOptions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CGameOptions)
{
    ui->setupUi(this);

    if ( m_colorList.empty() ){
        for(qStringColor stringColor: g_colorList){
            QListWidgetItem item;
            item.setBackground(stringColor.second);
            m_colorList.append(item);
            ui->lstBackColor->addItem(m_colorList.last().clone());
            ui->lstBoardColor->addItem(m_colorList.last().clone());
        }
    }



}

CGameOptions::~CGameOptions()
{
    delete ui;
}
