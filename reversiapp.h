#ifndef REVERSIAPP_H
#define REVERSIAPP_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui {
class ReversiApp;
}
QT_END_NAMESPACE

class ReversiApp : public QMainWindow
{
    Q_OBJECT

public:
    ReversiApp(QWidget *parent = nullptr);
    ~ReversiApp();

private:
    Ui::ReversiApp *ui;
    QTimer gameTimer;

};
#endif // REVERSIAPP_H
