#include "DisplaySettings.h"

#define MYLOG(...) fprintf(logger, __VA_ARGS__); fflush(logger);        

#ifdef DS_FOUND
#include "dsMgr.h"
#include "libIBusDaemon.h"
#include "host.hpp"
#include "exception.hpp"
#include "videoOutputPort.hpp"
#include "videoOutputPortType.hpp"
#include "videoOutputPortConfig.hpp"
#include "videoResolution.hpp"
#include "audioOutputPort.hpp"
#include "audioOutputPortType.hpp"
#include "audioOutputPortConfig.hpp"
#include "manager.hpp"
#include "dsUtl.h"
#include "dsError.h"
#include "list.hpp"
#include "libIBus.h"
#include "dsDisplay.h"

#define IARM_CHECK(FUNC) \
  if ((res = FUNC) != IARM_RESULT_SUCCESS) { \
    MYLOG("DisplaySettings %s: %s\n", #FUNC, \
        res == IARM_RESULT_INVALID_PARAM ? "invalid param" : ( \
        res == IARM_RESULT_INVALID_STATE ? "invalid state" : ( \
        res == IARM_RESULT_IPCCORE_FAIL ? "ipcore fail" : ( \
        res == IARM_RESULT_OOM ? "oom" : "unknown")))); \
  } else { \
    MYLOG("DisplaySettings %s: success\n", #FUNC); \
  }

#endif

namespace WPEFramework {

    namespace Plugin {

        static FILE* logger = nullptr;
        
        SERVICE_REGISTRATION(DisplaySettings, 1, 0);

        DisplaySettings* DisplaySettings::_instance = nullptr;

        DisplaySettings::DisplaySettings()
            : PluginHost::JSONRPC()
        {
            logger = fopen("/opt/logs/ds.log", "w");
            printf("DisplaySettings logger=%p\n", logger);
            fflush(stdout);
            
            MYLOG("DisplaySettings CTOR!\n");
            DisplaySettings::_instance = this;
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
            MYLOG("DisplaySettings DTOR!\n");
            DisplaySettings::_instance = nullptr;
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
            MYLOG("%s\n", __PRETTY_FUNCTION__);        
            IARM_Result_t res;
            IARM_CHECK( IARM_Bus_Init("Display_Settings") );
            IARM_CHECK( IARM_Bus_Connect() );
            IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_RX_SENSE, DisplResolutionHandler) );
            IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS, DisplResolutionHandler) );
            //TODO/FIXME localinput.cpp has PreChange guared with #if !defined(DISABLE_PRE_RES_CHANGE_EVENTS)
            //Can we set it all the time from inside here and let localinput put guards around listening for our event?
            IARM_CHECK( IARM_Bus_RegisterCall(IARM_BUS_COMMON_API_ResolutionPreChange, ResolutionPreChange) );
            IARM_CHECK( IARM_Bus_RegisterCall(IARM_BUS_COMMON_API_ResolutionPostChange, ResolutionPostChange) );
            //TODO/FIXME IS THE FOLLOWING REQUIRED -- localinput.cpp was doing it
            try
            {
                device::Manager::Initialize();
                MYLOG("device::Manager::Initialize success\n");
            }
            catch(...)
            {
                MYLOG("device::Manager::Initialize fail\n");            
            }
        }
        //TODO/FIXME - we need to install crash handler to ensure DeinitializeIARM gets called
        void DisplaySettings::DeinitializeIARM()
        {
            MYLOG("%s\n", __PRETTY_FUNCTION__);        
            IARM_Result_t res;        
            IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_RX_SENSE) );
            IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME, IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS) );
            IARM_CHECK( IARM_Bus_Disconnect() );
            IARM_CHECK( IARM_Bus_Term() );
            //TODO/FIXME IS THE FOLLOWING REQUIRED -- localinput.cpp was doing it
            try
            {
                device::Manager::DeInitialize();
                MYLOG("device::Manager::DeInitialize success\n");
            }
            catch(...)
            {
                MYLOG("device::Manager::DeInitialize fail\n");            
            }
            
        }
        IARM_Result_t DisplaySettings::ResolutionPreChange(void *arg)
        {
            MYLOG("%s\n", __PRETTY_FUNCTION__);
            return IARM_RESULT_SUCCESS;
        }
        IARM_Result_t DisplaySettings::ResolutionPostChange(void *arg)
        {
            MYLOG("%s\n", __PRETTY_FUNCTION__);        
            int dw = 1280;
            int dh = 720;
            IARM_Bus_CommonAPI_ResChange_Param_t *eventData = (IARM_Bus_CommonAPI_ResChange_Param_t *)arg;
            dw = eventData->width ;
            dh = eventData->height ;
            if(DisplaySettings::_instance)
                DisplaySettings::_instance->resolutionChanged(dw,dh);        
            return IARM_RESULT_SUCCESS;
        }
        void DisplaySettings::DisplResolutionHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
        {
               MYLOG("%s\n", __PRETTY_FUNCTION__);
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
                            if(DisplaySettings::_instance)
                                DisplaySettings::_instance->resolutionChanged(dw,dh);
                        }
                        break;
                    case IARM_BUS_DSMGR_EVENT_ZOOM_SETTINGS:
                        {
                            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
                            if(eventData->data.dfc.zoomsettings == dsVIDEO_ZOOM_NONE)
                            {
                                MYLOG("%s: dsVIDEO_ZOOM_NONE Settings\n",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifySetZoom(1);
                            }
                            else if(eventData->data.dfc.zoomsettings == dsVIDEO_ZOOM_FULL)
                            {
                                MYLOG("%s: dsVIDEO_ZOOM_FULL Settinmgs\n",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifySetZoom(0);
                            }
                        }
                        break;
                        case IARM_BUS_DSMGR_EVENT_RX_SENSE:
                        {
                            
                            IARM_Bus_DSMgr_EventData_t *eventData = (IARM_Bus_DSMgr_EventData_t *)data;
                            if(eventData->data.hdmi_rxsense.status == dsDISPLAY_RXSENSE_ON)
                            {
                                MYLOG("%s: Got dsDISPLAY_RXSENSE_ON -> notifyactiveInputChanged(true)\n",__FUNCTION__);
                                //ServiceManagerNotifier::getInstance()->notifyactiveInputChanged(true); 
                            }
                            else if(eventData->data.hdmi_rxsense.status == dsDISPLAY_RXSENSE_OFF)
                            {
                                MYLOG("%s: Got dsDISPLAY_RXSENSE_OFF -> notifyactiveInputChanged(false)\n",__FUNCTION__);
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
