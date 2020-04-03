WiFiClient espClient;
PubSubClient client(espClient);


char mqttbuffer[256] = "";
const uint8_t MSG_BUFFER_SIZE = 20;
char m_msg_buffer[MSG_BUFFER_SIZE]; 

// function called to publish the state of the led (on/off)
void publishLightState() {
        //RGBS
      #if defined(CH1_RGB) || defined(CH1_RGBW)
      if (lampe_CH1.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH1, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH1, LIGHT_OFF, true);
      }
      #endif      
      #ifdef CH2_RGB
      if (lampe_CH2.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH2, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH2, LIGHT_OFF, true);
      }
      #endif
      //RGB end
      //Single lights:
      #ifdef CH1_SINGLE
      if (lampe_CH1.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH1, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH1, LIGHT_OFF, true);
      }   
      #endif
      #ifdef CH2_SINGLE
      if (lampe_CH2.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH2, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH2, LIGHT_OFF, true);
      }     
      #endif
      #ifdef CH3_SINGLE
      if (lampe_CH3.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH3, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH3, LIGHT_OFF, true);
      }     
      #endif
      #ifdef CH4_SINGLE
      if (lampe_CH4.state) {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH4, LIGHT_ON, true);
      } else {
        client.publish(MQTT_LIGHT_STATE_TOPIC_CH4, LIGHT_OFF, true);
      }     
      #endif
      #ifdef SERIAL_LED
      boolean state_val = false;
      if(subscribe_sections == true) {
        for(int i = 0; i < SERIAL_COUNT; i++) {
          state_val |= serial_master.section[i].lampe.state;
          if (serial_master.section[i].lampe.state) {
            client.publish(MQTT_LIGHT_SERIAL_SECTION_STATE_TOPIC[i], LIGHT_ON, true);
          } else {
            client.publish(MQTT_LIGHT_SERIAL_SECTION_STATE_TOPIC[i], LIGHT_OFF, true);
          }
        }
      }
      if(state_val) {
        serial_master.lampe_master.lampe.state = true;
        client.publish(MQTT_LIGHT_SERIAL_STATE_TOPIC, LIGHT_ON, true);
      } else {
        serial_master.lampe_master.lampe.state = false;
        client.publish(MQTT_LIGHT_SERIAL_STATE_TOPIC, LIGHT_OFF, true);
      }
      #endif
}

// function called to publish the brightness of the led (0-100)
void publishLIGHTBrightness() {
      //RGBS
      #if defined(CH1_RGB) || defined(CH1_RGBW)
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH1.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1, m_msg_buffer, true);
      #endif
      #ifdef CH2_RGB
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH2.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2, m_msg_buffer, true);
      #endif
      //RGB end
      //Single lights:
      #ifdef CH1_SINGLE
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH1.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1, m_msg_buffer, true);   
      #endif
      #ifdef CH2_SINGLE  
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH2.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2, m_msg_buffer, true);   
      #endif
      #ifdef CH3_SINGLE  
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH3.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH3, m_msg_buffer, true);   
      #endif
      #ifdef CH4_SINGLE
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH4.brightness_next);
      client.publish(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH4, m_msg_buffer, true);     
      #endif
      #ifdef SERIAL_LED
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", serial_master.lampe_master.lampe.brightness_next);
      client.publish(MQTT_LIGHT_SERIAL_BRIGHTNESS_STATE_TOPIC, m_msg_buffer, true);
      if(subscribe_sections == true) {
        for(int i = 0; i < SERIAL_COUNT; i++) {
          snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", serial_master.section[i].lampe.brightness_next);
          client.publish(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_STATE_TOPIC[i], m_msg_buffer, true);          
        }
      }
      #endif
}

// function called to publish the colors of the led (xx(x),xx(x),xx(x))
void publishRGBColor() {
        //RGBS
      #if defined(CH1_RGB) || defined(CH1_RGBW)
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d,%d,%d", lampe_CH1.r_next / 16, lampe_CH1.g_next /16, lampe_CH1.b_next/16);
      client.publish(MQTT_LIGHT_RGB_STATE_TOPIC_CH1, m_msg_buffer, true);
      #endif
      #ifdef CH2_RGB
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d,%d,%d", lampe_CH2.r_next /16, lampe_CH2.g_next/16, lampe_CH2.b_next/16);
      client.publish(MQTT_LIGHT_RGB_STATE_TOPIC_CH2, m_msg_buffer, true);
      #endif
      #ifdef SERIAL_LED
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d,%d,%d", serial_master.lampe_master.lampe.r_next, serial_master.lampe_master.lampe.g_next, serial_master.lampe_master.lampe.b_next);
      client.publish(MQTT_LIGHT_SERIAL_RGB_STATE_TOPIC, m_msg_buffer, true);
      if(subscribe_sections == true) {
        for(int i = 0; i < SERIAL_COUNT; i++) {
          snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d,%d,%d", serial_master.section[i].lampe.r_next, serial_master.section[i].lampe.g_next, serial_master.section[i].lampe.b_next);
          client.publish(MQTT_LIGHT_SERIAL_SECTION_RGB_STATE_TOPIC[i], m_msg_buffer, true);  
        }
      }
      #endif
}

#ifdef CH1_RGBW
void publishWhiteState() {
      //RGBS
      #if defined(CH1_RGBW)
      snprintf(m_msg_buffer, MSG_BUFFER_SIZE, "%d", lampe_CH1.w_next);
      client.publish(MQTT_LIGHT_WHITE_STATE_TOPIC_CH1, m_msg_buffer, true);
      #endif
}
#endif



void publishEffect() {
      #if defined(CH1_RGB) || defined(CH1_RGBW)
        client.publish(MQTT_LIGHT_EFFECT_STATE_TOPIC_CH1, lampe_CH1.effectname, true);
      #endif
      #ifdef CH2_RGB
        client.publish(MQTT_LIGHT_EFFECT_STATE_TOPIC_CH2, lampe_CH2.effectname, true);
      #endif
      #ifdef SERIAL_LED
        client.publish(MQTT_LIGHT_SERIAL_EFFECT_STATE_TOPIC, serial_master.lampe_master.lampe.effectname, true);
        if(subscribe_sections == true) {
          for(int i = 0; i < SERIAL_COUNT; i++) {
            client.publish(MQTT_LIGHT_SERIAL_SECTION_EFFECT_STATE_TOPIC[i], serial_master.section[i].lampe.effectname, true);
          }
        }
      #endif
}

