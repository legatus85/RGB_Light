//#include <FastLED.h>
#include <math.h>
float pi = 3.141459;
#define RGB12BIT 4095
#define RGB10BIT 1023
#define RGB8BIT 255

typedef struct  {
  uint16_t r;
  uint16_t g;
  uint16_t b;
}rgb;

typedef struct {
  uint16_t r;
  uint16_t g;
  uint16_t b;
} rgb8bit;

typedef struct {
  int hue;
  long nextframe;
  long framedelay;
  long nextposition;
}effect_rainbow;

typedef struct {
  long MinChangeTime;
  long MaxChangeTime;
} effect_colorchanger;



typedef struct {
  long framedelay;
  long nextframe;
  int r;
  int g;
  int b;
  int w;
  int stage;
  boolean state;
} effect_sunrise;

typedef struct {
  long nextframe;
  long framedelay;
  long nextposition;
  long positiondelay; 
  int position;
} effect_serialrainbow;
//fireworks

typedef struct {

  int framedelay;
  int velocity;
  int pos;
  int hue;  
} particle;

typedef struct {

  int start;
  int framedelay;
  particle particles[3];
} bomb;

typedef struct {
  int maxvelocity;
  int minvelocity;
  int maxframedelay;
  int minframedelay;
  int framedelay;
  int maxtail;
  int brake;
  int maxX;
  int maxbombs;
  int faderate;

  long picturedelay;
  long nextframe;
  int mystatus;
  bomb mybombs[10];
} effect_firework;

typedef struct {
  long start_countdown; 
  long duration; 
  long blackbefore; 
  int stage; 
  long start_counting; 
  long nextframe;
  long framedelay;
  
} effect_countdown;

typedef struct {
  int minframedelay;
  int maxframedelay;
  byte maxflicker;
  long nextframe;
} effect_fire;


#define FLAMES_FIRST_PIXEL 25
typedef struct {
  int heat[SERIAL_LEDS + FLAMES_FIRST_PIXEL];
  long bubble_heat;
  char bubbleoffset;
  int heatmax;
  int heatmin;
  int knister;
  int sparkle;
  double bubblewidth;
  char movingspeed;  //moving speed for pixels
  char coolingrate;  //coolingrate for each heatvalue
  char coolingoffset; // when does the cooling start
  char offsetred;    //offset for red 
  char offsetgreen; //for green
  char offsetblue;  //for blue  
  
  long nextframe;
  int framedelay;
} effect_flames;

//


typedef struct {
  effect_rainbow rainbow;
  effect_colorchanger colorchanger;
  effect_sunrise sunrise;
  effect_serialrainbow serialrainbow;
  effect_firework firework;
  effect_countdown countdown;
  effect_fire fire;
  effect_flames flames;
}effects;


void Hue2RGB(uint16_t hue, rgb *rgb_ptr, uint16_t resolution) {
  if(hue <= resolution) {
    rgb_ptr->r = resolution - hue;
    rgb_ptr->g = hue;
    rgb_ptr->b = 0;
  }
  if(hue > resolution && hue <=(2*resolution)) {
    rgb_ptr->r = 0;
    rgb_ptr->g = resolution - (hue - resolution);
    rgb_ptr->b = hue - resolution;
  }
  if(hue > (2*resolution) && hue <=(3*resolution)) {
    rgb_ptr->r = hue -  (2*resolution);
    rgb_ptr->g = 0;
    rgb_ptr->b = resolution - (hue - (2*resolution));
  }
  
}

void fadetoBlack(rgb8bit *vector, int decrease) {
  //The intensity is described by the value of the 3D-vector from r,g,b. The value is then normalized to 255 and decreased. 
  float value_max = 441.6729559;
  //calculate value of vector
  int s_red = vector->r * vector->r;
  //Serial.println(s_red);
  int s_green = vector->g * vector->g;
  int s_blue = vector->b * vector->b;
  float value = sqrt(s_red+s_green+s_blue);
  if(value < 1) {return;}
  //Serial.println(value);
  //calculate the unit vector
  float u_r = vector->r / value;
  //Serial.println(u_r);
  float u_g = vector->g / value;
  float u_b = vector->b / value;
  //calculate the new value of the faded vector
  float f_decrease = float(decrease) / 65535;
  //Serial.println(f_decrease);
  float new_value = value -(f_decrease*value_max);
  //Serial.println(new_value);
  if(new_value < 1) {new_value = 0;}
  
  vector->r = round(new_value * u_r);
  vector->g = round(new_value * u_g);
  vector->b = round(new_value * u_b);
  return;
}

void fadeSINGLE(lampeSINGLE *lampe_ptr) {                               //See colors
  if(millis() < lampe_ptr->nextframe) {return;}
  lampe_ptr->nextframe = millis() + 24;
  if(millis() - lampe_ptr->lastchange < changetime) {
      float diff;
      diff =  lampe_ptr->brightness_next - lampe_ptr->brightness_old;
      diff = (diff/changetime);
      diff = round(diff * (millis() - lampe_ptr->lastchange));
      lampe_ptr->brightness = lampe_ptr->brightness_old + diff; 

    }
    if(millis() - lampe_ptr->lastchange > changetime && lampe_ptr->change) {
      lampe_ptr->brightness = lampe_ptr->brightness_next;      
      lampe_ptr->brightness_old = lampe_ptr->brightness_next;
      lampe_ptr->change = false;
      
    }
}

