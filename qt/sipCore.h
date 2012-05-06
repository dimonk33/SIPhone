#ifndef _SIPCORE_H_
#define _SIPCORE_H_


#include <stdio.h>
#include <stdlib.h>

#include <pjlib.h>
#include <pjlib-util.h>
#include <pjnath.h>
#include <pjsip.h>
#include <pjsip_ua.h>
#include <pjsip_simple.h>
#include <pjsua-lib/pjsua.h>
#include <pjmedia.h>
#include <pjmedia-codec.h>

#include <qsettings.h>
#include <QTextCodec.h>

#include "callWindow.h"
//#include "globals.h"

#define THIS_FILE       __FILE__

#define SIP_DOMAIN      "192.168.1.9"
#define SIP_USER        "200"
#define SIP_PASSWD      "diskonnect"


#define PUBLIC_ADDRESS "192.168.1.9"
#define CONFIG_PORT 5065


#define POOL_MEMORY_CREATION 1024
#define POOL_MEMORY_CREATION_INCREMENT 32

#define BUDDY_URI "sip:201@192.168.1.9"

#define FILE_CONFIG_ACCOUNT_SETTINGS "account.inf"
#define REALM "asterisk"

static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(status);
}

class sipCore: public QObject
{
	Q_OBJECT 
	void * mainWindow;
	pjsua_acc_id acc_id;
	pjsua_buddy_id * ids;
	int numberOfBuddies;
	

public:
	sipCore();
	~sipCore();
	int init();
	void load_config();
	void makeWindow(void * _mainWindow);


	//how to make it non-static


	int createTransport();
	int registerToServer();
	void addBuddies();
	int makeCall(char * to);


	void addBuddy(char * name, char * icon);


signals:
	void son_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);
	void addNewBuddy(char *, char *);
};

void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);
void on_call_media_state(pjsua_call_id call_id);
void on_call_state(pjsua_call_id call_id, pjsip_event *e);



#endif