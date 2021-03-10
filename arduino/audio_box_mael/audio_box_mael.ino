#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
//#include <SerialFlash.h>
//#include <ArduinoJson.h>
#include <EEPROM.h>

// GUItool: begin automatically generated code
AudioInputUSB            usb1;           //xy=55,660.600118637085
AudioAnalyzePeak         peak1;          //xy=122,412.20001220703125
AudioAnalyzeRMS          rms1;           //xy=122,450.20001888275146
AudioAnalyzeFFT256      fft256_1;       //xy=123.78960800170898,370.5199031829834
AudioAnalyzePeak         peak2;          //xy=273.00000381469727,479.2000198364258
AudioAnalyzeRMS          rms2;           //xy=274.00000381469727,526.2000198364258
AudioAnalyzeFFT256      fft256_2;       //xy=276.8770065307617,441.34928703308105
AudioEffectDelay         delay2; //xy=351.0000286102295,806.0000228881836
AudioEffectDelay         delay1;         //xy=352.75002670288086,652.2500686645508
AudioFilterStateVariable filter4; //xy=566.9166126251221,937.555591583252
AudioFilterStateVariable filter3; //xy=608.9166164398193,742.5555782318115
AudioFilterBiquad        biquad3;        //xy=699.7143135070801,1069.1427536010742
AudioFilterBiquad        biquad4;        //xy=706.8570976257324,1121.9999656677246
AudioFilterBiquad        biquad2;        //xy=720.5714307512555,522.000002179827
AudioFilterBiquad        biquad1;        //xy=722.0000305175781,473.42858123779297
AudioAmplifier           amp10; //xy=726.9166126251221,890.755578994751
AudioAmplifier           amp11; //xy=726.9166126251221,931.755578994751
AudioAmplifier           amp12; //xy=726.9166126251221,975.755578994751
AudioAmplifier           amp7; //xy=746.9166126251221,706.5555667877197
AudioAmplifier           amp8; //xy=746.9166126251221,747.5555667877197
AudioAmplifier           amp9; //xy=746.9166126251221,791.5555667877197
AudioMixer4              mixer3; //xy=886.9166202545166,715.5555782318115
AudioMixer4              mixer4; //xy=903.916618347168,898.5555934906006
AudioAmplifier           amp2;           //xy=933.9999847412109,534.5714530944824
AudioAmplifier           amp1;           //xy=942.5714329310825,477.42856706891735
AudioAmplifier           amp3;           //xy=974.5714263916016,1069.1427364349365
AudioAmplifier           amp4;           //xy=998.8571384974888,1121.9999912806918
AudioOutputI2SHex        i2s_hex1;       //xy=1526.571434020996,691.7142486572266
AudioConnection          patchCord1(usb1, 0, fft256_1, 0);
AudioConnection          patchCord2(usb1, 0, delay1, 0);
AudioConnection          patchCord3(usb1, 0, peak1, 0);
AudioConnection          patchCord4(usb1, 0, rms1, 0);
AudioConnection          patchCord5(usb1, 1, fft256_2, 0);
AudioConnection          patchCord6(usb1, 1, delay2, 0);
AudioConnection          patchCord7(usb1, 1, peak2, 0);
AudioConnection          patchCord8(usb1, 1, rms2, 0);
AudioConnection          patchCord9(delay2, 1, filter4, 0);
AudioConnection          patchCord10(delay2, 2, biquad3, 0);
AudioConnection          patchCord11(delay2, 2, biquad4, 0);
AudioConnection          patchCord12(delay1, 0, biquad1, 0);
AudioConnection          patchCord13(delay1, 0, biquad2, 0);
AudioConnection          patchCord14(delay1, 1, filter3, 0);
AudioConnection          patchCord15(filter4, 0, amp10, 0);
AudioConnection          patchCord16(filter4, 1, amp11, 0);
AudioConnection          patchCord17(filter4, 2, amp12, 0);
AudioConnection          patchCord18(filter3, 0, amp7, 0);
AudioConnection          patchCord19(filter3, 1, amp8, 0);
AudioConnection          patchCord20(filter3, 2, amp9, 0);
AudioConnection          patchCord21(biquad3, amp3);
AudioConnection          patchCord22(biquad4, amp4);
AudioConnection          patchCord23(biquad2, amp2);
AudioConnection          patchCord24(biquad1, amp1);
AudioConnection          patchCord25(amp10, 0, mixer4, 0);
AudioConnection          patchCord26(amp11, 0, mixer4, 1);
AudioConnection          patchCord27(amp12, 0, mixer4, 2);
AudioConnection          patchCord28(amp7, 0, mixer3, 0);
AudioConnection          patchCord29(amp8, 0, mixer3, 1);
AudioConnection          patchCord30(amp9, 0, mixer3, 2);
AudioConnection          patchCord31(mixer3, 0, i2s_hex1, 2);
AudioConnection          patchCord32(mixer4, 0, i2s_hex1, 3);
AudioConnection          patchCord33(amp2, 0, i2s_hex1, 1);
AudioConnection          patchCord34(amp1, 0, i2s_hex1, 0);
AudioConnection          patchCord35(amp3, 0, i2s_hex1, 5);
AudioConnection          patchCord36(amp4, 0, i2s_hex1, 4);
// GUItool: end automatically generated code

