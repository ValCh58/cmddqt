#include "myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget *parent, int minVal, int maxVal) : QWidget(parent)
{
    progress = new QProgressBar(parent);
    progress->setRange(minVal, maxVal);
    progress->setValue(0);
    progress->hide();
    QRect parentSize = parent->geometry();
    int w = parentSize.width()/2 - 250;
    int h = parentSize.height()/2 - 20;
    progress->setGeometry(w,h,500,40);



}

MyProgressBar::~MyProgressBar()
{

}

void MyProgressBar::showProgress()
{
    progress->setValue(0);
    progress->show();

}

void MyProgressBar::hideProgress()
{
    progress->hide();

}

void MyProgressBar::nextStep(int step)
{
    int curVal = progress->value()+step;
    int maxVal = progress->maximum();
    progress->setValue(curVal + (maxVal - curVal) / 100);
}
