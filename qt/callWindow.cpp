#include "callWindow.h"

#include "callTextForm.h"

CallWindow::CallWindow(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
//	ui.progressBar->hide();
	QObject::connect(ui.answerButton, SIGNAL(pressed()), this, SLOT(answerButtonPressed()));
	QObject::connect(ui.hangUpButton, SIGNAL(pressed()), this, SLOT(hangUpButtonPressed()));
	QObject::connect(sipCore::object, SIGNAL(son_call_ended()), this, SLOT(on_call_end_slot()));
	QObject::connect(ui.keyboardButton, SIGNAL(pressed()), this, SLOT(keypad_show()));
	QObject::connect(sipCore::object, SIGNAL(son_call_answered()), this, SLOT(call_answered()));
	status = 0;
	holdStatus = 0;
	keypad = NULL;
	setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint);
	console = new callTextForm();
	//console->show();
	QObject::connect(sipCore::object, SIGNAL(son_dtmf_digit(char)), (callTextForm*)console, SLOT(dtfmDigitIsComing(char)));
	QObject::connect(sipCore::object, SIGNAL(son_message_received(char *)), (callTextForm*)console, SLOT(messageReceived(char *)));
	QObject::connect(ui.consoleButton, SIGNAL(pressed()), this, SLOT(consoleButtonPressed()));
	QObject::connect(ui.holdButton, SIGNAL(pressed()), this, SLOT(holdButtonPressed()));
}

CallWindow::~CallWindow()
{
	delete console;
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
	pj_status_t status = pjsua_call_answer(callId, 200 /* what code??*/, NULL, NULL);
	if(status != PJ_SUCCESS) error_exit("cannot answer call! ", status);
	ui.statusLabel->setText("Speaking...");
	sipCore::object->status.callStatus = 2;
}

void CallWindow::hangUpButtonPressed()
{
	if(status == 0)
	{
		pj_status_t status = pjsua_call_hangup(callId, 100/*what code???*/, NULL, NULL);
		if (status == 171140)	//session has been already terminated
		{
			//
		}
		else 
		if(status != PJ_SUCCESS) error_exit("cannot hang up call! ", status);
	}
	this->hide();
	if(keypad != NULL)
		((keypadForm*)keypad)->closeButtonPressed();
	sipCore::object->status.callStatus = 0;

	delete this;
}

void CallWindow::hideAnswerButton()
{
	ui.answerButton->hide();
}

void CallWindow::showAnswerButton()
{
	ui.answerButton->show();
}

/*void startRecord();

void stopRecord();*/

void CallWindow::on_call_end_slot()
{
	if(sipCore::object->status.callStatus == -1)
	{
		QString errorMessage = sipCore::object->status.lastError.ptr;
		errorMessage.resize(sipCore::object->status.lastError.slen);
		QMessageBox::information(this, "Cannot make call!", errorMessage);
	}
	status = 1;
	ui.statusLabel->setText("Call ended");
	ui.hangUpButton->setText("close");
//	this->hide();
//	delete(this);
//	stopRecord();
}

void CallWindow::call_answered()
{
	ui.statusLabel->setText("Speaking...");
//	startRecord();
}

void CallWindow::keypad_show()
{
	keypad = new keypadForm(this);
	keypad->show();
}

void CallWindow::consoleButtonPressed()
{

	console->setVisible(!(console->isVisible()));
}

void CallWindow::holdButtonPressed()
{
	if(holdStatus == 0)
	{
		holdStatus = 1;
		pjsua_call_set_hold(callId, NULL);
		ui.holdButton->setText("Unhold");
		((callTextForm*)console)->messageReceived("Holding call...");
	}
	else 
	{
		holdStatus = 0;
		pjsua_call_reinvite(callId, PJSUA_CALL_UNHOLD, NULL);
		ui.holdButton->setText("Hold");
		((callTextForm*)console)->messageReceived("Unholding call...");
	}

}

/*pjsua_recorder_id * id;

void startRecord()
{

	char * fileName = "d:\\record.wav";
	pj_str_t ff = pj_str(fileName);
	pj_status_t status = pjsua_recorder_create(&ff, 0, NULL, 0,	0,	id)	;
	if(status != PJ_SUCCESS)
	{
		printf("cannot create recorder");
	}
}

void stopRecord()
{
	pj_status_t status = pjsua_recorder_destroy( 0 );
	if(status != PJ_SUCCESS)
	{
		printf("cannot destroy recorder");
	}
}*/