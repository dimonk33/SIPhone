#include "sipCore.h"

sipCore::sipCore()
{

	//initializing members of class
	ids = NULL;
	numberOfBuddies = 0;


	pj_status_t status;
	status = pjsua_create();
	if(status != PJ_SUCCESS)	error_exit("cannot create pjsua!", status);

	/*pj_pool_t pool = pjsua_pool_create("pool", POOL_MEMORY_CREATION, POOL_MEMORY_CREATION_INCREMENT);
	if( pool == NULL ) error_exit("cannot create pool!");*/

	pjsua_config cfg;
	pjsua_logging_config log_cfg;
	pjsua_media_config media_cfg;

	pjsua_config_default(&cfg);
	pjsua_logging_config_default(&log_cfg);
	pjsua_media_config_default(&media_cfg);

	log_cfg.console_level = 4;
	log_cfg.log_filename = pj_str("logfile.log");
	log_cfg.log_file_flags = PJ_O_APPEND;

	// setup callbacks for application
	void (* on_incoming_callcb)(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rx_data) = on_incoming_call;
	cfg.cb.on_incoming_call = on_incoming_callcb;
	void (* on_call_media_statecb)(pjsua_call_id call_id) = on_call_media_state;
	void (* on_call_statecb)(pjsua_call_id call_id, pjsip_event *e) = on_call_state;

	cfg.cb.on_call_media_state = &on_call_media_state;
	cfg.cb.on_call_state = &on_call_state;

	status = pjsua_init(&cfg, &log_cfg, &media_cfg);
	if (status != PJ_SUCCESS) error_exit("cannot initialize pjsua!", status);


	// creating transport
	int result = createTransport();

	status = pjsua_start();
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

	registerToServer();

	addBuddy();

	//pj_str_t uri = pj_str(BUDDY_URI);
	//status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
	//if (status != PJ_SUCCESS) error_exit("Error making call", status);

	CallWindow * w = new CallWindow();
//	w->setCallerInfo(caller_info.acc_uri.ptr);
	w->setStatusInfo("trying window");
//	w->setCallId(call_id);
	w->show();


	//TODO: create one or more SIP accounts

	//TODO: add one or more buddies 

	//TODO: configure sound device,	codec settings, and other media settings
}

sipCore::~sipCore()
{
	//подумать, что надо закрыть и т.д.
	pj_status_t status;
	status = pjsua_destroy();
	if (status != PJ_SUCCESS) error_exit("Error destrioy pjsua!", status);
}

int sipCore::init()
{
	return 1;
};

int sipCore::createTransport()
{
	//TODO: make a loading config from a file or GUI
	
	QSettings settings("transport.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	
	pjsua_transport_config transportConfig;
//	pjsua_transport_info transportInfo;

	pjsua_transport_config_default(&transportConfig);

	//transportConfig.tls_setting = ???
	//transportConfig.bound_addr = ???
	//transportConfig.qos_params = ????
	//transportConfig.qos_type = ???

	//transportConfig.public_addr.ptr = settings.value("public_address", PUBLIC_ADDRESS).toString().toAscii().data();
	//transportConfig.public_addr.slen = strlen(transportConfig.public_addr.ptr);
	int size = strlen(settings.value("public_address", PUBLIC_ADDRESS).toString().toAscii().data()) + 1;
	char * address = (char*)malloc(sizeof(char) * size);
	memcpy(address, settings.value("public_address", PUBLIC_ADDRESS).toString().toAscii().data(), size);
	transportConfig.public_addr = pj_str(address);
	transportConfig.port = settings.value("port", CONFIG_PORT).toInt();

	pj_status_t status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportConfig, NULL);

	if (status != PJ_SUCCESS) error_exit("Error creating transport", status);

	free(address); // TODO: needs or not?
	return 1;
}

char * copyString(char * string)
{
	int size = strlen(string) + 1;
	char * temp = (char*)malloc(sizeof(char) * size);
	memcpy(temp,string, size);
	return temp;
}