void fadeRGBBrightness(lampeRGB *lampe_ptr) {                               //See colors
  if(millis() < lampe_ptr->brightness_nextframe) {return;}
  lampe_ptr->brightness_nextframe = millis() + 24;
  if(millis() - lampe_ptr->brightness_lastchange < changetime) {
      float diff;
      diff =  lampe_ptr->brightness_next - lampe_ptr->brightness_old;
      diff = (diff/changetime);
      diff = round(diff * (millis() - lampe_ptr->brightness_lastchange));
      lampe_ptr->brightness = lampe_ptr->brightness_old + diff; 

    }
    if(millis() - lampe_ptr->brightness_lastchange > changetime && lampe_ptr->brightness_change) {
      lampe_ptr->brightness = lampe_ptr->brightness_next;      
      lampe_ptr->brightness_old = lampe_ptr->brightness_next;
      lampe_ptr->brightness_change = false;
      
    }
}

#ifdef CH1_RGBW
void fadeWhite(lampeRGB *lampe_ptr) {                               //See colors
   if(millis() < lampe_ptr->w_nextframe) {return;}
  lampe_ptr->w_nextframe = millis() + 24;
  if(millis() - lampe_ptr->w_lastchange < changetime) {
      float diff;
      diff =  lampe_ptr->w_next - lampe_ptr->w_old;
      diff = (diff/changetime);
      diff = round(diff * (millis() - lampe_ptr->w_lastchange));
      lampe_ptr->w = lampe_ptr->w_old + diff; 

    }
    if(millis() - lampe_ptr->w_lastchange > changetime && lampe_ptr->w_change) {
      lampe_ptr->w = lampe_ptr->w_next;      
      lampe_ptr->w_old = lampe_ptr->w_next;
      lampe_ptr->w_change = false;
      
    }
}
#endif

void fadeRGB(lampeRGB *lampe_ptr) {
  if(millis() < lampe_ptr->nextframe) {return;}  //limit framerate 
  lampe_ptr->nextframe = millis() + 24;          //Calculate when the next frame will be processed
  if(millis() - lampe_ptr->lastchange < changetime) {  //is the fading still in process?
      float diff;                                       //difference between the two values
      //colors
      diff =  lampe_ptr->r_next - lampe_ptr->r_old;     //calculate the difference between the two values (here red)
      diff = (diff/changetime);                         //calculate the difference per millisecond
      diff = round(diff * (millis() - lampe_ptr->lastchange));  //calculate the distance which was already faded 
      lampe_ptr->r = lampe_ptr->r_old + diff;           //add it to the old value 

      diff =  lampe_ptr->g_next - lampe_ptr->g_old;     
      diff = (diff/changetime);
      diff = round(diff * (millis() - lampe_ptr->lastchange));
      lampe_ptr->g = lampe_ptr->g_old + diff; 

      diff =  lampe_ptr->b_next - lampe_ptr->b_old;
      diff = (diff/changetime);
      diff = round(diff * (millis() - lampe_ptr->lastchange));     
      lampe_ptr->b = lampe_ptr->b_old + diff; 

    }
    if(millis() - lampe_ptr->lastchange > changetime && lampe_ptr->change) { //fading complete when fading time is run out, store alle values correctly
      lampe_ptr->r = lampe_ptr->r_next;
      lampe_ptr->g = lampe_ptr->g_next;
      lampe_ptr->b = lampe_ptr->b_next;
      
      lampe_ptr->r_old = lampe_ptr->r_next;
      lampe_ptr->g_old = lampe_ptr->g_next;
      lampe_ptr->b_old = lampe_ptr->b_next;

      lampe_ptr->change = false;                                            //tell the program, that no more fading has to be calculated
    }
    
}

boolean  SingleChange(lampeRGB *lampe_ptr, effects *effects_ptr) {
  if(lampe_ptr->change) {
    fadeRGB(lampe_ptr);
    return true; //Something has chaged --> PWM will be updated
  }
  else {
    return false;
  }
}

long MinChangeTime = 300000;//480000;
long MaxChangeTime = 420000;//720000;
void initColorChanger(lampeRGB *lampe_ptr) {
    int hue;
    rgb conv;
    randomSeed(esp_random());
    hue = random(3*RGB12BIT);
    Hue2RGB(hue, &conv, RGB12BIT);
    //hue = random(255);
    //CHSV hsv(hue, 255, 128);
    //CRGB rgb;
    //hsv2rgb_spectrum(hsv, rgb);
    //rgb.maximizeBrightness();
    lampe_ptr->r_next = conv.r;//rgb[0] * 16;
    lampe_ptr->g_next = conv.g;//rgb[1] * 16;
    lampe_ptr->b_next = conv.b;//rgb[2] * 16;
    lampe_ptr->lastchange = millis();
    lampe_ptr->change = true;
    lampe_ptr->nextchange = millis() + random(MinChangeTime, MaxChangeTime)+changetime;
}

boolean ColorChanger(lampeRGB *lampe_ptr, effects *effects_ptr) {
  //Serial.println("colorchanger");
  //Serial.println(lampe_ptr->nextchange);
  if(millis() > lampe_ptr->nextchange) {
    int hue;
    rgb conv;
    randomSeed(esp_random());
    hue = random(3*RGB12BIT);
    Hue2RGB(hue, &conv, RGB12BIT);
    //hue = random(255);
    //CHSV hsv(hue, 255, 128);
    //CRGB rgb;
    //hsv2rgb_spectrum(hsv, rgb);
    //rgb.maximizeBrightness();
    lampe_ptr->r_next = conv.r;//rgb[0] * 16;
    lampe_ptr->g_next = conv.g;//rgb[1] * 16;
    lampe_ptr->b_next = conv.b;//rgb[2] * 16;
    lampe_ptr->lastchange = millis();
    lampe_ptr->change = true;
    lampe_ptr->nextchange = millis() + random(MinChangeTime, MaxChangeTime)+changetime;
    //Serial.println(lampe_ptr->nextchange);
    return false;
  } 
  else {
    if(lampe_ptr->change) {
      fadeRGB(lampe_ptr);
        return true; //Something has chaged --> PWM will be updated
      }
    else {
      return false;
    }
  }
}

