-----------------
Build:

bitbake displaysettings

-----------------
Install:

cp
tmp/work/cortexa15t2hf-neon-rdk-linux-gnueabi/displaysettings/3.0+gitAUTOINC+70c598136c-r0/git/DisplaySettings.json
to 
/etc/WPEFramework/plugins/DisplaySettings.json

cp
tmp/work/cortexa15t2hf-neon-rdk-linux-gnueabi/displaysettings/3.0+gitAUTOINC+70c598136c-r0/package/usr/lib/wpeframework/plugins/libWPEFrameworkDisplaySettings.so
to
/usr/lib/wpeframework/plugins/libWPEFrameworkDisplaySettings.so

systemctl restart wpeframework

-----------------
Test:

curl --header "Content-Type: application/json"   --request POST   --data '{"jsonrpc":"2.0","id":"3","method": "DisplaySettings.1.time"}'   http://10.0.0.247:80/jsonrpc
