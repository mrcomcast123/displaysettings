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
			// On success return empty, to indicate there is no error text.
			return (string());
		}
		void DisplaySettings::Deinitialize(PluginHost::IShell* /* service */)
		{
		}
		string DisplaySettings::Information() const
		{
			// No additional info to report.
			return (string());
		}
		
	} // namespace Plugin
	
} // namespace WPEFramework
