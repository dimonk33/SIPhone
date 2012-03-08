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

#define THIS_FILE       __FIlE__

#define SIP_DOMAIN      "example.com"
#define SIP_USER        "alice"
#define SIP_PASSWD      "secret"

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


	void on_incoming_call (pjsua_acc_id acc_id, )
};





#endif