////////////////////////////////////////////////////////////////////
/* Rainbow RGB
 *  
 */

void initRainbowFade(effects *effects_ptr) {
  Serial.println("init Rainbow");
  effects_ptr->rainbow.framedelay = 80;
  effects_ptr->rainbow.hue = random(0, 3*RGB12BIT);
  effects_ptr->rainbow.nextframe = millis();
}


boolean RainbowFade(lampeRGB *lampe_ptr, effects *effects_ptr) {
  //Serial.println("rainbow");
  if(millis() < effects_ptr->rainbow.nextframe) { return false; }
  rgb conv;
  Hue2RGB(effects_ptr->rainbow.hue, &conv, RGB12BIT);
  lampe_ptr->r = conv.r;
  lampe_ptr->g = conv.g;
  lampe_ptr->b = conv.b;
  //Serial.println(lampe_ptr->r);
  //Serial.println(effects_ptr->rainbow.hue);
  effects_ptr->rainbow.hue=(++effects_ptr->rainbow.hue) % (3*RGB12BIT);
  effects_ptr->rainbow.nextframe = millis() + effects_ptr->rainbow.framedelay;
  return true;
  
  
}

//////////////////////////////////////////////////////////////////////
/*
 * SUNRISE
 */
effect_sunrise sunrise;
void initSunrise() {
  sunrise.nextframe = millis();
  sunrise.framedelay = int((SUNRISE_TIME_S*1000) / (SUNRISE_RED + SUNRISE_GREEN + SUNRISE_BLUE + SUNRISE_WHITE - (SUNRISE_GREEN / 4)));
  sunrise.stage = 4;
  sunrise.r = 0;
  sunrise.g = 0;
  sunrise.b = 0;
  sunrise.w = 0;
  sunrise.state = false;
  #ifdef CH1_RGB
        sunrise.stage = 1;
  #endif
  #ifdef CH2_RGB
        sunrise.stage = 1;
  #endif

}
boolean sunrise_fade() {
  if(millis() < sunrise.nextframe) { return false; }
  sunrise.nextframe = millis() + sunrise.framedelay;
  //Serial.println(sunrise.stage);
  switch(sunrise.stage) {
    case 1:
      sunrise.b++;
      if(sunrise.b == SUNRISE_BLUE) {
        sunrise.stage = 2;
      }
      break;
    case 2:
      sunrise.r++;
      if(sunrise.r == SUNRISE_BLUE) {
        sunrise.stage = 3;
      }
      break;
    case 3:
      sunrise.g++;
      if(sunrise.g >= SUNRISE_GREEN / 4) {
        sunrise.r++;
      }
      if(sunrise.g == SUNRISE_GREEN) {
        sunrise.stage = 4;
      }
      break;
    case 4:
      sunrise.r++;
      if(sunrise.r == SUNRISE_RED) {
        sunrise.stage = 5;
      }
      break;
    case 5:
      sunrise.w++;
      if(sunrise.w == SUNRISE_WHITE) {
        sunrise.stage = 6;
      }
      break;  
    case 6:
      sunrise.state = true;
      sunrise.stage = 7;
      break; 
    case 7:
      break;      
  }
  //Serial.print("r   ");Serial.println(sunrise.r);
  //Serial.print("g   ");Serial.println(sunrise.g);
  //Serial.print("b   ");Serial.println(sunrise.b);
  //Serial.print("w   ");Serial.println(sunrise.w);
  return true;
}

void sunrise_get_RGB(lampeRGB *lampe_ptr) {
  lampe_ptr->r_old = sunrise.r;
  lampe_ptr->g_old = sunrise.g;
  lampe_ptr->b_old = sunrise.b;
  lampe_ptr->r = sunrise.r;
  lampe_ptr->g = sunrise.g;
  lampe_ptr->b = sunrise.b;
  lampe_ptr->state = sunrise.state;
  return;
}
void sunrise_get_SINGLE(lampeSINGLE *lampe_ptr) {
  lampe_ptr->brightness_old = sunrise.w;
  lampe_ptr->brightness = sunrise.w;
  lampe_ptr->state = sunrise.state;
  return;
}

int sunrise_get_stage() {
  return sunrise.stage;
}

#ifdef SERIAL_LED
//SERIAL EFFECTS
//to run fastled.show on core 1

void setPixelRGB(int i, uint8_t r, uint8_t g, uint8_t b, uint8_t brightness, uint8_t temperature = 0) {
  float norm_brightness;
  norm_brightness = float(brightness)/255;
  r = round(r * norm_brightness);
  g = round(g * norm_brightness);
  b = round(b * norm_brightness);
  leds[i] = CRGB(r,g,b); 
}

void setPixelHSV(int i, uint8_t h, uint8_t s, uint8_t v, uint8_t brightness, uint8_t temperature = 0) {
  CRGB rgb;
  hsv2rgb_spectrum( CHSV(h,s,v), rgb);
  rgb.maximizeBrightness();
  float norm_brightness;
  norm_brightness = float(brightness)/255;
  uint8_t r = round(rgb.r * norm_brightness);
  uint8_t g = round(rgb.g * norm_brightness);
  uint8_t b = round(rgb.b * norm_brightness);
  leds[i] = CRGB(r,g,b);
}

