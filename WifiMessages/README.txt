WI-FI MESSAGES FOR ANDROID

The aim of this project is to allow Android devices to communicate directly between them without access to a carrier.

The application may be used either as a server or as a client.
In versions 1 and 2, the server and the client are separate applications.
In the first release, each device is configured either as a server or as a client. 
In future releases, a device might be alternatively server and client. Servers can communicate only with clients and reciproquely.

The project has been built with the AIDE Android application and has been tested with a Samsung GT-S5570 phone with FroYo configured as a server and a Polaroid MID0714 tablet with Jelly Bean configured as a client.

ON A SERVER DEVICE :

A server must be configured as Wi-Fi mobile hotspot : 
On FroYo : Settings / Wireless and networks / Tethering and portable hotspots / Mobile AP settings / Configure mobile AP, choose a network SSID, security Open.
On Jelly Bean : Settings / WIRELESS & NETWORKS / More... / Portable hotspot / Set up Wi-Fi hotspot, choose a network SSID, Security open, then check Portable Wi-Fi hotspot.

Versions 1 and 2 :Install the WifiServer application (WifiServer.apk) and start it.
Versions 3 and later : Install the WifiMessages application (WifiMessages.apk)
Configure the parameters. Select the parameter to view or modify. If you modify it, click Set to save the modification.
The port number must be the same on the server and on the client.
Choose a user id.
If you want, you can indicate a list of group names to which you want to subscribe, separated by ",", and modify the default number of messages kept and sent.
Then click Start (WifiServer) or Server (WifiMessages).


ON A CLIENT DEVICE :

A client must be configured to be able to connect to Wi-Fi hotspots : 
On FroYo : Settings / Wireless and networks / Wi-Fi settings, check Wi-Fi
On Jelly Bean : Settings / WIRELESS & NETWORKS / turn Wi-Fi on.

Versions 1 and 2 : Install the WifiClient application (WifiClient.apk) and start it.
Version 3 and later : Install the WifiMessages application (WifiMessages.apk) and start it.
Configure the parameters. Select the parameter to view or modify. If you modify it, click Set to save the modification.
Indicate the SSID prefix of your network. The SSID configured on the servers must begin with this prefix.
Then click Start (WifiClient) or Client (WifiMessages).


ON BOTH SERVERS AND CLIENTS :

To send a message :
In the "To:" field, indicate a list of recipients separated by ",". A recipient may be a user id or a group name.
In the field below type the text of your message.
Then click Send.


POSSIBLE FURTHER IMPROVEMENTS

- Save and load configuration : done in version 2
- Save and load messages : done in version 2
- Send files
- Web version of the server
- API for sending and receiving messages


CHANGE LOG

Version 2
Save and load configuration and messages
Correction of bugs : 
- messages order
- special characters in messages

Version 3
To avoid code duplication, the WifiServer and WifiClient application have been merged into one application WifiMessages.





