#pragma once

#include "Module.h"

#ifdef DS_FOUND
#include "libIBus.h"
#include "irMgr.h"
#endif

namespace WPEFramework {

    namespace Plugin {

		// This is a server for a JSONRPC communication channel. 
		// For a plugin to be capable to handle JSONRPC, inherit from PluginHost::JSONRPC.
		// By inheriting from this class, the plugin realizes the interface PluginHost::IDispatcher.
		// This realization of this interface implements, by default, the following methods on this plugin
		// - exists
		// - register
		// - unregister
		// Any other methood to be handled by this plugin  can be added can be added by using the
		// templated methods Register on the PluginHost::JSONRPC class.
		// As the registration/unregistration of notifications is realized by the class PluginHost::JSONRPC,
		// this class exposes a public method called, Notify(), using this methods, all subscribed clients
		// will receive a JSONRPC message as a notification, in case this method is called.
        class DisplaySettings : public PluginHost::IPlugin, public PluginHost::JSONRPC {
        private:
            // We do not allow this plugin to be copied !!
            DisplaySettings(const DisplaySettings&) = delete;
            DisplaySettings& operator=(const DisplaySettings&) = delete;

            class Params : public Core::JSON::Container {
            public:
                Params() : Core::JSON::Container()
                {
                    Add("videoDisplay", &videoDisplay);
                    Add("resolution", &resolution);
                    Add("zoomLevel", &zoomLevel);
                    Add("soundMode", &soundMode);
                    Add("audioPort", &audioPort);
                    Add("width", &width);
                    Add("height", &height);
                    Add("hdmiHotPlugEvent", &hdmiHotPlugEvent);
                    Add("activeInput", &activeInput);                    
                }
                Params(const Params& copy)
                {
                    Add("videoDisplay", &videoDisplay);
                    Add("resolution", &resolution);
                    Add("zoomLevel", &zoomLevel);
                    Add("soundMode", &soundMode);
                    Add("audioPort", &audioPort);
                    Add("width", &width);
                    Add("height", &height);
                    Add("hdmiHotPlugEvent", &hdmiHotPlugEvent);
                    Add("activeInput", &activeInput);
                    videoDisplay = copy.videoDisplay;
                    resolution = copy.resolution;
                    zoomLevel = copy.zoomLevel;
                    soundMode = copy.soundMode;
                    audioPort = copy.audioPort;
                    width = copy.width;
                    height = copy.height;
                    hdmiHotPlugEvent = copy.hdmiHotPlugEvent;
                    activeInput = copy.activeInput;
                }

                Params& operator=(const Params& copy)
                {
                    videoDisplay = copy.videoDisplay;
                    resolution = copy.resolution;
                    zoomLevel = copy.zoomLevel;
                    soundMode = copy.soundMode;
                    audioPort = copy.audioPort;
                    width = copy.width;
                    height = copy.height;
                    hdmiHotPlugEvent = copy.hdmiHotPlugEvent;
                    activeInput = copy.activeInput;
                    return *this;
                }
            public:
                Core::JSON::String videoDisplay;
                Core::JSON::String resolution;
                Core::JSON::String zoomLevel;
                Core::JSON::String soundMode;
                Core::JSON::String audioPort;
                Core::JSON::DecSInt32 width;
                Core::JSON::DecSInt32 height;
                Core::JSON::DecSInt32 hdmiHotPlugEvent;
                Core::JSON::Boolean activeInput;
            };

            typedef Core::JSON::String JString;
            typedef Core::JSON::ArrayType<JString> JStringArray;
            typedef Core::JSON::Boolean JBool;
            //Begin methods
            uint32_t getQuirks(const Params& parameters, JStringArray& response);//FIXME/TODO - this was on the Service base class and implemented in DisplaySettings so i guess its needed
            uint32_t getConnectedVideoDisplays(const Params& parameters, JStringArray& response);
            uint32_t getConnectedAudioPorts(const Params& parameters, JStringArray& response);
            uint32_t getSupportedResolutions(const Params& parameters, JStringArray& response);
            uint32_t getSupportedTvResolutions(const Params& parameters, JStringArray& response);
            uint32_t getSupportedAudioPorts(const Params& parameters, JStringArray& response);
            uint32_t getSupportedAudioModes(const Params& parameters, JStringArray& response);
            uint32_t getZoomSetting(const Params& parameters, JString& response);
            uint32_t setZoomSetting(const Params& parameters, JString& response);
            uint32_t getCurrentResolution(const Params& parameters, JString& response);
            uint32_t setCurrentResolution(const Params& parameters, JString& response);
            uint32_t getSoundMode(const Params& parameters, JString& response);
            uint32_t setSoundMode(const Params& parameters, JString& response);
            uint32_t readEDID(const Params& parameters, JString& response);
            uint32_t readHostEDID(const Params& parameters, JString& response);
            uint32_t getActiveInput(const Params& parameters, JBool& response);
            //End methods
            //Begin events
            void connectedVideoDisplaysUpdated(int hdmiHotPlugEvent);
            void resolutionChanged(int width, int height);
            void zoomSettingUpdated(const string& zoomLevel);
            void activeInputChanged(bool activeInput);
            //End events
        public:
            DisplaySettings();
            virtual ~DisplaySettings();
            //Build QueryInterface implementation, specifying all possible interfaces to be returned.
            BEGIN_INTERFACE_MAP(DisplaySettings)
            INTERFACE_ENTRY(PluginHost::IPlugin)
            INTERFACE_ENTRY(PluginHost::IDispatcher)
            END_INTERFACE_MAP
            //IPlugin methods
            virtual const string Initialize(PluginHost::IShell* service) override;
            virtual void Deinitialize(PluginHost::IShell* service) override;
            virtual string Information() const override;
        private:
#ifdef DS_FOUND
            void InitializeIARM();
            void DeinitializeIARM();
		    static IARM_Result_t ResolutionPreChange(void *arg);
		    static IARM_Result_t ResolutionPostChange(void *arg);
            static void DisplResolutionHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);
            static void dsHdmiEventHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len);
#endif
            //TODO/FIXME -- these are carried over from ServiceManager DisplaySettings - we need to munge this around to support the Thunder plugin version number
            int getApiVersionNumber();
            void setApiVersionNumber(unsigned int apiVersionNumber);
        public:
            static DisplaySettings* _instance;
        private:
            unsigned int m_apiVersionNumber;
        };
	} // namespace Plugin
} // namespace WPEFramework
