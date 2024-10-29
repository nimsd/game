#include "gameroom.h"
#include "ui_gameroom.h"
#include "gameselect.h"
#include <QIcon>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFile>
#include <QDebug>

GameRoom::GameRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameRoom)
{
    ui->setupUi(this);
    this->setFixedSize(1000,700);
    this->setWindowIcon(QIcon(":/src source/snake.png"));
    this->setWindowTitle("游戏房间");

    //初始化声音
    sound = new QSound(":/src source/bgm.wav",this);

    //初始化贪吃蛇
    snakeList.push_back(QRectF(this->width() * 0.5,this->height() * 0.5,kSnakeNodeWidth,kSnakeNodeHeight));
    moveUp();
    moveUp();

    //创建食物
    createNewFood();

    //使用定时器
    timer = new QTimer(this);

    connect(timer,&QTimer::timeout,[=]{
        int cnt = 1;
        if(snakeList.front().intersects(foodRect)){//intersects方法作用是判断两个矩形框是否有相交
            createNewFood();
            cnt++;
            QSound::play(":/src source/succ.wav");
        }

        while (cnt--) {
            switch (moveDirect) {
            case SnakeDirect::UP:
                moveUp();
                break;
            case SnakeDirect::DOWN:
                moveDown();
                break;
            case SnakeDirect::LEFT:
                moveLeft();
                break;
            case SnakeDirect::RIGHT:
                moveRight();
                break;
            }
        }

        snakeList.pop_back();
        update();//用于刷新链表
    });

    QPushButton *start = new QPushButton("开始",this);
    QPushButton *stop = new QPushButton("停止",this);

    start->setObjectName("start");
    stop->setObjectName("stop");

    QFont font("宋体",10);
    start->setFont(font);
    stop->setFont(font);

    start->move(820,180);
    stop->move(820,240);

    connect(start,&QPushButton::clicked,[=]{
        if(!isGameStart){
            isGameStart = true;
            timer->start(moveTimeout);//设置定时器触发时间
            sound = new QSound(":/src source/bgm.wav",this);
            sound->play();
            sound->setLoops(-1);
        }
    });
    connect(stop,&QPushButton::clicked,[=]{
        if(isGameStart){
            isGameStart = false;
            timer->stop();
            sound->stop();
        }
    });

    //方向控制
    QPushButton *up = new QPushButton(QIcon(":/src source/up.png"),"上",this);
    QPushButton *down = new QPushButton(QIcon(":/src source/down.png"),"下",this);
    QPushButton *left = new QPushButton(QIcon(":/src source/left.png"),"左",this);
    QPushButton *right = new QPushButton(QIcon(":/src source/right.png"),"右",this);

    up->move(840,400);
    down->move(840,480);
    left->move(780,440);
    right->move(880,440);

    connect(up,&QPushButton::clicked,[=]{
        if(moveDirect != SnakeDirect::DOWN)
            moveDirect = SnakeDirect::UP;
    });
    connect(down,&QPushButton::clicked,[=]{
        if(moveDirect != SnakeDirect::UP    )
            moveDirect = SnakeDirect::DOWN;
    });
    connect(left,&QPushButton::clicked,[=]{
        if(moveDirect != SnakeDirect::RIGHT)
            moveDirect = SnakeDirect::LEFT;
    });
    connect(right,&QPushButton::clicked,[=]{
        if(moveDirect != SnakeDirect::LEFT)
            moveDirect = SnakeDirect::RIGHT;
    });

    up->setShortcut(QKeySequence("Up"));
    down->setShortcut(QKeySequence("Down"));
    left->setShortcut(QKeySequence("Left"));
    right->setShortcut(QKeySequence("Right"));

    start->setShortcut((QKeySequence("Space")));

    //退出游戏
    QPushButton *exitBtn = new QPushButton("退出",this);
    exitBtn->setFont(QFont("宋体",20));
    exitBtn->move(840,600);

    QMessageBox *messageBox = new QMessageBox(this);
    QPushButton *ok = new QPushButton("ok");
    QPushButton *cancel = new QPushButton("cancel");

    messageBox->setWindowTitle("退出游戏");
    messageBox->setText("确认退出游戏吗？");

    messageBox->addButton(ok,QMessageBox::AcceptRole);
    messageBox->addButton(cancel,QMessageBox::RejectRole);

    connect(exitBtn,&QPushButton::clicked,[=]{
        timer->stop();
        sound->stop();
        messageBox->show();
        messageBox->exec();
        QSound::play(":/src source/tip_music.wav");

        if(messageBox->clickedButton() == ok){
            GameSelect *gSelect = new GameSelect;
            this->close();
            gSelect->show();
        }
        else{
            messageBox->close();
            timer->start();
            sound->play();
        }
    });
}

GameRoom::~GameRoom()
{
    delete ui;
}

