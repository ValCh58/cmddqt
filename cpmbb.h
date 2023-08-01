#ifndef PMBB_H
#define PMBB_H

#include <QObject>

#define PMBB_OK            0
#define PMBB_PARAM         1
#define PMBB_TIMEOUT       2
#define PMBB_CRC           3
#define PMBB_PROTOCOL      4

class PMBB : public QObject
{
    Q_OBJECT
public:
    explicit PMBB(QObject *parent = 0);
    ~PMBB();

signals:

public slots:
};

#endif // PMBB_H
