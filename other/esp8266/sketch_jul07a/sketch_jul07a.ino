#include <ESP8266WiFi.h>
//#include <DNSServer.h>
//#include <ESP8266WebServer.h>
//#include <WiFiManager.h>  
#define MAX_SRV_CLIENTS 3   //最大同时联接数，即你想要接入的设备数量，8266tcpserver只能接入五个，

String comdata;


String SendTemp;
WiFiServer server(502);//你要的端口号，随意修改，范围0-65535
WiFiClient serverClients[MAX_SRV_CLIENTS];

void read_and_send(void);


void setup()
{   
    // 建立WiFiManager对象
    //WiFiManager wifiManager;
    //wifiManager.resetSettings();
    // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
    //wifiManager.autoConnect("ESP_EEESP", "12345678");

    //WiFi.begin("L_PC","4433221100");
    
    IPAddress softLocal(192,168,128,2);   // 1 设置内网WIFI IP地址
    IPAddress softGateway(192,168,128,2);
    IPAddress softSubnet(255,255,255,0);
    WiFi.softAPConfig(softLocal, softGateway, softSubnet);

    
    String apName = ("cameraCore");  // 2 设置WIFI名称
    const char *softAPName = apName.c_str();
  
    WiFi.softAP(softAPName, "11223344");      // 3创建wifi  名称 +密码 adminadmin
  
//    while(WiFi.status()!=WL_CONNECTED){
//    delay(500);}
    
    server.begin();
    server.setNoDelay(true);  //加上后才正常些

    Serial.begin(921600);     
//
//    IPAddress myIP = WiFi.softAPIP();
//    String ipadd1 = WiFi.localIP().toString().c_str();
//    SendTemp = "IP address:"+myIP;
//    Serial.println(SendTemp);           // IP
}


//1.配置WIFI SSID和密码
//2.配置完成WIFI连接之后，用串口发送自己的IP地址

void loop()
{
  read_and_send();
}



void read_and_send()
{
  uint8_t i;
  if (server.hasClient())
  {
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (!serverClients[i ] || !serverClients[i ].connected())
      {
        if (serverClients[i ]) serverClients[i ].stop();//未联接,就释放
        serverClients[i ] = server.available();//分配新的
        continue;
      }

    }
    WiFiClient serverClient = server.available();
    serverClient.stop();
  }
  for (i = 0; i < MAX_SRV_CLIENTS; i++)
  {
    if (serverClients[i ] && serverClients[i ].connected())
    {
      if (serverClients[i ].available())
      {
        while (serverClients[i].available())
          Serial.write(serverClients[i].read());
      }
    }
  }


  if (Serial.available())
  {
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    //push UART data to all connected telnet clients
    for (i = 0; i < MAX_SRV_CLIENTS; i++)
    {
      if (serverClients[i ] && serverClients[i ].connected())
      {
        serverClients[i].write(sbuf, len);  //向所有客户端发送数据
        delay(1);
      }
    }
  }
}