#define FFT_MIN_1 2
#define FFT_MIN_2 10
#define FFT_MIN_3 127

#define RMS_MUL     2
#define FFT_MUL_1   4
#define FFT_MUL_2   7
#define FFT_MUL_3   5


#define Q 0.7071
#define CUT_OFF_TWEETTER 2800
#define CUT_OFF_SUB 2800


struct audio_mix {
  float bass;
  float mid;
  float high;
};

audio_mix speakers[3];
int delays[3] = {0, 0, 0};

void setup(){
  //Gauche MAUEUR droite 
  
  //Basse, useless, aigus * 3 entre 0 et 1
  speakers = {{1, 1, 1},{1, 1, 1},{1, 1, 1}}
  //Délais entre les caissons en ms
  delays = {0, 0, 0}
  
  /*for(int i = 0; i < 3; i++){
    EEPROM.get( i*sizeof(audio_mix), speakers[i]);
  }
  for(int i = 0; i < 3; i++){
    EEPROM.get( 3*sizeof(audio_mix)+i*sizeof(int), delays[i]);
  }
  */
  AudioMemory(500);
  //SerialUSB.begin(9600);
  for(int i = 0; i < 3; i++){
    delay1.delay(i,delays[i]);
    delay2.delay(i,delays[i]);
  }
  Serial3.begin(115200);
  biquad2.setHighpass(0, CUT_OFF_TWEETTER, Q);
  biquad1.setLowpass(0, CUT_OFF_SUB, Q);
  biquad3.setHighpass(0, CUT_OFF_TWEETTER, Q);
  biquad4.setLowpass(0, CUT_OFF_SUB, Q);
  //SerialUSB.println("Hello");

}

int multipleRead(){
  int res = 0;
  for(int i = 0; i < 50; i++){
    res += analogRead(A11);
  }
  res/=50;
  return res;
}

unsigned long last_slider_time = 0;
int last_slider_val = 0;

int offset_rainbow = 0;

String serial_buff = "";

