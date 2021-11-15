#define M5_BRD

#ifdef M5_BRD
#include <M5Core2.h> 
#else
#include "heltec.h"
#endif

#include "Spark.h"
#include "SparkIO.h"

#define PGM_NAME "Spark UI Test"

SparkPreset preset0{0x0,0x03,"F00DF00D-FEED-0123-4567-987654321000","Preset Test1","0.7","AAAAAA","icon.png",120.000000,{ 
  {"bias.noisegate", false, 3, {0.138313, 0.224643, 0.000000}}, 
  {"LA2AComp", true, 3, {0.000000, 0.852394, 0.373072}}, 
  {"Booster", false, 1, {0.722592}}, 
  {"RolandJC120", true, 5, {0.632231, 0.281820, 0.158359, 0.671320, 0.805785}}, 
  {"Cloner", true, 2, {0.199593, 0.000000}}, 
  {"VintageDelay", false, 4, {0.378739, 0.425745, 0.419816, 1.000000}}, 
  {"bias.reverb", true, 7, {0.285714, 0.408354, 0.289489, 0.388317, 0.582143, 0.650000, 0.200000}} },0xb4 };

SparkPreset preset1{0x0,0x03,"F00DF00D-FEED-0123-4567-987654321001","Preset Test2","0.7","BBBBBBB","icon.png",120.000000,{ 
  {"bias.noisegate", false, 3, {0.099251, 0.570997, 0.000000}}, 
  {"BlueComp", false, 4, {0.430518, 0.663291, 0.355048, 0.557014}}, 
  {"DistortionTS9", false, 3, {0.058011, 0.741722, 0.595924}}, 
  {"94MatchDCV2", true, 5, {0.528926, 0.500905, 0.246163, 0.417119, 0.782293}}, 
  {"Flanger", false, 3, {0.413793, 0.663043, 0.655172}}, 
  {"DelayRe201", true, 5, {0.097778, 0.312182, 0.485182, 0.369640, 1.000000}}, 
  {"bias.reverb", true, 7, {0.561185, 0.506659, 0.417857, 0.300847, 0.602287, 0.594118, 0.000000}} },0xeb };

SparkPreset preset2{0x0,0x03,"F00DF00D-FEED-0123-4567-987654321002","Preset Test3","0.7","CCCCCCC","icon.png",120.000000,{ 
  {"bias.noisegate", false, 2, {0.283019, 0.304245}}, 
  {"Compressor", true, 2, {0.325460, 0.789062}}, 
  {"Booster", false, 1, {0.666735}}, 
  {"Twin", true, 5, {0.613433, 0.371715, 0.453167, 0.676660, 0.805785}}, 
  {"ChorusAnalog", true, 4, {0.185431, 0.086409, 0.485027, 0.567797}}, 
  {"DelayEchoFilt", false, 5, {0.533909, 0.275554, 0.455372, 0.457702, 1.000000}}, 
  {"bias.reverb", true, 7, {0.508871, 0.317935, 0.461957, 0.349689, 0.339286, 0.481753, 0.700000}} },0x48 };

SparkPreset preset3{0x0,0x03,"F00DF00D-FEED-0123-4567-987654321003","Preset Test4","0.7","DDDDDDDD","icon.png",120.000000,{ 
  {"bias.noisegate", true, 3, {0.211230, 0.570997, 0.000000}}, 
  {"Compressor", true, 2, {0.172004, 0.538197}}, 
  {"DistortionTS9", false, 3, {0.703110, 0.278146, 0.689846}}, 
  {"ADClean", true, 5, {0.677083, 0.501099, 0.382828, 0.585946, 0.812231}}, 
  {"ChorusAnalog", true, 4, {0.519976, 0.402152, 0.240642, 0.740579}}, 
  {"DelayMono", true, 5, {0.173729, 0.233051, 0.493579, 0.600000, 1.000000}}, 
  {"bias.reverb", true, 7, {0.688801, 0.392857, 0.461138, 0.693705, 0.488235, 0.466387, 0.300000}} },0xa2 };

