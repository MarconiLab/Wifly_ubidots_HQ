Wifly_ubidots_LP Working
================
Sending data from RTC temperature and Battery value to Ubidots with Wifly RN-XV (wifly shield) connected to Stalker board. Putting device in sleeping mode while not measuring every SLPNG seconds defined in code.

#Wires
  RNXV <----> Stalker
  
  2 (data out)    <----> PD6 
  
  3 (data in)   <----> PD7
  
  5 (reset)    <----> PD8
  
  12 (CTS)   <----> PA0
  
  16 (RTS)   <----> PA1

##Libraries
WiflyHQ modified
https://github.com/harlequin-tech/WiFlyHQ

##Hardware
Stalker 
http://www.seeedstudio.com/wiki/Seeeduino_Stalker_v2.3

Wifly Shield
https://www.sparkfun.com/products/10822

##RN-XV firmware update!
update RN-XV firmware last from https://serialio.com/support/WiSnap/QuickSettings1.php

Associating RN-XV Device with a Network

set wlan ssid mywifissid //Set WiFi SSID. 

set wlan pass mypassphrase //set network passphrase, (for WEP auth "use set wlan key <value>")  

set wlan auth # //Set the encryption mode, in this case this network is using Open=0, 1=WEP, ... 4 = WPA2-PSK etc 

set wlan channel # //Enter channel number if known, or enter 0 to scan all channels. 

set wlan join 1 //Set network to auto join 

set ip dhcp 1 //Enable DHCP 

save // Save configuration in the config file 

reboot // Reboots the module so that the stored parameters take effect


Understanding Firmware Versioning

The key detail to note is that Ad-Hoc mode is support through firmware 2.36. 
Access Point mode is not supported on firmware 2.36. 
Access Point mode is supported on Firmware 2.42 and above. 
Ad-Hoc mode is not supported on firmware 2.45.

Updating Firmware
Before you can update the device’s firmware, you will need to Associate the RN-XV with an internet routed WiFi network. 
Please see the previous section for more information on this process. 
The following commands will walk you through the process for updating device firmware.

set ftp address 0 //Remove IP address, will be using dns 

set dns name rn.microchip.com //Set dns to rn.microchip.com, this will fill in the IP address. 

set ftp user roving //username 

set ftp pass Pass123 //FTP password 

set ftp timeout 200 // Optional - set the time out to a higher value, better for slower devices 

ftp update yourfirmware.mg //This command will execute the update for the newest FW version. See the notes on firmware below. 
ls //list files on the RN-XV and confirm it was downloaded correctly, and the new FW flag is set to "boot" 

boot image ## //Set the boot image to the flag number. 

factory R //do a factory reset after the new 

reboot // reboot the RN-XV device, it should now be using the new firmware

Firmware Versions and the .img file
Firmware image names are different for the different modules, and depending which version of firmware you seek. 
Different firmware versions offer different levels of support.

RN171

wifly-EZX.img = Ad Hoc mode support 

wifly-EZX-AP.img = Soft AP mode support

RN131

wifly-GSX.img = Ad Hoc mode support 

wifly-GSX-AP.img = Soft AP mode support

Contact Serialio when updating to a specific firmware version.

Once updated
wifly-EZX Ver: 4.41 Build: r1057, Jan 17 2014 10:23:54 on RN-171
