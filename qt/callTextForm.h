#ifndef _CALL_TEXT_FORM_H_
#define _CALL_TEXT_FORM_H_

#include <QtGui/QWidget>
#include <QMessageBox>


#include "ui_callTextForm.h"

#include "sipCore.h"


class callTextForm : public QWidget
{
	Q_OBJECT

public:
	callTextForm(QWidget * _parent = 0, Qt::WFlags flags = 0);

	~callTextForm();

	Ui::callTextForm ui;

	QWidget * parent;

public slots:
	void closeButtonPressed();
	void sendButtonPressed();
	void dtfmDigitIsComing(char digit);
	void messageReceived(char * message);
};



#endif //_CALL_TEXT_FORM_H_