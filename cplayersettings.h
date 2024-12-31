#ifndef CPLAYERSETTINGS_H
#define CPLAYERSETTINGS_H

#pragma once
#include <QDialog>
#include "creversigame.h"

namespace Ui {
class CPlayerSettings;
}

class CPlayerSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CPlayerSettings(QWidget *parent = nullptr);
    ~CPlayerSettings();

    void editPlayer(ReversiPlayer EditPlayer = {eColor::None, "Not Found", ePlayerType::Observer});

private:
    Ui::CPlayerSettings *ui;
    ReversiPlayer m_EditPlayer;

    void applyChanges();
    void cancelChangers();
};

#endif // CPLAYERSETTINGS_H
