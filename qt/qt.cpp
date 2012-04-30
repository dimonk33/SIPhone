#include "qt.h"

qt::qt(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.actionAbout, SIGNAL(activated()), this, SLOT(showAboutForm()));
	//core = new sipCore();
	connect(&core, SIGNAL(addNewBuddy(char *, char *)), this, SLOT(addBuddy(char *, char *)));
//	core.makeWindow(this);
//	connect(this, SIGNAL(bla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)), this, SLOT(blabla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)));
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

void qt::blabla(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)
{
	pjsua_acc_info caller_info;

	pj_status_t status = pjsua_acc_get_info(acc_id, &caller_info);
	if(status == PJ_SUCCESS)
	{
		CallWindow * w = new CallWindow();
		w->setCallerInfo(caller_info.acc_uri.ptr);
		w->setStatusInfo("incoming call");
		w->setCallId(call_id);
		w->show();
	}

}

void qt::bbb(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)
{
	 emit bla(acc_id, call_id, rdata);
}

void qt::addBuddy(char * name, char * URI)
{
	QListWidgetItem * newItem = new QListWidgetItem(name, ui.listWidget);
}