void write_Section(serialSection *lampe_ptr) {
    int length = lampe_ptr->length;
    int start = lampe_ptr->start;
    
    for(int i = 0; i < length; i++) {
      uint8_t r = (uint8_t)(lampe_ptr->lampe.r & 0x00FF);
      uint8_t g = (uint8_t)(lampe_ptr->lampe.g & 0x00FF);
      uint8_t b = (uint8_t)(lampe_ptr->lampe.b & 0x00FF);
      //setPixelRGB(start + i, r,g,b,lampe_ptr->lampe.brightness);
      leds[start + i] = CRGB(r,g,b);
    }
    return;
}


boolean SerialColorChange(serialSection *lampe_ptr, effects *effects_ptr) { //Single changed triggerd by user for a section of the stripe
  if(lampe_ptr->lampe.change) {
    fadeRGB(&lampe_ptr->lampe);
    write_Section(lampe_ptr);  
    return true; //Something has chaged --> PWM will be updated
  }
  else {
    return false;
  }

}

///Brightness

void updateSerialBrightness(serialSection *lampe_ptr) {
  int length = lampe_ptr->length;
  int start = lampe_ptr->start;
  float norm_brightness;
  norm_brightness = float(lampe_ptr->lampe.brightness)/255;
  for(int i = 0; i < length; i++) {
    uint8_t r = leds[i+start].r;
    uint8_t g = leds[i+start].g;
    uint8_t b = leds[i+start].b;
    r = round(r*norm_brightness);
    g = round(g*norm_brightness);
    b = round(b*norm_brightness);
    //Serial.println(norm_brightness);
    //Serial.print("r");Serial.print(r);Serial.print(" ");Serial.print("g");Serial.print(g);Serial.print(" ");Serial.print("b");Serial.print(b);Serial.println(" ");
    frontbuffer[i + start] = CRGB(r,g,b);  
  }
}

void Master_fadeSerialBrightness(lampeSERIAL *lampe_ptr) {
  //Serial.print("Master: ");Serial.println(lampe_ptr->lampe_master.lampe.brightness);
  fadeRGBBrightness(&lampe_ptr->lampe_master.lampe);
  //char m_msg_buffer[32];
  //snprintf(m_msg_buffer, 32, "%d", &lampe_ptr->lampe_master.lampe.brightness);
  //Serial.print("B: ");Serial.println(m_msg_buffer);
  for(int i = 0; i < SERIAL_COUNT; i++) {
     fadeRGBBrightness(&lampe_ptr->section[i].lampe);
     Serial.print("S");Serial.print(i);
     //updateSerialBrightness(&lampe_ptr->section[i]);
     //write_Section(&lampe_ptr->section[i]);
  }
  //updateSerialBrightness(&lampe_ptr->lampe_master);
}

void Section_fadeSerialBrightness(serialSection *lampe_ptr) {
  //Serial.print("Section: ");Serial.println(lampe_ptr->lampe.brightness);
  fadeRGBBrightness(&lampe_ptr->lampe);
  //updateSerialBrightness(lampe_ptr);
}


/////////////////////////////////////////////////////////
/*Color Changer
 * Changes the color if ALL sections randomly
 */
void Serial_initColorChanger(lampeRGB *lampe_ptr) {
    int hue;
    rgb conv;
    randomSeed(esp_random());
    hue = random(3*RGB8BIT);
    Hue2RGB(hue, &conv, RGB8BIT);
    //hue = random(255);
    //CHSV hsv(hue, 255, 128);
    //CRGB rgb;
    //hsv2rgb_spectrum(hsv, rgb);
    //rgb.maximizeBrightness();
    lampe_ptr->r_next = conv.r;//rgb[0] * 16;
    lampe_ptr->g_next = conv.g;//rgb[1] * 16;
    lampe_ptr->b_next = conv.b;//rgb[2] * 16;
    lampe_ptr->lastchange = millis();
    lampe_ptr->change = true;
    lampe_ptr->nextchange = millis() + random(MinChangeTime, MaxChangeTime)+changetime;
}

boolean Serial_ColorChanger(lampeRGB *lampe_ptr, effects *effects_ptr) {
  //Serial.println("colorchanger");
  //Serial.println(lampe_ptr->nextchange);
  if(millis() > lampe_ptr->nextchange) {
    int hue;
    rgb conv;
    randomSeed(esp_random());
    hue = random(3*RGB8BIT);
    Hue2RGB(hue, &conv, RGB8BIT);
    //hue = random(255);
    //CHSV hsv(hue, 255, 128);
    //CRGB rgb;
    //hsv2rgb_spectrum(hsv, rgb);
    //rgb.maximizeBrightness();
    lampe_ptr->r_next = conv.r;//rgb[0] * 16;
    lampe_ptr->g_next = conv.g;//rgb[1] * 16;
    lampe_ptr->b_next = conv.b;//rgb[2] * 16;
    lampe_ptr->lastchange = millis();
    lampe_ptr->change = true;
    lampe_ptr->nextchange = millis() + random(MinChangeTime, MaxChangeTime)+changetime;
    //Serial.println(lampe_ptr->nextchange);
    return false;
  } 
  else {
    if(lampe_ptr->change) {
      fadeRGB(lampe_ptr);
        return true; //Something has chaged --> PWM will be updated
      }
    else {
      return false;
    }
  }
}



 
void Master_initSectionColorChanger(lampeSERIAL *lampe_ptr) { //initializes the random color change for every section
  for(int i = 0; i < SERIAL_COUNT;i++) {
    Serial_initColorChanger(&lampe_ptr->section[i].lampe);
  }
}

