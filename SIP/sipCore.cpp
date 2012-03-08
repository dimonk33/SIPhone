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





}

sipCore::~sipCore()
{

	pj_status_t status;
	status = pjsua_destroy();
}

int sipCore::init()
{

};
