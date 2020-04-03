#define SW_VERSION  "0.5.0"
//#include "config_schlafzimmer.h"
#include "config_wohnzimmer.h"
//#include "config_tuerme.h"
//#include "config_Wohnzimmer_Deckenlampe.h"
//#include "config_test.h"
//#include "config_iTanne.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include "webupdate.h"
#include <FastLED.h>

TaskHandle_t serial_led_task;
void serial_led_loop( void * pvParameters );

#include "wifitools.h"
#ifdef SERIAL_LED
CRGB leds[SERIAL_LEDS];
CRGB frontbuffer[SERIAL_LEDS];
#endif

boolean is_sunrise = false;

typedef struct {
  uint16_t r;       //Actual colors of (PWM Value)
  uint16_t g;
  uint16_t b;
  
  uint16_t r_next;  //Colors to come (PWM Value)
  uint16_t g_next;
  uint16_t b_next;
  uint16_t r_old;   //Colors from which the fading started
  uint16_t g_old;
  uint16_t b_old;
  
  uint16_t w;
  uint16_t w_next;
  uint16_t w_old;
  boolean w_change;
  uint16_t w_standby;
  long w_nextframe; //When was the last frame while changing
  long w_lastchange; //When did the colors change
  long w_nextchange;
  
  uint16_t brightness; //see colors
  uint16_t brightness_next;
  uint16_t brightness_old;
  uint16_t brightness_standby;
  boolean brightness_change;
  long brightness_nextframe;
  long brightness_lastchange;
  uint16_t r_standby; //if the lamp is switchted off the actual values will be stored here
  uint16_t g_standby;
  uint16_t b_standby;
  uint16_t state;
  long nextframe; //When was the last frame while changing
  long lastchange; //When did the colors change
  long nextchange;
  boolean change; //is there any fading in process
  
  char effectname[20];
}lampeRGB;



typedef struct {  //see colors
  uint16_t brightness;
  uint16_t brightness_next;
  uint16_t brightness_old;
  uint16_t brightness_standby;
  boolean state;
  long nextframe; //When was the last frame while changing
  long lastchange; //When did the colors change
  boolean change;
} lampeSINGLE;



typedef struct { //Type for adressable leds (see colors
  lampeRGB lampe;
  int start; //Additional length (how many adressable leds build up one lamp
  int length;
} serialSection;


#ifdef SERIAL_LED
  typedef struct {
  serialSection lampe_master; //Information for whole stripe
  
  serialSection section[SERIAL_COUNT]; //defined subsection of the stripe
  
  int length; //length of the whole stripe
  boolean is_Master; //is the effect a mastereffect
  
} lampeSERIAL;
#endif


void publishRGBColor();
#include "gfxfunc.h"
  //RGBS
#if defined(CH1_RGB) || defined(CH1_RGBW)
#warning "C1 definieren"
lampeRGB lampe_CH1;
boolean (*effect_CH1)(lampeRGB*, effects*);
#endif
#ifdef CH2_RGB
lampeRGB lampe_CH2;
boolean (*effect_CH1)(lampeRGB*, effects*);
#endif
//RGB end
//Single lights:
#ifdef CH1_SINGLE 
lampeSINGLE lampe_CH1;
#endif
#ifdef CH2_SINGLE
lampeSINGLE lampe_CH2;
#endif
#ifdef CH3_SINGLE
lampeSINGLE lampe_CH3;  
#endif
#ifdef CH4_SINGLE
lampeSINGLE lampe_CH4;
#endif

//Which effects are available
#if defined(CH1_RGB) || defined(CH1_RGBW)
  effects CH1_effects;
#endif
#ifdef CH2_RGB
  effects CH2_effects;
#endif


#ifdef SERIAL_LED
  lampeSERIAL serial_master;
  
  boolean (*section_effect[SERIAL_COUNT])(serialSection*, effects*); //effects for the subsection
  effects section_effects[SERIAL_COUNT]; 
  
  boolean (*effect_serial_master)(lampeSERIAL*, effects*);
  effects serial_master_effects;
  //effects serial_master_effects 
#endif


