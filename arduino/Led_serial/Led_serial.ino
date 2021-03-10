#include <Adafruit_NeoPixel.h>
#define LED_COUNT 81
#define LED_PIN_1 5
#define LED_PIN_2 6

Adafruit_NeoPixel l_pixels(LED_COUNT, LED_PIN_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel r_pixels(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800);

void setup() {
  // put your setup code here, to run once:
  digitalWrite(13,HIGH);
  delay(2500);
  digitalWrite(13,LOW);
  delay(2500);
  
  Serial.begin(115200);
  l_pixels.begin();
  r_pixels.begin();
  l_pixels.setBrightness(150);
  r_pixels.setBrightness(150);
  l_pixels.show();
  r_pixels.show();
}

#define PATTERN_RAINBOW 0
#define PATTERN_LIST 1

struct packet{
  float fft[8];
  uint8_t brightness;
  uint8_t pattern;
};

packet new_packet = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 100, 0};

uint32_t Wheel(byte WheelPos){
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85){
    return l_pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }else if(WheelPos < 170){
    WheelPos -= 85;
    return l_pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }else{
    WheelPos -= 170;
    return l_pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

String incoming_message = "";
bool is_reading = false;
uint8_t value_index = 0;

uint8_t rainbow_offset = 0;
void display_leds(){
  switch(new_packet.pattern){
    case PATTERN_RAINBOW:
      for(int i =0; i < (int)(new_packet.fft[0]*LED_COUNT); i ++){
        l_pixels.setPixelColor(i, Wheel((i+rainbow_offset)%255));
      }
      for(int i = (int)(new_packet.fft[0]*LED_COUNT); i < LED_COUNT; i ++){
        l_pixels.setPixelColor(i, 0);
      }
      for(int i = 0; i < (int)(new_packet.fft[4]*LED_COUNT); i ++){
        r_pixels.setPixelColor(i, Wheel((i+rainbow_offset)%255));
      }
      for(int i = (int)(new_packet.fft[4]*LED_COUNT); i < LED_COUNT; i ++){
        r_pixels.setPixelColor(i, 0);
      }
      l_pixels.show();
      r_pixels.show();
      
      rainbow_offset++;
      break;
    case PATTERN_LIST:
      l_pixels.fill(l_pixels.Color(0, 0, 0), 0, LED_COUNT);
      r_pixels.fill(r_pixels.Color(0, 0, 0), 0, LED_COUNT);
      for(int i =0; i < (int)(new_packet.fft[2]*LED_COUNT); i ++){
        l_pixels.setPixelColor(i, l_pixels.Color(149, 0, 191));
      }
      for(int i =0; i < (int)(new_packet.fft[1]*(LED_COUNT/2)); i ++){
        l_pixels.setPixelColor(i, l_pixels.Color(240, 240, 240));
      }
      for(int i =LED_COUNT-1; i > LED_COUNT - (int)(new_packet.fft[3]*(LED_COUNT/4)); i ++){
        l_pixels.setPixelColor(i, l_pixels.Color(195, 0, 255));
      }
      
      for(int i =0; i < (int)(new_packet.fft[6]*LED_COUNT); i ++){
        r_pixels.setPixelColor(i, r_pixels.Color(149, 0, 191));
      }
      for(int i =0; i < (int)(new_packet.fft[5]*(LED_COUNT/2)); i ++){
        r_pixels.setPixelColor(i, r_pixels.Color(240, 240, 240));
      }
      for(int i =LED_COUNT-1; i > LED_COUNT - (int)(new_packet.fft[7]*(LED_COUNT/4)); i ++){
        r_pixels.setPixelColor(i, r_pixels.Color(195, 0, 255));
      }
      l_pixels.show();
      r_pixels.show();
      break;
  }
}

void debug_vals(){
  Serial.println("Floats :");
  Serial.print(new_packet.fft[0]);
  Serial.print("   ");
  Serial.print(new_packet.fft[1]);
  Serial.print("   ");
  Serial.print(new_packet.fft[2]);
  Serial.print("   ");
  Serial.print(new_packet.fft[3]);
  Serial.print("   ");
  Serial.print(new_packet.fft[4]);
  Serial.print("   ");
  Serial.print(new_packet.fft[5]);
  Serial.print("   ");
  Serial.print(new_packet.fft[6]);
  Serial.print("   ");
  Serial.print(new_packet.fft[7]);
  Serial.println("   ");
  Serial.println(new_packet.brightness);
  Serial.println(new_packet.pattern);
  Serial.println("End of packet");
  
  
  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>0){
    digitalWrite(13, HIGH);
    char incoming_char = Serial.read();
    Serial.print(incoming_char);
    switch(incoming_char){
      case '[':
        is_reading = true;
        value_index = 0;
        incoming_message = "";
        break;
      case ',':
        if(is_reading){
          if(value_index < 8 ){
            new_packet.fft[value_index] = incoming_message.toFloat();
          }
          if(value_index == 8 ){
            new_packet.brightness = incoming_message.toInt();
          }
          if(value_index == 9 ){
            new_packet.pattern = incoming_message.toInt();
          }
          incoming_message = "";
          value_index++;
        }
        break;
      case ']':
        if(is_reading){
          is_reading = false;
          debug_vals();
          display_leds();
          new_packet = {{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 100, 0};
          
        }
        break;
      default:
        if(is_reading){
          incoming_message += incoming_char;
        }
        break;
    }
    
  }
  digitalWrite(13,LOW);
}