void callback_new(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  char* token_payload;
  for(int i = 0; i < length; i++) {
    mqttbuffer[i] = payload[i];
  }
  mqttbuffer[length] = '\0';
  is_sunrise = false;
  if (!strcmp(topic, MQTT_LIGHT_GLOBAL_TOPIC)) {
    if (!strcmp(mqttbuffer, SUNRISE)) {
        is_sunrise = true;
        #if defined(CH1_RGB) || defined(CH1_RGBW)
        effect_CH1 = SingleChange;
        lampe_CH1.r_next = SUNRISE_RED;
        lampe_CH1.g_next = SUNRISE_GREEN;
        lampe_CH1.b_next = SUNRISE_BLUE;
        
        lampe_CH1.r_standby = START_RED;
        lampe_CH1.g_standby = START_GREEN;
        lampe_CH1.b_standby = START_BLUE;
        lampe_CH1.brightness_standby = START_WHITE;
        
        lampe_CH1.brightness_next = 4095;
        lampe_CH1.brightness = 4095;
        lampe_CH1.state = false;
        lampe_CH1.change = false;
        lampe_CH1.brightness_change = false;
        #endif
        #ifdef CH2_RGB
        effect_CH2 = SingleChange;
        lampe_CH2.r_next = SUNRISE_RED;
        lampe_CH2.g_next = SUNRISE_GREEN;
        lampe_CH2.b_next = SUNRISE_BLUE;

        lampe_CH2.r_standby = START_RED;
        lampe_CH2.g_standby = START_GREEN;
        lampe_CH2.b_standby = START_BLUE;
        lampe_CH2.brightness_standby = START_WHITE;

        lampe_CH2.brightness_next = 4095;
        lampe_CH2.brightness = 4095;
        lampe_CH2.state = false;
        lampe_CH2.change = false;
        lampe_CH2.brightness_change = false;
        #endif
        #ifdef CH1_SINGLE
        lampe_CH1.state = false;
        lampe_CH1.change = false;  
        lampe_CH1.brightness_next = 4095;
        lampe_CH1.brightness_standby = START_WHITE;
        lampe_CH1.brightness = 0;
        #endif
        #ifdef CH2_SINGLE  
        lampe_CH2.state = false;
        lampe_CH2.change = false;  
        lampe_CH2.brightness_next = 4095;
        lampe_CH2.brightness_standby = START_WHITE;
        lampe_CH2.brightness = 0;
        #endif
        #ifdef CH3_SINGLE  
        lampe_CH3.state = false;
        lampe_CH3.change = false; 
        lampe_CH3.brightness_next = 4095;
        lampe_CH2.brightness_standby = START_WHITE;
        lampe_CH3.brightness = 0;
        #endif
        #ifdef CH4_SINGLE
        lampe_CH4.state = false;
        lampe_CH4.change = false; 
        lampe_CH4.brightness_next = 4095;
        lampe_CH2.brightness_standby = START_WHITE;
        lampe_CH4.brightness = 0;
        #endif
        initSunrise();
        //publishLightState();
        //publishLIGHTBrightness();
        //publishRGBColor();
        //publishEffect();
    }
    if (!strcmp(mqttbuffer, "countdown")) {
        #if defined(CH1_RGB) || defined(CH1_RGBW)
        effect_CH1 = SingleChange;
        lampe_CH1.r_next = 0;
        lampe_CH1.g_next = 0;
        lampe_CH1.b_next = 0;
        
        lampe_CH1.r_standby = lampe_CH1.r;
        lampe_CH1.g_standby = lampe_CH1.g;
        lampe_CH1.b_standby = lampe_CH1.b;
        lampe_CH1.brightness_standby = lampe_CH1.brightness;
        
        lampe_CH1.brightness_next = 0;
        lampe_CH1.brightness = 4095;
        lampe_CH1.state = false;
        lampe_CH1.change = true;
        lampe_CH1.brightness_change = true;
        #endif
        #ifdef CH2_RGB
        effect_CH2 = SingleChange;
        lampe_CH2.r_next = 0;
        lampe_CH2.g_next = 0;
        lampe_CH2.b_next = 0;
        
        lampe_CH2.r_standby = lampe_CH2.r;
        lampe_CH2.g_standby = lampe_CH2.g;
        lampe_CH2.b_standby = lampe_CH2.b;
        lampe_CH2.brightness_standby = lampe_CH2.brightness;
        
        lampe_CH2.brightness_next = 0;
        lampe_CH2.brightness = 4095;
        lampe_CH2.state = false;
        lampe_CH2.change = true;
        lampe_CH2.brightness_change = true;
        #endif
        #ifdef CH1_SINGLE
        lampe_CH1.state = false;
        lampe_CH1.change = true;  
        lampe_CH1.brightness_next = 0;
        lampe_CH1.brightness_standby = lampe_CH1.brightness;
        lampe_CH1.brightness = 0;
        #endif
        #ifdef CH2_SINGLE  
        lampe_CH2.state = false;
        lampe_CH2.change = true;  
        lampe_CH2.brightness_next = 0;
        lampe_CH2.brightness_standby = lampe_CH2.brightness;
        lampe_CH2.brightness = 0;
        #endif
        #ifdef CH3_SINGLE  
        lampe_CH3.state = false;
        lampe_CH3.change = true; 
        lampe_CH3.brightness_next = 0;
        lampe_CH2.brightness_standby = lampe_CH3.brightness;
        lampe_CH3.brightness = 0;
        #endif
        #ifdef CH4_SINGLE
        lampe_CH4.state = false;
        lampe_CH4.change = true; 
        lampe_CH4.brightness_next = 0;
        lampe_CH2.brightness_standby = lampe_CH4.brightness;
        lampe_CH4.brightness = 0;
        #endif
        Serial.println("countdown");
        #ifdef SECTION_COUNTDOWN
        serial_master.is_Master = false;
        for(int i = 0; i < SERIAL_COUNT; i++) {
          init_countdown(&section_effects[i]);
          section_effect[i] = countdown;
        }
        #endif
        #ifdef MASTER_COUNTDOWN
        serial_master.is_Master = true;
        init_countdown(&serial_master_effects);
        effect_serial_master = countdown_master;
        #endif
        publishLightState();
        publishLIGHTBrightness();
        publishRGBColor();
        #ifdef CH1_RGBW
        publishWhiteState();
        #endif
        
    }
  }

  
  #if defined(CH1_RGB) || defined(CH1_RGBW)
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH1)) {
    // test if the payload is equal to "ON" or "OFF"
    Serial.println("SWITCHING");
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      Serial.println("Checking on");
      if (lampe_CH1.state != true) {
        Serial.println("switching on");
        effect_CH1 = SingleChange;
        strncpy(lampe_CH1.effectname, "SingleChange", sizeof(lampe_CH1.effectname));
        lampe_CH1.r_next = lampe_CH1.r_standby;
        lampe_CH1.g_next = lampe_CH1.g_standby;
        lampe_CH1.b_next = lampe_CH1.b_standby;
        lampe_CH1.state = true;
        lampe_CH1.change = true;
        lampe_CH1.lastchange = millis();
        
        lampe_CH1.brightness_next = lampe_CH1.brightness_standby;
        lampe_CH1.brightness_change = true;
        lampe_CH1.brightness_lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
        publishRGBColor();
        publishEffect();
        #ifdef CH1_RGBW
        lampe_CH1.w_next = lampe_CH1.w_standby;
        lampe_CH1.w_change = true;
        lampe_CH1.w_lastchange = millis();       
        publishWhiteState();
        #endif
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)){
       Serial.println("Checking OFF");
      if (lampe_CH1.state != false) {
        Serial.println("switching OFF");
        lampe_CH1.state = false;
        effect_CH1 = SingleChange;
        strncpy(lampe_CH1.effectname, "SingleChange", sizeof(lampe_CH1.effectname));
        lampe_CH1.r_standby = lampe_CH1.r;
        lampe_CH1.g_standby = lampe_CH1.g;
        lampe_CH1.b_standby = lampe_CH1.b;
        lampe_CH1.brightness_standby =  lampe_CH1.brightness;
        lampe_CH1.r_next = 0;
        lampe_CH1.g_next = 0;
        lampe_CH1.b_next = 0;
        lampe_CH1.brightness_next = 0;
        lampe_CH1.change = true;
        lampe_CH1.brightness_change = true;
        lampe_CH1.lastchange = millis();
        lampe_CH1.brightness_lastchange = millis();
        #ifdef CH1_RGBW
        lampe_CH1.w_standby =  lampe_CH1.w;
        lampe_CH1.w_next = 0;
        lampe_CH1.w_change = true;
        lampe_CH1.w_lastchange = millis();
        #endif
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1)) {
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH1.brightness_next = brightness;
      lampe_CH1.brightness_old = lampe_CH1.brightness;
      lampe_CH1.brightness_change = true;
      lampe_CH1.brightness_lastchange = millis();
      publishLIGHTBrightness();
    }
  } 
  #ifdef CH1_RGBW 
  else if (!strcmp(topic, MQTT_LIGHT_WHITE_COMMAND_TOPIC_CH1)) {
    uint16_t w = atoi(mqttbuffer);
    if (w < 0 || w > 4095) {
      // do nothing...
      return;
    } else {
      if(w == 16) {w = 8;}
      lampe_CH1.w_next = w;
      lampe_CH1.w_old = lampe_CH1.w;
      lampe_CH1.w_change = true;
      lampe_CH1.w_lastchange = millis();
      publishWhiteState();
    }
  }
  #endif
  else if (!strcmp(topic, MQTT_LIGHT_RGB_COMMAND_TOPIC_CH1)) {
    effect_CH1 = SingleChange;
    // get the position of the first and second commas
    token_payload = strtok(mqttbuffer, ",");
    uint8_t rgb_red = atoi(token_payload);
    if (rgb_red < 0 || rgb_red > 255) {
      return;
    } else {
      lampe_CH1.r_next = rgb_red * 16;
      lampe_CH1.r_old = lampe_CH1.r;
    }
    
    token_payload = strtok(NULL, ",");
    uint8_t rgb_green = atoi(token_payload);
    if (rgb_green < 0 || rgb_green > 255) {
      return;
    } else {
      lampe_CH1.g_next = rgb_green * 16;
      lampe_CH1.g_old = lampe_CH1.g;
    }
    
    token_payload = strtok(NULL, ",");
    uint8_t rgb_blue = atoi(token_payload);
    if (rgb_blue < 0 || rgb_blue > 255) {
      return;
    } else {
      lampe_CH1.b_next = rgb_blue * 16;
      lampe_CH1.b_old = lampe_CH1.b;
    }
    lampe_CH1.change = true;
    lampe_CH1.lastchange = millis();
    publishRGBColor();
  }
  if(!strcmp(topic, MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH1)) {
    strncpy(lampe_CH1.effectname, mqttbuffer, sizeof(lampe_CH1.effectname));
    if(!strcmp(mqttbuffer, "colorchanger")) {
      Serial.println("colorchanger");
      initColorChanger(&lampe_CH1);
      effect_CH1 = ColorChanger;
    }
    if(!strcmp(mqttbuffer, "SingleChange")) {
      Serial.println("SingleChange");
      effect_CH1 = SingleChange;
    }
    if(!strcmp(mqttbuffer, "rainbow")) {
      Serial.println("RAINBOW");
      initRainbowFade(&CH1_effects);
      effect_CH1 = RainbowFade;
    }
    publishEffect();
  }
