#include "sipCore.h"
#include "qt.h"

sipCore::sipCore()
{

	//initializing members of class
	ids = NULL;
	numberOfBuddies = 0;
	object = this;
	callId = -1;
	
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
	void (* on_call_media_statecb)(pjsua_call_id call_id) = on_call_media_state;
	void (* on_call_statecb)(pjsua_call_id call_id, pjsip_event *e) = on_call_state;
	void (* on_reg_state2cb)(pjsua_acc_id acc_id, pjsua_reg_info *info) = on_reg_state2;
	void (* on_call_transfer_statuscb)(pjsua_call_id call_id, int st_code, const pj_str_t *st_text, pj_bool_t final, pj_bool_t *p_cont) = on_call_transfer_status;
	void (* on_buddy_statecb)(pjsua_buddy_id buddy_id) = ::on_buddy_state;
	pjsip_redirect_op(*  on_call_redirectedcb)(pjsua_call_id call_id, const pjsip_uri *target, const pjsip_event *e) = ::on_call_redirected;
	void (*on_dtmf_digitscb) (pjsua_call_id call_id, int dtmf) = on_dtmf_digits;
	void (* on_pagercb)(pjsua_call_id call_id, const pj_str_t *from, const pj_str_t *to, const pj_str_t *contact,
		const pj_str_t *mime_type, const pj_str_t *text) = on_pager;



	cfg.cb.on_incoming_call = on_incoming_callcb;
	cfg.cb.on_call_media_state = on_call_media_statecb;
	cfg.cb.on_call_state = on_call_statecb;
	cfg.cb.on_reg_state2 = on_reg_state2cb;
	cfg.cb.on_call_transfer_status = on_call_transfer_statuscb;
	cfg.cb.on_buddy_state = on_buddy_statecb;
	cfg.cb.on_call_redirected = on_call_redirectedcb;
	cfg.cb.on_dtmf_digit = on_dtmf_digitscb;
	cfg.cb.on_pager = on_pagercb;
	

	status = pjsua_init(&cfg, &log_cfg, &media_cfg);
	if (status != PJ_SUCCESS) error_exit("cannot initialize pjsua!", status);


	// creating transport
	int result = createTransport();

	status = pjsua_start();
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

	registerToServer();

	/*pjmedia_aud_dev_info * audioDevices = new pjmedia_aud_dev_info[100];
	unsigned int numOfAudioDevices = 100;
	status = pjsua_enum_aud_devs(audioDevices, &numOfAudioDevices);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);



	int captureDevice = 1, playbackDevice = 2;

	status = pjsua_get_snd_dev(&captureDevice, &playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);


	status = pjsua_set_snd_dev(captureDevice, playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);*/


//	

	//pj_str_t uri = pj_str(BUDDY_URI);
	//status = pjsua_call_make_call(acc_id, &uri, 0, NULL, NULL, NULL);
	//if (status != PJ_SUCCESS) error_exit("Error making call", status);

//	CallWindow * w = new CallWindow();
//	w->setCallerInfo(caller_info.acc_uri.ptr);
//	w->setStatusInfo("trying window");
//	w->setCallId(call_id);
//	w->show();


	//TODO: create one or more SIP accounts

	//TODO: add one or more buddies 

	//TODO: configure sound device,	codec settings, and other media settings
	load_devices();

	addBuddies();

//	emit addNewBuddy(BUDDY_URI, BUDDY_URI);
//	status = pjsua_buddy_update_pres(ids[0]);
	char buf[256];

//	status = pjsua_playlist_create(&fileName, 1, &fileName, 0, NULL);

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

	
	config.publicAddress = fromQString(settings.value("public_address", PUBLIC_ADDRESS).toString());
	//	copyString(settings.value("public_address", PUBLIC_ADDRESS).toString().toAscii().data());

	transportConfig.public_addr = pj_str(config.publicAddress);

	config.portNumber = settings.value("port", CONFIG_PORT).toInt();
	transportConfig.port = config.portNumber;

	pj_status_t status = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportConfig, NULL);


	if (status == 130048) error_exit("Cannot create transport: may be port, using by this program, is busy!",status);
	if (status != PJ_SUCCESS) error_exit("Error creating transport", status);

//	free(address); // TODO: needs or not?
	return 1;
}

char * copyString(char * string)
{
	int size = strlen(string) + 1;
	char * temp = (char*)malloc(sizeof(char) * size);
	if(temp == NULL) return NULL;
	memcpy(temp,string, size);
	return temp;
}

