#define M5_BRD

#ifdef M5_BRD
#include <M5Core2.h> 
#else
#include "heltec.h"
#endif

#include "Spark.h"
#include "SparkIO.h"

#define PGM_NAME "Spark UI Test"


/*********************************************************************************/
// variables required to track spark state and also for communications generally
unsigned int cmdsub;
SparkMessage msg;
SparkPreset preset;
SparkPreset presets[6];

int selected_preset;
bool ui_update_in_progress;


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
  ui_update_in_progress = false;

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
  int pres, ind;

  connect_spark();  // reconnects if any disconnects happen
  spark_process();
  app_process();
  
  // K&R: Expressions connected by && or || are evaluated left to right, 
  // and it is guaranteed that evaluation will stop as soon as the truth or falsehood is known.
  
  if (spark_msg_in.get_message(&cmdsub, &msg, &preset) || app_msg_in.get_message(&cmdsub, &msg, & preset)) {
    Serial.print("Message: ");
    Serial.println(cmdsub, HEX);

    // all the processing for sync
    switch (cmdsub) {
      // full preset details
      case 0x0301:  
      case 0x0101:
        pres = (preset.preset_num == 0x7f) ? 4 : preset.preset_num;
        if (preset.curr_preset == 0x01)
          pres = 5;
        presets[pres] = preset;
        break;
      // change of amp model
      case 0x0306:
        strcpy(presets[5].effects[3].EffectName, msg.str2);
        break;
      // change of effect
      case 0x0106:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          strcpy(presets[5].effects[ind].EffectName, msg.str2);
        break;
      // effect on/off  
      case 0x0315:
      case 0x0115:
        ind = get_effect_index(msg.str1);
        if (ind >= 0) 
          presets[5].effects[ind].OnOff = msg.onoff;
        break;
      // change parameter value  
      case 0x0337:
      case 0x0104:
        ind = get_effect_index(msg.str1);
        if (ind >= 0)
          presets[5].effects[ind].Parameters[msg.param1] = msg.val;
        break;  
      // change to preset  
      case 0x0338:
      case 0x0138:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        presets[5] = presets[selected_preset];
        break;
      // store to preset  
      case 0x0327:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        presets[selected_preset] = presets[5];
        break;
      // current selected preset
      case 0x0310:
        selected_preset = (msg.param2 == 0x7f) ? 4 : msg.param2;
        if (msg.param1 == 0x01) 
          selected_preset = 5;
        presets[5] = presets[selected_preset];
        break;
      default:
        break;
    }

    // all the processing for UI update
    switch (cmdsub) {
      case 0x0201:  
         if (ui_update_in_progress) {
           Serial.println("Updating UI");

           strcpy(presets[5].Name, "SyncPreset");
           strcpy(presets[5].UUID, "F00DF00D-FEED-0123-4567-987654321000");  
           presets[5].curr_preset = 0x00;
           presets[5].preset_num = 0x03;
           app_msg_out.create_preset(&presets[5]);
           app_process();
           delay(100);
           
           app_msg_out.change_hardware_preset(0x00, 0x00);
           app_process();
           app_msg_out.change_hardware_preset(0x00, 0x03);     
           app_process();

//           spark_msg_out.change_hardware_preset(0x00, 0x7f);     
//           spark_process();

           sp_bin.pass_through = true;
           app_bin.pass_through = true;   
           ui_update_in_progress = false;
         }
       break;
    }
          
    return true;
  }
  else
    return false;
}

void update_ui() {
  sp_bin.pass_through = false;
  app_bin.pass_through = false;
    
  app_msg_out.save_hardware_preset(0x00, 0x03);
  app_process();
  ui_update_in_progress = true;
}


void change_amp_model(char *new_eff) {
  Serial.printf("Old %s New %s\n", presets[5].effects[3].EffectName, new_eff);
  
  spark_msg_out.change_effect(presets[5].effects[3].EffectName, new_eff);
  app_msg_out.change_effect(presets[5].effects[3].EffectName, new_eff);
  strcpy(presets[5].effects[3].EffectName, new_eff);
  spark_process();
  app_process();
  delay(100);
}

void change_drive_model(char *new_eff) {
  spark_msg_out.change_effect(presets[5].effects[2].EffectName, new_eff);
  strcpy(presets[5].effects[2].EffectName, new_eff);
  spark_process();
  delay(100);
}

void change_comp_model(char *new_eff) {
  spark_msg_out.change_effect(presets[5].effects[1].EffectName, new_eff);
  strcpy(presets[5].effects[1].EffectName, new_eff);
  spark_process();
  delay(100);
}

void change_mod_model(char *new_eff) {
  spark_msg_out.change_effect(presets[5].effects[4].EffectName, new_eff);
  strcpy(presets[5].effects[4].EffectName, new_eff);
  spark_process();
  delay(100);
}

void change_delay_model(char *new_eff) {
  spark_msg_out.change_effect(presets[5].effects[5].EffectName, new_eff);
  strcpy(presets[5].effects[5].EffectName, new_eff);
  spark_process();
  delay(100);
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
}


void loop() {
  // put your main code here, to run repeatedly:
#ifdef M5_BRD
  M5.update();

  if (M5.BtnA.wasPressed()) {
    Serial.println("BUTTON A PRESS");

    change_amp_model("94MatchDCV2");
    change_drive_model("Booster");  
    change_mod_model("GuitarEQ6");
    change_delay_model("DelayMono");
    update_ui();
  }

  if (M5.BtnB.wasPressed()) {
    Serial.println("BUTTON B PRESS");

    change_amp_model("Bogner");
    change_drive_model("KlonCentaurSilver");
    change_mod_model("Tremolo");
    update_ui();
  }    

  if (M5.BtnC.wasPressed()) {
    Serial.println("BUTTON C PRESS");

    change_amp_model("Twin");
//    change_drive_model("Overdrive");
  }    

#endif


  if (update_spark_state()) {
    // do your own checks and processing here    
  }
  


}