boolean Master_SectionColorChanger(lampeSERIAL *lampe_ptr, effects *effects_ptr) { //Sections will be changed from time to time randomly
  boolean return_val = false;
  for(int i = 0; i < SERIAL_COUNT;i++) {
    if(Serial_ColorChanger(&lampe_ptr->section[i].lampe, effects_ptr)) {
      /*
       * Hier nicht den ganzen streifen Schreiben:
       * An einzelne Sections übergeben und dann einzeln synchron faden. 
       * eventuell Master fade und dann an streifen übergeben
       * ACHTUNG: Auschalten der einzelnen Streifen lässt dann den Master eingeschaltet
       *          Sprung beim Ausschalten vermeiden!
       *          State überprfüng der sections vielleicht erforderlich?
       */
      write_Section(&lampe_ptr->section[i]);
      return_val |= true;
      
    }
    
  }
  return return_val;
}

void Master_initColorChanger(lampeSERIAL *lampe_ptr) { //initializes the random color change for the whole stripe
  Serial_initColorChanger(&lampe_ptr->lampe_master.lampe);
  for(int i = 0; i < SERIAL_COUNT; i++) {
    lampe_ptr->section[i].lampe.r_next = lampe_ptr->lampe_master.lampe.r_next;
    lampe_ptr->section[i].lampe.g_next = lampe_ptr->lampe_master.lampe.g_next;
    lampe_ptr->section[i].lampe.b_next = lampe_ptr->lampe_master.lampe.b_next;
    lampe_ptr->section[i].lampe.lastchange = lampe_ptr->lampe_master.lampe.lastchange;
    lampe_ptr->section[i].lampe.change = lampe_ptr->lampe_master.lampe.change;
  }
}
 
boolean Master_ColorChanger(lampeSERIAL *lampe_ptr, effects *effects_ptr) { //Whole stripe will be changes from time to time randomly
     //Serial.println("colorchanger");
  //Serial.println(lampe_ptr->nextchange);
  if(millis() > lampe_ptr->lampe_master.lampe.nextchange) {
    int hue;
    rgb conv;
    randomSeed(esp_random());
    hue = random(3*RGB8BIT);
    Hue2RGB(hue, &conv, RGB8BIT);
    lampe_ptr->lampe_master.lampe.r_next = conv.r;//rgb[0] * 16;
    lampe_ptr->lampe_master.lampe.g_next = conv.g;//rgb[1] * 16;
    lampe_ptr->lampe_master.lampe.b_next = conv.b;//rgb[2] * 16;
    lampe_ptr->lampe_master.lampe.lastchange = millis();
    lampe_ptr->lampe_master.lampe.change = true;
    lampe_ptr->lampe_master.lampe.nextchange = millis() + random(MinChangeTime, MaxChangeTime)+changetime;
    publishRGBColor();
    for(int i = 0; i < SERIAL_COUNT; i++) {
      lampe_ptr->section[i].lampe.r_next = lampe_ptr->lampe_master.lampe.r_next;
      lampe_ptr->section[i].lampe.g_next = lampe_ptr->lampe_master.lampe.g_next;
      lampe_ptr->section[i].lampe.b_next = lampe_ptr->lampe_master.lampe.b_next;
      lampe_ptr->section[i].lampe.lastchange = lampe_ptr->lampe_master.lampe.lastchange;
      lampe_ptr->section[i].lampe.change = lampe_ptr->lampe_master.lampe.change;
    }
    return false;
  } 
  else {
    boolean return_val = false;
    for(int i = 0; i < SERIAL_COUNT; i++) {
      return_val |= SerialColorChange(&lampe_ptr->section[i], effects_ptr);
    }
    return return_val;
  }
  
}

boolean Master_SingleChange(lampeSERIAL *lampe_ptr, effects *effects_ptr) { //Single change triggerd by user for the whole stripe
   boolean return_val;
   for(int i = 0; i < SERIAL_COUNT; i++) {
     return_val |= SerialColorChange(&lampe_ptr->section[i], effects_ptr);
   }
   return return_val;
}

void init_Master_Rainbow(effects *effects_ptr) {
  effects_ptr->serialrainbow.nextposition = millis();
  effects_ptr->serialrainbow.positiondelay = 200;
  effects_ptr->serialrainbow.framedelay = 20;
  randomSeed(esp_random());
  effects_ptr->serialrainbow.position = random(SERIAL_LEDS);
  effects_ptr->serialrainbow.nextframe = millis();
}