void led_setup() {
  ledcAttachPin(PIN_CH_0, 0);
  ledcSetup(0, FREQUENCY, RESOLUTION);
  ledcWrite(0, 0);
  
  ledcAttachPin(PIN_CH_1, 1);
  ledcSetup(1, FREQUENCY, RESOLUTION);
  ledcWrite(1, 0);
  
  ledcAttachPin(PIN_CH_2, 2);
  ledcSetup(2, FREQUENCY, RESOLUTION);
  ledcWrite(2, 0);
  
  ledcAttachPin(PIN_CH_3, 3);
  ledcSetup(3, FREQUENCY, RESOLUTION);
  ledcWrite(3, 0);
  
  ledcAttachPin(PIN_CH_4, 4);
  ledcSetup(4, FREQUENCY, RESOLUTION);
  ledcWrite(4, 0);
    
  ledcAttachPin(PIN_CH_5, 5);
  ledcSetup(5, FREQUENCY, RESOLUTION);
  ledcWrite(5, 0); 
}

void led_print() {
float norm_brightness;
float scaled_pwm;
#if defined(CH1_RGB) || defined(CH1_RGBW)
      //Serial.print("B: ");Serial.println(lampe_CH1.brightness);
      //Serial.print("R: ");Serial.println(lampe_CH1.r);
      norm_brightness = float(lampe_CH1.brightness) / 4095;
      ledcWrite(0, round(lampe_CH1.r * norm_brightness));
      ledcWrite(1, round(lampe_CH1.g * norm_brightness));
      ledcWrite(2, round(lampe_CH1.b * norm_brightness));
#ifdef CH1_RGBW
      ledcWrite(3, lampe_CH1.w);
#endif
#endif
#ifdef CH2_RGB
      norm_brightness = float(lampe_CH2.brightness) / 4095;
      ledcWrite(3, round(lampe_CH2.r * norm_brightness));
      ledcWrite(4, round(lampe_CH2.g * norm_brightness));
      ledcWrite(5, round(lampe_CH2.b * norm_brightness));
#endif
//RGB end
//Single lights:
#ifdef CH1_SINGLE 
      ledcWrite(1, lampe_CH1.brightness);
#endif
#ifdef CH2_SINGLE  
      ledcWrite(3, lampe_CH2.brightness);
#endif
#ifdef CH3_SINGLE  
      ledcWrite(4, lampe_CH3.brightness);
#endif
#ifdef CH4_SINGLE
      ledcWrite(5, lampe_CH4.brightness);
#endif  
} 


