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
	connect(ui.actionAdd_contact_2, SIGNAL(activated()), this, SLOT(addContact()));
	connect(ui.actionDelete_contact, SIGNAL(activated()), this, SLOT(deleteContact()));
	connect(ui.actionEdit_contact, SIGNAL(activated()), this, SLOT(editContact()));
	connect(ui.actionRun_help, SIGNAL(activated()), this, SLOT(showHelp()));
	connect(ui.actionCall, SIGNAL(activated()), this, SLOT(makeCall()));
	connect(ui.addButton, SIGNAL(pressed()), this, SLOT(addContact()));
	connect(ui.actionOptions, SIGNAL(activated()), this, SLOT(showOptions()));
	connect(ui.optionsButton, SIGNAL(pressed()), this, SLOT(showOptions()));
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

	int row = ui.listWidget->currentRow();
	if(row == -1)
	{
		QMessageBox::information(this, "!!!", "Please choose abonent");
		return;
	}


	/*QListWidgetItem * item = selectedItems.at(0);
	QString address = item->text();
	QByteArray ba = address.toLatin1();
	core.makeCall(ba.data());*/
	core.makeCall(row);
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
	//sipCore::object->saveConfig();
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

void qt::addContact()
{
	addUserForm * form =new addUserForm(this);
	form->show();
}

void qt::deleteContact()
{
	int row = ui.listWidget->currentRow();
	if(row == -1)
	{
		QMessageBox::information(this, "Warning", "Cannot delete empty contact");
		return;
	}

	QListWidgetItem * item = ui.listWidget->item(row);
	ui.listWidget->removeItemWidget(item);

//	item->setHidden(1);
	delete item;
	////
	ui.listWidget->update();
	sipCore::object->deleteContact(row);
}

void qt::editContact()
{
	addUserForm form(this);

	int row = ui.listWidget->currentRow();
	
	char * URI_old = sipCore::object->getBuddyURI(row);
	form.setURI(URI_old);
	free(URI_old);

	QString name = ui.listWidget->item(row)->text();
	form.setName(name);
	form.forChanges = -1;
	form.exec();
	if(form.forChanges == -1) return;

	name = form.getName();
	QString URI = form.getURI();

	char * cName = copyString(name.toAscii().data());
	char * cURI = copyString(URI.toAscii().data());

	sipCore::object->editContact(row, cName, cURI);
	(ui.listWidget->item(row))->setText(name);
	ui.listWidget->update();

	free(cName);
	free(cURI);
}

void qt::showHelp()
{
	QMessageBox::information(this, "Help", "Not implemented yet :)");
}

