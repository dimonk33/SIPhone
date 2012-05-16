#ifndef _ADD_USER_FORM_H_
#define _ADD_USER_FORM_H_

#include <QtGui/QWidget>
#include <QMessageBox>
#include <QRegExp>

#include "ui_addUserForm.h"

#include <sipCore.h>

class addUserForm : public QDialog
{
	Q_OBJECT

public:
	addUserForm(QWidget * _parent = 0, Qt::WFlags flags = 0);

	~addUserForm();
	void setName(char * name);
	void setName(QString name);
	void setURI(char * URI);
	QString getName();
	QString getURI();
	int forChanges;
	
private:
	Ui::addUserForm ui;
	
		QWidget * parent;

public slots:
	void okButtonPressed();
	void cancelButtonPressed();
};



#endif //_ADD_USER_FORM_H_