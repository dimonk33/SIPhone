#include "qt.h"
//#include "globals.h"
#include <QtGui/QApplication>
#include "sipCore.h"

qt * win;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	win = new qt();;
	win->show();
	return a.exec();
}
