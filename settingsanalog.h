#ifndef SETTINGSANALOG_H
#define SETTINGSANALOG_H

#include <QObject>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>

#include "datamodules.h"
#include "sport.h"
#include "modules.h"
#include "myprogressbar.h"


class SettingsAnalog : public QDialog
{

    Q_OBJECT

public:
    SettingsAnalog(DataModules *data, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~SettingsAnalog();

private:
   QLineEdit *edAcp1Low;
   QLineEdit *edAcp2Low;
   QLineEdit *edAcp3Low;
   QLineEdit *edAcp4Low;
   QLineEdit *edAcp5Low;
   QLineEdit *edAcp6Low;
   QLineEdit *edAcp7Low;
   QLineEdit *edAcp8Low;
   QLineEdit *edAcp1Hi;
   QLineEdit *edAcp2Hi;
   QLineEdit *edAcp3Hi;
   QLineEdit *edAcp4Hi;
   QLineEdit *edAcp5Hi;
   QLineEdit *edAcp6Hi;
   QLineEdit *edAcp7Hi;
   QLineEdit *edAcp8Hi;
   QLabel *shortName1;
   QLabel *shortName2;
   QLabel *shortName3;
   QLabel *shortName4;
   QLabel *shortName5;
   QLabel *shortName6;
   QLabel *shortName7;
   QLabel *shortName8;
   QLabel *name1;
   QLabel *name2;
   QLabel *name3;
   QLabel *name4;
   QLabel *name5;
   QLabel *name6;
   QLabel *name7;
   QLabel *name8;

   QPushButton *btWrite;
   QPushButton *btClose;

   DataModules *modul;
   void init();

   QGroupBox *minValACp();
   QGroupBox *maxValACp();
   QGroupBox *labLayout();
   QHBoxLayout *btLayout();

   QList<SPort*> sPorts;
   uint lowVar[8];
   uint  hiVar[8];
   QString labVar[8];
   void readSettings();
   void fillForm_1_8();
   void fillLabel();
   void readFromArrSet();
   //MainWindow *mainWin;
   QMap<QString, AdcPloAddr> *mds;
   QMap<QString, Adcs> *adcs;
   void setDisBtn(bool flag);
   QStatusBar *sbar;
   MyProgressBar *vProgrBar;

private slots:
   void writeSettings();

signals:
   void sigReadSettings0(DataModules*, uint*, uint*);
   void sigReadSettings1(DataModules*, uint*, uint*);
   void sigWriteSettings0(DataModules*, uint*, uint*);
   void sigWriteSettings1(DataModules*, uint*, uint*);


};

#endif // SETTINGSANALOG_H