#include "mqtt.h"
long mqtt_lastry;
//WiFiManager wifiManager;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  #if defined(CH1_RGB) || defined(CH1_RGBW)
  lampe_CH1.r = 0;
  lampe_CH1.g = 0;
  lampe_CH1.b = 0; 
  
  lampe_CH1.r_next = START_RED;
  lampe_CH1.g_next = START_GREEN;
  lampe_CH1.b_next = START_BLUE;

  lampe_CH1.r_standby = START_RED;
  lampe_CH1.g_standby = START_GREEN;
  lampe_CH1.b_standby = START_BLUE;
  lampe_CH1.brightness_standby = START_WHITE;
  
  lampe_CH1.r_old = 0;
  lampe_CH1.g_old = 0;
  lampe_CH1.b_old = 0; 
  
  lampe_CH1.brightness = 0;
  lampe_CH1.brightness_next = START_WHITE;
  lampe_CH1.brightness_change = true;
  lampe_CH1.brightness_old = 0;
  #ifdef CH1_RGBW
  lampe_CH1.w = 0;
  lampe_CH1.w_next = START_WHITE;
  lampe_CH1.w_change = true;
  lampe_CH1.w_old = 0;
  #endif

  lampe_CH1.state = true;

  lampe_CH1.lastchange = millis();
  lampe_CH1.change = true;
  lampe_CH1.nextchange = 0;
  effect_CH1 = SingleChange;
  #endif
  #ifdef CH2_RGB
  lampe_CH2.r = 0;
  lampe_CH2.g = 0;
  lampe_CH2.b = 0; 
  
  lampe_CH2.r_next = START_RED;
  lampe_CH2.g_next = START_GREEN;
  lampe_CH2.b_next = START_BLUE;

  lampe_CH1.r_standby = START_RED;
  lampe_CH1.g_standby = START_GREEN;
  lampe_CH1.b_standby = START_BLUE;
  lampe_CH1.brightness_standby = START_WHITE;
  
  lampe_CH2.r_old = 0;
  lampe_CH2.g_old = 0;
  lampe_CH2.b_old = 0; 
  
  lampe_CH2.brightness = 0;
  lampe_CH2.brightness_next = START_WHITE;
  lampe_CH2.brightness_change = true;
  lampe_CH2.brightness_old = 0;
  lampe_CH2.state = true;
  
  lampe_CH2.lastchange = millis();
  lampe_CH2.change = true;
  lampe_CH2.nextchange = 0;
  effect_CH2 = SingleChange;
  #endif
  //RGB end
  //Single lights:
  #ifdef CH1_SINGLE 
  lampe_CH1.brightness = 0;
  lampe_CH1.brightness_next = SUNRISE_WHITE;
  lampe_CH1.brightness_standby = START_WHITE;
  lampe_CH1.brightness_old  = 0;
  lampe_CH1.state = true;
  #endif
  #ifdef CH2_SINGLE
  lampe_CH2.brightness = 0;
  lampe_CH2.brightness_next = SUNRISE_WHITE;
  lampe_CH2.brightness_standby = START_WHITE;
  lampe_CH2.brightness_old  = 0; 
  lampe_CH2.state = true; 

  lampe_CH2.lastchange = millis();
  lampe_CH2.change = true;
  #endif
  #ifdef CH3_SINGLE
  lampe_CH3.brightness = 0;
  lampe_CH3.brightness_next = SUNRISE_WHITE;
  lampe_CH1.brightness_standby = START_WHITE;
  lampe_CH3.brightness_old  = 0;  
  lampe_CH3.state = true;  
  #endif
  #ifdef CH4_SINGLE
  lampe_CH4.brightness = 0;
  lampe_CH4.brightness_next = SUNRISE_WHITE;
  lampe_CH1.brightness_standby = START_WHITE;
  lampe_CH4.brightness_old  = 0;  
  lampe_CH4.state = true;
  #endif
  led_setup();
  led_print();

