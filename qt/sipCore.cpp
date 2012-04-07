#include "sipCore.h"

sipCore::sipCore()
{
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
	log_cfg.log_filename = "logfile.log";
	log_cfg.log_file_flags = PJ_O_APPEND;

	// setup callbacks for application
	cfg.cb.on_incoming_call = &on_incoming_call;
	cfg.cb.on_call_media_state = &on_call_media_state;
	cfg.cb.on_call_state = &on_call_state;

	status = pjsua_init(&cfg, &log_cfg, &media_cfg);
	if (status != PJ_SUCCESS) error_exit("cannot initialize pjsua!", status);


	// creating transport
	int result = createTransport();

	status = pjsua_start();
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

	createTransport();

	registerToServer();

	addBuddy();



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

};

int sipCore::createTransport()
{
	//TODO: make a loading config from a file or GUI
	
	QSettings settings ("transport.ini", QSettings:IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	
	pjsua_transport_config transportConfig;
//	pjsua_transport_info transportInfo;

	pjsua_transport_config_default(&transportConfig);

	//transportConfig.tls_setting = ???
	//transportConfig.bound_addr = ???
	//transportConfig.qos_params = ????
	//transportConfig.qos_type = ???

	transportConfig.public_addr = settings.value("public_address", PUBLIC_ADDRESS).toString();
	transportConfig.port = settings.value("port", CONFIG_PORT).toInt;

	pj_status_t status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportConfig, NULL);

	if (status != PJ_SUCCESS) error_exit("Error creating transport", status);
}

int sipCore::registerToServer()
{
	//TODO: load config from file or GUI
	
	pjsua_acc_config cfg;

	pjsua_acc_config_default(&cfg);
	cfg.id = pj_str("sip:" SIP_USER "@" SIP_DOMAIN);
	cfg.reg_uri = pj_str("sip:" SIP_DOMAIN);
	cfg.cred_count = 1;
	cfg.cred_info[0].realm = pj_str(SIP_DOMAIN);
	cfg.cred_info[0].scheme = pj_str("digest");
	cfg.cred_info[0].username = pj_str(SIP_USER);
	cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	cfg.cred_info[0].data = pj_str(SIP_PASSWD);
	
	pj_status_t status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
	
}

void sipCore::addBuddy()
{
	//TODO: load buddies from file or GUI
	pjsua_buddy_config buddyConfig;
	buddyConfig.uri = BUDDY_URI;
	buddyConfig.user_data = NULL;

	pjsua_buddy_id id;

	pj_status_t status = pjsua_buddy_add(&buddyConfig, &id);
	if (status != PJ_SUCCESS) error_exit("Error adding buddy!", status);

}

