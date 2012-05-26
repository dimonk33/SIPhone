#ifndef _SIPCORE_H_
#define _SIPCORE_H_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
#include <QMessageBox>

#include "callWindow.h"
//#include "globals.h"

#define THIS_FILE       __FILE__

#define SIP_DOMAIN      "192.168.1.9"
#define SIP_USER        "201"
#define SIP_PASSWD      "201"


#define PUBLIC_ADDRESS "192.168.1.9"
#define CONFIG_PORT 5065


#define POOL_MEMORY_CREATION 1024
#define POOL_MEMORY_CREATION_INCREMENT 32

#define BUDDY_URI "sip:200@192.168.1.9"

#define FILE_CONFIG_ACCOUNT_SETTINGS "account.inf"
#define REALM "asterisk"

static void error_exit(const char *title, pj_status_t status)
{
	pjsua_perror(THIS_FILE, title, status);
	pjsua_destroy();
	exit(status);
}

struct statuses
{
	int registerStatus;
	int callStatus;
	pj_str_t lastError;
};

struct config_struct
{
	char * sipUser;
	char * sipPassword;
	char * publicAddress; //192.168.1.9
	char * id; // sip:user@domain
	int portNumber;	//port
	char * uri; //sip:domain
	char * realm; //realm
	char * inputDevice;
	char * outputDevice;
};

enum CALL_STATUSES
{
	// 0 - nothing
	// 1 - try to call
	// 2 - speaking
	CALL_STATUS_INACTIVE,
	CALL_STATUS_CALLING,
	CALL_STATUS_WAITING,
	CALL_STATUS_THINKING,
	CALL_STATUS_SPEAKING
};

class sipCore: public QObject
{
	Q_OBJECT 
	void * mainWindow;
	pjsua_acc_id acc_id;
	pjsua_buddy_id * ids;
	int numberOfBuddies;
	pjsua_player_id * playerId;

public:
	static sipCore * object;
	statuses status;
	config_struct config;
	pjsua_call_id callId;
public:
	sipCore();
	~sipCore();
	int init();
	void load_config();	//not used yet
	void saveConfig();

	void saveDevices(char * inputDevice, char * outputDevice);

	int createTransport();
	int registerToServer();
	void load_devices();
	void addBuddies();
	int makeCall(int index);

	void playMedia(char * fileName);


	void addBuddy(char * name, char * icon);

	//wrappers for emit signals

	void incom(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)	{ emit son_incoming_call(acc_id, call_id, rdata);}
	void on_reg_state2_emit(pjsua_acc_id acc_id, pjsua_reg_info *info)	{ emit son_reg_state2(acc_id, info);}
	void on_call_ended() { emit son_call_ended(); }
	void on_buddy_state(pjsua_buddy_id buddy_id);
	void addContact(char * name, char * URI);
	void deleteContact(int row);
	void editContact(int row, char * name, char * URI);
	void on_call_answered() {emit son_call_answered();}

	char * getBuddyURI(int row);

signals:
	void son_incoming_call(int acc_id, int call_id, void * rdata);
	void addNewBuddy(char *, char *);
	void son_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);
	void son_call_ended();
	void son_buddy_status_change(int row, int status);
	void son_call_answered();
};

char * copyString(char *);


void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata);
void on_call_media_state(pjsua_call_id call_id);
void on_call_state(pjsua_call_id call_id, pjsip_event *e);
void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info);
void on_call_transfer_status(pjsua_call_id call_id, int st_code, const pj_str_t *st_text, pj_bool_t final, pj_bool_t *p_cont);
void on_buddy_state(pjsua_buddy_id buddy_id);
pjsip_redirect_op  on_call_redirected(pjsua_call_id call_id, const pjsip_uri *target, const pjsip_event *e);



#endif