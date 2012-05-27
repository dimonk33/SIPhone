#ifndef _CALL_H_
#define _CALL_H_

#include <QtGui/QMainWindow>
#include <QObject>

#include <QMessageBox.h>


#include "ui_callForm.h"
#include "sipCore.h"
#include "keypadForm.h"


#include <pjlib.h>
#include <pjlib-util.h>
#include <pjnath.h>
#include <pjsip.h>
#include <pjsip_ua.h>
#include <pjsip_simple.h>
#include <pjsua-lib/pjsua.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>

class CallWindow : public QMainWindow
{
	Q_OBJECT

public:
	CallWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CallWindow();
	void setCallerInfo(char * callerInfo); 
	void setStatusInfo(char * statusInfo);
	void setCallId(pjsua_call_id _callId);
	pjsua_call_id getCallId();
	void hideAnswerButton();
	void showAnswerButton();


	int status;
	QWidget * keypad;

private:
	Ui::callForm ui;
	pjsua_call_id callId;
	
	QWidget * console;


	public slots:
		void answerButtonPressed();
		void hangUpButtonPressed();
		void on_call_end_slot();
		void keypad_show();
		void call_answered();
};

#endif