//Done with PWM LEDs
//Init Serial LEDS
//serial_master.lampe_master.
#ifdef SERIAL_LED
  serial_master.lampe_master.lampe.r = 0;
  serial_master.lampe_master.lampe.g = 0;
  serial_master.lampe_master.lampe.b = 0; 
  if(STATE_SERIAL == true) {
  serial_master.lampe_master.lampe.r_next = START_RED_SERIAL;
  serial_master.lampe_master.lampe.g_next = START_GREEN_SERIAL;
  serial_master.lampe_master.lampe.b_next = START_BLUE_SERIAL;
  }
  else {
  serial_master.lampe_master.lampe.r_next = 0;
  serial_master.lampe_master.lampe.g_next = 0;
  serial_master.lampe_master.lampe.b_next = 0;
  }
  serial_master.lampe_master.lampe.r_standby = START_RED_SERIAL;
  serial_master.lampe_master.lampe.g_standby = START_GREEN_SERIAL;
  serial_master.lampe_master.lampe.b_standby = START_BLUE_SERIAL;
  serial_master.lampe_master.lampe.brightness_standby = BRIGHTNESS_SERIAL;
  
  serial_master.lampe_master.lampe.r_old = 0;
  serial_master.lampe_master.lampe.g_old = 0;
  serial_master.lampe_master.lampe.b_old = 0; 
  
  serial_master.lampe_master.lampe.brightness = BRIGHTNESS_SERIAL;
  serial_master.lampe_master.lampe.brightness_next = BRIGHTNESS_SERIAL;
  serial_master.lampe_master.lampe.brightness_change = STATE_SERIAL;
  serial_master.lampe_master.lampe.brightness_old = 0;
  serial_master.lampe_master.lampe.brightness_lastchange = millis();
  serial_master.lampe_master.lampe.state = STATE_SERIAL;

  serial_master.lampe_master.lampe.lastchange = millis();
  serial_master.lampe_master.lampe.change = STATE_SERIAL;
  serial_master.lampe_master.lampe.nextchange = 0;
  serial_master.lampe_master.length = SERIAL_LEDS;
  serial_master.lampe_master.start = 0;
  serial_master.is_Master = true;
  effect_serial_master = Master_SingleChange;
  Serial.println(serial_master.lampe_master.length);
  int mark_start = 0;
  for(int i = 0; i < SERIAL_COUNT; i++) {
    serial_master.section[i].lampe.r = START_RED_SERIAL;
    serial_master.section[i].lampe.g = START_GREEN_SERIAL;
    serial_master.section[i].lampe.b = START_BLUE_SERIAL; 

    if(STATE_SERIAL == true) {
    serial_master.section[i].lampe.r_next = START_RED_SERIAL;
    serial_master.section[i].lampe.g_next = START_GREEN_SERIAL;
    serial_master.section[i].lampe.b_next = START_BLUE_SERIAL;
    }
    else {
    serial_master.section[i].lampe.r_standby = 0;
    serial_master.section[i].lampe.g_standby = 0;
    serial_master.section[i].lampe.b_standby = 0;
    }
    serial_master.section[i].lampe.brightness_standby = BRIGHTNESS_SERIAL;
    
  
    serial_master.section[i].lampe.r_old = 0;
    serial_master.section[i].lampe.g_old = 0;
    serial_master.section[i].lampe.b_old = 0; 
  
    serial_master.section[i].lampe.brightness = BRIGHTNESS_SERIAL;
    serial_master.section[i].lampe.brightness_next = BRIGHTNESS_SERIAL;
    serial_master.section[i].lampe.brightness_change = STATE_SERIAL;
    serial_master.section[i].lampe.brightness_old = BRIGHTNESS_SERIAL;
    serial_master.section[i].lampe.state = STATE_SERIAL;
    serial_master.section[i].lampe.brightness_lastchange = millis();
    serial_master.section[i].lampe.brightness_nextframe = 0;
    
    serial_master.section[i].lampe.lastchange = millis();
    serial_master.section[i].lampe.change = STATE_SERIAL;
    serial_master.section[i].lampe.nextchange = 0;
    serial_master.section[i].lampe.nextframe = 0;
    serial_master.section[i].start = mark_start;
    serial_master.section[i].length = section_count[i];
    section_effect[i] = SerialColorChange;
    mark_start += section_count[i];
  }
  //Change here for other chipsets:
  FastLED.addLeds<WS2812B, data_pin, GRB>(frontbuffer, SERIAL_LEDS); //For other chipsets look into the FastLED documentation
//  xTaskCreatePinnedToCore(
//                    serial_led_loop,   /* Task function. */
//                    "SERIAL LEDs",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    1,           /* priority of the task */
//                    &serial_led_task,      /* Task handle to keep track of created task */
//                    1);          /* pin task to core 1 */
#endif
// END Serial LEDs
////////////////////////////////
  
//  delay(5000);
  
//  wifiManager.setConnectTimeout(10);
//  #ifdef AP_PASS
//    wifiManager.autoConnect(AP_NAME, AP_PASS);
//  #else
//    wifiManager.autoConnect(AP_NAME);
//  #endif
  setup_wifi();
  initMQTTChannel();
  client.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  client.setCallback(callback_new);
  if (!MDNS.begin(MQTT_CLIENT_ID)) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  setupWebUpdate();
  Serial.println("Web Update Service startet: User: admin; PW: admin");
  Serial.print("Software: ");Serial.println(SW_VERSION);

}

void loopIPServices() {
 
  static long wifi_last_try = millis();
  static long mqtt_last_check = millis();
  if(WiFi.status() != WL_CONNECTED && millis() - wifi_last_try > WIFI_RETRY ) {
    wifi_last_try = millis();
    Serial.println("Wifi connection lost, trying to reconnect");
    WiFi.reconnect();
  }
  //Update Server

  //Webserver: enables Webupdate Service
  server.handleClient();
  //MQTT 
  if (!client.connected() && millis() - mqtt_last_check > MQTT_RETRY && WiFi.status() == WL_CONNECTED) {
    mqtt_last_check = millis();
    connect_mqtt();
  }
  if(client.connected()) {
    client.loop();
    mqtt_lastry = millis();
  }

  
}


