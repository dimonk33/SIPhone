#include "optionsForm.h"



optionsForm::optionsForm(QWidget * parent /* = 0 */, Qt::WFlags flags /* = 0 */)
{
	ui.setupUi(this);
	connect(ui.okButton, SIGNAL(pressed()), this, SLOT(okButtonPressed()));
	connect(ui.cancelButton, SIGNAL(pressed()), this, SLOT(cancelButtonPressed()));
}

optionsForm::~optionsForm()
{

}

void optionsForm::okButtonPressed()
{
	this->done(1);
}

void optionsForm::cancelButtonPressed()
{
	this->done(-1);
}