#include "DisplaySettings.h"

namespace WPEFramework {

	namespace Plugin {

        SERVICE_REGISTRATION(DisplaySettings, 1, 0);

		DisplaySettings::DisplaySettings()
			: PluginHost::JSONRPC()
		{
            printf("DisplaySettings CTOR!\n");
			Register<Params, JStringArray >(_T("getConnectedVideoDisplays"), &DisplaySettings::getConnectedVideoDisplays, this);
			Register<Params, JString>(_T("getConnectedAudioPorts"), &DisplaySettings::getConnectedAudioPorts, this);
			Register<Params, JString>(_T("getSupportedResolutions"), &DisplaySettings::getSupportedResolutions, this);
			Register<Params, JString>(_T("getSupportedTvResolutions"), &DisplaySettings::getSupportedTvResolutions, this);
			Register<Params, JString>(_T("getSupportedAudioPorts"), &DisplaySettings::getSupportedAudioPorts, this);
			Register<Params, JString>(_T("getSupportedAudioModes"), &DisplaySettings::getSupportedAudioModes, this);
			Register<Params, JString>(_T("getZoomSetting"), &DisplaySettings::getZoomSetting, this);
			Register<Params, JString>(_T("setZoomSetting"), &DisplaySettings::setZoomSetting, this);
			Register<Params, JString>(_T("getCurrentResolution"), &DisplaySettings::getCurrentResolution, this);
			Register<Params, JString>(_T("setCurrentResolution"), &DisplaySettings::setCurrentResolution, this);
			Register<Params, JString>(_T("getSoundMode"), &DisplaySettings::getSoundMode, this);
			Register<Params, JString>(_T("setSoundMode"), &DisplaySettings::setSoundMode, this);
			Register<Params, JString>(_T("readEDID"), &DisplaySettings::readEDID, this);
			Register<Params, JString>(_T("readHostEDID"), &DisplaySettings::readHostEDID, this);
			Register<Params, JString>(_T("getActiveInput"), &DisplaySettings::getActiveInput, this);
		}
		DisplaySettings::~DisplaySettings()
		{
            printf("DisplaySettings DTOR!\n");
			Unregister(_T("getConnectedVideoDisplays"));
			Unregister(_T("getConnectedAudioPorts"));
			Unregister(_T("getSupportedResolutions"));
			Unregister(_T("getSupportedTvResolutions"));
			Unregister(_T("getSupportedAudioPorts"));
			Unregister(_T("getSupportedAudioModes"));
			Unregister(_T("getZoomSetting"));
			Unregister(_T("setZoomSetting"));
			Unregister(_T("getCurrentResolution"));
			Unregister(_T("setCurrentResolution"));
			Unregister(_T("getSoundMode"));
			Unregister(_T("setSoundMode"));
			Unregister(_T("readEDID"));
			Unregister(_T("readHostEDID"));
			Unregister(_T("getActiveInput"));
		}
		const string DisplaySettings::Initialize(PluginHost::IShell* /* service */)
		{
#ifdef DS_FOUND
            InitializeIARM();
#endif		
			// On success return empty, to indicate there is no error text.
			return (string());
		}
		void DisplaySettings::Deinitialize(PluginHost::IShell* /* service */)
		{
#ifdef DS_FOUND
            DeinitializeIARM();
#endif		
		}
		string DisplaySettings::Information() const
		{
			// No additional info to report.
			return (string());
		}
#ifdef DS_FOUND
        void DisplaySettings::InitializeIARM()
        {
            IARM_Bus_Init("Display_Settings");
            IARM_Bus_Connect();
            IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_RX_SENSE, DisplResolutionHandler);
            IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS,DisplResolutionHandler);
            //TODO/FIXME localinput.cpp has PreChange guared with #if !defined(DISABLE_PRE_RES_CHANGE_EVENTS)
            //Can we set it all the time from inside here and let localinput put guards around listening for our event?
            IARM_Bus_RegisterCall(IARM_BUS_COMMON_API_ResolutionPreChange, ResolutionPreChange);
            IARM_Bus_RegisterCall(IARM_BUS_COMMON_API_ResolutionPostChange, ResolutionPostChange);            
            //TODO/FIXME IS THE FOLLOWING REQUIRED -- localinput.cpp was doing it
            try
            {
                device::Manager::Initialize();
            }
            catch(...)
            {
            }
        }
        //TODO/FIXME - we need to install crash handler to ensure DeinitializeIARM gets called
        void DisplaySettings::DeinitializeIARM()
        {
            IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_RX_SENSE);
            IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS);
            IARM_Bus_Disconnect();
            IARM_Bus_Term();
            //TODO/FIXME IS THE FOLLOWING REQUIRED -- localinput.cpp was doing it
            try
            {
                device::Manager::DeInitialize();
            }
            catch(...)
            {
            }
            
        }
		static IARM_Result_t DisplaySettings::ResolutionPreChange(void *arg)
		{
    		return IARM_RESULT_SUCCESS;
		}
		static IARM_Result_t DisplaySettings::ResolutionPostChange(void *arg)
		{
            int dw = 1280;
            int dh = 720;
            IARM_Bus_CommonAPI_ResChange_Param_t *eventData = (IARM_Bus_CommonAPI_ResChange_Param_t *)arg;
            dw = eventData->width ;
            dh = eventData->height ;
		
    		return IARM_RESULT_SUCCESS;
        }
        static void DisplResolutionHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
        {
            //TODO/FIXME Receiver has this whole think guarded by #ifndef HEADLESS_GW
            if (strcmp(owner,IARM_BUS_DSMGR_NAME) == 0)
            {
                switch (eventId) {
                    case IARM_BUS_DSMGR_EVENT_RES_PRECHANGE:
                        break;
                    case IARM_BUS_DSMGR_EVENT_RES_POSTCHANGE:
                        {
                            int dw = 1280;
                            int dh = 720;
                            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
                            dw = eventData->data.resn.width ;
                            dh = eventData->data.resn.height ;
                            //postResolutionChange(dw,dh);
                        }
                        break;
                    case IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS:
                        {
                            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
                            if(eventData->data.dfc.zoomsettings == dsVIDEO_ZOOM_NONE)
                            {
                                printf("%s: dsVIDEO_ZOOM_NONE Settings  ",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifySetZoom(1);
                            }
                            else if(eventData->data.dfc.zoomsettings == dsVIDEO_ZOOM_FULL)
                            {
                                printf("%s: dsVIDEO_ZOOM_FULL Settinmgs ",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifySetZoom(0);
                            }
                        }
                        break;
                        case IARM_BUS_DSMGR_EVENT_RX_SENSE:
                        {
                            
                            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
                            if(eventData->data.hdmi_rxsense.status == dsDISPLAY_RXSENSE_ON)
                            {
                                printf("%s: Got dsDISPLAY_RXSENSE_ON -> notifyactiveInputChanged(true)!!!!  ",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifyactiveInputChanged(true); 
                            }
                            else if(eventData->data.hdmi_rxsense.status == dsDISPLAY_RXSENSE_OFF)
                            {
                                printf("%s: Got dsDISPLAY_RXSENSE_OFF -> notifyactiveInputChanged(false)!!!! ",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifyactiveInputChanged(false);
                            }
                        }
                        break;
                      default:
                       break;
                }
            }
        }
#endif
        
	} // namespace Plugin
	
} // namespace WPEFramework