#endif

  #ifdef CH2_RGB
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH1)) {
    // test if the payload is equal to "ON" or "OFF"
    Serial.println("SWITCHING");
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      Serial.println("Checking on");
      if (lampe_CH2.state != true) {
        Serial.println("switching on");
        effect_CH1 = SingleChange;
        strncpy(lampe_CH2.effectname, "SingleChange", sizeof(lampe_CH2.effectname));
        lampe_CH2.r_next = lampe_CH2.r_standby;
        lampe_CH2.g_next = lampe_CH2.g_standby;
        lampe_CH2.b_next = lampe_CH2.b_standby;
        lampe_CH2.brightness_next = lampe_CH2.brightness_standby;
        lampe_CH2.state = true;
        lampe_CH2.change = true;
        lampe_CH2.brightness_change = true;
        lampe_CH2.lastchange = millis();
        lampe_CH2.brightness_lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
        publishRGBColor();
        publishEffect();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)){
       Serial.println("Checking OFF");
      if (lampe_CH2.state != false) {
        Serial.println("switching OFF");
        lampe_CH2.state = false;
        effect_CH1 = SingleChange;
        strncpy(lampe_CH2.effectname, "SingleChange", sizeof(lampe_CH2.effectname));
        lampe_CH2.r_standby = lampe_CH2.r;
        lampe_CH2.g_standby = lampe_CH2.g;
        lampe_CH2.b_standby = lampe_CH2.b;
        lampe_CH2.brightness_standby =  lampe_CH2.brightness;
        lampe_CH2.r_next = 0;
        lampe_CH2.g_next = 0;
        lampe_CH2.b_next = 0;
        lampe_CH2.brightness_next = 0;
        lampe_CH2.change = true;
        lampe_CH2.brightness_change = true;
        lampe_CH2.lastchange = millis();
        lampe_CH2.brightness_lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2)) {
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH2.brightness_next = brightness;
      lampe_CH2.brightness_old = lampe_CH2.brightness;
      lampe_CH2.brightness_change = true;
      lampe_CH2.brightness_lastchange = millis();
      publishLIGHTBrightness();
    }
  } else if (!strcmp(topic, MQTT_LIGHT_RGB_COMMAND_TOPIC_CH2)) {
    effect_CH1 = SingleChange;
    // get the position of the first and second commas
    token_payload = strtok(mqttbuffer, ",");
    uint8_t rgb_red = atoi(token_payload);
    if (rgb_red < 0 || rgb_red > 255) {
      return;
    } else {
      lampe_CH2.r_next = rgb_red * 16;
      lampe_CH2.r_old = lampe_CH2.r;
    }
    
    token_payload = strtok(NULL, ",");
    uint8_t rgb_green = atoi(token_payload);
    if (rgb_green < 0 || rgb_green > 255) {
      return;
    } else {
      lampe_CH2.g_next = rgb_green * 16;
      lampe_CH2.g_old = lampe_CH2.g;
    }
    
    token_payload = strtok(NULL, ",");
    uint8_t rgb_blue = atoi(token_payload);
    if (rgb_blue < 0 || rgb_blue > 255) {
      return;
    } else {
      lampe_CH2.b_next = rgb_blue * 16;
      lampe_CH2.b_old = lampe_CH2.b;
    }
    lampe_CH2.change = true;
    lampe_CH2.lastchange = millis();
    publishRGBColor();
  }
  if(!strcmp(topic, MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH2)) {
    strncpy(lampe_CH2.effectname, mqttbuffer, sizeof(lampe_CH2.effectname));
    if(!strcmp(mqttbuffer, "colorchanger")) {
      Serial.println("colorchanger");
      initColorChanger(&lampe_CH2);
      effect_CH1 = ColorChanger;
    }
    if(!strcmp(mqttbuffer, "SingleChange")) {
      Serial.println("SingleChange");
      effect_CH1 = SingleChange;
    }
    if(!strcmp(mqttbuffer, "rainbow")) {
      Serial.println("RAINBOW");
      initRainbowFade(&CH1_effects);
      effect_CH1 = RainbowFade;
    }
    publishEffect();
  }
