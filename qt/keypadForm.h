#ifndef _KEYPAD_FORM_H_
#define _KEYPAD_FORM_H_

#include <QtGui/QWidget>
#include <QMessageBox>
#include <QRegExp>


#include "ui_keypad.h"

#include <sipCore.h>


class keypadForm : public QWidget
{
	Q_OBJECT

public:
	keypadForm(QWidget * _parent = 0, Qt::WFlags flags = 0);

	~keypadForm();

	Ui::keypadForm ui;

	QWidget * parent;

public slots:
	void closeButtonPressed();
	void playSound(QString key);
	void keyPressed();
};



#endif //_OPTIONS_FORM_H_