boolean Master_Rainbow(lampeSERIAL *lampe_ptr, effects *effects_ptr) {
  if(millis() < effects_ptr->serialrainbow.nextframe) {return false;}  //limit framerate
  for(int i = 0; i < SERIAL_LEDS; i++) {
    uint16_t hue = round((i * ((3*RGB8BIT) / (SERIAL_LEDS))));
    rgb conv;
    Hue2RGB(hue, &conv, RGB8BIT);
    int pos = i + effects_ptr->serialrainbow.position;
    pos = pos % SERIAL_LEDS;
    uint8_t r = (uint8_t)(conv.r & 0x00FF);
    uint8_t g = (uint8_t)(conv.g & 0x00FF);
    uint8_t b = (uint8_t)(conv.b & 0x00FF);
    //setPixelRGB(pos, r,g,b,lampe_ptr->lampe_master.lampe.brightness);
    leds[pos] = CRGB(r,g,b); 
  }
  effects_ptr->serialrainbow.nextframe = effects_ptr->serialrainbow.nextframe + effects_ptr->serialrainbow.framedelay;
  if(millis() > effects_ptr->serialrainbow.nextposition) {
    effects_ptr->serialrainbow.position++;
    effects_ptr->serialrainbow.position = effects_ptr->serialrainbow.position % SERIAL_LEDS;
    effects_ptr->serialrainbow.nextposition = effects_ptr->serialrainbow.nextposition + effects_ptr->serialrainbow.positiondelay;
  }
  return true;
}

/*//////////////////////////////////////////////////////////////
 * Fireworks
*/

void init_fireworks(effects *effects_ptr) {
  effects_ptr->firework.maxvelocity = 160;
  effects_ptr->firework.minvelocity = 70;
  effects_ptr->firework.maxframedelay = 70;
  effects_ptr->firework.minframedelay = 30;
  effects_ptr->firework.framedelay = 10;
  effects_ptr->firework.maxtail = 10;
  effects_ptr->firework.brake = 10;
  effects_ptr->firework.maxX = 5000;
  effects_ptr->firework.maxbombs = 4;
  effects_ptr->firework.faderate = 255;
  effects_ptr->firework.mystatus = 0;
  effects_ptr->firework.picturedelay = 40;
  effects_ptr->firework.nextframe = millis();
}

void initbomb(int ibomb, effects *effects_ptr) {
  effects_ptr->firework.mybombs[ibomb].framedelay = random(effects_ptr->firework.minframedelay, effects_ptr->firework.maxframedelay);
  effects_ptr->firework.mybombs[ibomb].start = random(0, effects_ptr->firework.maxX);
  effects_ptr->firework.mybombs[ibomb].particles[0].velocity = random(effects_ptr->firework.minvelocity, effects_ptr->firework.maxvelocity);
  effects_ptr->firework.mybombs[ibomb].particles[0].hue = random(0, 255);
  effects_ptr->firework.mybombs[ibomb].particles[0].pos = 0;
}


void bombexplode(serialSection *lampe_ptr, effects *effects_ptr) {
int length = lampe_ptr->length;
int start = lampe_ptr->start;
CHSV hsv;
//   rgb8bit fade;
//   Serial.println("testfade");
//   fade.r = leds[5+start].r;
//   Serial.println(fade.r);
//   fade.g = leds[5+start].g;
//   fade.b = leds[5+start].b;
//   fadetoBlack(&fade, effects_ptr->firework.faderate);
//   leds[5+start].r = fade.r;
//   Serial.println(fade.r);
//   leds[5+start].g = fade.g;
//   leds[5+start].b = fade.b;
for(int p = 0; p < length; p++) {
   rgb8bit fade;
   fade.r = leds[p+start].r;
   fade.g = leds[p+start].g;
   fade.b = leds[p+start].b;
   fadetoBlack(&fade, effects_ptr->firework.faderate);
   leds[p+start].r = fade.r;
   leds[p+start].g = fade.g;
   leds[p+start].b = fade.b;
   //leds[p+start].fadeToBlackBy(effects_ptr->firework.faderate);
}

for (int i = 0; i < effects_ptr->firework.maxbombs; i++) {

  

  if(effects_ptr->firework.mybombs[i].framedelay < 0) {

    effects_ptr->firework.mybombs[i].particles[0].pos =  effects_ptr->firework.mybombs[i].particles[0].pos +  effects_ptr->firework.mybombs[i].particles[0].velocity; //doing the necessary physics: moving the particle
    effects_ptr->firework.mybombs[i].particles[0].velocity =  effects_ptr->firework.mybombs[i].particles[0].velocity - effects_ptr->firework.brake;                   //braking the particle down

    int pos_pos = effects_ptr->firework.mybombs[i].start + effects_ptr->firework.mybombs[i].particles[0].pos;
    int neg_pos = effects_ptr->firework.mybombs[i].start - effects_ptr->firework.mybombs[i].particles[0].pos;
//    boolean isnegative = 0;
//    if(neg_pos < 0) {
//      neg_pos *= -1;
//      isnegative = 1;
//    }
    int map_pos_pos = map(pos_pos, 0, effects_ptr->firework.maxX, 0, length - 1); //111
    int map_neg_pos = map(neg_pos, 0, effects_ptr->firework.maxX, 0, length - 1); //111
    
    if(map_pos_pos > length - 1) {map_pos_pos = map_pos_pos - (length - 1);}
      hsv = CHSV(effects_ptr->firework.mybombs[i].particles[0].hue, 255, 128);
      leds[start + map_pos_pos] = CHSV(effects_ptr->firework.mybombs[i].particles[0].hue, 255, 128);
      //setPixelHSV(start + map_pos_pos, hsv.h, hsv.s, hsv.v, lampe_ptr->lampe.brightness);

    if(map_neg_pos < 0) {map_neg_pos = length - 1 + map_neg_pos;}
      hsv = CHSV(effects_ptr->firework.mybombs[i].particles[0].hue, 255, 128);
      leds[start + map_neg_pos] = CHSV(effects_ptr->firework.mybombs[i].particles[0].hue, 255, 128);
      //setPixelHSV(start + map_neg_pos, hsv.h, hsv.s, hsv.v, lampe_ptr->lampe.brightness);

    if(effects_ptr->firework.mybombs[i].particles[0].velocity <=0) {
      initbomb(i, effects_ptr);
    }
  }


  if(effects_ptr->firework.mybombs[i].framedelay == 0) {
    int start_pos = map(effects_ptr->firework.mybombs[i].start, 0, effects_ptr->firework.maxX, 0, lampe_ptr->length - 1); 
    //setPixelRGB(start_pos + lampe_ptr->start, 128,128,128, lampe_ptr->lampe.brightness);
    leds[start_pos + lampe_ptr->start] = CRGB(128,128,128);
    effects_ptr->firework.mybombs[i].framedelay = -1;
  }
  if(effects_ptr->firework.mybombs[i].framedelay > 0) {
    effects_ptr->firework.mybombs[i].framedelay--;
  }
}
}