#endif

#ifdef CH1_SINGLE
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH1)) {
    // test if the payload is equal to "ON" or "OFF"
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      if (lampe_CH1.state != true) {
        Serial.println("switching CH1 on");
        lampe_CH1.brightness_next = lampe_CH1.brightness_standby;
        lampe_CH1.state = true;
        lampe_CH1.change = true;
        lampe_CH1.lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)) {
      if (lampe_CH1.state != false) {
        lampe_CH1.state = false;
        lampe_CH1.brightness_standby = lampe_CH1.brightness;
        lampe_CH1.brightness_next = 0;
        lampe_CH1.change = true;
        lampe_CH1.lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1)) {
      if (!strcmp(mqttbuffer, BRIGHTNESS_INCREASE)) {
        lampe_CH1.brightness_next = lampe_CH1.brightness_next + (4095 / 5);
        if(lampe_CH1.brightness_next > 3800) {
          lampe_CH1.brightness_next = 4095;
        }
        lampe_CH1.brightness_old = lampe_CH1.brightness;
        lampe_CH1.change = true;
        lampe_CH1.lastchange = millis();
        publishLIGHTBrightness();
        return;       
      } 
      else if (!strcmp(mqttbuffer, BRIGHTNESS_DECREASE)) {
        lampe_CH1.brightness_next = lampe_CH1.brightness_next - (4095 / 5);
        if(lampe_CH1.brightness_next <0 ) {
          lampe_CH1.brightness_next = 0;
        }
        lampe_CH1.brightness_old = lampe_CH1.brightness;
        lampe_CH1.change = true;
        lampe_CH1.lastchange = millis();
        publishLIGHTBrightness();
        return;  
      }  
      uint16_t brightness = atoi(mqttbuffer);
      if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH1.brightness_next = brightness;
      lampe_CH1.brightness_old = lampe_CH1.brightness;
      lampe_CH1.change = true;
      lampe_CH1.lastchange = millis();
      publishLIGHTBrightness();
    }
  }
#endif

#ifdef CH2_SINGLE
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH2)) {
    // test if the payload is equal to "ON" or "OFF"
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      if (lampe_CH2.state != true) {
        Serial.println("switching CH1 on");
        lampe_CH2.brightness_next = lampe_CH2.brightness_standby;
        lampe_CH2.state = true;
        lampe_CH2.change = true;
        lampe_CH2.lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)) {
      if (lampe_CH2.state != false) {
        lampe_CH2.state = false;
        lampe_CH2.brightness_standby = lampe_CH2.brightness;
        lampe_CH2.brightness_next = 0;
        lampe_CH2.change = true;
        lampe_CH2.lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2)) {  
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH2.brightness_next = brightness;
      lampe_CH2.brightness_old = lampe_CH2.brightness;
      lampe_CH2.change = true;
      lampe_CH2.lastchange = millis();
      publishLIGHTBrightness();
    }
  }
#endif

#ifdef CH3_SINGLE
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH3)) {
    // test if the payload is equal to "ON" or "OFF"
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      if (lampe_CH3.state != true) {
        Serial.println(lampe_CH3.brightness_old);
        lampe_CH3.brightness_next = lampe_CH3.brightness_standby;
        lampe_CH3.state = true;
        lampe_CH3.change = true;
        lampe_CH3.lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)) {
      if (lampe_CH3.state != false) {
        lampe_CH3.state = false;
        lampe_CH3.brightness_standby = lampe_CH3.brightness;
        lampe_CH3.brightness_next = 0;
        lampe_CH3.change = true;
        lampe_CH3.lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH3)) { 
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH3.brightness_next = brightness;
      lampe_CH3.brightness_old = lampe_CH3.brightness;
      lampe_CH3.change = true;
      lampe_CH3.lastchange = millis();
      publishLIGHTBrightness();
    }
  }
#endif

#ifdef CH4_SINGLE
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH4)) {
    // test if the payload is equal to "ON" or "OFF"
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      if (lampe_CH4.state != true) {
        Serial.println(lampe_CH4.brightness_old);
        lampe_CH4.brightness_next = lampe_CH4.brightness_standby;
        lampe_CH4.state = true;
        lampe_CH4.change = true;
        lampe_CH4.lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)) {
      if (lampe_CH4.state != false) {
        lampe_CH4.state = false;
        lampe_CH4.brightness_standby = lampe_CH4.brightness;
        lampe_CH4.brightness_next = 0;
        lampe_CH4.change = true;
        lampe_CH4.lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH4)) {
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH4.brightness_next = brightness;
      lampe_CH4.brightness_old = lampe_CH4.brightness;
      lampe_CH4.change = true;
      lampe_CH4.lastchange = millis();
      publishLIGHTBrightness();
    }
  }
#endif

