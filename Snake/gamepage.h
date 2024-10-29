#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class GamePage; }
QT_END_NAMESPACE

class GamePage : public QWidget
{
    Q_OBJECT

public:
    GamePage(QWidget *parent = nullptr);
    ~GamePage();
    void paintEvent(QPaintEvent *event);

private:
    Ui::GamePage *ui;
};
#endif // GAMEPAGE_H
