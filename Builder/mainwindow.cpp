#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "engine.h"

#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <math.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(800, 600);

    ui->label->setGeometry(10, 60, width() * 0.6625, height() - 60);

    ui->lineEdit->setGeometry(10, 10, width() * 0.55 - 10, 0.0667 * height());

    //ui->label->setStyleSheet("{border-color: rgb(0, 0, 0)}");
    //ui->label->setStyleSheet("QLabel background-color: rgb(0, 0, 0)")

    QPushButton* button = new QPushButton(this);
    (*button).setGeometry((10 + width() * 0.6625) - (0.1125 * width()), 10, 0.1125 * width(), int(0.0667 * height()));
    (*button).setText("Build!");

    connect(button, SIGNAL(clicked()), this, SLOT(our_pushButton_clicked()));
}

bool Solve(QString f, double x, double &res) {
    f.replace('x', '(' + QString::number(x) + ')');

    double tmp = 0;

    if (calc(f.toStdString(), tmp) == "Ok.") {
        res = tmp;
        return true;
    } else {
        res = 0;
        return false;
    }
}

void MainWindow::our_pushButton_clicked()
{

    QString Fx = ui->lineEdit->text();

    QPixmap PM(width() * 0.6625, height() * 0.8833);

    QPainter painter;
    painter.begin(&PM);

    PM.fill(Qt::white);

    double xmin = -2 * M_PI, xmax = 2 * M_PI;
    double ymin = -2 * M_PI, ymax = 2 * M_PI;

    int xgmin = 0, xgmax = PM.width();
    int ygmin = 0, ygmax = PM.height();

    double kx = (xgmax - xgmin) / (xmax - xmin);
    double ky = (ygmin - ygmax) / (ymax - ymin);

    int x0 = xgmin - kx * xmin, y0 = ygmin - ky * ymax;

    double x = xmin, y;
    int xg = 0, yg = 0;


    QPen pen;
    pen.setColor(Qt::black);

    painter.setPen(pen);

    painter.drawLine(x0, 0, x0, PM.height());
    painter.drawLine(0, y0, PM.width(), y0);

    pen.setColor(Qt::red);
    painter.setPen(pen);

    double stepx = (xmax - xmin) / (xgmax - xgmin);

    QPainterPath path;

    if (Fx == "") {
        ui->label->setPixmap(PM);
        return;
    }

    bool pr = Solve(Fx, x, y);

    xg = x0 + kx * x;
    yg = y0 + ky * y;

    path.moveTo(xg, yg);

    while (x <= xmax) {

        pr = Solve(Fx, x, y);

        xg = x0 + kx * x;
        yg = y0 + ky * y;

        if (!pr) {
            path.moveTo(xg, yg);
        }

        while (x <= xmax && !(ygmin <= yg && yg <= ygmax)) {

            x += stepx;
            pr = Solve(Fx, x, y);

            xg = x0 + kx * x;
            yg = y0 + ky * y;

            path.moveTo(xg, yg);

        }

        if ((ygmin <= yg && yg <= ygmax)) {
            path.lineTo(xg, yg);
            painter.drawPath(path);
        }

        else {
            path.moveTo(xg, yg);
        }

        x += stepx;

    }

    ui->label->setPixmap(PM);

}

MainWindow::~MainWindow()
{
    delete ui;
}