#ifdef CH5_SINGLE
  if (!strcmp(topic, MQTT_LIGHT_COMMAND_TOPIC_CH5)) {
    // test if the payload is equal to "ON" or "OFF"
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      if (lampe_CH5.state != true) {
        Serial.println(lampe_CH5.brightness_old);
        lampe_CH5.brightness_next = lampe_CH5.brightness_standby;
        lampe_CH5.state = true;
        lampe_CH5.change = true;
        lampe_CH5.lastchange = millis();
        publishLightState();
        publishLIGHTBrightness();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)) {
      if (lampe_CH5.state != false) {
        lampe_CH5.state = false;
        lampe_CH5.brightness_standby = lampe_CH5.brightness;
        lampe_CH5.brightness_next = 0;
        lampe_CH5.change = true;
        lampe_CH5.lastchange = millis();
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH5)) {
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 4095) {
      // do nothing...
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      lampe_CH5.brightness_next = brightness;
      lampe_CH5.brightness_old = lampe_CH5.brightness;
      lampe_CH5.change = true;
      lampe_CH5.lastchange = millis();
      publishLIGHTBrightness();
    }
  }
#endif


  #ifdef SERIAL_LED
  if (!strcmp(topic, MQTT_LIGHT_SERIAL_COMMAND_TOPIC)) {
    
    // test if the payload is equal to "ON" or "OFF"
    Serial.println("SWITCHING");
    if (!strcmp(mqttbuffer, LIGHT_ON)) {
      Serial.println("Checking on");
      if (serial_master.lampe_master.lampe.state != true) {
        Serial.println("switching on");
        effect_serial_master = Master_SingleChange;
        serial_master.is_Master = true;
        strncpy(serial_master.lampe_master.lampe.effectname, "SingleChange", sizeof(serial_master.lampe_master.lampe.effectname));
        serial_master.lampe_master.lampe.r_next = serial_master.lampe_master.lampe.r_standby;
        serial_master.lampe_master.lampe.g_next = serial_master.lampe_master.lampe.g_standby;
        serial_master.lampe_master.lampe.b_next = serial_master.lampe_master.lampe.b_standby;
        serial_master.lampe_master.lampe.brightness_next = serial_master.lampe_master.lampe.brightness_standby;
        serial_master.lampe_master.lampe.state = true;
        serial_master.lampe_master.lampe.change = true;
        serial_master.lampe_master.lampe.brightness_change = true;
        serial_master.lampe_master.lampe.lastchange = millis();
        serial_master.lampe_master.lampe.brightness_lastchange = millis();
        long sync_millis = millis();
        for(int i = 0; i < SERIAL_COUNT; i++) {
          serial_master.section[i].lampe.r_next = serial_master.section[i].lampe.r_standby;
          serial_master.section[i].lampe.g_next = serial_master.section[i].lampe.g_standby;
          serial_master.section[i].lampe.b_next = serial_master.section[i].lampe.b_standby;
          serial_master.section[i].lampe.lastchange = sync_millis;
          serial_master.section[i].lampe.change = true;

          serial_master.section[i].lampe.brightness_next = serial_master.section[i].lampe.brightness_standby;
          serial_master.section[i].lampe.state = true;
          serial_master.section[i].lampe.brightness_change = true;;
          serial_master.section[i].lampe.brightness_lastchange = sync_millis;
        }
        publishLightState();
        publishLIGHTBrightness();
        publishRGBColor();
        publishEffect();
      }
    } else if (!strcmp(mqttbuffer, LIGHT_OFF)){
       Serial.println("Checking OFF");
      if (serial_master.lampe_master.lampe.state != false) {
        Serial.println("switching OFF");
        serial_master.lampe_master.lampe.state = false;
        //effect_serial_master = Master_SingleChange;
        //serial_master.is_Master = true;
        strncpy(serial_master.lampe_master.lampe.effectname, "SingleChange", sizeof(serial_master.lampe_master.lampe.effectname));
        
        serial_master.lampe_master.lampe.r_standby = serial_master.lampe_master.lampe.r;
        serial_master.lampe_master.lampe.g_standby = serial_master.lampe_master.lampe.g;
        serial_master.lampe_master.lampe.b_standby = serial_master.lampe_master.lampe.b;
        serial_master.lampe_master.lampe.brightness_standby =  serial_master.lampe_master.lampe.brightness;
        serial_master.lampe_master.lampe.r_next = 0;
        serial_master.lampe_master.lampe.g_next = 0;
        serial_master.lampe_master.lampe.b_next = 0;
        serial_master.lampe_master.lampe.brightness_next = 0;
        serial_master.lampe_master.lampe.change = true;
        serial_master.lampe_master.lampe.brightness_change = true;
        serial_master.lampe_master.lampe.lastchange = millis();
        serial_master.lampe_master.lampe.brightness_lastchange = millis();
        

        long sync_millis = millis();
        for(int i = 0; i < SERIAL_COUNT; i++) {
          serial_master.section[i].lampe.r_standby = serial_master.section[i].lampe.r;
          serial_master.section[i].lampe.g_standby = serial_master.section[i].lampe.g;
          serial_master.section[i].lampe.b_standby = serial_master.section[i].lampe.b;
          serial_master.section[i].lampe.r_next = 0;
          serial_master.section[i].lampe.g_next = 0;
          serial_master.section[i].lampe.b_next = 0;
          serial_master.section[i].lampe.lastchange = sync_millis;
          serial_master.section[i].lampe.change = true;
          serial_master.section[i].lampe.state = false;

          serial_master.section[i].lampe.brightness_standby = serial_master.section[i].lampe.brightness;
          serial_master.section[i].lampe.brightness_next = 0;
          serial_master.section[i].lampe.state = serial_master.section[i].lampe.state;
          serial_master.section[i].lampe.brightness_change = true;
          serial_master.section[i].lampe.brightness_lastchange = sync_millis;
        }
        publishLightState();
      }
    }
  } else if (!strcmp(topic, MQTT_LIGHT_SERIAL_BRIGHTNESS_COMMAND_TOPIC)) {
    uint16_t brightness = atoi(mqttbuffer);
    if (brightness < 0 || brightness > 255) {
      // do nothing..
      return;
    } else {
      if(brightness == 16) {brightness = 8;}
      serial_master.lampe_master.lampe.brightness_next = brightness;
      serial_master.lampe_master.lampe.brightness_old = serial_master.lampe_master.lampe.brightness;
      serial_master.lampe_master.lampe.brightness_change = true;
      serial_master.lampe_master.lampe.brightness_lastchange = millis();
      long sync_millis = millis();
    
      for(int i = 0; i < SERIAL_COUNT; i++) {
        serial_master.section[i].lampe.brightness_next = brightness;
        serial_master.section[i].lampe.brightness_old = serial_master.section[i].lampe.brightness;
        serial_master.section[i].lampe.brightness_lastchange = sync_millis;
        serial_master.section[i].lampe.brightness_change = true;
      }
      publishLIGHTBrightness();
    }
  } else if (!strcmp(topic, MQTT_LIGHT_SERIAL_RGB_COMMAND_TOPIC)) {
    effect_serial_master = Master_SingleChange;
    serial_master.is_Master = true;
    // get the position of the first and second commas
    token_payload = strtok(mqttbuffer, ",");
    uint8_t rgb_red = atoi(token_payload);
    if (rgb_red < 0 || rgb_red > 255) {
      return;
    } 
    token_payload = strtok(NULL, ",");
    uint8_t rgb_green = atoi(token_payload);
    if (rgb_green < 0 || rgb_green > 255) {
      return;
    } 
    token_payload = strtok(NULL, ",");
    uint8_t rgb_blue = atoi(token_payload);
    if (rgb_blue < 0 || rgb_blue > 255) {
      return;
    } 
    serial_master.lampe_master.lampe.change = true;
    long sync_millis = millis();
    
    for(int i = 0; i < SERIAL_COUNT; i++) {
      serial_master.section[i].lampe.r_next = rgb_red;
      serial_master.section[i].lampe.r_old = serial_master.section[i].lampe.r;
      serial_master.section[i].lampe.g_next = rgb_green;
      serial_master.section[i].lampe.g_old = serial_master.section[i].lampe.g;
      serial_master.section[i].lampe.b_next = rgb_blue;
      serial_master.section[i].lampe.b_old = serial_master.section[i].lampe.b;
      serial_master.section[i].lampe.lastchange = sync_millis;
      serial_master.section[i].lampe.change = true;
    }
    publishRGBColor();
  }
  if(!strcmp(topic, MQTT_LIGHT_SERIAL_EFFECT_COMMAND_TOPIC)) {
    strncpy(serial_master.lampe_master.lampe.effectname, mqttbuffer, sizeof(serial_master.lampe_master.lampe.effectname));
    serial_master.is_Master = true;
    if(!strcmp(mqttbuffer, "master_colorchanger")) {
      Serial.println("colorchanger");
      Master_initColorChanger(&serial_master);
      effect_serial_master = Master_ColorChanger;
    }
    if(!strcmp(mqttbuffer, "master_SingleChange")) {
      Serial.println("SingleChange");
      effect_serial_master = Master_SingleChange;
    }
    if(!strcmp(mqttbuffer, "master_section_colorchanger")) {
      Serial.println("SingleChange");
      Master_initSectionColorChanger(&serial_master);
      effect_serial_master = Master_SectionColorChanger;
    }
    if(!strcmp(mqttbuffer, "master_section_control")) {
      Serial.println("Section Control");
      serial_master.is_Master = false;
      return;
    }
    if(!strcmp(mqttbuffer, "master_rainbow")) {
      Serial.println("Section Control");
      init_Master_Rainbow(&serial_master_effects);
      effect_serial_master = Master_Rainbow;
      return;
    }
    if(!strcmp(mqttbuffer, "master_flickerfire")) {
      Serial.println("master flickerfire");
      init_flickerfire(&serial_master_effects);
      effect_serial_master = flickerfire_master;
      return;
    }
    if(!strcmp(mqttbuffer, "master_flames")) {
      Serial.println("master flames");
      init_flames(&serial_master_effects);
      effect_serial_master = master_flames;
      return;
    }
    if(!strcmp(mqttbuffer, "master_section_flames")) {
      Serial.println("master section flames");
      serial_master.is_Master = false;
       for(int i = 0; i < SERIAL_COUNT; i++) {
        init_flames(&section_effects[i]);
        section_effect[i] = flames;
       }
      
      
      return;
    }
    if(!strcmp(mqttbuffer, "master_section_firework")) {
      Serial.println("master section flames");
      serial_master.is_Master = false;
       for(int i = 0; i < SERIAL_COUNT; i++) {
        init_fireworks(&section_effects[i]);
        section_effect[i] = mybomb;
       }
      return;
    }
    if(!strcmp(mqttbuffer, "firework")) {
      Serial.println("firework");
      init_fireworks(&serial_master_effects);
      effect_serial_master = mybomb_Master;
      return;
    }
      
    publishEffect();
  }
  
  if(subscribe_sections) {
    for(int i = 0; i < SERIAL_COUNT; i++) {
      if (!strcmp(topic, MQTT_LIGHT_SERIAL_SECTION_COMMAND_TOPIC[i])) {
            // test if the payload is equal to "ON" or "OFF"
            Serial.println("SWITCHING");
            if (!strcmp(mqttbuffer, LIGHT_ON)) {
                  Serial.println("Checking on");
                  if (serial_master.section[i].lampe.state != true) {
                        Serial.println("switching on");
                        section_effect[i] = SerialColorChange;
                        strncpy(serial_master.section[i].lampe.effectname, "SingleChangeSection", sizeof(serial_master.section[i].lampe.effectname));
                        serial_master.section[i].lampe.r_next = serial_master.section[i].lampe.r_standby;
                        serial_master.section[i].lampe.g_next = serial_master.section[i].lampe.g_standby;
                        serial_master.section[i].lampe.b_next = serial_master.section[i].lampe.b_standby;
                        serial_master.section[i].lampe.brightness_next = serial_master.section[i].lampe.brightness_standby;
                        serial_master.section[i].lampe.state = true;
                        serial_master.section[i].lampe.change = true;
                        serial_master.section[i].lampe.brightness_change = true;
                        serial_master.section[i].lampe.lastchange = millis();
                        serial_master.section[i].lampe.brightness_lastchange = millis();
                        publishLightState();
                        publishLIGHTBrightness();
                        publishRGBColor();
                        publishEffect();
                  }
            } else if (!strcmp(mqttbuffer, LIGHT_OFF)){
                   Serial.println("Checking OFF");
                  if (serial_master.section[i].lampe.state != false) {
                        Serial.println("switching OFF");
                        serial_master.section[i].lampe.state = false;
                        section_effect[i] = SerialColorChange;
                        strncpy(serial_master.section[i].lampe.effectname, "SingleChangeSection", sizeof(serial_master.section[i].lampe.effectname));
                        serial_master.section[i].lampe.r_standby = serial_master.section[i].lampe.r;
                        serial_master.section[i].lampe.g_standby = serial_master.section[i].lampe.g;
                        serial_master.section[i].lampe.b_standby = serial_master.section[i].lampe.b;
                        serial_master.section[i].lampe.brightness_standby =      serial_master.section[i].lampe.brightness;
                        serial_master.section[i].lampe.r_next = 0;
                        serial_master.section[i].lampe.g_next = 0;
                        serial_master.section[i].lampe.b_next = 0;
                        serial_master.section[i].lampe.brightness_next = 0;
                        serial_master.section[i].lampe.change = true;
                        serial_master.section[i].lampe.brightness_change = true;
                        serial_master.section[i].lampe.lastchange = millis();
                        serial_master.section[i].lampe.brightness_lastchange = millis();
                        publishLightState();
                  }
            }
      } else if (!strcmp(topic, MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_COMMAND_TOPIC[i])) {
            uint16_t brightness = atoi(mqttbuffer);
            if (brightness < 0 || brightness > 4095) {
                  // do nothing..
                  return;
            } else {
                  if(brightness == 16) {brightness = 8;}
                  serial_master.section[i].lampe.brightness_next = brightness;
                  serial_master.section[i].lampe.brightness_old = serial_master.section[i].lampe.brightness;
                  serial_master.section[i].lampe.brightness_change = true;
                  serial_master.section[i].lampe.brightness_lastchange = millis();
                  publishLIGHTBrightness();
            }
      } else if (!strcmp(topic, MQTT_LIGHT_SERIAL_SECTION_RGB_COMMAND_TOPIC[i])) {
            section_effect[i] = SerialColorChange;
            // get the position of the first and second commas
            token_payload = strtok(mqttbuffer, ",");
            uint8_t rgb_red = atoi(token_payload);
            if (rgb_red < 0 || rgb_red > 255) {
                  return;
            } else {
                  serial_master.section[i].lampe.r_next = rgb_red;
                  serial_master.section[i].lampe.r_old = serial_master.section[i].lampe.r;
            }
            
            token_payload = strtok(NULL, ",");
            uint8_t rgb_green = atoi(token_payload);
            if (rgb_green < 0 || rgb_green > 255) {
                  return;
            } else {
                  serial_master.section[i].lampe.g_next = rgb_green;
                  serial_master.section[i].lampe.g_old = serial_master.section[i].lampe.g;
            }
            
            token_payload = strtok(NULL, ",");
            uint8_t rgb_blue = atoi(token_payload);
            if (rgb_blue < 0 || rgb_blue > 255) {
                  return;
            } else {
                  serial_master.section[i].lampe.b_next = rgb_blue;
                  serial_master.section[i].lampe.b_old = serial_master.section[i].lampe.b;
            }
            serial_master.section[i].lampe.change = true;
            serial_master.section[i].lampe.lastchange = millis();
            publishRGBColor();
      }
      /*if(!strcmp(topic, MQTT_LIGHT_SERIAL_SECTION_EFFECT_COMMAND_TOPIC[i])) {
            strncpy(serial_master.section[i].lampe.effectname, mqttbuffer, sizeof(serial_master.section[i].lampe.effectname));
            if(!strcmp(mqttbuffer, "master_colorchanger")) {
                  Serial.println("colorchanger");
                  Master_initColorChanger(&serial_master);
                  section_effect = Master_ColorChanger;
            }
            if(!strcmp(mqttbuffer, "master_SingleChangeSection")) {
                  Serial.println("SingleChangeSection");
                  section_effect = Master_SingleChangeSection;
            }
            if(!strcmp(mqttbuffer, "master_section_colorchanger")) {
                  Serial.println("SingleChangeSection");
                  Master_initSectionColorChanger(&serial_master);
                  section_effect = Master_SectionColorChanger;
            }
            publishEffect();
      }     */
    }
  }
