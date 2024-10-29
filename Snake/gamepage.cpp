#include "gamepage.h"
#include "ui_gamepage.h"
#include <gameselect.h>
#include <QPainter>
#include <QIcon>
#include <QPushButton>
#include <QSound>

GamePage::GamePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GamePage)
{
    ui->setupUi(this);

    //设置窗口固定大小
    this->setFixedSize(1000,700);
    //设置图标
    this->setWindowIcon(QIcon(":/src source/snake.png"));
    this->setWindowTitle("贪吃蛇游戏");

    //添加按钮
    QPushButton *start = new QPushButton(this);
    //移动按钮位置
    start->setObjectName("start");
    start->move(400,500);
    start->setText("开始游戏");
    //设置字体和大小
    start->setFont(QFont("宋体",25));
    //利用QSS去修改按钮样式
    this->setStyleSheet("#start{"
                        "border: 0px;"
                        "border-radius: 30px;"
                        "background-color: #dadbde;"
                        "padding:20px;}"
                        "#start:hover{"
                        "border: 2px solid #8f8f91}"
                        "#start:pressed {"
                        "background-color: #f6f7fa;};");
    connect(start,&QPushButton::clicked,[=]{
        GameSelect *gameSelect = new GameSelect();
        gameSelect->setGeometry(this->geometry());
        this->close();
        QSound::play(":/src source/tip_music.wav");

        gameSelect->show();

    });


}

GamePage::~GamePage()
{
    delete ui;
}

void GamePage::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    QPixmap pix(":/src source/interface.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}

