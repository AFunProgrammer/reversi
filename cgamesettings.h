#ifndef CGAMESETTINGS_H
#define CGAMESETTINGS_H

#pragma once

#include <QDialog>
namespace Ui {
class CGameSettings;
}

class CGameSettings : public QDialog
{
    Q_OBJECT

public:
    explicit CGameSettings(QWidget *parent = nullptr);
    ~CGameSettings();

protected:
    void showEvent(QShowEvent *event) override;

private:
    Ui::CGameSettings *ui;
    void setCtlsToGameSettings();


    void applyChanges();
};

#endif // CGAMEOPTIONS_H
