#include <qapplication.h>
#include <qpushbutton.h>
#include "KuhMainWindow.h"

int main(int argc, char **argv)
{
 QApplication app (argc, argv);

 KuhMainWindow mainwindow;

 mainwindow.resize(300,300);
 mainwindow.move(200,200);
 mainwindow.show();

 return app.exec();
}