void GameRoom::paintEvent(QPaintEvent *event)
{
    (void)event;

    //绘制游戏背景
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/src source/game_img.jpg");
    painter.drawPixmap(0,0,750,700,pix);
    pix.load(":/src source/game_back.jpg");
    painter.drawPixmap(750,0,300,700,pix);

    //绘制蛇 蛇头 + 蛇身体 + 蛇尾巴
    //绘制蛇头：上 下 左 右
    if(moveDirect == SnakeDirect::UP){
        pix.load(":/src source/head_up.png");
    }
    else if(moveDirect == SnakeDirect::DOWN){
        pix.load(":/src source/head_down.png");
    }
    else if(moveDirect == SnakeDirect::LEFT){
        pix.load(":/src source/head_left.png");
    }
    else if(moveDirect == SnakeDirect::RIGHT){
        pix.load(":/src source/head_right.png");
    }

    auto snakeHead = snakeList.front();
    painter.drawPixmap(snakeHead.x(),snakeHead.y(),snakeHead.width(),snakeHead.height(),pix);

    //绘制蛇身
    pix.load(":/src source/body.png");
    for (int i = 1; i < snakeList.size() - 1; i++) {
        auto node = snakeList.at(i);
        painter.drawPixmap(node.x(),node.y(),node.width(),node.height(),pix);
    }

    //绘制蛇尾
    auto snakeTail = snakeList.back();
    painter.drawPixmap(snakeTail.x(),snakeTail.y(),snakeTail.width(),snakeTail.height(),pix);

    //绘制食物
    pix.load(":/src source/meal.png");
    painter.drawPixmap(foodRect.x(),foodRect.y(),foodRect.width(),foodRect.height(),pix);

    //绘制积分
    pix.load(":/src source/integral.png");
    painter.drawPixmap(800,60,50,40,pix);

    QPen pen;
    pen.setColor(Qt::black);
    painter.setPen(pen);
    QFont font("方正舒体",35);
    painter.setFont(font);
    painter.drawText(875,95,QString("%1").arg(snakeList.size()));

    //往文件写分数
    int c = snakeList.size();
    //初始创建文件
    QFile file("./1.txt");

    // 打开文件以写入模式,记录分数
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out (&file);
        int num = c;
        out << num;
        file.close();
    }



    //绘制游戏失败效果
    if(checkFail()){
        pen.setColor(Qt::red);
        painter.setPen(pen);
        painter.setFont(QFont("方正舒体",50));
        painter.drawText(this->width()*0.25,this->height()*0.5,QString("GAME OVER!"));
        timer->stop();
        sound->deleteLater();

        sound = new QSound(":/src source/fail.wav");
        sound->setLoops(0);
        sound->play();
    }

}

void GameRoom::moveUp()
{
    QPointF leftTop;    //左上角坐标
    QPointF rightBottom;//右下角坐标

    auto snakeNode = snakeList.front();//蛇头
    int headX = snakeNode.x();
    int headY = snakeNode.y();

    if(headY < 0){
        leftTop = QPointF(headX,this->height() - kSnakeNodeHeight);
    }
    else{
        leftTop = QPointF(headX,headY - kSnakeNodeHeight);
    }

    rightBottom = leftTop + QPointF(kSnakeNodeWidth,kSnakeNodeHeight);

    snakeList.push_front(QRectF(leftTop,rightBottom));

}

void GameRoom::moveDown()
{
    QPointF leftTop;    //左上角坐标
    QPointF rightBottom;//右下角坐标

    auto snakeNode = snakeList.front();//蛇头
    int headX = snakeNode.x();
    int headY = snakeNode.y();

    if(headY > this->height()){
        leftTop = QPointF(headX,0);
    }
    else{
        leftTop = snakeNode.bottomLeft();//获取矩形左下角的坐标
    }

    rightBottom = leftTop + QPointF(kSnakeNodeWidth,kSnakeNodeHeight);

    snakeList.push_front(QRectF(leftTop,rightBottom));
}

void GameRoom::moveLeft()
{
    QPointF leftTop;    //左上角坐标
    QPointF rightBottom;//右下角坐标

    auto snakeNode = snakeList.front();//蛇头
    int headX = snakeNode.x();
    int headY = snakeNode.y();

    if(headX < 0){
        leftTop = QPointF(700 - kSnakeNodeWidth,headY);
    }
    else{
        leftTop = QPointF(headX - kSnakeNodeWidth,headY);
    }

    rightBottom = leftTop + QPointF(kSnakeNodeWidth,kSnakeNodeHeight);

    snakeList.push_front(QRectF(leftTop,rightBottom));
}

void GameRoom::moveRight()
{
    QPointF leftTop;    //左上角坐标
    QPointF rightBottom;//右下角坐标

    auto snakeNode = snakeList.front();//蛇头
    int headX = snakeNode.x();
    int headY = snakeNode.y();

    if(headX > 700){
        leftTop = QPointF(0,headY);
    }
    else{
        leftTop = snakeNode.topRight();//获取矩形右上角坐标
    }

    rightBottom = leftTop + QPointF(kSnakeNodeWidth,kSnakeNodeHeight);

    snakeList.push_front(QRectF(leftTop,rightBottom));
}

bool GameRoom::checkFail()
{
    for(int i = 0; i < snakeList.size(); i++){
        for(int  j = i + 1; j < snakeList.size(); j++){
            if(snakeList.at(i) == snakeList.at(j)){
                return true;
            }
        }
    }
    return false;
}

void GameRoom::createNewFood()
{
    foodRect = QRectF(qrand() % (750 / kSnakeNodeWidth) * kSnakeNodeWidth,
                      qrand() % (this->height() / kSnakeNodeHeight) * kSnakeNodeHeight,
                      kSnakeNodeWidth, kSnakeNodeHeight);
}
