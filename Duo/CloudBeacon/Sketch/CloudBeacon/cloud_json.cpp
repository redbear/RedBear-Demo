#include <ArduinoJson.h>
#include "cloud_json.h"


uint8_t json_enable = 0;

uint8_t json_UUID[16];

uint8_t json_namespaceID[10];

uint8_t json_instanceID[6];

uint8_t json_url[18]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

char* json_key = new char[16];

uint32_t json_interv;

uint8_t url_len=0;

void str2hex(const char* p,uint8_t *arr,uint8_t leng)
{
  for(uint8_t h;h<leng;h=h+2)
  {
    

    if(p[h]>57&&p[h]<91) {
      arr[h/2] = (p[h]-55)<<4;
    }
    else if(p[h]>96){
      arr[h/2] = (p[h]-87)<<4;
    }
    else {
      arr[h/2] =(p[h]-'0')<<4;
    }
    
    
    if(p[h+1]>57&&p[h+1]<91){
      arr[h/2] = arr[h/2]+p[h+1]-55;
    }
    else if(p[h+1]>96){
      arr[h/2] = arr[h/2]+(p[h+1]-87);
    }
    else arr[h/2] =arr[h/2]+(p[h+1]-'0');  

    if(p[h]=='2'&&p[h+1]=='e') {
      arr[h/2] =0x2e;
    }
    
  }
  /*Serial.print("array  ");
  for(uint8_t i = 0;i<leng/2;i++)
  {
    Serial.print(arr[i],HEX );
    Serial.print(" ");
  }
  Serial.println(" ");*/
}

/**
   Parse the JSON String. Uses aJson library

   Refer to http://hardwarefun.com/tutorials/parsing-json-in-arduino
*/
char* parseJson(char *jsonString)
{
  json_enable = 0;

  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(jsonString);
  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return 0;
  }


  uint8_t enable = root["en"];
  uint8_t instanceID[6];
  const char* UUID = root["UUID"];
  const char* namespaceID = root["nID"];
  const char* url = root["url"];
  const char* key = root["key"];
  uint32_t interv = root["int"];
  
  Serial.print("enable  ");
  Serial.println(enable  );


  Serial.print("interv  ");
  Serial.println(interv  );

  Serial.print("UUID  ");
  Serial.println(UUID  );
  
  
  Serial.print("namespaceID  ");
  Serial.println(namespaceID  );

  Serial.print("instanceID  ");
  for (uint8_t i = 0; i < 6; i++)
  {
    instanceID[i] = root["iID"][i];
    Serial.print(instanceID[i]);
    Serial.print(" ");
  }
  Serial.println(" ");

  Serial.print("url  ");
  Serial.println(url  );

  Serial.print("key  ");
  Serial.println(key  );

  json_enable = enable; 
  Serial.print("json_enable  ");
  Serial.println(json_enable  );
  
  str2hex(UUID,json_UUID,32);
  
  Serial.print("json_UUID  ");
  for(uint8_t i=0;i<16;i++)
  {
      Serial.print(json_UUID[i],HEX);
      Serial.print(" ");
  }
  Serial.println(" ");

  url_len = strlen(url)/2;
  Serial.print("url_len  ");
  Serial.println(url_len );
  
  str2hex(url,json_url,strlen(url));
  Serial.print("json_url  ");
  for(uint8_t i=0;i<strlen(url);i++)
  {
    Serial.print(json_url[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
  
  str2hex(namespaceID,json_namespaceID,20);
  Serial.print("json_namespaceID  ");
  for(uint8_t i=0;i<10;i++)
  {

  Serial.print(json_namespaceID[i],HEX);
  Serial.print(" ");
  }
  Serial.println(" ");
  
  memcpy(json_instanceID, instanceID, sizeof(instanceID));
  for(uint8_t i=0;i<6;i++)
  {

  Serial.print(json_instanceID[i],DEC);
  Serial.print(" ");
  }
  Serial.println(" ");
  
  strcpy(json_key,key);
  Serial.print("json_key  ");
  Serial.println(json_key );
  
  for(uint8_t i=0;i<16;i++)
  {

  Serial.print(json_key[i],DEC);
  Serial.print(" ");
  }
  Serial.println(" ");

  json_interv = interv;
  
  Serial.print("json_interv  ");
  Serial.println(json_interv );
  
}