void loop(){
  
  int new_potentiometer = multipleRead();
  //SerialUSB.println(new_potentiometer);
  if(new_potentiometer < last_slider_val - 3 || last_slider_val + 3 < new_potentiometer){
    last_slider_time = millis();
    last_slider_val = new_potentiometer;
  }
  if(last_slider_time + 1500 > millis()){
    analogWrite(11, (1023 - new_potentiometer)>>2);
    analogWrite(12, new_potentiometer>>2);
  }else{
    analogWrite(11, 0);
    analogWrite(12, 0);
  }
  float potard = (float)new_potentiometer/1023;
  float vol = usb1.volume()* potard;
  
  amp1.gain(vol*speakers[0].bass);
  amp2.gain(vol*speakers[0].high);
  
  amp7.gain(vol*speakers[1].bass);
  amp10.gain(vol*speakers[1].bass);
  amp8.gain(vol*speakers[1].mid);
  amp11.gain(vol*speakers[1].mid);
  amp9.gain(vol*speakers[1].high);
  amp12.gain(vol*speakers[1].high);
  
  amp3.gain(vol*speakers[2].bass);
  amp4.gain(vol*speakers[2].high);
  
  if(fft256_1.available() && fft256_2.available() && rms1.available() && rms2.available()){
    float l_rms = min(1.0, rms1.read()*3);
    float r_rms = min(1.0, rms2.read()*3);
    
    float l_low = 0.0;
    float l_mid = 0.0;
    float l_high = 0.0;

    float r_low = 0.0;
    float r_mid = 0.0;
    float r_high = 0.0;
    /*
    for(int i = 0; i < FFT_MIN_1; i++){
      float val = fft256_1.read(i);
      if(val > l_low){
        l_low = val;
      }
    }
    for(int i = FFT_MIN_1; i < FFT_MIN_2; i++){
      float val = fft256_1.read(i);
      if(val > l_mid){
        l_mid = val;
      }
    }
    for(int i = FFT_MIN_2; i < FFT_MIN_3; i++){
      float val = fft256_1.read(i);
      if(val > l_high){
        l_high = val;
      }
    }
    
    for(int i = 0; i < FFT_MIN_1; i++){
      float val = fft256_2.read(i);
      if(val > r_low){
        r_low = val;
      }
    }
    for(int i = FFT_MIN_1; i < FFT_MIN_2; i++){
      float val = fft256_2.read(i);
      if(val > r_mid){
        r_mid = val;
      }
    }
    for(int i = FFT_MIN_2; i < FFT_MIN_3; i++){
      float val = fft256_2.read(i);
      if(val > r_high){
        r_high = val;
      }
    }*/
    String output = String("[") + min(1,RMS_MUL*l_rms)+", "+min(1,FFT_MUL_1*l_low)+", "+min(1,FFT_MUL_2*l_mid)+", "+min(1,FFT_MUL_3 * l_high)+", "+min(1,RMS_MUL*r_rms)+", "+min(1,FFT_MUL_1*r_low)+", "+min(1,FFT_MUL_2 * r_mid)+", "+min(1,FFT_MUL_3 * r_high)+", "+ "150, 0,]";
    Serial3.println(output);
    delay(65);
  }

  /*
  if (SerialUSB.available() > 0) {
    SerialUSB.println("GOT SOMETHING");
    while(SerialUSB.available()> 0){
      serial_buff += char(SerialUSB.read());
      SerialUSB.println(SerialUSB.available());
    }
    SerialUSB.println("End of read");
  
  }
  if(serial_buff.length() > 5 ){
    SerialUSB.println("Transcript");
    SerialUSB.println(serial_buff);
    StaticJsonDocument<1000> doc;
    SerialUSB.println("Deserializing");
    DeserializationError error = deserializeJson(doc, serial_buff);
    SerialUSB.println("Please...");
    if (error) {
      SerialUSB.println("Flushing");
      SerialUSB.flush();
      SerialUSB.println(SerialUSB.available());
      SerialUSB.println("?");
      SerialUSB.print(F("deserializeJson() failed: "));
      SerialUSB.println(error.c_str());
      return;
    }
    SerialUSB.println("Maybe ?");
    JsonArray arr = doc.as<JsonArray>();
    for (JsonVariant value : arr) {
      int val_id = value["id"].as<int>();
      int val_delay = value["delay"].as<int>();
      float val_bass = value["bass"].as<int>();
      float val_mid = value["mid"].as<int>();
      float val_high = value["high"].as<int>();
      val_bass /= 50;
      val_mid /= 50;
      val_high /= 50;
      
      SerialUSB.print("id : ");
      SerialUSB.print(val_id);
      SerialUSB.print("      Delay : ");
      SerialUSB.print(val_delay);
      SerialUSB.print(", Bass : ");
      SerialUSB.print(val_bass);
      SerialUSB.print(", Mid : ");
      SerialUSB.print(val_mid);
      SerialUSB.print(", high : ");
      SerialUSB.print(val_high);
      SerialUSB.println("");
      delay1.delay(val_id, val_delay);
      delay2.delay(val_id, val_delay);
      speakers[val_id] = {val_bass, val_mid, val_high};
      delays[val_id] = val_delay;
    }
    for(int i = 0; i < 3; i++){
      EEPROM.put( i*sizeof(audio_mix), speakers[i]);
    }
    for(int i = 0; i < 3; i++){
      EEPROM.put( 3*sizeof(audio_mix)+i*sizeof(int), delays[i]);
    }
    serial_buff = String("");
  }else{
    serial_buff = "";
    //SerialUSB.println("GOT NOTTHING");
  }
  //SerialUSB.println(AudioMemoryUsageMax());
  */
}
