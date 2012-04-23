#ifndef QT_H
#define QT_H

#include <QtGui/QMainWindow>
#include "ui_qt.h"
#include "sipCore.h"

class qt : public QMainWindow
{
	Q_OBJECT

	sipCore core;
public:
	qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~qt();

private:
	Ui::qtClass ui;
};

#endif // QT_H
