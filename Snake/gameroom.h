#ifndef GAMEROOM_H
#define GAMEROOM_H

#include <QWidget>
#include <QSound>

//表示蛇移动的方向
enum class SnakeDirect{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};


namespace Ui {
class GameRoom;
}

class GameRoom : public QWidget
{
    Q_OBJECT

public:
    explicit GameRoom(QWidget *parent = nullptr);
    ~GameRoom();
    void paintEvent(QPaintEvent *event);

    //蛇方向移动的方法
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    //检测游戏是否结束
    bool checkFail();

    //创建食物
    void createNewFood();

    //设置蛇的移动时间
    void setTimeout(int timeout){
        moveTimeout = timeout;
    }

private:
    Ui::GameRoom *ui;

    const int kSnakeNodeWidth = 20; //表示蛇身体节点的宽度
    const int kSnakeNodeHeight = 20;//表示蛇身体节点的高度
    const int kDefaultTimeout = 200;//表示蛇默认移动速度

    QList<QRectF> snakeList;//表示贪吃蛇链表
    QRectF foodRect;//表示食物节点

    SnakeDirect moveDirect = SnakeDirect::UP;//表示蛇默认移动方向
    QTimer *timer;//定时器

    bool isGameStart = false;//表示是否开始游戏

    //游戏背景音乐
    QSound *sound;

    //设置移动速度变量
    int moveTimeout = kDefaultTimeout;
};

#endif // GAMEROOM_H