#endif
  
}

void subscribeChannels() {
      client.subscribe(MQTT_LIGHT_GLOBAL_TOPIC);
      #if defined(CH1_RGB) || defined(CH1_RGBW)
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH1);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1);
      client.subscribe(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH1);
      client.subscribe(MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH1);
      #ifdef CH1_RGBW
      client.subscribe(MQTT_LIGHT_WHITE_COMMAND_TOPIC_CH1);
      #endif 
      #endif
      #ifdef CH2_RGB
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH2);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2);
      client.subscribe(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH2);
      //client.subscribe(MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH2);
      #endif
      //RGB end
      //Single lights:
      #ifdef CH1_SINGLE
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH1);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1);      
      #endif
      #ifdef CH2_SINGLE
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH2);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2);      
      #endif
      #ifdef CH3_SINGLE
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH3);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH3);      
      #endif
      #ifdef CH4_SINGLE
      client.subscribe(MQTT_LIGHT_COMMAND_TOPIC_CH4);
      client.subscribe(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH4);      
      #endif
      #ifdef SERIAL_LED
      client.subscribe(MQTT_LIGHT_SERIAL_COMMAND_TOPIC);
      client.subscribe(MQTT_LIGHT_SERIAL_BRIGHTNESS_COMMAND_TOPIC);
      client.subscribe(MQTT_LIGHT_SERIAL_RGB_COMMAND_TOPIC);
      client.subscribe(MQTT_LIGHT_SERIAL_EFFECT_COMMAND_TOPIC);
      if(subscribe_sections == true) {
        for(int i = 0; i < SERIAL_COUNT; i++) {
          client.subscribe(MQTT_LIGHT_SERIAL_SECTION_COMMAND_TOPIC[i]);
          client.subscribe(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_COMMAND_TOPIC[i]);
          client.subscribe(MQTT_LIGHT_SERIAL_SECTION_RGB_COMMAND_TOPIC[i]);
          client.subscribe(MQTT_LIGHT_SERIAL_SECTION_EFFECT_COMMAND_TOPIC[i]);          
        }
      }
      #endif
}

