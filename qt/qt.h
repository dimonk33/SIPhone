#ifndef QT_H
#define QT_H

#include <QtGui/QMainWindow>
#include <QtGui/QListWidgetItem>
#include "ui_qt.h"
#include "sipCore.h"
#include "aboutForm.h"

class qt : public QMainWindow
{
	Q_OBJECT

	sipCore  core;

public:
	qt(QWidget *parent = 0, Qt::WFlags flags = 0);
//	void bbb(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);
	~qt();
//	void blalba(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata) {emit bla(acc_id, call_id, rdata);}

private:
	Ui::qtClass ui;

signals:
//	void bla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);

public slots:
	
	void showAboutForm();
	void on_incoming_call_slot(int acc_id, int call_id, void * rdata);
	void addBuddy(char * name, char * URI);
	void makeCall();
	void on_reg_state2_slot(pjsua_acc_id acc_id, pjsua_reg_info *info);
	void on_buddy_state_change(int row, int status);
	void on_exit();
};

#endif // QT_H
