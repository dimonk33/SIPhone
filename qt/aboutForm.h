#ifndef _ABOUT_FORM_H_
#define _ABOUT_FORM_H_

#include <QtGui/QMainWindow>
#include "ui_about.h"




class aboutForm : public QDialog
{
	Q_OBJECT

public:
	aboutForm(QWidget *parent = 0, Qt::WFlags flags = 0);
	~aboutForm();

private:
	Ui::About ui;
};

#endif // _ABOUT_FORM_H_