void initbombs(serialSection *lampe_ptr, effects *effects_ptr) {
  
  int length = lampe_ptr->length;
  int start = lampe_ptr->start;
  Serial.println(length);
  for(int i = 0; i < length; i++) {
    //setPixelRGB(i+start, 0,0,0, lampe_ptr->lampe.brightness);
    leds[i+start] = CRGB::Black;
  }
  for(int i = 0; i < effects_ptr->firework.maxbombs; i++) {
  initbomb(i, effects_ptr);
  effects_ptr->firework.mybombs[i].framedelay = effects_ptr->firework.framedelay * i + 1;
  effects_ptr->firework.mystatus = 1;
  }
}



boolean mybomb(serialSection *lampe_ptr, effects *effects_ptr) { //Master effect
  if(millis() < effects_ptr->firework.nextframe) {return false;}
  switch (effects_ptr->firework.mystatus) {
    case 0:
      initbombs(lampe_ptr, effects_ptr);
      effects_ptr->firework.mystatus = 1;
      break;
    case 1:
      bombexplode(lampe_ptr, effects_ptr);
      break;
  }
  effects_ptr->firework.nextframe = millis() + effects_ptr->firework.picturedelay;
  return true;
}
boolean mybomb_Master(lampeSERIAL *lampe_ptr, effects *effects_ptr) {
  return mybomb(&lampe_ptr->lampe_master, effects_ptr);
}

//Countdown für Silvester
void init_countdown(effects *effects_ptr) {
  effects_ptr->countdown.start_countdown = millis(); //
  effects_ptr->countdown.duration = 60000; //
  effects_ptr->countdown.blackbefore = 1000; //
  effects_ptr->countdown.stage = 0; //
  effects_ptr->countdown.start_counting = 10000; //
  effects_ptr->countdown.nextframe = millis();
  effects_ptr->countdown.framedelay = 24;
}

boolean countdown(serialSection *lampe_ptr, effects *effects_ptr) {
  if(effects_ptr->countdown.stage == 3) {
    return mybomb(lampe_ptr, effects_ptr);
  }
  if(millis() < effects_ptr->countdown.nextframe) {return false;}
  long nowmillis = millis();
  float left;
  long leds_left;
  switch (effects_ptr->countdown.stage) {
    case 0:
      for(int i = 0; i < lampe_ptr->length; i++) {
        CRGB rgb = CRGB::Gold;
        //setPixelRGB(lampe_ptr->start + i, rgb.r, rgb.g, rgb.b, lampe_ptr->lampe.brightness);
        leds[lampe_ptr->start + i] = CRGB::Gold;
      }
      effects_ptr->countdown.nextframe = (effects_ptr->countdown.start_countdown + effects_ptr->countdown.duration) - effects_ptr->countdown.start_counting;
      effects_ptr->countdown.stage = 1;
      return true;
      break;
    case 1:
      left = (effects_ptr->countdown.start_countdown + effects_ptr->countdown.duration-effects_ptr->countdown.blackbefore) - nowmillis;
      left = left / (effects_ptr->countdown.start_counting-effects_ptr->countdown.blackbefore);
      left *= 100;
      leds_left = lampe_ptr->length * left;
      leds_left = int(leds_left / 100);
      Serial.println(leds_left);
      if(leds_left == 0) {
        effects_ptr->countdown.stage = 2;
      }
      for(int i = 0; i < lampe_ptr->length; i++) {
        CRGB rgb = i<leds_left?CRGB::Gold:CRGB::Black;
        //setPixelRGB(i + lampe_ptr->start, rgb.r, rgb.g, rgb.b, lampe_ptr->lampe.brightness);
        leds[i + lampe_ptr->start] = i<leds_left?CRGB::Gold:CRGB::Black;
      }
      effects_ptr->countdown.nextframe = millis() + effects_ptr->countdown.framedelay;
      return true;
      break;
    case 2:
      if(millis() >= (effects_ptr->countdown.start_countdown + effects_ptr->countdown.duration)) {
         init_fireworks(effects_ptr);
         effects_ptr->countdown.stage = 3;
      }
      return false;
      break;
  }
}

boolean countdown_master(lampeSERIAL *lampe_ptr, effects *effects_ptr) {
  return countdown(&lampe_ptr->lampe_master, effects_ptr);
}

//Flickerfire
void init_flickerfire(effects *effects_ptr) {
  effects_ptr->fire.minframedelay = 80;
  effects_ptr->fire.maxframedelay = 120;
  effects_ptr->fire.maxflicker = 150;
  effects_ptr->fire.nextframe = millis();
}