int sipCore::registerToServer()
{
	pjsua_acc_config cfg;

	QSettings settings("registerInformation.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	pjsua_acc_config_default(&cfg);


	//cfg.id.ptr = settings.value("id", "sip:" SIP_USER "@" SIP_DOMAIN).toString().toAscii().data();
	//cfg.id.slen = strlen(cfg.id.ptr);

//	int size = strlen(settings.value("id", "sip:" SIP_USER "@" SIP_DOMAIN).toString().toAscii().data()) + 1;
//	char * temp = (char*)malloc(sizeof(char) * size);
//	memcpy(temp, settings.value("id", "sip:" SIP_USER "@" SIP_DOMAIN).toString().toAscii().data(), size);

	cfg.id = pj_str(copyString(settings.value("id", "sip:" SIP_USER "@" SIP_DOMAIN).toString().toAscii().data()));

	
	cfg.reg_uri = pj_str(copyString(settings.value("uri", "sip:" SIP_DOMAIN).toString().toAscii().data()));
	cfg.cred_count = 1;
	//cfg.cred_info[0].realm.ptr = settings.value("uri", SIP_DOMAIN).toString().toAscii().data();
	//cfg.cred_info[0].realm.slen = strlen(cfg.cred_info[0].realm.ptr);
	cfg.cred_info[0].realm = pj_str(copyString(settings.value("uri", SIP_DOMAIN).toString().toAscii().data()));

	//stupid way to do like this. Much more easier to do
	//cfg.cred_info[0].realm = pj_str(settings.value("uri", SIP_DOMAIN).toString().toAscii().data());
	//too sad that i saw it too late 
	// :,(
	//shit...
	
	cfg.cred_info[0].scheme = pj_str("digest");   ////////////////////////////////////////////////////////////////////////// ??????????????????
	//cfg.cred_info[0].username.ptr = settings.value("username", SIP_USER).toString().toAscii().data();
	//cfg.cred_info[0].username.slen = strlen(cfg.cred_info[0].username.ptr);
	cfg.cred_info[0].username = pj_str(copyString(settings.value("username", SIP_USER).toString().toAscii().data()));
	cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;	////////////////////////////////////////////////////////////////////////// ????????????????
	cfg.cred_info[0].data = pj_str(copyString(settings.value("password", SIP_PASSWD).toString().toAscii().data()));
	
	pj_status_t status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
	return 1; // TODO: нужно ли очищать память? или она ещё пригодится?
}

void sipCore::addBuddy()
{
	QSettings settings("buddies.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	int numberOfBuddies = settings.value("numberOfBuddies", 0).toInt();
	pjsua_buddy_config buddyConfig;
	
	if(numberOfBuddies == 0)
	{
		ids = new pjsua_buddy_id[1];
		buddyConfig.uri = pj_str(BUDDY_URI);
		buddyConfig.user_data = NULL;
		
		pj_status_t status = pjsua_buddy_add(&buddyConfig, &ids[0]);
		if (status != PJ_SUCCESS) error_exit("Error! cannot add one buddy :(", status);
	}
	else
	{
		ids = new pjsua_buddy_id[numberOfBuddies];
		for(int i = 0; i < numberOfBuddies; i++)
		{
			buddyConfig.uri = pj_str(settings.value(QString("buddy%d").arg(i)).toByteArray().data());
			buddyConfig.user_data = NULL;
					
			pj_status_t status = pjsua_buddy_add(&buddyConfig, &ids[i]);
			if (status != PJ_SUCCESS) error_exit("Error! cannot add buddy", status);

		}
	}
}

void sipCore::on_call_media_state(pjsua_call_id call_id) { }

void sipCore::on_call_state(pjsua_call_id call_id, pjsip_event *e) { }

void sipCore::on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)
{
	pjsua_acc_info caller_info;

	pj_status_t status = pjsua_acc_get_info(acc_id, &caller_info);
	if(status == PJ_SUCCESS)
	{
		CallWindow * w = new CallWindow();
		w->setCallerInfo(caller_info.acc_uri.ptr);
		w->setStatusInfo("incoming call");
		w->setCallId(call_id);
		w->show();
	}
}