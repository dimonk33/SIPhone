#include "addUserForm.h"



addUserForm::addUserForm(QWidget * _parent /* = 0 */, Qt::WFlags flags /* = 0 */)
{
	ui.setupUi(this);
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(okButtonPressed()));
	connect(ui.cancelButton, SIGNAL(pressed()), this, SLOT(cancelButtonPressed()));
	forChanges = 0;
	parent = _parent;
}

addUserForm::~addUserForm()
{

}

void addUserForm::cancelButtonPressed()
{
	if(forChanges == 0)
	{
		this->hide();
		delete this;
	}
	else
		forChanges = -1;
}

void addUserForm::setName(char * name)
{
	ui.nameEdit->setText(name);
}

void addUserForm::setName(QString name)
{
	ui.nameEdit->setText(name);
}

void addUserForm::setURI(char * URI)
{
	ui.addressEdit->setText(URI);
}

QString addUserForm::getName()
{
	return ui.nameEdit->text();
}

QString addUserForm::getURI()
{
	return ui.addressEdit->text();
}

void addUserForm::okButtonPressed()
{
	/*
	*	1. Проверяем на то, ввели ли мы что-нить в sip adress 
	*	2. Проверяем на то, является ли данный адрес валидным
	*	
	*
	*/

	QString sipAddress = ui.addressEdit->text();
	if(sipAddress.size() == 0)
	{
		QMessageBox::warning(this, "Error", "Please enter sip address");
		return;
	}

	QRegExp validAddress("{sip:}[0-9]*");

/*	if(sipAddress.count(validAddress) == 0);
	{
		QMessageBox::warning(this, "Error", "Please enter valid sip address");
		return;
	}*/

	////////////////подгоняем до сюда адрес до валидного состояния
	if(forChanges == 0)	//adding contact
	{
		char * name;
		char * URI;

		QByteArray ba = sipAddress.toLatin1();
		URI = copyString(ba.data());
		ba = ui.nameEdit->text().toLatin1();
		if(ba.size() == 0)
			name = copyString(URI);
		else
			name = copyString(ba.data());

		sipCore::object->addContact(name, URI);

		this->hide();
		delete this;
		return;
	}
	forChanges = 1;
	done(1);
}