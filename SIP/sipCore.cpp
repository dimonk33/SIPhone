#include <sipCore.h>

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

	int result = createTransport();


	//TODO: create one or more SIP accounts

	//TODO: add one or more buddies 

	//TODO: configure sound device,		codec settings, and other media settings
}

sipCore::~sipCore()
{

	pj_status_t status;
	status = pjsua_destroy();
}

int sipCore::init()
{

};

int sipCore::createTransport()
{

	pjsua_transport_config transportConfig;
	pjsua_transport_info transportInfo;

	pjsua_transport_config_default(&transportConfig);

	transportConfig.public_addr = public_address;

	transportConfig.port = 5060;

	pj_status_t status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportConfig, NULL)
	


}