#include "callTextForm.h"

#include "qt.h"

callTextForm::callTextForm(QWidget * _parent /* = 0 */, Qt::WFlags flags /* = 0 */)
{
	ui.setupUi(this);
	connect(ui.sendButton, SIGNAL(pressed()), this, SLOT(sendButtonPressed()));
	connect(ui.pushButton_2, SIGNAL(pressed()), this, SLOT(closeButtonPressed()));
}

callTextForm::~callTextForm()
{

}

void callTextForm::closeButtonPressed()
{
	this->hide();
}

void callTextForm::sendButtonPressed()
{
//	QString text = ui.lineEdit->text();
	char * text = fromQString(ui.lineEdit->text());
	sipCore::object->sendMessage(text);
	messageReceived(text);
	free(text);
}

void callTextForm::dtfmDigitIsComing(char digit)
{
	QString newText = ui.textBrowser->toPlainText();
	newText.append(QString("\n Received new digit: %1").arg(digit));
	ui.textBrowser->setPlainText(newText);
	this->setVisible(1);
}

void callTextForm::messageReceived(char * message)
{
	QString newText = ui.textBrowser->toPlainText();
	newText.append(message);
	ui.textBrowser->setPlainText(newText);
}
