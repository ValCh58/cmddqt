#ifndef ADCOPERATIONS_H
#define ADCOPERATIONS_H

#include <QObject>

class ADCOperations : public Worker
{
public:
    ADCOperations();
    ~ADCOperations();
    //Что будет запрашиваться и отображаться!
    enum ACPTYPE {
                  acpCorrect, //Исправленное за калибровку значение АЦП//
                  acpReal,    //Реальное значение АЦП//
                  fainReal    //Значение частоты//
    };

};

#endif // ADCOPERATIONS_H
