#include "keypadForm.h"

#include <qt.h>

#include "callWindow.h"


keypadForm::keypadForm(QWidget * _parent /* = 0 */, Qt::WFlags flags /* = 0 */)
{
	ui.setupUi(this);
	connect(ui.pushButtonClose, SIGNAL(pressed()), this, SLOT(closeButtonPressed()));
	connect(ui.pushButtonStar, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton0, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton1, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton2, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton3, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton4, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton5, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton6, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton7, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButton8, SIGNAL(pressed()), this, SLOT(keyPressed()));
	
	connect(ui.pushButton9, SIGNAL(pressed()), this, SLOT(keyPressed()));
	connect(ui.pushButtonSharp, SIGNAL(pressed()), this, SLOT(keyPressed()));
	parent = _parent;
}

keypadForm::~keypadForm()
{

}


void keypadForm::closeButtonPressed()
{
	((CallWindow*)parent)->keypad = NULL;
	this->hide();
	delete this;
}

void keypadForm::playSound(QString key)
{
	pjsua_call_id callId = ((CallWindow*)parent)->getCallId();
	pj_str_t string;
	int flag = 0;
	char * str;
	if(key.compare("*") == 0)
	{
		string = pj_str("*");
	}
	else if(key.compare("#") == 0)
	{
		string = pj_str("#");
	}
	else 
	{
		str = fromQString(key);
		string = pj_str(str);
		flag = 1;
	}
	pj_status_t status = pjsua_call_dial_dtmf(callId, &string);
	if(status != PJ_SUCCESS)
	{
		QMessageBox::information(NULL, "Error!", QString("Cannot play %1 !").arg(key));
	}
	if(flag == 1)
	{
		free(str);
	}
}

void keypadForm::keyPressed()
{
	QObject * s = sender();
	playSound(((QPushButton*)s)->text());
}