SparkPreset preset4{0x0,0x03,"F00DF00D-FEED-0123-4567-987654321004","Preset Test5","0.7","EEEEEEE","icon.png",120.000000,{ 
  {"bias.noisegate", true, 2, {0.316873, 0.304245}}, 
  {"Compressor", false, 2, {0.341085, 0.665754}}, 
  {"Booster", true, 1, {0.661412}}, 
  {"Bassman", true, 5, {0.768152, 0.491509, 0.476547, 0.284314, 0.389779}}, 
  {"UniVibe", false, 3, {0.500000, 1.000000, 0.700000}}, 
  {"VintageDelay", true, 4, {0.152219, 0.663314, 0.144982, 1.000000}}, 
  {"bias.reverb", true, 7, {0.120109, 0.150000, 0.500000, 0.406755, 0.299253, 0.768478, 0.100000}} },0x12 };

SparkPreset *pres[]={&preset0, &preset1, &preset2, &preset3, &preset4};
int this_pres = 0;
unsigned long timer;

// variables required to track spark state and also for communications generally
unsigned int cmdsub;
SparkMessage msg;
SparkPreset preset;
SparkPreset presets[6];
int selected_preset;
bool got_presets;


bool ui_update;
SparkPreset fake_preset;

void dump_preset(SparkPreset preset) {
  int i,j;

  Serial.print(preset.curr_preset); Serial.print(" ");
  Serial.print(preset.preset_num); Serial.print(" ");
  Serial.print(preset.Name); Serial.print(" : ");
  Serial.println(preset.Description);

  for (j = 0; j < 7; j++) {
    Serial.printf("     %-30s", preset.effects[j].EffectName); Serial.print(" ");
    if (preset.effects[j].OnOff == true) Serial.print(" On  "); else Serial.print (" Off ");
    for (i = 0; i < preset.effects[j].NumParameters; i++) {
      Serial.print(preset.effects[j].Parameters[i]); Serial.print(" ");
    }
    Serial.println();
  }
}

int get_effect_index(char *str) {
  int ind, i;

  ind = -1;
  for (i = 0; ind == -1 && i <= 6; i++) {
    if (strcmp(presets[5].effects[i].EffectName, str) == 0) {
      ind  = i;
    }
  }
  return ind;
}

void  spark_state_tracker_start() {
  selected_preset = 0;
  got_presets = false;

  // send commands to get preset details for all presets and current state (0x0100)
  spark_msg_out.get_preset_details(0x0000);
  spark_msg_out.get_preset_details(0x0001);
  spark_msg_out.get_preset_details(0x0002);
  spark_msg_out.get_preset_details(0x0003);
  spark_msg_out.get_preset_details(0x0100);
}

// get changes from app or Spark and update internal state to reflect this
// this function has the side-effect of loading cmdsub, msg and preset which can be used later

