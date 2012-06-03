#ifndef QT_H
#define QT_H

#include <QtGui/QMainWindow>
#include <QtGui/QListWidgetItem>
#include "ui_qt.h"
#include "sipCore.h"
#include "aboutForm.h"
#include "addUserForm.h"
#include "optionsForm.h"
#include <QSettings>

class qt : public QMainWindow
{
	Q_OBJECT

//		sipCore  core;

public:
	qt(QWidget *parent = 0, Qt::WFlags flags = 0);
	~qt();

private:
	Ui::qtClass ui;

signals:

public slots:

		void showAboutForm();
		void on_incoming_call_slot(int acc_id, int call_id, void * rdata);
		void addBuddy(char * name, char * URI);
		void makeCall();
		void addContact();
		void deleteContact();
		void editContact();
		void showHelp();
		void on_reg_state2_slot(pjsua_acc_id acc_id, pjsua_reg_info *info);
		void on_buddy_state_change(int row, int status);
		void on_exit();
		void showOptions();
		void makeCustomCall();
	
};

char * fromQString(QString & string);

#endif // QT_H
