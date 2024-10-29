#include "gameselect.h"
#include "ui_gameselect.h"
#include "gamepage.h"
#include "gameroom.h"
#include <QIcon>
#include <QPushButton>
#include <QSound>
#include <QPainter>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>

GameSelect::GameSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameSelect)
{
    ui->setupUi(this);

    //设置窗口固定大小
    this->setFixedSize(1000,700);
    //设置图标
    this->setWindowIcon(QIcon(":/src source/snake.png"));
    this->setWindowTitle("游戏关卡选择");

    //设置一个返回的按钮
    QPushButton *backBtn = new QPushButton(this);
    backBtn->setObjectName("backBrn");
    backBtn->setIcon(QIcon(":/src source/return.png"));
    backBtn->setStyleSheet("#backBrn{border: 1px solid #8f8f91; padding: 10px;background-color:#e1ffff;}"
                           "#backBrn:hover{border: 2px solid #ff0000;}");
    backBtn->move(this->geometry().width()-200,this->geometry().height()-180);
    connect(backBtn,&QPushButton::clicked,[=]{
       this->close();
       GamePage *gamePage = new GamePage;
       gamePage->show();
       QSound::play(":/src source/tip_music.wav");
    });

    //创建游戏房间界面
    GameRoom *gRoom = new GameRoom();


    //设置按钮的属性
    QFont font("宋体",25);
    QString style("QPushButton{"
                  "border: 0px;"
                  "border-radius: 30px;"
                  "background-color: #dadbde;"
                  "padding:20px;}"
                  "QPushButton:hover{"
                  "border: 2px solid #8f8f91}"
                  "QPushButton:pressed {"
                  "background-color: #f6f7fa;};");
    QPushButton *simpleBut = new QPushButton(this);
    simpleBut->move(390,120);
    simpleBut->setText("简单模式");
    simpleBut->setFont(font);
    simpleBut->setStyleSheet(style);
    connect(simpleBut,&QPushButton::clicked,[=]{
        this->close();
        gRoom->setGeometry(this->geometry());
        QSound::play(":/src source/tip_music.wav");
        gRoom->show();
        gRoom->setTimeout(300);
    });


    QPushButton *normalBut = new QPushButton(this);
    normalBut->move(390,240);
    normalBut->setText("普通模式");
    normalBut->setFont(font);
    normalBut->setStyleSheet(style);
    connect(normalBut,&QPushButton::clicked,[=]{
        this->close();
        gRoom->setGeometry(this->geometry());
        QSound::play(":/src source/tip_music.wav");
        gRoom->show();
        gRoom->setTimeout(200);
    });

    QPushButton *hardBut = new QPushButton(this);
    hardBut->move(390,360);
    hardBut->setText("困难模式");
    hardBut->setFont(font);
    hardBut->setStyleSheet(style);
    connect(hardBut,&QPushButton::clicked,[=]{
        this->close();
        gRoom->setGeometry(this->geometry());
        QSound::play(":/src source/tip_music.wav");
        gRoom->show();
        gRoom->setTimeout(100);
    });

    QPushButton *hisBut = new QPushButton(this);
    hisBut->move(390,480);
    hisBut->setText("历史战绩");
    hisBut->setFont(font);
    hisBut->setStyleSheet(style);

    connect(hisBut,&QPushButton::clicked,[=]{
        QWidget *widget = new QWidget();
        widget->setWindowTitle("历史战绩");
        widget->setFixedSize(400,240);

        QTextEdit *edit = new QTextEdit(widget);
        edit->setFont(font);
        edit->setFixedSize(400,240);

        QFile file("./1.txt");
        file.open(QIODevice::ReadOnly);

        QTextStream in(&file);
        int date = in.readLine().toInt();
        edit->setText("上次分数为：");
        edit->append(QString::number(date) + QString("分"));

        widget->show();
        file.close();
    });
}

GameSelect::~GameSelect()
{
    delete ui;
}

void GameSelect::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    QPixmap pix(":/src source/game_select.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