void loop() {

loopIPServices();
/*
 if(millis() > keepalive) {
  Serial.println("alive");
  keepalive = millis() + 60000;
}
*/
boolean anythingChanged = false; //Variable to check if PWM has to be updated
//Checking all channels if anything has changed
if(!is_sunrise) {
#if defined(CH1_RGB) || defined(CH1_RGBW)
  anythingChanged |= effect_CH1(&lampe_CH1, &CH1_effects);
  if(lampe_CH1.brightness_change) {
    fadeRGBBrightness(&lampe_CH1);
    anythingChanged = true;
  }
  #ifdef CH1_RGBW
  if(lampe_CH1.w_change) {
    fadeWhite(&lampe_CH1);
    anythingChanged = true;
  }
  #endif

#endif
#ifdef CH2_RGB
  anythingChanged |= effect_CH2(&lampe_CH2, &CH2_effects);
  if(lampe_CH2.brightness_change) {
    fadeRGBBrightness(&lampe_CH2);
    anythingChanged = true;
  }
#endif


#ifdef CH1_SINGLE
  if(lampe_CH1.change) {
    fadeSINGLE(&lampe_CH1);
    anythingChanged = true;
  }
#endif
#ifdef CH2_SINGLE
  if(lampe_CH2.change) {
    fadeSINGLE(&lampe_CH2);
    anythingChanged = true;
  }
#endif
#ifdef CH3_SINGLE
  if(lampe_CH3.change) {
    fadeSINGLE(&lampe_CH3);
    anythingChanged = true;
  }
#endif
#ifdef CH4_SINGLE
  if(lampe_CH4.change) {
    fadeSINGLE(&lampe_CH4);
    anythingChanged = true;
  }
#endif
#ifdef CH5_SINGLE
  if(lampe_CH5.change) {
    fadeSINGLE(&lampe_CH5);
    anythingChanged = true;
  }
#endif
} else {
  anythingChanged = sunrise_fade();
  #ifdef CH1_RGB
  sunrise_get_RGB(&lampe_CH1);
  #endif
  #ifdef CH2_RGB
  sunrise_get_RGB(&lampe_CH2);
  #endif
  //RGB end
  //Single lights:
  #ifdef CH1_SINGLE
  sunrise_get_SINGLE(&lampe_CH1);   
  #endif
  #ifdef CH2_SINGLE
  sunrise_get_SINGLE(&lampe_CH2);   
  #endif
  #ifdef CH3_SINGLE
  sunrise_get_SINGLE(&lampe_CH3);  
  #endif
  #ifdef CH4_SINGLE
  sunrise_get_SINGLE(&lampe_CH4);
  #endif
  if(sunrise_get_stage() == 7) {
    publishLightState();
    publishLIGHTBrightness();
    publishRGBColor();
    #ifdef CH1_RGBW
    publishWhiteState();
    #endif
    publishEffect();
    is_sunrise = false;
  }
}

  if(anythingChanged) { //Has anything Changed?
    led_print();        //Update PWM
  }
  #ifdef SERIAL_LED
    boolean anythingChanged_Serial = false;

    if(serial_master.is_Master) {
      if(serial_master.lampe_master.lampe.brightness_change) {
        Master_fadeSerialBrightness(&serial_master);
        anythingChanged_Serial = true;
      }
      anythingChanged_Serial |= effect_serial_master(&serial_master, &serial_master_effects);
    }
    else {
      for(int i = 0; i < SERIAL_COUNT; i++) {
        if(serial_master.section[i].lampe.brightness_change) {
          Section_fadeSerialBrightness(&serial_master.section[i]);
          anythingChanged_Serial = true;
        }
        
        anythingChanged_Serial |= section_effect[i](&serial_master.section[i], &section_effects[i]);
      }
    }
  

    if(anythingChanged_Serial) {
      for(int i = 0; i < SERIAL_COUNT; i++) {
        updateSerialBrightness(&serial_master.section[i]);
      }
      FastLED.show();
    }
  #endif
}


void serial_led_loop( void * pvParameters ) {
  while(1) {
  }
}





/*    
#ifdef CH1_RGB
#endif
#ifdef CH2_RGB
#endif
//RGB end
//Single lights:
#ifdef CH1_SINGLE   
#endif
#ifdef CH2_SINGLE   
#endif
#ifdef CH3_SINGLE  
#endif
#ifdef CH4_SINGLE
#endif
 */
