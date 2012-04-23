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


#define THIS_FILE       __FILE__

#define SIP_DOMAIN      "192.168.222.1"
#define SIP_USER        "201"
#define SIP_PASSWD      "201"


#define PUBLIC_ADDRESS "192.168.222.1"
#define CONFIG_PORT 5060


#define POOL_MEMORY_CREATION 1024
#define POOL_MEMORY_CREATION_INCREMENT 32

#define BUDDY_URI "sip:202@example.com"

#define FILE_CONFIG_ACCOUNT_SETTINGS "account.inf"

class sipCore
{
	pjsua_acc_id acc_id;
	pjsua_buddy_id * ids;
	int numberOfBuddies;

public:
	sipCore();
	~sipCore();
	int init();
	void load_config();


	//how to make it non-static
	static void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);
	static void on_call_media_state(pjsua_call_id call_id);
	static void on_call_state(pjsua_call_id call_id, pjsip_event *e);

	int createTransport();
	int registerToServer();
	void addBuddy();


};

static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(1);
}



#endif