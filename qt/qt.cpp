#include "qt.h"

qt::qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.actionAbout, SIGNAL(activated()), this, SLOT(showAboutForm()));
	connect(ui.actionExit,  SIGNAL(activated()), this, SLOT(on_exit()));
	//core = new sipCore();
	connect(&core, SIGNAL(addNewBuddy(char *, char *)), this, SLOT(addBuddy(char *, char *)));
//	core.makeWindow(this);
//	connect(this, SIGNAL(bla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)), this, SLOT(blabla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)));
	connect(ui.callButton, SIGNAL(pressed()), SLOT(makeCall()));
	connect(&core, SIGNAL(son_incoming_call(int, int, void * )), this, SLOT(on_incoming_call_slot(int, int, void *)));
	connect(&core, SIGNAL(son_reg_state2(pjsua_acc_id , pjsua_reg_info *)), this, SLOT(on_reg_state2_slot(pjsua_acc_id, pjsua_reg_info *)));
	core.init();
}

qt::~qt()
{

}

void qt::showAboutForm()
{
	aboutForm about;
	about.exec();
}

void qt::on_incoming_call_slot(int acc_id, int call_id, void * rdata)
{
	pjsua_acc_info caller_info;
	pj_status_t status = pjsua_acc_get_info(acc_id, &caller_info);
	
	pjsua_call_info call_info;
	status = pjsua_call_get_info(call_id, &call_info);
	
	if(status == PJ_SUCCESS)
	{
		CallWindow * w = new CallWindow();
		w->setCallerInfo(call_info.remote_contact.ptr);
		w->setStatusInfo("incoming call");
		w->setCallId(call_id);
		w->show();
	}
}

/*void qt::bbb(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)
{
	 emit bla(acc_id, call_id, rdata);
}*/

void qt::addBuddy(char * name, char * URI)
{
	QListWidgetItem * newItem = new QListWidgetItem(name, ui.listWidget);
	newItem->setTextColor(QColor(Qt::gray));
}

void qt::makeCall()
{
	if(core.status.registerStatus != 1)
	{
		QMessageBox::information(this, "Error", "Cannot make call - you are not registered to server");
		return;
	}
	QList<QListWidgetItem*> selectedItems = ui.listWidget->selectedItems();
	if(selectedItems.size() == 0)
	{
		QMessageBox::information(this, "!!!", "Please choose abonent");
		return;
	}

	QListWidgetItem * item = selectedItems.at(0);
	QString address = item->text();
	QByteArray ba = address.toLatin1();
	core.makeCall(ba.data());
}

void qt::on_reg_state2_slot(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
	//pjsip_regc_info info;
	//	pjsip_regc_get_info(info->cbparam->regc, &info);
	pjsua_acc_info acc_info;
	pjsua_acc_get_info(acc_id, &acc_info);
	if(acc_info.status == PJSIP_SC_OK) // разобраться что за коды
	{
		core.status.registerStatus = 1;
		ui.registerStatus->setText("registered");
	}
	else if (acc_info.status == PJSIP_SC_FORBIDDEN)
	{
		core.status.registerStatus = 0;
		ui.registerStatus->setText("error - forbidden");
	}
	else if (acc_info.status == PJSIP_SC_REQUEST_TIMEOUT)
	{
		core.status.registerStatus = 0;
		ui.registerStatus->setText("error - timeout");
	}
}

void qt::on_exit()
{
	exit(0);
}

void qt::on_buddy_state_change(int row, int status)
{
	QListWidgetItem * item = ui.listWidget->item(row);
	if(item == NULL) return;
	if(status == 0) item->setTextColor(QColor(Qt::gray));
	else if(status == 1) item->setTextColor(QColor(Qt::green));
	else if(status == 2) item->setTextColor(QColor(Qt::yellow));
}