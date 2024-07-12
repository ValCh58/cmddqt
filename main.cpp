
#include <QDir>
#include <QApplication>
#include <QTextCodec>


#include "config.h"
#include "ccmbb.h"

/**
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{

    QDir().setCurrent(QFileInfo(argv[0]).absolutePath());
    QCoreApplication::addLibraryPath("./plugins");

    Q_INIT_RESOURCE(images);

    QApplication a(argc, argv);
    QTextCodec::codecForName("utf-8");
    MainWindow w;
    std::unique_ptr<config> cnf = std::make_unique<config>(&w);
    if(cnf->exec() == QDialog::Accepted){
       w.setGeometry(200,200,810,472);
       w.setWindowTitle("CMBB");
       w.show();
    }else{
        QCoreApplication::exit(0);
    }
    return a.exec();
}
