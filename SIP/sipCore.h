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

#define THIS_FILE       __FILE__

#define SIP_DOMAIN      "example.com"
#define SIP_USER        "alice"
#define SIP_PASSWD      "secret"


#define public_address "192.168.222.1"


#define POOL_MEMORY_CREATION 1024
#define POOL_MEMORY_CREATION_INCREMENT 32


class sipCore
{
	pjsua_acc_id acc_id;


public:
	sipCore();
	~sipCore();
	int init();
	void load_config();


	virtual void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata) = 0;
	virtual void on_call_media_state(pjsua_call_id call_id) = 0;
	virtual void on_call_state(pjsua_call_id call_id, pjsip_event *e) = 0;

	int createTransport();


};

static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(1);
}



#endif