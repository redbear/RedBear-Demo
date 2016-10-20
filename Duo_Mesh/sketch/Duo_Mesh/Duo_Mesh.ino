#if defined(ARDUINO) 
SYSTEM_MODE(MANUAL);//do not connect to cloud
#else
SYSTEM_MODE(AUTOMATIC);//connect to cloud
#endif

#define MAX_CLIENT_NUM   3
#define NANO_NUM         2
#define led              D7

/******************************************************
 *                      Type Define
 ******************************************************/
typedef struct {
    uint16_t  connected_handle;
    uint8_t   addr_type;
    bd_addr_t addr;
    struct {
        gatt_client_service_t service;
        struct{
            gatt_client_characteristic_t chars;
            gatt_client_characteristic_descriptor_t descriptor[2];
        }chars[2];
    }service;
}Device_t;

static advParams_t adv_params;

static uint8_t rx_len;
static uint16_t version_count_low = 0;
static uint16_t version_count_high = 0;

// Server Port
TCPServer server = TCPServer(8888);
TCPClient client[MAX_CLIENT_NUM];
Device_t device;

static uint8_t advdata_temp[31]={0};//the data Duo adv
static uint8_t client_adv_temp[31]={0};//the data nano adv

static uint8_t adv_update_flag = 0;

static void scan_timer_intrp();

Timer t0(500, scan_timer_intrp);

/******************************************************
 *               Function Definitions
 ******************************************************/
 /**
 * @brief Find the data given the type in advertising data.
 *
 * @param[in]  type          The type of field data.
 * @param[in]  advdata_len   Length of advertising data.
 * @param[in]  *p_advdata    The pointer of advertising data.
 * @param[out] *len          The length of found data.
 * @param[out] *p_field_data The pointer of buffer to store field data.
 *
 * @retval 0 Find the data
 *         1 Not find.
 */

uint32_t ble_advdata_decode(uint8_t type, uint8_t advdata_len, uint8_t *p_advdata, uint8_t *len, uint8_t *p_field_data)
{
    uint8_t index=0;
    uint8_t field_length, field_type;

    while(index<advdata_len)
    {
        field_length = p_advdata[index];
        field_type   = p_advdata[index+1];
        if(field_type == type)
        {
            memcpy(p_field_data, &p_advdata[index+2], (field_length-1));
            *len = field_length - 1;
            return 0;
        }
        index += field_length + 1;
    }
    return 1;
}

/**
 * @brief Callback for scanning device.
 *
 * @param[in]  *report
 *
 * @retval None
 */
void reportCallback(advertisementReport_t *report)
{
    uint8_t index;
    Serial.println("BLE scan ");
    Serial.print("The ADV data: ");
    for(index=0; index<report->advDataLen; index++)
        {
            Serial.print(report->advData[index], HEX);
            Serial.print(" ");
        }
        
        Serial.println(" ");
    
    if((report->advData[0] ==  0x08)&&(report->advData[1] == 0x16)&&(report->advData[2] == 0xE4)&&(report->advData[3] == 0xFE))
    {
        if(memcmp(report->advData,client_adv_temp,sizeof(report->advData)) != 0)//confirm not same data
        {
            if(client_adv_temp[8]==report->advData[0])
            {
              Serial.println("same data1");
              return;
            }
            memcpy(client_adv_temp,report->advData,report->advDataLen);
            
            adv_update_flag = 1;
        }
    }
    else if((report->advData[0] ==  0x07)&&(report->advData[1] == 0x16)&&(report->advData[2] == 0xE4)&&(report->advData[3] == 0xFE))
    {
        ble.stopScanning();
      
        version_count_low++;
        if(version_count_low>=0xFF)
        {
          version_count_low = version_count_low-0x100;
          version_count_high++;
          if(version_count_high>=0xFF)
          {
            version_count_high = 0;
          }
        }
        client_adv_temp[6] = version_count_low;
        client_adv_temp[7] = version_count_high;

        ble.setAdvertisementData(sizeof(client_adv_temp), client_adv_temp);
        ble.startAdvertising();

        Serial.println("BLE start advertising.");
        ble.stopAdvertising();
        ble.startScanning();
    }

}


