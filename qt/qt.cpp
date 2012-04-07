#include "qt.h"

qt::qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
}

qt::~qt()
{

}
