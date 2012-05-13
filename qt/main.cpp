#include "qt.h"
//#include "globals.h"
#include <QtGui/QApplication>
#include "sipCore.h"
#include <QMetaType>


sipCore * sipCore::object;
qt * win;
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qRegisterMetaType<pjsua_acc_id>("pjsua_acc_id");
	qRegisterMetaType<pjsua_reg_info>("pjsua_reg_info");
	win = new qt();
	win->show();
	return a.exec();
}
