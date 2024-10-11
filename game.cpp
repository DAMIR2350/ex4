#include "game.h"
#include "QFile"
#include <QPainter>
#include <QTime>
#include <QMessageBox>




Game::Game() {

    resize(DOT_WIDTH * FIELD_WIDTH,DOT_HEIGHT * FIELD_HEIGHT);
    setWindowTitle("Snake");
    setMinimumHeight(100);
    setMinimumWidth(150);
    score = new QLabel("0",this);


    score->move(0, 0);




    initGame();
}

void Game::timerEvent(QTimerEvent * e)
{
    Q_UNUSED(e);
    if (m_inGame){

        check_apple();
        move();
        check_field();
    }

    this->repaint();
}

void Game::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if (key == Qt::Key_Left  && m_dir  != Directions::right)  {m_dir = Directions::left; }
    if (key == Qt::Key_Right && m_dir  != Directions::left)   {m_dir = Directions::right;}
    if (key == Qt::Key_Up    && m_dir  != Directions::down)   {m_dir = Directions::up;   }
    if (key == Qt::Key_Down  && m_dir  != Directions::up)     {m_dir = Directions::down; }
}

void Game::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    doDrawing();
}

void Game::doDrawing()
{


    QPainter qp(this);
    if (m_inGame){

        qp.drawImage(QRect(m_apple.x()*DOT_WIDTH,m_apple.y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT), QImage(":/apple.png"));



        for (int i = 0; i < m_dots.size();++i){
            if (i==0){
                qp.setBrush(Qt::black);
                qp.drawEllipse(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT);
                qp.setBrush(Qt::white);
                if (m_dir == right)
                {
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.65,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.65,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.65,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.25,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                }else if(m_dir == left)
                {
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.25,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.65,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.25,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.25,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                }else if(m_dir == down)
                {
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.65,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.65,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.25,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.65,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                }else
                {
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.65,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.25,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                    qp.drawEllipse(m_dots[i].x()*DOT_WIDTH+DOT_WIDTH*0.25,m_dots[i].y()*DOT_HEIGHT+DOT_HEIGHT*0.25,DOT_WIDTH*0.15,DOT_HEIGHT*0.15);
                }
            }else{
                qp.setBrush(Qt::black);
                qp.drawEllipse(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGHT,DOT_WIDTH,DOT_HEIGHT);
            }
        }
    }
}

void Game::localApple()
{
    QTime time = QTime::currentTime();
    srand((uint) time.msec());

    m_apple.rx() = rand() % int(FIELD_WIDTH/1);
    m_apple.ry() = rand() % int(FIELD_HEIGHT/1);
}

void Game::move()
{

    for (int i = m_dots.size()-1; i>0; --i){
        m_dots[i] = m_dots[i-1];
    }
    switch (m_dir) {
        case left: {m_dots[0].rx() -=1; break;}
        case right:{m_dots[0].rx() +=1; break;}
        case up:   {m_dots[0].ry() -=1; break;}
        case down: {m_dots[0].ry() +=1; break;}
    }
}

void Game::check_field()
{

    FIELD_WIDTH = ceil(Game::size().width()/20);
    FIELD_HEIGHT = ceil(Game::size().height()/20);

    if (m_dots.size() > 4)
    {
        for (int i = 1; i < m_dots.size(); ++i)
        {
            if(m_dots[0] == m_dots[i])
            {
                m_inGame = false;
            }
        }
    }

    if (m_dots[0].x() >= FIELD_WIDTH)   { m_inGame = false;}
    if (m_dots[0].x() < 0)              { m_inGame = false;}
    if (m_dots[0].y() >= FIELD_HEIGHT)  { m_inGame = false;}
    if (m_dots[0].y() < 0)              { m_inGame = false;}

    if(!m_inGame){
        killTimer(timerId);
        gameOver();
    }
}

void Game::gameOver()
{

    QMessageBox msg;
    msg.setButtonText(QMessageBox::Ok,"Restart");
    QFile file1("record.txt");
    QFile file2("record.txt");
    file1.open(QIODevice::ReadOnly);
    file2.open(QIODevice::ReadWrite);
    int OldRecord = file1.readLine().toInt();
    if (OldRecord >= (m_dots.size()-3)){
        msg.setText("Score: "+QString::number(m_dots.size()-3)+"\n"+"Record: " + QString::number(OldRecord));
    }else{
        file2.write(QString::number(m_dots.size()-3).toUtf8());
        msg.setText("Score: "+QString::number(m_dots.size()-3)+"\n"+"Record: " + QString::number(m_dots.size()-3));
    }
    file1.close();
    file2.close();

    msg.exec();
    initGame();
}

void Game::check_apple()
{
    if (m_apple == m_dots[0])
    {
        m_dots.push_back(QPoint(0,0));
        score->setText(QString::number(m_dots.size()-3));
        localApple();
    }else if(m_apple.rx() > FIELD_WIDTH or m_apple.ry() > FIELD_HEIGHT){
        localApple();
    }
}

void Game::initGame()
{


    score->setText("0");
    m_inGame = true;
    m_dir = right;
    m_dots.resize(3);

    for (int i = 0;i<m_dots.size(); ++i){
        m_dots[i].rx() = m_dots.size() - i-1;
        m_dots[i].ry() = 0;
    }

    localApple();

    timerId = startTimer(DELAY);
}