//static void scan_timer_intrp(btstack_timer_source_t *ts)
static void scan_timer_intrp()
{  
        
    if(adv_update_flag == 1)
    {
        ble.stopScanning();
        
        if(client_adv_temp[8]== 0x01)
        {
            digitalWrite(led, HIGH);
        }
        else if(client_adv_temp[8]== 0x00)
        {

            digitalWrite(led, LOW);
        }
        version_count_low = client_adv_temp[6];//upadte the version num
        version_count_high = client_adv_temp[7];
        version_count_low= version_count_low+NANO_NUM*5;
        if(version_count_low>=0xFF)
        {
          version_count_low = version_count_low-0x100;
          version_count_high++;
          if(version_count_high>=0xFF)
          {
            version_count_high = 0;
          }
        }
        client_adv_temp[6] = version_count_low;
        client_adv_temp[7] = version_count_high;
        ble.setAdvertisementData(sizeof(client_adv_temp), client_adv_temp);
        ble.startAdvertising();         
        ble.stopAdvertising();
        adv_update_flag = 0;
        ble.startScanning();
    }

}


void setup()
{
    uint8_t adv_data[8]={0x07,0x16,0xE4,0xFE,0,0,0,0};
    char addr[16];

    Serial.begin(115200);
    delay(5000);

    pinMode(led,OUTPUT);
    
    
    
    WiFi.on();
    WiFi.connect();
  
    IPAddress localIP = WiFi.localIP();
    
    while (localIP[0] == 0)
    {
        localIP = WiFi.localIP();
        Serial.println("waiting for an IP address");
        delay(1000);
    }
  
    sprintf(addr, "%u.%u.%u.%u", localIP[0], localIP[1], localIP[2], localIP[3]);
  
    Serial.println(addr);

    server.begin();  

    ble.init();
    ble.onScanReportCallback(reportCallback);


    adv_params.adv_int_min = 0x00A0;
    adv_params.adv_int_max = 0x01A0;
    adv_params.adv_type    = 0;
    adv_params.dir_addr_type = 0;
    memset(adv_params.dir_addr,0,6);
    adv_params.channel_map = 0x07;//channel 38
    adv_params.filter_policy = 0x00;
    
    ble.setAdvertisementParams(&adv_params);
    // Set scan parameters.
    ble.setScanParams(0, 4, 0x0030);//interval[0x0004,0x4000],unit:0.625ms
    ble.startScanning();
    Serial.println("Start scanning ");

    t0.start();
       
    ble.setAdvertisementData(sizeof(adv_data), adv_data);
    ble.startAdvertising();

    Serial.println("BLE start advertising.");
    ble.stopAdvertising();
    
}

void loop()
{
    char c ;
    uint8_t adv_data[9]={0x08,0x16,0xE4,0xFE,0,0,0,0,0};
    for(uint8_t client_num = 0;client_num<MAX_CLIENT_NUM;client_num++)
    {
        if (client[client_num].connected())
        {
            if (client[client_num].available()) 
            {
              ble.stopScanning();
              uint8_t i = 0;            
              
              rx_len =client[client_num].available();
              while(client[client_num].available())
              {
                c = client[client_num].read();             // read a byte, then
                adv_data[8+i] = c;
                Serial.print(adv_data[8+i],HEX);  
                Serial.println("");            
                i++;
              }
              Serial.println("");
              delay(100);
              
              
              version_count_low=version_count_low+NANO_NUM*5;
              if(version_count_low>=0xFF)
              {
                version_count_low = version_count_low-0x100;
                version_count_high++;
                if(version_count_high>=0xFF)
                {
                  version_count_high = 0;
                }
              }
              adv_data[6] = version_count_low;
              adv_data[7] = version_count_high;
              
              if(adv_data[8]==1)
              {
                    Serial.println("Turn On");                    
                    digitalWrite(led, HIGH);
                    
              }
              else if(adv_data[8]==0)
              {
                    Serial.println("Turn Off");                    
                    digitalWrite(led, LOW);
                    
              }
              
              Serial.print("adv_data: ");
              for(uint8_t h = 0;h<sizeof(adv_data);h++)
              {
                  Serial.print(adv_data[h],HEX);  
                  Serial.print("");
              }   
              Serial.println("");
              ble.setAdvertisementData(sizeof(adv_data), adv_data);
              ble.startAdvertising();
              Serial.println("BLE start advertising.");
              delay(10);
              ble.stopAdvertising();
              delay(190);     
              
            }

            ble.startScanning();
            
        }
        else
        {
            client[client_num] = server.available();
            
        }
    }
    delay(100);
 
}