int sipCore::registerToServer()
{
	pjsua_acc_config cfg;
	QSettings settings("registerInformation.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	pjsua_acc_config_default(&cfg);

	config.id = copyString(settings.value("id", "sip:" SIP_USER "@" SIP_DOMAIN).toString().toAscii().data());
	config.uri = copyString(settings.value("uri", "sip:" SIP_DOMAIN).toString().toAscii().data());
	config.realm = copyString(settings.value("realm", REALM).toString().toAscii().data());
	config.sipUser = copyString(settings.value("username", SIP_USER).toString().toAscii().data());
	config.sipPassword = copyString(settings.value("password", SIP_PASSWD).toString().toAscii().data());

	cfg.id = pj_str(config.id);
	cfg.reg_uri = pj_str(config.uri);
	cfg.cred_count = 1;

	cfg.cred_info[0].realm = pj_str(config.realm);
	cfg.cred_info[0].scheme = pj_str("digest");   ////////////////////////////////////////////////////////////////////////// ??????????????????
	//cfg.cred_info[0].username.ptr = settings.value("username", SIP_USER).toString().toAscii().data();
	//cfg.cred_info[0].username.slen = strlen(cfg.cred_info[0].username.ptr);
	cfg.cred_info[0].username = pj_str(config.sipUser);
	cfg.cred_info[0].data_type = PJSIP_CRED_DATA_PLAIN_PASSWD;
	cfg.cred_info[0].data = pj_str(config.sipPassword);

	pj_status_t status = pjsua_acc_add(&cfg, PJ_TRUE, &acc_id);
	if (status != PJ_SUCCESS) error_exit("Error adding account", status);
	return 1; // TODO: нужно ли очищать память? или она ещё пригодится?
}

void sipCore::load_devices()
{
	int captureDevice, playbackDevice;

	pjmedia_aud_dev_info * audioDevices = new pjmedia_aud_dev_info[100];
	unsigned int numOfAudioDevices = 100;
	pj_status_t status = pjsua_enum_aud_devs(audioDevices, &numOfAudioDevices);
	if (status != PJ_SUCCESS) error_exit("Error getting sound devices", status);

	status = pjsua_get_snd_dev(&captureDevice, &playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);

	captureDevice = abs(captureDevice);
	playbackDevice = abs(playbackDevice);

	QSettings settings("devices.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	char * inputDevice = copyString(settings.value("inputDevice", NULL).toString().toAscii().data());
	if(inputDevice == NULL)
	{
		settings.setValue("inputDevice", audioDevices[captureDevice].name);
		inputDevice = copyString(audioDevices[captureDevice].name);
	}
	else
	{
		int i;
		for(i = 0; i < numOfAudioDevices; i++)
			if(strcmp(inputDevice, audioDevices[i].name) == 0)
			{
				captureDevice = i;
				break;
			}
			if( i == numOfAudioDevices)
			{
				QMessageBox::information(NULL, "Warning", "Capture device not found. Using standart device");
				inputDevice = copyString(audioDevices[captureDevice].name);
				settings.setValue("inputDevice", audioDevices[captureDevice].name);
			}
	}

	char * outputDevice = copyString(settings.value("outputDevice", NULL).toString().toAscii().data());
	if(outputDevice == NULL)
	{
		settings.setValue("outputDevice", audioDevices[playbackDevice].name);
		outputDevice = copyString(audioDevices[playbackDevice].name);
	}
	else
	{
		int i;
		for(i = 0; i < numOfAudioDevices; i++)
			if(strcmp(outputDevice, audioDevices[i].name) == 0)
			{
				playbackDevice = i;
				break;
			}
			if( i == numOfAudioDevices)
			{
				QMessageBox::information(NULL, "Warning", "Playback device not found. Using standart device");
				outputDevice = copyString(audioDevices[playbackDevice].name);
				settings.setValue("outputDevice", audioDevices[playbackDevice].name);
			}

	}

	status = pjsua_set_snd_dev(captureDevice, playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);
	config.inputDevice = inputDevice;
	config.outputDevice = outputDevice;
}

void sipCore::saveDevices(char * inputDevice, char * outputDevice)
{
	QSettings settings("devices.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	if(inputDevice != NULL)
	{
		settings.setValue("inputDevice", inputDevice);
	}

	if(outputDevice != NULL)
	{
		settings.setValue("outputDevice", outputDevice);
	}
}

void sipCore::addBuddies()
{
	QSettings settings("buddies.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	numberOfBuddies = settings.value("numberOfBuddies", 0).toInt();
	pjsua_buddy_config buddyConfig;
	
	if(numberOfBuddies == 0)
	{
		numberOfBuddies = 1;
		ids = new pjsua_buddy_id[1];
		QByteArray ba = BUDDY_URI;
		ba.insert(0, "sip:");
		ba.append("@");
		ba.append(config.publicAddress);
		buddyConfig.uri = pj_str(ba.data());
		buddyConfig.user_data = NULL;
		
		emit addNewBuddy("numberoUno", BUDDY_URI);

		pj_status_t status = pjsua_buddy_add(&buddyConfig, &ids[0]);
		if (status != PJ_SUCCESS) error_exit("Error! cannot add one buddy :(", status);
		settings.setValue("numberOfBuddies", 1);
		settings.setValue("buddy0", BUDDY_URI);
		settings.setValue("buddy0.name", "numberoUno");
	}
	else
	{
		ids = new pjsua_buddy_id[numberOfBuddies];
		for(int i = 0; i < numberOfBuddies; i++)
		{
			QByteArray newBa = (settings.value(QString("buddy%1").arg(i))).toByteArray();
			QByteArray ba = newBa;
			ba.insert(0, "sip:");
			ba.append("@");
			ba.append(config.publicAddress);
			buddyConfig.uri = pj_str(ba.data());
			buddyConfig.user_data = NULL;
					
			pj_status_t status = pjsua_buddy_add(&buddyConfig, &ids[i]);
			if (status != PJ_SUCCESS)
			{
			//	error_exit("Error! cannot add buddy", status);
				QMessageBox::information(NULL, "Error", QString("Cannot add buddy %1! Error %2").arg(ba.data()).arg((int)status));
			}
			ba = (settings.value(QString("buddy%1.name").arg(i))).toByteArray();
			emit addNewBuddy(ba.data(), BUDDY_URI);//////////////
		}
	}
	for (int i = 0; i < numberOfBuddies; i++)
	{
		pj_status_t status = pjsua_buddy_subscribe_pres	(ids[i], 1);		
		if (status != PJ_SUCCESS)
		{
			printf("cannot subcsribe!");
		}
	}
}

int sipCore::makeCall(int index)
{
	status.callStatus = 1;
	pjsua_call_id callId;
	pjsua_buddy_id in= ids[index];
	pjsua_buddy_info info;
	pjsua_buddy_get_info(in, &info);
	pj_status_t pjstatus = pjsua_call_make_call(acc_id, &info.uri, 0, NULL, NULL, &callId);
	if (pjstatus != PJ_SUCCESS)
	{
		status.callStatus = 0;
		error_exit("cannot make call", pjstatus);
	}

	CallWindow * window = new CallWindow(NULL);
	char * callerInfo = (char *) malloc (sizeof(char) * (info.uri.slen + 1));
	memcpy(callerInfo, info.uri.ptr, info.uri.slen);
	callerInfo[info.uri.slen] = 0;
	window->setCallerInfo(callerInfo);
	window->setCallId(callId);
	window->setStatusInfo("calling...");
	window->hideAnswerButton();
	window->show();

	free(callerInfo);

	return 1;
}

void sipCore::playMedia(char * fileName)
{
	/*if(fileName == NULL) return;
	pj_str_t f = pj_str(fileName);
	pj_status_t status = pjsua_playlist_create(&f, 1, &f, PJMEDIA_FILE_NO_LOOP, NULL);
	if(status != PJ_SUCCESS)
	{
		QMessageBox::information(NULL, "Warning!", "Cannot create media player");
		return;
	}*/
/*		char buf[256];
	char * ff = "sounds/DtmfStar.wav";
	memcpy(buf, ff, strlen(ff) + 1);
	pj_str_t ffileName = pj_str(buf);
	playerId = (pjsua_player_id *)malloc(sizeof(pjsua_player_id));
	pj_status_t status = pjsua_player_create(&ffileName, 0, playerId);


	status = pjsua_player_set_pos(*playerId, 1);*/
	pj_caching_pool cp;
    pjmedia_endpt *med_endpt;
    pj_pool_t *pool;
    pjmedia_port *file_port;
    pjmedia_snd_port *snd_port;
    char tmp[10];
    pj_status_t status;

  /* Must create a pool factory before we can allocate any memory. */
    pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);

    /* 
     * Initialize media endpoint.
     * This will implicitly initialize PJMEDIA too.
     */
    status = pjmedia_endpt_create(&cp.factory, NULL, 1, &med_endpt);

    /* Create memory pool for our file player */
    pool = pj_pool_create( &cp.factory,	    /* pool factory	    */
			   "wav",	    /* pool name.	    */
			   4000,	    /* init size	    */
			   4000,	    /* increment size	    */
			   NULL		    /* callback on error    */
			   );

    /* Create file media port from the WAV file */
    status = pjmedia_wav_player_port_create(  pool,	/* memory pool	    */
					      fileName,	/* file to play	    */
					      20,	/* ptime.	    */
					      0,	/* flags	    */
					      0,	/* default buffer   */
					      &file_port/* returned port    */
					      );
	if(status != 0)
	{
		if(status == 70006) 
		{
			QMessageBox::information(NULL, "error!", "Cannot open sound file: not found");
			pj_pool_release( pool );
			return;
		}
	}

    /* Create sound player port. */
    status = pjmedia_snd_port_create_player( 
		 pool,				    /* po ol		    */
		 -1,				    /* use default dev.	    */
		 file_port->info.clock_rate,	    /* clock rate.	    */
		 file_port->info.channel_count,	    /* # of channels.	    */
		 file_port->info.samples_per_frame, /* samples per frame.   */
		 file_port->info.bits_per_sample,   /* bits per sample.	    */
		 0,				    /* options		    */
		 &snd_port			    /* returned port	    */
		 );
  
    /* Connect file port to the sound player.
     * Stream playing will commence immediately.
     */
    status = pjmedia_snd_port_connect( snd_port, file_port);
	

	  pj_thread_sleep(1000);

	status = pjmedia_snd_port_disconnect(snd_port);
 
    /* Without this sleep, Windows/DirectSound will repeteadly
     * play the last frame during destroy.
     */
    pj_thread_sleep(100);

    /* Destroy sound device */
   //status = pjmedia_snd_port_destroy( snd_port );


    /* Destroy file port */
    //status = pjmedia_port_destroy( file_port );
 

    /* Release application pool */
    //pj_pool_release( pool );
}

void sipCore::on_buddy_state(pjsua_buddy_id buddy_id)
{
	if(numberOfBuddies == 0) return;
	int number = 0;
	for(int number = 0; number < numberOfBuddies; number++)
	{
		if(ids[number] == buddy_id) break;
	}
	if(number == numberOfBuddies)
	{
		printf("error! cannot find buddy!!!");
		exit(-5);
	}

	pjsua_buddy_info info;
	pjsua_buddy_get_info(buddy_id, &info);

	emit son_buddy_status_change(number, (info.status == PJSUA_BUDDY_STATUS_UNKNOWN)  ? 2 : (info.status == PJSUA_BUDDY_STATUS_ONLINE ? 1 : 0) );
}

void sipCore::addContact(char * name, char * URI)
{
	QSettings settings("buddies.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	numberOfBuddies = settings.value("numberOfBuddies", 0).toInt();
	pjsua_buddy_config buddyConfig;

	pjsua_buddy_id * newIds = new pjsua_buddy_id[numberOfBuddies + 1];
	for(int i = 0; i < numberOfBuddies; i++)
	{
		newIds[i] = ids[i];
	}

	QByteArray ba = URI;
	ba.insert(0, "sip:");
	ba.append("@");
	ba.append(config.publicAddress);
	buddyConfig.uri = pj_str(ba.data());
	buddyConfig.user_data = NULL;

	pj_status_t status = pjsua_buddy_add(&buddyConfig, &newIds[numberOfBuddies]);
	if (status != PJ_SUCCESS) error_exit("Error! cannot add buddy", status);
	delete[] ids;
	ids = newIds;
	numberOfBuddies++;
	emit addNewBuddy(name, URI); 

	settings.setValue("numberOfBuddies", numberOfBuddies);
	settings.setValue(QString("buddy%1").arg(numberOfBuddies - 1), URI);
	settings.setValue(QString("buddy%1.name").arg(numberOfBuddies - 1), name);
}

void sipCore::deleteContact(int row)
{
	if(row > numberOfBuddies)
	{
		printf("Eror! row > nubmerOfBuddies");
		return;
	}

	QSettings settings("buddies.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	pjsua_buddy_config buddyConfig;
	pjsua_buddy_id * newIds = new pjsua_buddy_id[numberOfBuddies - 1];
	int i;
	for(i = 0 ; i < row; i++)
		newIds[i] = ids[i];
	for(; i < numberOfBuddies - 1; i++)
	{
		settings.setValue(QString("buddy%1").arg(i), settings.value(QString("buddy%1").arg(i+1)));
		settings.setValue(QString("buddy%1.name").arg(i), settings.value(QString("buddy%1.name").arg(i+1)));
		newIds[i] = ids[i+1];
	}
	pj_status_t status = pjsua_buddy_del(ids[row]);
	if(status != PJ_SUCCESS) printf("error! cannot delete buddy");
	delete[] ids;

	ids = newIds;
	numberOfBuddies--;

	settings.setValue("numberOfBuddies", numberOfBuddies);


}

char * sipCore::getBuddyURI(int row)
{
	pjsua_buddy_info info;
	pjsua_buddy_get_info(ids[row], &info);
	char * tmp = info.uri.ptr;
	while(*tmp != ':')
	 tmp++;
	tmp++;
	int i = 0;
	while(*(tmp + i) != '@') i++;
	
	char * returnString = (char*)malloc(i + 1);

	memcpy(returnString, tmp, i);

	returnString[i] = 0;
	return returnString;
}

void sipCore::editContact(int row, char* name, char * URI)
{
	QSettings settings("buddies.ini", QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));


	settings.setValue(QString("buddy%1").arg(row), URI);
	settings.setValue(QString("buddy%1.name").arg(row), name);
}

void sipCore::saveConfig()
{
	saveDevices(config.inputDevice, config.outputDevice);

	QSettings transportSettings("transport.ini", QSettings::IniFormat);
	transportSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	transportSettings.setValue("public_address", config.publicAddress);
	transportSettings.setValue("port", config.portNumber);


	QSettings registerSettings("registerInformation.ini", QSettings::IniFormat);
	registerSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));

	registerSettings.setValue("id", config.id);
	registerSettings.setValue("uri", config.uri);
	registerSettings.setValue("realm", config.realm);
	registerSettings.setValue("username", config.sipUser);
	registerSettings.setValue("password", config.sipPassword);
}

void sipCore::sendMessage(char * message)
{
	if(callId == -1)
	{
		QMessageBox::information(NULL, "Error!", "Cannot send im! No active calls!");
		return;
	}

	pj_str_t mes = pj_str(message);
	pj_status_t status = pjsua_call_send_im(callId, NULL, &mes, NULL, NULL);
	if(status != PJ_SUCCESS) 
	{
		QMessageBox::information(NULL, "Error!", QString("Cannot send im! Error %1").arg(status));
	}
}

void on_call_transfer_status(pjsua_call_id call_id, int st_code, const pj_str_t *st_text, pj_bool_t final, pj_bool_t *p_cont)
{
	printf("!!!");
	QMessageBox::information(NULL, "!!", "on_call_transfer_status called!"); //TODO: delete this!

	if (st_code/100 == 2) //?????????????????? Ok status?
	{
			pjsua_call_hangup(call_id, PJSIP_SC_GONE, NULL, NULL);
	}

}

void on_buddy_state(pjsua_buddy_id buddy_id)
{
	sipCore::object->on_buddy_state(buddy_id);
}

void on_call_media_state(pjsua_call_id call_id)
{
	printf(" on_call_media_state called ");

	pjsua_call_info ci;
	pjsua_call_get_info(call_id, &ci);
	if (ci.media_status == PJSUA_CALL_MEDIA_ACTIVE)
	{
		pjsua_conf_connect(ci.conf_slot, 0);
		pjsua_conf_connect(0, ci.conf_slot);
	}
	sipCore::object->callId = call_id;

}

void on_call_state(pjsua_call_id call_id, pjsip_event * e)
{
	if(e->type == PJSIP_EVENT_TSX_STATE)
	{
		if(e->body.tsx_state.tsx->state == PJSIP_TSX_STATE_CALLING)
			sipCore::object->status.callStatus = 1;
		else if(e->body.tsx_state.tsx->state == PJSIP_TSX_STATE_TERMINATED)
		{
			if(e->body.tsx_state.tsx->status_code == 0x0C8)
				{
					//200 - OK
					sipCore::object->status.callStatus = 2;
					sipCore::object->on_call_answered();
					sipCore::object->callId = call_id;
				}
			if(e->body.tsx_state.tsx->status_code == 408 && sipCore::object->status.callStatus == 1)	//timeout
			{
				sipCore::object->status.callStatus = -1;  //cannot make a call 
				sipCore::object->status.lastError = e->body.tsx_state.tsx->status_text;	//error text
				sipCore::object->on_call_ended();
				sipCore::object->callId = -1;
			}

		}
		if(e->body.tsx_state.tsx->state == PJSIP_TSX_STATE_COMPLETED && (sipCore::object->status.callStatus == 2))
		{
//			if(call_id != sipCore::object->callId ) return; //not that call
			sipCore::object->status.callStatus = 0;	//end of call
			sipCore::object->on_call_ended();
			sipCore::object->callId = -1;
		}
		if(e->body.tsx_state.tsx->state == PJSIP_TSX_STATE_COMPLETED && (sipCore::object->status.callStatus == 1))
		{
			sipCore::object->status.callStatus = -1;  //cannot make a call 
			sipCore::object->status.lastError = e->body.tsx_state.tsx->status_text;	//error text
			sipCore::object->on_call_ended();
			sipCore::object->callId = -1;
		}
	}
}

void on_incoming_call(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsip_rx_data * rdata)
{
/*
// 	pjsua_acc_info caller_info;
// 
// 	pj_status_t status = pjsua_acc_get_info(acc_id, &caller_info);
*/
	/*if(status == PJ_SUCCESS)
	{
		CallWindow * w = new CallWindow();
		w->setCallerInfo(caller_info.acc_uri.ptr);
		w->setStatusInfo("incoming call");
		w->setCallId(call_id);
		w->show();
	}
	//((qt * ) mainWindow)->bbb(acc_id,call_id,rdata);


/*	pjmedia_aud_dev_info * audioDevices = new pjmedia_aud_dev_info[100];
	unsigned int numOfAudioDevices = 100;
	pj_status_t status = pjsua_enum_aud_devs(audioDevices, &numOfAudioDevices);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);



	int captureDevice = 1, playbackDevice = 2;

	status = pjsua_get_snd_dev(&captureDevice, &playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);


	status = pjsua_set_snd_dev(captureDevice, playbackDevice);
	if (status != PJ_SUCCESS) error_exit("Error starting pjsua", status);*/



	/*pj_status_t*/ 
/*
// 	status = pjsua_call_answer(call_id, 200 / * what code??* /, NULL, NULL);
// 	if(status != PJ_SUCCESS) error_exit("cannot answer call! ", status);
*/
	if((sipCore::object->status.callStatus == 2) || (sipCore::object->status.callStatus == 1))
	{
		//busy
		pjsua_call_answer(call_id, PJSIP_SC_BUSY_HERE, NULL, NULL);
	}
	sipCore::object->incom(acc_id, call_id, rdata);
}

void on_reg_state2(pjsua_acc_id acc_id, pjsua_reg_info *info)
{
	sipCore::object->on_reg_state2_emit(acc_id, info);
}

pjsip_redirect_op  on_call_redirected (pjsua_call_id call_id, const pjsip_uri *target, const pjsip_event *e)
{
	char uristr[PJSIP_MAX_URL_SIZE];
	int len;

	len = pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, target, uristr, 
		sizeof(uristr));

	if (len < 1) 
	{
		pj_ansi_strcpy(uristr, "--URI too long--");
	}
	int result = QMessageBox::question(NULL, "Caller wants to redirect", QString("Redirect to %1 ?").arg(uristr));
	if(result == QMessageBox::Ok) return PJSIP_REDIRECT_ACCEPT;
	else return PJSIP_REDIRECT_REJECT;
}

void on_pager(pjsua_call_id call_id, const pj_str_t *from, const pj_str_t *to, const pj_str_t *contact,
	const pj_str_t *mime_type, const pj_str_t *text)
{
/*	QString fr = from->ptr;
	fr.resize(from->slen);
	QString txt = text->ptr;
	txt.resize(text->slen);
	QMessageBox::information(NULL, "Incoming message", QString("Incoming message from %1 : %2").arg(fr).arg(txt));*/
	
	char * message = (char *) malloc (sizeof(char) * (text->slen + 1));
	memcpy(message, text->ptr, text->slen);
	message[text->slen] = 0;

	sipCore::object->on_message_recieved_emit(message);

	free(message); /////////!!!!!!!!!!!!! can be problems, need sync.

}

void on_dtmf_digits(pjsua_call_id call_id, int dtmf)
{
	/*if(call_id == sipCore::object->callId)
	{
		//not implemented - only 1 call at time
	}*/
	sipCore::object->on_dtmf_digits_emit(dtmf);
}

void on_typing(pjsua_call_id call_id, const pj_str_t *from,
	const pj_str_t *to, const pj_str_t *contact,
	pj_bool_t is_typing)
{
	//is_typing ? "is typing..":"has stopped typing";
}