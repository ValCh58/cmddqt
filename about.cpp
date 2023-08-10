#include "about.h"

/**
 * @brief About::About
 * @param parent
 * @param f
 */
About::About(QWidget *parent, Qt::WindowFlags f):QDialog(parent,f)
{
   setModal(true);

   //=======================================================================================//
   QPixmap pix(":/cmbbImages/tools folder.png");
   imageLab = new QLabel;
   //imageLab->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   imageLab->setScaledContents(true);
   imageLab->setPixmap(pix);
   //=======================================================================================//
   aboutName = new QLabel("Программа CMBB.\nКонфигуратор сети модулей\nввода-вывода САУ ГПА,\n"
                          " версия 6.0.1\n");
   aboutName->setStyleSheet(QString("font-size: 15px;color: gray;font-style:italic;"));
   //aboutName->setFrameStyle(QFrame::Panel | QFrame::Sunken);
   ownerName = new QLabel("\"Электронные информационные системы\",\nРоссия, Екатеринбург.");
   devName = new QLabel("Разработчики Орехов Николай Сергеевич,\n Лопатин Даниил Алексеевич.");
   qtName = new QLabel("Перевод на платформу <img src=':/cmbbImages/qt.png'> и дальнейшая разработка В.В. Черных.");
   QPushButton *button = new QPushButton("Ok");

   QHBoxLayout *layoutHBox = new QHBoxLayout;
   layoutHBox->addWidget(imageLab);
   layoutHBox->addWidget(aboutName);

   QVBoxLayout *nextBox = new QVBoxLayout;
   nextBox->addWidget(ownerName);
   nextBox->addWidget(devName);
   nextBox->addWidget(qtName);


   QVBoxLayout *mainBox = new QVBoxLayout;
   mainBox->addLayout(layoutHBox);
   mainBox->addLayout(nextBox);
   mainBox->addWidget(button);

   setLayout( mainBox);
   setWindowTitle(tr("О программе"));
   setFixedHeight(sizeHint().height());
   setFixedWidth(sizeHint().width());

   connect(button, SIGNAL(clicked()), this, SLOT(close()));

}

/**
 * @brief About::~About
 */
About::~About()
{

}

