#include "callWindow.h"

CallWindow::CallWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	ui.progressBar->hide();
	QObject::connect(ui.answerButton, SIGNAL(pressed()), this, SLOT(answerButtonPressed()));
	QObject::connect(ui.hangUpButton, SIGNAL(pressed()), this, SLOT(hangUpButtonPressed()));
}

CallWindow::~CallWindow()
{
	
}

void CallWindow::setCallerInfo(char * callerInfo)
{
	ui.callingLabel->setText(callerInfo); 
}

void CallWindow::setStatusInfo(char * statusInfo)
{
	ui.statusLabel->setText(statusInfo);
}

void CallWindow::setCallId(pjsua_call_id _callId)
{
	callId = _callId;
}

pjsua_call_id CallWindow::getCallId()
{
	return callId;
}

void CallWindow::answerButtonPressed()
{
//	QMessageBox msgBox;
//	msgBox.setText("Button pressed!");
//	msgBox.exec();

	//QDebug() << " answerButtonPressed";

	ui.answerButton->hide();
	pj_status_t status = pjsua_call_answer(callId, 100 /* what code??*/, NULL, NULL);
	if(status != PJ_SUCCESS) error_exit("cannot answer call! ", status);
}

void CallWindow::hangUpButtonPressed()
{
	pj_status_t status = pjsua_call_hangup(callId, 100/*what code???*/, NULL, NULL);
	if(status != PJ_SUCCESS) error_exit("cannot hang up call! ", status);
	this->hide();

	delete(this);
}