void connect_mqtt() {
//  if(WiFi.status() != WL_CONNECTED) {
//    Serial.println("MQTT INFO: no wifi connection found");
//    Serial.print("DEBUG: try again in ");Serial.print(MQTTWAIT/1000);Serial.println(" seconds");
//    return;
//  }
  Serial.println("MQTT INFO: Attempting MQTT connection...");
  if (client.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD)) {
    Serial.println("MQTT INFO: connected");
    subscribeChannels();
    publishLightState();
    publishLIGHTBrightness();
    publishRGBColor();
    publishEffect();
    #ifdef CH1_RGBW
    publishWhiteState();
    #endif
  }
  else {
    Serial.print("ERROR: failed, rc=");
    Serial.print(client.state());
    Serial.print("DEBUG: try again in ");Serial.print(MQTT_RETRY/1000);Serial.println(" seconds");
  }
}


void initMQTTChannel() {
  snprintf(MQTT_LIGHT_GLOBAL_TOPIC, sizeof(MQTT_LIGHT_GLOBAL_TOPIC), "%s/%s", MQTT_PREFIX, MQTT_GLOBAL);
  Serial.println("Global topic:");
  Serial.println(MQTT_LIGHT_GLOBAL_TOPIC);
#if defined(CH1_RGB) || defined(CH1_RGBW)
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_BRIGHTNESS_SET);

  snprintf(MQTT_LIGHT_RGB_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_RGB_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_RGB_STATE);
  snprintf(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_RGB_SET);

  snprintf(MQTT_LIGHT_EFFECT_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_EFFECT_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_EFFECT_STATE);
  snprintf(MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_EFFECT_SET);

#ifdef CH1_RGBW
  snprintf(MQTT_LIGHT_WHITE_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_WHITE_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_WHITE_STATE);
  snprintf(MQTT_LIGHT_WHITE_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_WHITE_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_WHITE_SET);
