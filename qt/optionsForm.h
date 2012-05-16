#ifndef _OPTIONS_FORM_H_
#define _OPTIONS_FORM_H_

#include <QtGui/QWidget>
#include <QMessageBox>
#include <QRegExp>


#include "ui_optionsForm.h"

#include <sipCore.h>

class optionsForm : public QDialog
{
	Q_OBJECT

public:
	optionsForm(QWidget * _parent = 0, Qt::WFlags flags = 0);

	~optionsForm();

	Ui::optionsForm ui;

public slots:
	void okButtonPressed();
	void cancelButtonPressed();
};



#endif //_OPTIONS_FORM_H_