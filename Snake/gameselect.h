#ifndef GAMESELECT_H
#define GAMESELECT_H

#include <QWidget>

namespace Ui {
class GameSelect;
}

class GameSelect : public QWidget
{
    Q_OBJECT

public:
    explicit GameSelect(QWidget *parent = nullptr);
    ~GameSelect();
    void paintEvent(QPaintEvent *event);

private:
    Ui::GameSelect *ui;

};

#endif // GAMESELECT_H