#endif
  
  Serial.println("Lampe 1 RGB: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1);
  #ifdef CH1_RGBW
  Serial.println(MQTT_LIGHT_WHITE_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_WHITE_COMMAND_TOPIC_CH1);
  #endif
  Serial.println(MQTT_LIGHT_RGB_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_EFFECT_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_EFFECT_COMMAND_TOPIC_CH1);
  
#endif

#ifdef CH2_RGB
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH2, sizeof(MQTT_LIGHT_STATE_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH2, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_BRIGHTNESS_SET);

  snprintf(MQTT_LIGHT_RGB_STATE_TOPIC_CH2, sizeof(MQTT_LIGHT_RGB_STATE_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_RGB_STATE);
  snprintf(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH2, sizeof(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_RGB_SET);
  Serial.println("Lampe 2 RGB: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_RGB_STATE_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_RGB_COMMAND_TOPIC_CH2);
#endif

#ifdef CH1_SINGLE
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L1_NAME, MQTT_BRIGHTNESS_SET); 
  Serial.println("Lampe 1 SINGLE: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH1);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH1);
#endif

#ifdef CH2_SINGLE   
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH2, sizeof(MQTT_LIGHT_STATE_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH2, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L2_NAME, MQTT_BRIGHTNESS_SET);
  Serial.println("Lampe 2 SINGLE: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH2);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH2);
#endif

#ifdef CH3_SINGLE  
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH3, sizeof(MQTT_LIGHT_STATE_TOPIC_CH3), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L3_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH3, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH3), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L3_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH3, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH3), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L3_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH3, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH3), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L3_NAME, MQTT_BRIGHTNESS_SET);
  Serial.println("Lampe 3 SINGLE: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH3);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH3);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH3);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH3);
#endif

#ifdef CH4_SINGLE
  snprintf(MQTT_LIGHT_STATE_TOPIC_CH4, sizeof(MQTT_LIGHT_STATE_TOPIC_CH4), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L4_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_COMMAND_TOPIC_CH4, sizeof(MQTT_LIGHT_COMMAND_TOPIC_CH4), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L4_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH4, sizeof(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH4), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L4_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH4, sizeof(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH4), "%s/%s/%s%s", MQTT_PREFIX, MASTER_NAME, L4_NAME, MQTT_BRIGHTNESS_SET);
  Serial.println("Lampe 4 SINGLE: Topics:");
  Serial.println(MQTT_LIGHT_STATE_TOPIC_CH4);
  Serial.println(MQTT_LIGHT_COMMAND_TOPIC_CH4);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_STATE_TOPIC_CH4);
  Serial.println(MQTT_LIGHT_BRIGHTNESS_COMMAND_TOPIC_CH4);
#endif

#ifdef SERIAL_LED
  snprintf(MQTT_LIGHT_SERIAL_STATE_TOPIC, sizeof(MQTT_LIGHT_SERIAL_STATE_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_LIGHT_STATE);
  snprintf(MQTT_LIGHT_SERIAL_COMMAND_TOPIC, sizeof(MQTT_LIGHT_SERIAL_COMMAND_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_LIGHT_SWITCH);
  
  snprintf(MQTT_LIGHT_SERIAL_BRIGHTNESS_STATE_TOPIC, sizeof(MQTT_LIGHT_SERIAL_BRIGHTNESS_STATE_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_BRIGHTNESS_STATE);
  snprintf(MQTT_LIGHT_SERIAL_BRIGHTNESS_COMMAND_TOPIC, sizeof(MQTT_LIGHT_SERIAL_BRIGHTNESS_COMMAND_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_BRIGHTNESS_SET);

  snprintf(MQTT_LIGHT_SERIAL_RGB_STATE_TOPIC, sizeof(MQTT_LIGHT_SERIAL_RGB_STATE_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_RGB_STATE);
  snprintf(MQTT_LIGHT_SERIAL_RGB_COMMAND_TOPIC, sizeof(MQTT_LIGHT_SERIAL_RGB_COMMAND_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_RGB_SET);

  snprintf(MQTT_LIGHT_SERIAL_EFFECT_STATE_TOPIC, sizeof(MQTT_LIGHT_SERIAL_EFFECT_STATE_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_EFFECT_STATE);
  snprintf(MQTT_LIGHT_SERIAL_EFFECT_COMMAND_TOPIC, sizeof(MQTT_LIGHT_SERIAL_EFFECT_COMMAND_TOPIC), "%s/%s%s", MQTT_PREFIX, MASTER_NAME, MQTT_EFFECT_SET);
  
  Serial.println("Serial Master: Topics:");
  Serial.println(MQTT_LIGHT_SERIAL_STATE_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_COMMAND_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_BRIGHTNESS_STATE_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_BRIGHTNESS_COMMAND_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_RGB_STATE_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_RGB_COMMAND_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_EFFECT_STATE_TOPIC);
  Serial.println(MQTT_LIGHT_SERIAL_EFFECT_COMMAND_TOPIC);
  if(subscribe_sections == true) {
    Serial.println("Serial Section: Topics:");
    for(int i = 0; i < SERIAL_COUNT; i++) {
      snprintf(MQTT_LIGHT_SERIAL_SECTION_STATE_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_STATE_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_LIGHT_STATE);
      snprintf(MQTT_LIGHT_SERIAL_SECTION_COMMAND_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_COMMAND_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_LIGHT_SWITCH);
  
      snprintf(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_STATE_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_STATE_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_BRIGHTNESS_STATE);
      snprintf(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_COMMAND_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_COMMAND_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_BRIGHTNESS_SET);

      snprintf(MQTT_LIGHT_SERIAL_SECTION_RGB_STATE_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_RGB_STATE_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_RGB_STATE);
      snprintf(MQTT_LIGHT_SERIAL_SECTION_RGB_COMMAND_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_RGB_COMMAND_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_RGB_SET);

      snprintf(MQTT_LIGHT_SERIAL_SECTION_EFFECT_STATE_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_EFFECT_STATE_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_EFFECT_STATE);
      snprintf(MQTT_LIGHT_SERIAL_SECTION_EFFECT_COMMAND_TOPIC[i], sizeof(MQTT_LIGHT_SERIAL_SECTION_EFFECT_COMMAND_TOPIC[i]), "%s/%s/%s%i%s", MQTT_PREFIX, MASTER_NAME, SECTION_PREFIX, i, MQTT_EFFECT_SET);
      Serial.print("Section ");Serial.println(i);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_STATE_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_COMMAND_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_STATE_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_BRIGHTNESS_COMMAND_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_RGB_STATE_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_RGB_COMMAND_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_EFFECT_STATE_TOPIC[i]);
      Serial.println(MQTT_LIGHT_SERIAL_SECTION_EFFECT_COMMAND_TOPIC[i]);
      Serial.println();
      
    }
  }
#endif

}
