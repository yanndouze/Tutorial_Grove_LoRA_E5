#include<SoftwareSerial.h>

SoftwareSerial e5(6, 7); // (RX, TX)
static char recv_buf[512];
static bool is_exist = false;
static bool is_join = false;
static int led = 0;
int ret=0;
short tmp =20;
short hum=50; 

static int at_send_check_response(char *p_ack, int timeout_ms, char *p_cmd, ...)
{
    int ch;
    int num = 0;
    int index = 0;
    int startMillis = 0;
    memset(recv_buf, 0, sizeof(recv_buf));
    e5.write(p_cmd);
    Serial.write(p_cmd);
    delay(200);
    startMillis = millis();
    do
    {
        while (e5.available() > 0)
        {
            ch = e5.read();
            recv_buf[index++] = ch;
            Serial.write(ch);
            delay(2);
        }
    } while (millis() - startMillis < timeout_ms);

    if (strstr(recv_buf, p_ack) != NULL)
        {
            return 1;
        }
    else return 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  e5.begin(9600);

  Serial.print("E5 LORAWAN TEST\r\n");

  if(at_send_check_response("+AT: OK", 100, "AT\r\n"))
  {
        is_exist = true;
        at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
        delay(2000);
        at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
        at_send_check_response("+DR: EU868", 1000, "AT+DR=EU868\r\n");
        //at_send_check_response("+CH: NUM", 1000, "AT+CH=NUM,0-2\r\n");
        at_send_check_response("+CH: 3,8671", 1000, "AT+CH=3,867.1,0,5\r\n");
        at_send_check_response("+CH: 4,8673", 1000, "AT+CH=4,867.3,0,5\r\n");
        at_send_check_response("+CH: 5,8675", 1000, "AT+CH=5,867.5,0,5\r\n");
        at_send_check_response("+CH: 6,8677", 1000, "AT+CH=6,867.7,0,5\r\n");
        at_send_check_response("+CH: 7,8679", 1000, "AT+CH=7,867.9,0,5\r\n");
        at_send_check_response("+LW: DC, OFF", 1000, "AT+LW=DC,OFF\r\n");
        at_send_check_response("+LW: JDC, OFF", 1000, "AT+LW=JDC,OFF\r\n");
         at_send_check_response("+ADR: OFF", 1000, "AT+ADR=OFF\r\n");
        //at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"35DE73F781531184CEB7EECE0DA9FB0F\"\r\n"); //Helium
        at_send_check_response("+KEY: APPKEY", 1000, "AT+KEY=APPKEY,\"C2352384E9647D8615A8CB59C9A53646\"\r\n");
        at_send_check_response("+KEY: DEVEUI", 1000, "AT+KEY=DEVEUI,\"2CF7F12032302911\"\r\n");
        at_send_check_response("+KEY: APPEUI", 1000, "AT+KEY=APPEUI,\"8000000000000016\"\r\n");
        
        
        at_send_check_response("+MODE: LWOTAA", 1000, "AT+MODE=LWOTAA\r\n");
        
       
       
        //ret=at_send_check_response("+PORT: 8", 1000, "AT+PORT=8\r\n");
        
        delay(200);
        is_join = true;
   }
   else
    {
        is_exist = false;
        Serial.print("No E5 module found.\r\n");
        
    }
}

void loop() {

  if (is_exist)
    {
        int ret = 0;
        if (is_join)
        {
 
            ret = at_send_check_response("+JOIN: Network joined", 12000, "AT+JOIN\r\n");
            if (ret)
            {
                is_join = false;
                Serial.println();
                Serial.print("Network JOIN !\r\n\r\n");
            }
            else
            {
                at_send_check_response("+ID: AppEui", 1000, "AT+ID\r\n");
                Serial.println();
                Serial.print("JOIN failed!\r\n\r\n");
                delay(5000);
            }
        }
        else 
        {
            char cmd[128];
            sprintf(cmd, "AT+CMSGHEX=%04X%04X\r\n", tmp++, hum++);
            at_send_check_response("ACK Received", 5000, cmd);
            delay(20000);
        }
    }
    else
    {
        delay(1000);
    }
}