void qt::showOptions()
{
	optionsForm form(this);

	int changes = 0;
	form.ui.realm->setText(sipCore::object->config.realm);
	form.ui.publicAddress->setText(sipCore::object->config.publicAddress);
	form.ui.portNumber->setText(QString("%1").arg(sipCore::object->config.portNumber));

	form.ui.sipPassword->setText(sipCore::object->config.sipPassword);
	form.ui.sipUser->setText(sipCore::object->config.sipUser);

	pjmedia_aud_dev_info * audioDevices = new pjmedia_aud_dev_info[100];
	unsigned int numOfAudioDevices = 100;
	pj_status_t status = pjsua_enum_aud_devs(audioDevices, &numOfAudioDevices);
	if (status != PJ_SUCCESS) error_exit("Error getting sound devices", status);
	
	for (int i = 0; i < numOfAudioDevices; i++)
	{
		form.ui.InputDevices->addItem(audioDevices[i].name);
		form.ui.outputDevices->addItem(audioDevices[i].name);
	}
	
	int row = form.ui.InputDevices->findText(sipCore::object->config.inputDevice);

	if(row != -1)
		form.ui.InputDevices->setCurrentIndex(row);
	else printf("Error! config.inputDevice not found in combobox!!");

	row = form.ui.outputDevices->findText(sipCore::object->config.outputDevice);
	
	if(row != -1)
		form.ui.outputDevices->setCurrentIndex(row);
	else printf("Error! config.outputDevice not found in combobox!!");

	char * newInputDevice;
	char * newOutputDevice;
	char * newSipUser;
	char * newSipPassword;
	char * newRealm;
	char * newPublicAddress;
	int newPortNumber;

	while(1)
	{
		form.exec();

		if(form.close() == -1) return;
		newInputDevice = fromQString(form.ui.InputDevices->currentText());
		newOutputDevice = fromQString(form.ui.outputDevices->currentText());
		newSipUser = fromQString(form.ui.sipUser->text());
		newSipPassword = fromQString(form.ui.sipPassword->text());
		newRealm = fromQString(form.ui.realm->text());
		newPublicAddress = fromQString(form.ui.publicAddress->text());

		bool success;
		newPortNumber = form.ui.portNumber->text().toInt(&success);

		/*
		*** проверка на валидность данных
		*/
		if((newPortNumber < 0 || newPortNumber > 65536) || !success) 
		{
			QMessageBox::information(this, "Error", "Port number must be number in interval 0..65536");
			continue;
		}

	//////
		break;
	}

	if(strcmp(newInputDevice, sipCore::object->config.inputDevice))
	{
		free(sipCore::object->config.inputDevice);
		sipCore::object->config.inputDevice = newInputDevice;
		changes++;
	}

	if(strcmp(newOutputDevice, sipCore::object->config.outputDevice))
	{
		free(sipCore::object->config.outputDevice);
		sipCore::object->config.outputDevice = newOutputDevice;
		changes++;
	}

	if(strcmp(newSipUser, sipCore::object->config.sipUser))
	{
		free(sipCore::object->config.sipUser);
		sipCore::object->config.sipUser = newSipUser;

		free(sipCore::object->config.id);
		sipCore::object->config.id = fromQString(QString("sip:%1@%2").arg(sipCore::object->config.sipUser).arg(sipCore::object->config.publicAddress));

		changes++;
	}


	if(strcmp(newSipPassword, sipCore::object->config.sipPassword))
	{
		free(sipCore::object->config.sipPassword);
		sipCore::object->config.sipPassword = newSipPassword;
		changes++;
	}


	if(newPortNumber != sipCore::object->config.portNumber)
	{
		sipCore::object->config.portNumber = newPortNumber;
		changes++;
	}
	
/*	if(strcmp(newPublicAddress, sipCore::object->config.publicAddress))
	{
		free(sipCore::object->config.publicAddress);
		sipCore::object->config.publicAddress = newPublicAddress;
		changes++;
	}*/


	if(strcmp(newPublicAddress, sipCore::object->config.publicAddress))
	{
		free(sipCore::object->config.publicAddress);
		sipCore::object->config.publicAddress = newPublicAddress;

		free(sipCore::object->config.uri);
		sipCore::object->config.uri = fromQString(QString("sip:%1").arg(sipCore::object->config.publicAddress));

		free(sipCore::object->config.id);
		sipCore::object->config.id = fromQString(QString("sip:%1@%2").arg(sipCore::object->config.sipUser).arg(sipCore::object->config.publicAddress));


		changes++;
	}

	if(strcmp(newRealm, sipCore::object->config.realm))
	{
		free(sipCore::object->config.realm);
		sipCore::object->config.realm = newRealm;
		changes++;
	}

	if(changes != 0)
	{
		QMessageBox::information(this, "Changes", "Changes will be activated on next launch");
		sipCore::object->saveConfig();
	}
}

char * fromQString(QString & string)
{
	QByteArray ba = string.toAscii();
	char * retString = (char*) malloc (sizeof(char) * ba.size() + 1);
	memcpy(retString, ba.data(), ba.size());
	retString[ba.size()] = 0;
	return retString;
}