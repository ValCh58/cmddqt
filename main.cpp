#include "ccmbb.h"
#include <QDir>
#include <QApplication>
#include <QTextCodec>
#include "config.h"

int main(int argc, char *argv[])
{
    QDir().setCurrent(QFileInfo(argv[0]).absolutePath());
    QCoreApplication::addLibraryPath("./plugins");

    Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);
    //QTextCodec::codecForName("KOI8-R");
    MainWindow w;
    config *cnf = new config(&w);
    if(cnf->exec() == QDialog::Accepted){
       w.setGeometry(200,200,681,472);
       w.setWindowTitle("CMBB");
       w.show();
    }else{
        delete cnf;
        exit(0);
    }
    return a.exec();
}
