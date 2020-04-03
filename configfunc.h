char CONFIG_ID[5] = {'E', 'S', 'P', '3', '\0'};
const char* appname = "test";
#include <Preferences.h>

Preferences preferences;

//Configuration
typedef struct {
char        id[5];
char        SSID[255] = "";
char        WLANPW[255] = "";
char        mqttServer[255] = "";
int         port = 1882;
char        clientname[32] = "";
char        username[32] = "";
char        password[64] = "";
char        ch_alert[255];
char        sensor[32];


int         timezone;
char        ntpServer[255];
long        ntpUpdateIntervalS;

int         min_on;
int         hour_on;
int         min_off;
int         hour_off;

int         touch_threshold;
} __attribute__((packed)) config_t;

config_t  config;


//////


// Config
void saveConfig() {  
    preferences.begin(appname, false);
    //Serial.println(config.id);
    preferences.putString("id", config.id);
    preferences.putString("SSID", config.SSID);
    preferences.putString("WLANPW", config.WLANPW);
    preferences.putString("mqttServer", config.mqttServer);
    preferences.putInt("port", config.port);
    preferences.putString("clientname", config.clientname);
    preferences.putString("username", config.username);
    preferences.putString("password", config.password);
    preferences.putString("ch_alert", config.ch_alert);
    preferences.putInt("timezone", config.timezone);
    preferences.putString("ntpServer", config.ntpServer);
    preferences.putInt("ntpUpdateIntervalS", config.ntpUpdateIntervalS);
    preferences.putInt("touch_threshold", config.touch_threshold);
    preferences.end();
}


void initConfig() {
  memset(&config, 0, sizeof(config));
  strncpy(config.id, CONFIG_ID, sizeof(config.id));
  strncpy(config.SSID, "ScreenZone3", sizeof(config.SSID));
  strncpy(config.WLANPW, "MachNurWasDuWillst2018!", sizeof(config.WLANPW));
  'strncpy(config.SSID, "Hundeleine", sizeof(config.SSID));
  'strncpy(config.WLANPW, "Basset1337und1338", sizeof(config.WLANPW));
  strncpy(config.mqttServer, "192.168.178.2", sizeof(config.mqttServer));
  config.port            = 1883;
  strncpy(config.clientname, "test", sizeof(config.clientname));
  strncpy(config.username, "arduino", sizeof(config.username));
  strncpy(config.password, "arduino", sizeof(config.password));
  strncpy(config.ch_alert, "alert", sizeof(config.ch_alert));

  config.timezone = 1;
  config.ntpUpdateIntervalS = 3600;
  strncpy(config.ntpServer, "ptbtime2.ptb.de", sizeof(config.ntpServer));
  config.touch_threshold = 40;
  preferences.begin(appname);
  preferences.clear();
  preferences.end();
}

void loadConfig() {
  
  preferences.begin(appname, false);
  Serial.print("config version: ");
  //Serial.println(preferences.getString("id", "fail"));
  preferences.getString("id", "fail").toCharArray(config.id, sizeof(config.id)+1);
  Serial.println(config.id);
  Serial.println(CONFIG_ID);
  if (strcmp(CONFIG_ID, config.id)) {
    Serial.println("config not found, initializing new config");
    initConfig();
    saveConfig();
  }
  else {
    Serial.println("Config found");
    preferences.getString("SSID", "fail").toCharArray(config.SSID, sizeof(config.SSID)+1);
    preferences.getString("WLANPW", "").toCharArray(config.WLANPW, sizeof(config.WLANPW)+1);
    preferences.getString("mqttServer", "").toCharArray(config.mqttServer, sizeof(config.mqttServer)+1);
    config.port = preferences.getInt("port", 1883);
    preferences.getString("clientname", "").toCharArray(config.clientname, sizeof(config.clientname)+1);
    preferences.getString("username", "").toCharArray(config.username, sizeof(config.username)+1);
    preferences.getString("password", "").toCharArray(config.password, sizeof(config.password)+1);
    preferences.getString("ch_alert", "").toCharArray(config.ch_alert, sizeof(config.ch_alert)+1);
    config.timezone = preferences.getInt("timezone", 1);
    preferences.getString("ntpServer", "").toCharArray(config.ntpServer, sizeof(config.ntpServer)+1);
    config.ntpUpdateIntervalS = preferences.getInt("ntpUpdateIntervalS", 600);
    config.touch_threshold = preferences.getInt("touch_threshold", 40);
  } 
  
  preferences.end();
  
}