bool  update_spark_state() {
  int j, p, ind;

  connect_spark();  // reconnects if any disconnects happen
  spark_process();
  app_process();
  
  // K&R: Expressions connected by && or || are evaluated left to right, and it is guaranteed that evaluation will stop as soon as the truth or falsehood is known.
  if (spark_msg_in.get_message(&cmdsub, &msg, &preset) || app_msg_in.get_message(&cmdsub, &msg, & preset)) {
    Serial.printf("Message: %4X  ", cmdsub);

    switch (cmdsub) {
      // full preset details
      case 0x0301:  
      case 0x0101:
        p = preset.preset_num;
        j = preset.curr_preset;
        if (p == 0x7f)       
          p = 4;
        if (j == 0x01)
          p = 5;
        presets[p] = preset;
        if (p==5) got_presets = true;
        Serial.printf("Send / receive new preset: %x\n", p);      
        dump_preset(preset);
        break;
      // change of amp  
      case 0x0306:
        strcpy(presets[5].effects[3].EffectName, msg.str2);
        Serial.printf("Change to new amp %s -> %s\n", msg.str1, msg.str2);
        break;
      // change of effect
      case 0x0106:
        Serial.printf("Change to new effect %s -> %s\n", msg.str1, msg.str2);
        ind = get_effect_index(msg.str1);
        if (ind >= 0) {
          strcpy(presets[5].effects[ind].EffectName, msg.str2);
        }
        break;
      // effect on/off  
      case 0x0315:
      case 0x0115:
        Serial.printf("Turn effect %s %s\n", msg.str1, msg.onoff ? "On " : "Off");
        ind = get_effect_index(msg.str1);
        if (ind >= 0) {
          presets[5].effects[ind].OnOff = msg.onoff;
        }
        break;
      // change parameter value  
      case 0x0337:
      case 0x0104:
        Serial.printf("Change model parameter %s %d %0.3f\n", msg.str1, msg.param1, msg.val);
        ind = get_effect_index(msg.str1);
        if (ind >= 0) {
          presets[5].effects[ind].Parameters[msg.param1] = msg.val;
        }
        break;  
      // change to preset  
      case 0x0338:
      case 0x0138:
        selected_preset = msg.param2;
        if (selected_preset == 0x7f) 
          selected_preset=4;
        presets[5] = presets[selected_preset];
        Serial.printf("Change to preset: %x\n", selected_preset);
        if (msg.param1 == 0x01) Serial.println("** Got a change to preset 0x100 from the app **");
        break;
      // store to preset  
      case 0x0327:
        selected_preset = msg.param2;
        if (selected_preset == 0x7f) 
          selected_preset=4;
        presets[selected_preset] = presets[5];
        Serial.printf("Store in preset: %x\n", selected_preset);
        break;
      // current selected preset
      case 0x0310:
        selected_preset = msg.param2;
        if (selected_preset == 0x7f) 
          selected_preset = 4;
        if (msg.param1 == 0x01) 
          selected_preset = 5;
        presets[5] = presets[selected_preset];
        Serial.printf("Hardware preset is: %x\n", selected_preset);
        break;
      default:
      Serial.printf("Unprocessed in sync: %x\n", cmdsub);
    }  
//    if (got_presets) dump_preset(presets[5]);
    return true;
  }
  else
    return false;
}

void setup() {
  // put your setup code here, to run once:
  
#ifdef M5_BRD
  M5.begin();
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.println(PGM_NAME);
 
#else
  Heltec.begin(true /*DisplayEnable Enable*/, false /*LoRa Enable*/, true /*Serial Enable*/);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, PGM_NAME);
  Heltec.display->display();
  Heltec.display->display();
#endif

  Serial.println("Started");
  
  connect_to_all();             // sort out bluetooth connections
  spark_start(true);            // set up the classes to communicate with Spark and app
  spark_state_tracker_start();  // set up data to track Spark and app state

  ui_update = false;
  timer = millis();
}


void loop() {
  // put your main code here, to run repeatedly:
#ifdef M5_BRD
  M5.update();
#endif

  if (millis() - timer > 60000) {
    timer = millis();
    this_pres++;
    if (this_pres > 4) this_pres = 0;

    Serial.println("Triggered");
    fake_preset = *pres[this_pres];  
    fake_preset.preset_num = 0x03;
    fake_preset.curr_preset = 0x00;
    presets[5]=fake_preset;
           
    sp_bin.pass_through = false;
    app_bin.pass_through = false;
    spark_msg_out.create_preset(&fake_preset);

    unsigned long t  = millis();
    while (millis() - t <700) {
      spark_process();
    }
    
    app_msg_out.save_hardware_preset(0x00, 0x03);
    app_process();

    ui_update = true;
  }


  // do your own checks and processing here
  if (update_spark_state()) {    // just by calling this we update the local stored state which can be used here
     switch (cmdsub) {
       case 0x0201:    
         if (ui_update) {
           Serial.println("Updating UI");
           dump_preset(fake_preset);       
           app_msg_out.create_preset(&fake_preset);
           app_process();

           delay(100);
           
           app_msg_out.change_hardware_preset(0x00, 0x00);
           app_process();
           app_msg_out.change_hardware_preset(0x00, 0x03);     
           app_process();

           spark_msg_out.change_hardware_preset(0x00, 0x03);     
           spark_process();

           sp_bin.pass_through = true;
           app_bin.pass_through = true;   
           ui_update = false;
         }       
         break;
    }


  }

  // do your own checks and processing here
}
