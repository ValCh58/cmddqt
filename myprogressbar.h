#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QObject>
#include <QProgressBar>

class MyProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyProgressBar(QWidget *parent = 0, int minVal=0, int maxVal=100);
    ~MyProgressBar();

    QProgressBar *progress;

signals:


public slots:
    void showProgress();
    void hideProgress();
    void nextStep(int step);

};

#endif // MYPROGRESSBAR_H
