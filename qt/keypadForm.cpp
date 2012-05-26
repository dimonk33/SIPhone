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
	if(key.compare("*") == 0)
	{
		sipCore::object->playMedia("sounds/DtmfStar.wav");
	}
	else if(key.compare("#") == 0)
	{
		sipCore::object->playMedia("sounds/Dtmf-.wav");
	}
	else 
	{
		QString playMediaString = QString("sounds/Dtmf%1.wav").arg(key);
		char * str = fromQString(playMediaString);
		sipCore::object->playMedia(str);
		free(str);
	}
}

void keypadForm::keyPressed()
{
	QObject * s = sender();
	playSound(((QPushButton*)s)->text());
}