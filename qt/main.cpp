#include "qt.h"
#include <QtGui/QApplication>
#include "sipCore.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qt w;
	w.show();
	return a.exec();
}
