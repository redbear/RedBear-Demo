/**
 * This file is part of Faros.
 * 
 * Copyright 2015 Frank Duerr
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 * Creat by duerrfk
 * 
 * Motify by Jackson_Lv 2016.5
 * 
 */
#include"application.h"
#include <AES.h>
#include"eddystone.h"
//#include"cloud_function.h"
#include"cloud_json.h"

//#define DEVICE_NAME                "CloudBeacon"

//#define Button D1

static advParams_t adv_params;

#define AP     "AP-02_2.4G"
#define PASSWORD "0098019777"

/******************** IBeacon Format ***************************/                  
/* 02 01 06 1A FF 4C 00 02 15: iBeacon prefix (fixed)
  71 3d 00 00 50 3e 4c 75 ba 94 31 48 f1 8d 94 1e: proximity UUID
  00 49: major
  00 0A: minor
  C5: 2’s complement of measured TX power*/
  
static uint8_t ibea_adv_data[31]={
    0x02,
    BLE_GAP_AD_TYPE_FLAGS,
    BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE, 
    0x1A,
    BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
    0x4C,0x00,0x02,0x15,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0xC5
};

// 10 byte namespace id. Google suggests different methods to create this:
// - Truncated hash: first 10 bytes of your SHA1 hash of your FQDN.
// - Elided Version 4 UUID: version 4 UUID with bytes 5 - 10 (inclusive) removed 
//const uint8_t eddystone_namespace_id[10] = {0x72, 0x65, 0x64, 0x62, 0x65, 0x61, 0x72, 0x6c, 0x61,0x62};

// 6 byte instance id (any scheme you like).
//const uint8_t eddystone_instance_id[6] = {0x65, 0x64, 0x64, 0x79, 0x73, 0x74};

/*Byte offset  Value Description Data Type
0 0x02  Length  Flags. CSS v5, Part A, § 1.3
1 0x01  Flags data type value 
2 0x06  Flags data  
3 0x03  Length  Complete list of 16-bit Service UUIDs. Ibid. § 1.1
4 0x03  Complete list of 16-bit Service UUIDs data type value 
5 0xAA  16-bit Eddystone UUID 
6 0xFE  ... 
7 0x??  Length  Service Data. Ibid. § 1.11
8 0x16  Service Data data type value  
9 0xAA  16-bit Eddystone UUID 
10  0xFE  ...*/
// 10 byte namespace id. Google suggests different methods to create this:
// - Truncated hash: first 10 bytes of your SHA1 hash of your FQDN.
// - Elided Version 4 UUID: version 4 UUID with bytes 5 - 10 (inclusive) removed 
// 6 byte instance id (any scheme you like).
static uint8_t uid_adv_data[31]={
    0x02,0x01,0x06,
    0x03,0x03,0xAA,0xFE,
    0x15,0x16,0xAA,0xFE,
    EDDYSTONE_FRAME_TYPE_UID,
    (uint8_t)EDDYSTONE_TXPWR,
    /*0x72, 0x65, 0x64, 0x62, 0x65, 0x61, 0x72, 0x6c, 0x61,0x62, 
    0x65, 0x64, 0x64, 0x79, 0x73, 0x74*/
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00
};


// Scheme of the encoded URL.
const url_schemes eddystone_url_scheme = http_www_dot;
// Encoded URL (max. 17 bytes)
// The following bytes expand to a sequence of characters:
// 0x00  .com/
// 0x01  .org/
// 0x02  .edu/
// 0x03  .net/
// 0x04  .info/
// 0x05  .biz/
// 0x06  .gov/
// 0x07  .com
// 0x08  .org
// 0x09  .edu
// 0x0a  .net
// 0x0b  .info
// 0x0c  .biz
// 0x0d  .gov
// 14..32  0x0e..0x20  Reserved for Future Use
// 127..255  0x7F..0xFF  Reserved for Future Use
// The following example encodes the URL frank-duerr.de
// ("http://www." is added by the schema definition)
static uint8_t url_adv_data[31]={
    0x02,0x01,0x06,
    0x03,0x03,0xAA,0xFE,
    0x11,0x16,0xAA,0xFE,
    EDDYSTONE_FRAME_TYPE_URL,
    (uint8_t)EDDYSTONE_TXPWR,
    //http_www_dot, 0x72, 0x65, 0x64, 0x62, 0x65, 0x61, 0x72, 0x6c, 0x61,0x62, 0x07//redbearlab.com
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};