boolean flickerfire(serialSection *lampe_ptr, effects *effects_ptr) {
  if(millis() < effects_ptr->fire.nextframe) {return false;}
  int r = 255;
  int g = r-100;
  int b = 40;

  for(int x = 0; x < lampe_ptr->length; x++) {
    int flicker = random(0,effects_ptr->fire.maxflicker);
    int r1 = r-flicker;
    int g1 = g-flicker;
    int b1 = b-flicker;
    if(g1<0) g1=0;
    if(r1<0) r1=0;
    if(b1<0) b1=0;
    //setPixelRGB(lampe_ptr->start + x, r1, g1, b1, lampe_ptr->lampe.brightness);
    leds[lampe_ptr->start + x].r = r1;
    leds[lampe_ptr->start + x].g = g1;
    leds[lampe_ptr->start + x].b = b1;
    }
  effects_ptr->fire.nextframe = millis() + random(effects_ptr->fire.minframedelay,effects_ptr->fire.maxframedelay);
  return true;
}

boolean flickerfire_master(lampeSERIAL *lampe_ptr, effects *effects_ptr) {
  return flickerfire(&lampe_ptr->lampe_master, effects_ptr);
}

/////END


///EFFECT Flames
void init_flames(effects *effects_ptr) {
  effects_ptr->flames.heatmax = 405;
  effects_ptr->flames.heatmin = 150;
  effects_ptr->flames.bubbleoffset = 12;
  effects_ptr->flames.knister = 3;
  effects_ptr->flames.bubblewidth = 4.6;
  effects_ptr->flames.sparkle = 40;
  effects_ptr->flames.movingspeed = 10;  //moving speed for pixels
  effects_ptr->flames.coolingrate = 6;  //coolingrate for each heatvalue
  effects_ptr->flames.coolingoffset = 4; // when does the cooling start
  effects_ptr->flames.offsetred = 0;    //offset for red 
  effects_ptr->flames.offsetgreen = 50; //for green
  effects_ptr->flames.offsetblue = 150;  //for blue  
  effects_ptr->flames.nextframe = millis();
  effects_ptr->flames.framedelay = 45;
  
}

boolean flames(serialSection *lampe_ptr, effects *effects_ptr) {
  if(millis() < effects_ptr->flames.nextframe) {return false;}
  double gauss;
  if(random(100) < effects_ptr->flames.sparkle) {
    effects_ptr->flames.bubble_heat = random(effects_ptr->flames.heatmin, effects_ptr->flames.heatmax);
    for(int i = 0; i < lampe_ptr->length + FLAMES_FIRST_PIXEL; i++) {
      gauss = pow((i-(FLAMES_FIRST_PIXEL-(FLAMES_FIRST_PIXEL/2))), 2);//^(i-FIRST_PIXEL);
      gauss = -gauss /(2*effects_ptr->flames.bubblewidth);
      gauss = exp(gauss);
      effects_ptr->flames.heat[i] = round(effects_ptr->flames.heat[i] + round(effects_ptr->flames.bubble_heat * gauss)/2);
    }
  }

  //cooling the fire
    for(int i = lampe_ptr->length + FLAMES_FIRST_PIXEL - 1; i >0; i--) {
      effects_ptr->flames.heat[i] = effects_ptr->flames.heat[i-1]; 
      if(i >= effects_ptr->flames.coolingoffset) {
      effects_ptr->flames.heat[i] = effects_ptr->flames.heat[i] - effects_ptr->flames.coolingrate;
      if(effects_ptr->flames.heat[i] < 0) {effects_ptr->flames.heat[i] = 0;}
      }
    }
  //sparkle at the bootom
  for(int i = 0; i < effects_ptr->flames.knister; i++){
  int heatsparkle = random(0, effects_ptr->flames.heatmax-50);
    int red = heatsparkle - effects_ptr->flames.offsetred;
    if(red > 255) {red = 255;}
    if(red < 0) {red = 0;}

    int green = heatsparkle - effects_ptr->flames.offsetgreen;
    if(green > 255) {green = 255;}
    if(green < 0) {green = 0;}

    
    int blue = heatsparkle - effects_ptr->flames.offsetblue;
    if(blue > 255) {blue = 255;}
    if(blue < 0) {blue = 0;}
    //setPixelRGB(lampe_ptr->start + i, red, green, blue, lampe_ptr->lampe.brightness);
    leds[lampe_ptr->start + i] = CRGB(red, green, blue);
  }
  for(int i = effects_ptr->flames.knister; i < lampe_ptr->length; i++) {
    int red = effects_ptr->flames.heat[i + FLAMES_FIRST_PIXEL] - effects_ptr->flames.offsetred;
    if(red > 255) {red = 255;}
    if(red < 0) {red = 0;}

    int green = effects_ptr->flames.heat[i + FLAMES_FIRST_PIXEL] - effects_ptr->flames.offsetgreen;
    if(green > 255) {green = 255;}
    if(green < 0) {green = 0;}

    
    int blue = effects_ptr->flames.heat[i + FLAMES_FIRST_PIXEL] - effects_ptr->flames.offsetblue;
    if(blue > 255) {blue = 255;}
    if(blue < 0) {blue = 0;}
    //setPixelRGB(lampe_ptr->start + i, red, green, blue, lampe_ptr->lampe.brightness);
    leds[lampe_ptr->start + i] = CRGB(red, green, blue);
  }
  effects_ptr->flames.nextframe = millis() + effects_ptr->flames.framedelay;
  return true;
}

boolean master_flames(lampeSERIAL *lampe_ptr, effects *effects_ptr) {
  return flames(&lampe_ptr->lampe_master, effects_ptr);
}
#endif
