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
                }
                Params(const Params& copy)
                {
                    Add("videoDisplay", &videoDisplay);
                    Add("resolution", &resolution);
                    Add("zoomLevel", &zoomLevel);
                    Add("soundMode", &soundMode);
                    Add("audioPort", &audioPort);
                    videoDisplay = copy.videoDisplay;
                    resolution = copy.resolution;
                    zoomLevel = copy.zoomLevel;
                    soundMode = copy.soundMode;
                    audioPort = copy.audioPort;
                }

                Params& operator=(const Params& copy)
                {
                    videoDisplay = copy.videoDisplay;
                    resolution = copy.resolution;
                    zoomLevel = copy.zoomLevel;
                    soundMode = copy.soundMode;
                    audioPort = copy.audioPort;
                    return *this;
                }
            public:
                Core::JSON::String videoDisplay;
                Core::JSON::String resolution;
                Core::JSON::String zoomLevel;
                Core::JSON::String soundMode;
                Core::JSON::String audioPort;
            };

            typedef Core::JSON::String JString;
            typedef Core::JSON::ArrayType<JString> JStringArray;
            //Begin methods
            //Params:empty Response:array of strings
            uint32_t getConnectedVideoDisplays(const Params& parameters, JStringArray& response) {
                Core::JSON::String str;
                str = "HDMI1";
                response.Add(str);
                str = "HDMI2";
                response.Add(str);
                str = "HDMI3";
                response.Add(str);
                return (Core::ERROR_NONE);
            }
            //Params:empty Response:array of strings
            uint32_t getConnectedAudioPorts(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay Response:array of strings
            uint32_t getSupportedResolutions(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay Response:array of strings
            uint32_t getSupportedTvResolutions(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:empty Response:array of strings
            uint32_t getSupportedAudioPorts(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:audioPort Response:array of strings
            uint32_t getSupportedAudioModes(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:empty Response:string
            uint32_t getZoomSetting(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:zoomLevel Response:empty
            uint32_t setZoomSetting(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay Response:string
            uint32_t getCurrentResolution(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay,resolution Response:empty
            uint32_t setCurrentResolution(const Params& parameters, JString& response) {
                printf("videoDisplay=%s\n", parameters.videoDisplay.Value().c_str());
                printf("resolution=%s\n", parameters.resolution.Value().c_str());
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay Response:string
            uint32_t getSoundMode(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay,soundMode Response:bool
            uint32_t setSoundMode(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:empty Response:string
            uint32_t readEDID(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:empty Response:string
            uint32_t readHostEDID(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //Params:videoDisplay Response:bool
            uint32_t getActiveInput(const Params& parameters, JString& response) {
                response = "\"NOTIMPL\"";
                return (Core::ERROR_NONE);
            }
            //End methods
            //Begin events
            void connectedVideoDisplaysUpdated() {
                Notify(_T("connectedVideoDisplaysUpdated"), Core::JSON::String("\"NOTIMPL\""));
            }
            void resolutionChanged() {
                Notify(_T("resolutionChanged"), Core::JSON::String("\"NOTIMPL\""));
            }
            void zoomSettingUpdated() {
                Notify(_T("zoomSettingUpdated"), Core::JSON::String("\"NOTIMPL\""));
            }
            void activeInputChanged() {
                Notify(_T("activeInputChanged"), Core::JSON::String("\"NOTIMPL\""));
            }
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
#endif
        };
	} // namespace Plugin
} // namespace WPEFramework
