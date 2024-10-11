#ifndef GAME_H
#define GAME_H

#include <QApplication>
#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QKeyEvent>
#include <QLabel>



class Game : public QWidget
{
public:
    Game();

protected:
    void timerEvent(QTimerEvent*) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:

    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    double FIELD_WIDTH = 20;
    double FIELD_HEIGHT = 20;
    static const int DELAY = 200;

    void doDrawing();
    void localApple();
    void move();
    void check_field();
    void gameOver();
    void check_apple();


    int timerId;

    QPoint m_apple;

    enum Directions{
        left,right,up,down
    };

    Directions m_dir;

    bool m_inGame;
    QVector<QPoint> m_dots;
    void initGame();


    QLabel *score;


};

#endif // GAME_H
