#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Hardware specific defines
#define Enc_Button  20
#define Enc_A       18
#define Enc_B       19

#define OLED_MOSI   11
#define OLED_CLK    12
#define OLED_DC     9
#define OLED_CS     8
#define OLED_RESET  10

// Software specific defines
#define max_menu    3
#define ZEILE1      0
#define ZEILE2      14
#define ZEILE3      24

#define X_OFFSET    25
#define TIT_OFFSET  25

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void set_menu (void);
void calc_batt (void);
void print_menu (void);

volatile uint8_t menu_state=0, Encoder_pos=0, enc_temp=0;

void setup() {
  pinMode (Enc_A, INPUT);
  pinMode (Enc_B, INPUT);
  digitalWrite (Enc_A, HIGH);
  digitalWrite (Enc_B, HIGH);
  attachInterrupt(5, doEncoder, RISING);

  display.begin(SSD1306_SWITCHCAPVCC);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.clearDisplay();
  Serial.begin(9600);
  Serial.println("Ready!");
}


void loop() {

  print_menu();

    delayMicroseconds(250);
  }


void doEncoder(){
  if (digitalRead(Enc_A) == HIGH){          // found a low-to-high on channel A
    if (digitalRead(Enc_B) == LOW){         // check channel B to see which way encoder is turning
      Encoder_pos += 1;                     // CCW
    }
    else{
      Encoder_pos -= 1;                     // CW
    }
  }
  else{                                     // found a high-to-low on channel A
    if (digitalRead(Enc_B) == LOW){
      Encoder_pos -= 1;                     // CW
    }
    else{
      Encoder_pos += 1;                     // CCW
    }
  }
  set_menu();
}

void set_menu (void){
  noInterrupts();

  if (Encoder_pos < enc_temp) {
    menu_state -= 1;
    //Serial.println(Encoder_pos);
    enc_temp = Encoder_pos;
  }
  else if(Encoder_pos > enc_temp) {
    menu_state += 1;
    //Serial.println(Encoder_pos);
    enc_temp = Encoder_pos;
  }

  if(menu_state <= 1)
    menu_state = 1;
  if(menu_state >= max_menu)
    menu_state = max_menu;

  interrupts();
}

void calc_batt (void){
  //read ref Voltage, or use ADC+Shunt? oder built in N-Cell Lipo Cuircuit?!
  uint8_t bat_perc=0;

  bat_perc = 123;

  if ((bat_perc <= 1)||(bat_perc >= 100)){
    bat_perc = 255;
  }

  // print dummy Value till implementation
  display.setCursor(109,ZEILE3);
  display.print(bat_perc);
}

void print_menu (void){
  uint8_t menu_layer=0;

  switch (menu_state) {
    // prototyp Funktion void FKT (uint8_t ebene, uint8_t menu_pkt, *titel_name)
    // wenn nicht ausgewählt durch fixwert aus sichtbereich schieben? "X_OFFSET-100" sonst -8
    case 1:{
      menu_layer=1;
      display.clearDisplay();
      display.setCursor(TIT_OFFSET,ZEILE1);
      display.print("-- Dolly --   ");
        display.print("[");
        display.print(menu_layer);  //HERE BATTERY STATUS
        display.print("]");

        display.setCursor(18,8);
        display.print("-----------------");

      display.setCursor(X_OFFSET-8,ZEILE2);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE2);
      display.print("Menu");
      display.setCursor(X_OFFSET-100,ZEILE3);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE3);
      display.print("Menu");
      calc_batt();  // HERE [BACK] BUTTON
      display.display();
    }
      break;
    case 2:{
      menu_layer=2;
      display.clearDisplay();
      display.setCursor(TIT_OFFSET,ZEILE1);
      display.print("-- Dolly --   ");
        display.print("[");
        display.print(menu_layer);
        display.print("]");

        display.setCursor(18,8);
        display.print("-----------------");

      display.setCursor(X_OFFSET-100,ZEILE2);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE2);
      display.print("Menu");
      display.setCursor(X_OFFSET-8,ZEILE3);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE3);
      display.print("Menu");
      calc_batt();
      display.display();
    }
      break;
    case 3:{
      menu_layer=3;
      display.clearDisplay();
      display.setCursor(TIT_OFFSET,ZEILE1);
      display.print("-- Dolly --   ");
        display.print("[");
        display.print(menu_layer);
        display.print("]");

        display.setCursor(18,8);
        display.print("-----------------");

      display.setCursor(X_OFFSET-8,ZEILE2);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE2);
      display.print("Menu");
      display.setCursor(X_OFFSET-100,ZEILE3);
      display.print(">");
      display.setCursor(X_OFFSET,ZEILE3);
      display.print("Menu");
      calc_batt();
      display.display();
    }
      break;
    default:{
      display.clearDisplay();
      display.setCursor(17,ZEILE1);
      display.print("X-Slider M.Tekaat");
      display.setCursor(TIT_OFFSET,ZEILE2);
      display.print("ver. 0.1");
      // print Battery charge
      calc_batt();
      display.display();
    }
      break;
  }
}