static uint8_t eid_adv_data[31]={
    0x02,0x01,0x06,
    0x03,0x03,0xAA,0xFE,
    0x11,0x16,0xAA,0xFE,
    EDDYSTONE_FRAME_TYPE_EID,
    (uint8_t)EDDYSTONE_TXPWR,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};



//identify key
static uint8_t identify_key[16]={};
//use to computing the temporary key
static uint8_t value_temp[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
//use to save the temporary key
static const uint8_t const_temp_key[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00};
static uint8_t temp_key[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00};
//use to computing the EID value 
static const uint8_t const_EID_value_temp[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
static uint8_t EID_value_temp[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
//use to save the EID value 
static uint8_t EID_value[16] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static btstack_timer_source_t sp_time_counter;
static btstack_timer_source_t time_counter;
static uint8_t K = 0;

static uint16_t time_count = 0;
static uint32_t Ktime_count = 1;

AES aes ;

void sp_time_counter_acheive();
void time_counter_acheive();
void adv_type_change();

// set one-shot timer
Timer t0(1000, sp_time_counter_acheive);

Timer t1(1000, time_counter_acheive);

Timer adv_timer(5000, adv_type_change);

static uint8_t eddystone_type = 0x10;

static uint8_t adv_data[31]={};

static uint8_t ibeacon_en = 0;
static uint8_t uid_en = 0;
static uint8_t url_en = 0;
static uint8_t eid_en = 0;

static uint8_t adv_loop_num = 0;

static uint8_t eid_init = 0;

/**
 * @brief Connect handle.
 *
 * @param[in]  status   BLE_STATUS_CONNECTION_ERROR or BLE_STATUS_OK.
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceConnectedCallback(BLEStatus_t status, uint16_t handle) {
    switch (status){
        case BLE_STATUS_OK:
            Serial.println("Device connected!");
            break;
        default:
            break;
    }
}

/**
 * @brief Disconnect handle.
 *
 * @param[in]  handle   Connect handle.
 *
 * @retval None
 */
void deviceDisconnectedCallback(uint16_t handle){
    Serial.println("Disconnected.");
}


void sp_time_counter_acheive()
{
  if(eid_en == 1)
  {
      byte succ ;
      time_count++;
      temp_key[14]=(0xFF00&time_count)>>8;
      temp_key[15]=0xFF&time_count;
      Serial.print("temp_key: ");
      for(uint8_t i = 0;i<sizeof(temp_key);i++)
      {
        Serial.print(temp_key[i],HEX);
        Serial.print(" ");
      }
      Serial.println("");
      
      succ = aes.set_key (identify_key, 128) ;
      succ = aes.encrypt (temp_key, value_temp) ;
      if (succ != SUCCESS)
      {
            Serial.println ("Failure encrypt") ;
      }
  }
}


void time_counter_acheive()
{
  
  if(eid_en == 1)
  {
      byte succ ;
      ble.stopAdvertising();
      K++;
      if(K ==16)
      {
          K = 0;
      }
      Ktime_count = pow(2,K);
      EID_value_temp[11] = K;
      Serial.println("");
      Serial.print("Ktime_count: ");
      Serial.print(Ktime_count);
      Serial.println(" sec");
      EID_value_temp[12] = (0xFF000000&(Ktime_count)>>24);
      EID_value_temp[13] = (0x00FF0000&(Ktime_count)>>16);
      EID_value_temp[14] = (0x0000FF00&Ktime_count)>>8;
      EID_value_temp[15] = (0x000000FF&Ktime_count);
      succ = aes.set_key (value_temp, 128) ;
      succ = aes.encrypt (EID_value_temp, EID_value) ;
      if (succ != SUCCESS)
            Serial.println ("Failure encrypt") ;
      else
      {
        for(uint8_t i=0;i<8;i++)
        {
          adv_data[i+13] = EID_value[i];
        }
        ble.setAdvertisementData(sizeof(adv_data), adv_data);
      
        ble.startAdvertising();
      }
      t1.changePeriod(Ktime_count*1000);
      t1.reset();
  }
  
  
}

void adv_type_change()
{
    Serial.println ("Adv Type Change") ;
    Serial.print("loop num:  ") ;
    Serial.println(adv_loop_num) ;
    switch(adv_loop_num)
    {
      case 0:
      {
        if(ibeacon_en == 1)
        {
          Serial.println ("ibeacon") ;
          memcpy(adv_data,ibea_adv_data,sizeof(ibea_adv_data));
          ble.setAdvertisementData(sizeof(adv_data), adv_data);
          
          ble.startAdvertising();
          Serial.println ("startAdvertising ") ;
          adv_loop_num++;
          break;
        }
        else {
          //ble.stopAdvertising();
          adv_loop_num++;
        }
        
      }
      case 1:
      {
        if(uid_en == 1)
        {
          Serial.println ("uid") ;
          memcpy(adv_data,uid_adv_data,sizeof(uid_adv_data));
          ble.setAdvertisementData(sizeof(adv_data), adv_data);
          
          ble.startAdvertising();
          Serial.println ("startAdvertising ") ;
          adv_loop_num++;
          break;
        }
        else {
          //ble.stopAdvertising();
          adv_loop_num++;
        }
      }
      case 2:
      {
        if(url_en == 1)
        {
          Serial.println ("url") ;
          memcpy(adv_data,url_adv_data,sizeof(url_adv_data));
          ble.setAdvertisementData(sizeof(adv_data), adv_data);
          
          ble.startAdvertising();
          Serial.println ("startAdvertising ") ;
          adv_loop_num++;
          break;
        }
        else {
          //ble.stopAdvertising();
          adv_loop_num++;
        }
      }

      case 3:
      {
        if(eid_en == 1)
        {
            Serial.println ("eid") ; 
            if(eid_init == 0)
            {
                time_count = 0;
                K = 0;
                t1.changePeriod(1000);
                
                               
                byte succ ;
                memcpy(adv_data,eid_adv_data,sizeof(eid_adv_data));
                memcpy(temp_key,const_temp_key,sizeof(const_temp_key));
                Serial.print("temp_key: ");
                for(uint8_t i = 0;i<sizeof(temp_key);i++)
                {
                  Serial.print(temp_key[i],HEX);
                  Serial.print(" ");
                }
                Serial.println("");
                memcpy(EID_value_temp,const_EID_value_temp,sizeof(EID_value_temp));
                succ = aes.set_key (identify_key, 128) ;
                succ = aes.encrypt (temp_key, value_temp) ;
                if (succ != SUCCESS)
                    Serial.println ("Failure encrypt") ;
                else
                {
                    Serial.print ("value temp: ") ;
                    for(uint8_t i = 0;i<sizeof(value_temp);i++)
                    {
                      Serial.print (value_temp[i],HEX) ;
                      Serial.print (" ") ;
                    }
                    Serial.println (" ") ;
                }
                    succ = aes.set_key (value_temp, 128) ;
                    succ = aes.encrypt (EID_value_temp, EID_value) ;
                if (succ != SUCCESS)
                    Serial.println ("Failure encrypt") ;  
                else
                {
          
                    Serial.print ("EID value: ") ;
                    for(uint8_t i = 0;i<sizeof(EID_value);i++)
                    {
                      Serial.print (EID_value[i],HEX) ;
                      Serial.print (" ") ;
                    }
                    Serial.println (" ") ;
          
                    for(uint8_t i=0;i<8;i++)
                    {
                      adv_data[i+13] = EID_value[i];
                    }  
                }
                ble.setAdvertisementData(sizeof(adv_data), adv_data);     
                ble.startAdvertising();
                Serial.println ("startAdvertising ") ;
                
                if(t0.isActive())
                {
                    t0.reset();
                    t1.reset();
                }
                else 
                {
                    
                    t0.start();
                    t1.start();
                }


                eid_init = 1;
              }
              adv_loop_num=0;
              break;
          }
      else 
          {
            
              t0.reset();
              t1.reset();
              t0.stop();
              t1.stop();
              //ble.stopAdvertising();
              eid_init = 0;
              adv_loop_num=0;
          }
      }
      
      
      default:break;
    }
    


    
}


/**********************************************************************************************
 * Function Name  : setIBeacon
 * Description    : send json to duo through particle cloud to set IBeacon UUID,enable/disable
 * Input          : Json
 * Output         : 
 * Return         : 
 **********************************************************************************************/
int setIBeacon(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    for(uint8_t i =0;i<16;i++){
        ibea_adv_data[i+9] =  json_UUID[i];
      }
      
    Serial.print("ibea_adv_data ");
    for(uint8_t i = 0; i<31;i++)
    {
        Serial.print(ibea_adv_data[i]);
        Serial.print(" ");
    }
    Serial.println(" ");
    
    if(json_enable == 1)
    {
      ibeacon_en = 1;
    }
    else ibeacon_en = 0;

    
}

/**********************************************************************************************
 * Function Name  : setIBUUID2
 * Description    : send json to duo through particle cloud to set IBeacon UUID2
 * Input          : Json
 * Output         : 
 * Return         : 
 **********************************************************************************************/
/*int setIBUUID2(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    for(uint8_t i =0;i<8;i++){
        ibea_adv_data[i+17] =  json_UUID2[i];
      }
      
    Serial.print("ibea_adv_data ");
    for(uint8_t i = 0; i<31;i++)
    {
        Serial.print(ibea_adv_data[i]);
        Serial.print(" ");
    }
    Serial.println(" ");

    
}*/

/*******************************************************************************************************
 * Function Name  : setUIDEn
 * Description    : send json to duo through particle cloud to set EddyStone enable ,instance id
 * Input          : Json 
 * Output         : 
 * Return         : 
 *******************************************************************************************************/
int setUIDEn(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    for(uint8_t i =0;i<6;i++){
        uid_adv_data[i+23] =  json_instanceID[i];
      } 

    Serial.print("uid_adv_data ");
    for(uint8_t i = 0; i<31;i++)
    {
        Serial.print(uid_adv_data[i]);
        Serial.print(" ");
    }
    Serial.println(" ");
    
    if(json_enable == 1)
    {
        uid_en = 1;
    }
    else uid_en = 0;

    
}

/*******************************************************************************************************
 * Function Name  : setUIDName
 * Description    : send json to duo through particle cloud to set EddyStone namespace id
 * Input          : Json 
 * Output         : 
 * Return         : 
 *******************************************************************************************************/
int setUIDName(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    
    for(uint8_t i =0;i<10;i++){
        uid_adv_data[i+13] =  json_namespaceID[i];
      }
    Serial.print("uid_adv_data ");
    for(uint8_t i = 0; i<31;i++)
    {
        Serial.print(uid_adv_data[i]);
        Serial.print(" ");
    }
    Serial.println(" ");
       
}




/*******************************************************************************************************
 * Function Name  : setURL
 * Description    : send json to duo through particle cloud to set EddyStones URL enable 
 * Input          : Json 
 * Output         : 
 * Return         : 
 *******************************************************************************************************/
int setURL(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);
 
    for(uint8_t i =0;i<url_len;i++){
        url_adv_data[i+13] =  json_url[i];
      }
    for(uint8_t i =url_len+13;i<31;i++)
    {
        url_adv_data[i] = 0xFF;
     }
    Serial.print("url_adv_data ");
    for(uint8_t i = 0; i<31;i++)
    {
        Serial.print(url_adv_data[i]);
        Serial.print(" ");
    }
    Serial.println(" ");
    
    if(json_enable == 1)
    {
        url_en = 1;
    }
    else url_en = 0;

    
}

/**********************************************************************************************
 * Function Name  : setEIDEn
 * Description    : send json to duo through particle cloud to set EID enable
 * Input          : Json
 * Output         : 
 * Return         : 
 **********************************************************************************************/
int setEID(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    memcpy(identify_key,json_key,16);
    Serial.print("identify_key: ");
    for(uint8_t i =0;i<16;i++)
    {
      Serial.print(identify_key[i],DEC);
      Serial.print(" ");
    }
    Serial.println("");
    
    if(json_enable == 1)
    {
      eid_en = 1;
    }
    else eid_en = 0;

    //reset eid 
    eid_init = 0;

    time_count = 0;
    K = 0;
    t1.changePeriod(1000);
    t0.stop();
    t1.stop();
}

/**********************************************************************************************
 * Function Name  : setInterval
 * Description    : send json to duo through particle cloud to set Interval
 * Input          : Json
 * Output         : 
 * Return         : 
 **********************************************************************************************/
int setInterval(String command)
{
    char json_rec_temp[255];
    Serial.print("length: ");
    Serial.println(command.length());
    command.toCharArray(json_rec_temp,command.length()+1);
    Serial.print("command: ");
    Serial.println(command);
    Serial.print("json: ");
    Serial.println(json_rec_temp);
    parseJson(json_rec_temp);

    adv_timer.changePeriod(json_interv);
    adv_timer.reset();

}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup()
{
  
    byte succ ;
    Serial.begin(115200);
    delay(5000);
    Serial.println("EddyStone demo.");

    WiFi.on();
    WiFi.setCredentials(AP, PASSWORD, WPA2);
    WiFi.connect();
  
    Serial.println("Waiting for an IP address...\n");
    while (!WiFi.ready()) {
      delay(1000);
    }
    
    // Wait IP address to be updated.
    IPAddress localIP = WiFi.localIP();
    while (localIP[0] == 0) {
      localIP = WiFi.localIP();
      delay(1000);
    }

    // you're connected now, so print out the status:
    printWifiStatus();
    //ble.debugLogger(true);
    ble.init();
    ble.onConnectedCallback(deviceConnectedCallback);
    ble.onDisconnectedCallback(deviceDisconnectedCallback);
    adv_params.adv_int_min = 0x00A0;
    adv_params.adv_int_max = 0x01A0;
    adv_params.adv_type    = 7;
    adv_params.dir_addr_type = 0;
    memset(adv_params.dir_addr,0,6);
    adv_params.channel_map = 0x07;
    adv_params.filter_policy = 0x00;
    
    ble.setAdvertisementParams(&adv_params);
    
    
    adv_timer.start();
    Particle.function("setIBeacon",setIBeacon);
    Particle.function("setUIDEn",setUIDEn);
    Particle.function("setUIDName",setUIDName);
    Particle.function("setURL",setURL);
    Particle.function("setEID",setEID);
    Particle.function("setInterval",setInterval);
    /*Particle.function("digitalread",tinkerDigitalRead);
    Particle.function("digitalwrite",tinkerDigitalWrite);
    Particle.function("analogread",tinkerAnalogRead);   
    Particle.function("analogwrite", tinkerAnalogWrite);*/
}

void loop()
{
    
}

