#include <BleKeyboard.h>

unsigned long ora=millis();
const int lung_bit=11;

int clock_pin=26;//rasp pin sopra 20 (19) 
int data_pin=25;// rasp pin 20 in basso a sx
int turnoff_pin=32;
int contatore, shift=0;
int valore_carattere_0, valore_carattere_1, valore_carattere_2;
char codice_carattere[lung_bit];
char conversione[256], speciali[256];


BleKeyboard bleKeyboard("retro_key", "EmaB4r");

void sleep_(){//funzione che si occupa di mettere in deepsleep la esp32, rimanendo in ascolto sul pin 26 per svegliarsi
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_26,0); //1 = High, 0 = Low
  Serial.println("Going to sleep now");
  delay(1000);
  esp_deep_sleep_start();
}

void turnoff_(){//quando chiamata attiva il pin che spegne l'alimentazione
  digitalWrite(turnoff_pin, HIGH);
  delay(100);
}



void riempi_memoria_tasti(){
  conversione[0x76]= KEY_ESC;
  conversione[0x05]= KEY_F1;
  conversione[0x06]= KEY_F2;
  conversione[0x04]= KEY_F3;
  conversione[0x0C]= KEY_F4;
  conversione[0x03]= KEY_F5;
  conversione[0x0B]= KEY_F6;
  conversione[0x83]= KEY_F7;
  conversione[0x0A]= KEY_F8;
  conversione[0x01]= KEY_F9;
  conversione[0x09]= KEY_F10;
  conversione[0x78]= KEY_F11;
  conversione[0x07]= KEY_F12;
  conversione[0x7E]= 207; //key_scroll_lock
  conversione[0x0E]= '`';
  conversione[0x16]= '1';
  conversione[0x1E]= '2';
  conversione[0x26]= '3';
  conversione[0x25]= '4';
  conversione[0x2E]= '5';
  conversione[0x36]= '6';
  conversione[0x3D]= '7';
  conversione[0x3E]= '8';
  conversione[0x46]= '9';
  conversione[0x45]= '0';
  conversione[0x4E]= '-'; //corrisponde al tasto a dx di 0 ," ' "
  conversione[0x55]= '=';//corrisponde al tasto a dx di " ' ", " ì "
  conversione[0x66]= KEY_BACKSPACE;
  conversione[0x0D]= KEY_TAB;
  conversione[0x15]= 'q';
  conversione[0x1D]= 'w';
  conversione[0x24]= 'e';
  conversione[0x2D]= 'r';
  conversione[0x2C]= 't';
  conversione[0x35]= 'y';
  conversione[0x3C]= 'u';
  conversione[0x43]= 'i';
  conversione[0x44]= 'o';
  conversione[0x4D]= 'p';
  conversione[0x54]= '[';
  conversione[0x5B]= ']';
  conversione[0x5D]= '\\';
  conversione[0x58]= KEY_CAPS_LOCK;
  conversione[0x1C]= 'a';
  conversione[0x1B]= 's';
  conversione[0x23]= 'd';
  conversione[0x2B]= 'f';
  conversione[0x34]= 'g';
  conversione[0x33]= 'h';
  conversione[0x3B]= 'j';
  conversione[0x42]= 'k';
  conversione[0x4B]= 'l';
  conversione[0x4C]= ';';
  conversione[0x52]= '\'';
  conversione[0x5A]= KEY_RETURN;
  conversione[0x12]= KEY_LEFT_SHIFT;
  conversione[0x1A]= 'z';
  conversione[0x22]= 'x';
  conversione[0x21]= 'c';
  conversione[0x2A]= 'v';
  conversione[0x32]= 'b';
  conversione[0x31]= 'n';
  conversione[0x3A]= 'm';
  conversione[0x41]= ',';
  conversione[0x49]= '.';
  conversione[0x4A]= '/';
  conversione[0x59]= KEY_RIGHT_SHIFT;
  conversione[0x14]= KEY_LEFT_CTRL;
  conversione[0x11]= KEY_LEFT_ALT;
  conversione[0x29]= ' ';
  conversione[0x77]= 219; //KEY_NUM_LOCK
  conversione[0x7C]= 221; //KEY_KP_ASTERISK;
  conversione[0x7B]= 222; //KEY_KP_MINUS;
  conversione[0x6C]= 231; //KEY_KP_7;
  conversione[0x75]= 232; //KEY_KP_8;
  conversione[0x7D]= 233; //KEY_KP_9;
  conversione[0x79]= 223; //KEY_KP_PLUS;
  conversione[0x6B]= 228; //KEY_KP_4;
  conversione[0x73]= 229; //KEY_KP_5;
  conversione[0x74]= 230; //KEY_KP_6;
  conversione[0x69]= 225; //KEY_KP_1;
  conversione[0x72]= 226; //KEY_KP_2;
  conversione[0x7A]= 227; //KEY_KP_3;
  conversione[0x70]= 234; //KEY_KP_0;
  conversione[0x71]= 235; //KEY_KP_DOT;
  //conversione[0x61]= '<';
  

  speciali[0x1F]=KEY_LEFT_GUI;
  speciali[0x11]=KEY_RIGHT_ALT;
  speciali[0x27]=KEY_RIGHT_GUI;
  speciali[0x2F]=237; //KEY_MENU;
  speciali[0x14]=132 ;//KEY_RIGHT_CONTROL;
  speciali[0x70]=KEY_INSERT;
  speciali[0x6C]=KEY_HOME;
  speciali[0x7D]=KEY_PAGE_UP;
  speciali[0x71]=KEY_DELETE;
  speciali[0x69]=KEY_END;
  speciali[0x7A]=KEY_PAGE_DOWN;
  speciali[0x75]=KEY_UP_ARROW;
  speciali[0x6B]=KEY_LEFT_ARROW;
  speciali[0x72]=KEY_DOWN_ARROW;
  speciali[0x74]=KEY_RIGHT_ARROW;
  speciali[0x4A]=220; //KEY_KP_SLASH;
  speciali[0x5A]=224; //KEY_KP_ENTER;

}

void shut_down_all(){//macro per spegnere un pc windows e dopo la tastiera
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press('x');
  bleKeyboard.releaseAll();
  delay(150);
  bleKeyboard.write(KEY_UP_ARROW);
  delay(150);
  bleKeyboard.write(KEY_UP_ARROW);
  delay(150);
  bleKeyboard.write(KEY_RIGHT_ARROW);
  delay(150);
  bleKeyboard.write(KEY_UP_ARROW);
  delay(150);
  bleKeyboard.write(KEY_UP_ARROW);
  delay(150);
  bleKeyboard.write(KEY_RETURN);
  delay(150);
  sleep_();
  
  
}

void less_than(){
  bleKeyboard.releaseAll();
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(230);
  bleKeyboard.release(230);
  bleKeyboard.press(234);
  bleKeyboard.release(234);
  bleKeyboard.releaseAll();
}

void greater_than(){
  bleKeyboard.releaseAll();
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(230);
  bleKeyboard.release(230);
  bleKeyboard.press(226);
  bleKeyboard.release(226);
  bleKeyboard.releaseAll();
}


void stampa_tasto(int numero_0, int numero_1, int numero_2){
  if(numero_0!=240 && numero_0!=224){//240 e 224 sono caratteri speciali da ignorare
    if(numero_1==224){//sto provando a premere un carattere speciale
      bleKeyboard.press(speciali[numero_0]);
    }
    else if(numero_1==240){//se ho rilasciato un tasto
          if(numero_2==224){//se speciale
            bleKeyboard.release(speciali[numero_0]);
          }
          else{//se normale
            (numero_0==0x12)? shift=0: 0;//se rilascio shift azzero il controllo
            bleKeyboard.release(conversione[numero_0]);               
          }
    }
    else if (numero_0==0x61){
      if (shift)
        greater_than();
      else
        less_than();
    }
    else if (numero_0==225){// se premo il tasto pausa-interr spengo il pc windows
      shut_down_all();
    }
    
      
    else{
      (numero_0==0x12)? shift=1: 0;
      bleKeyboard.press(conversione[numero_0]);
    }
  }
}




void setup() {//setup chiamato ogni volta che accendo/sveglio la scheda esp 

  if (bleKeyboard.isConnected())
    bleKeyboard.end();
  bleKeyboard.begin();
  Serial.begin(115200);
  
  bleKeyboard.setDelay(8);
  pinMode(clock_pin,INPUT);
  pinMode(data_pin,INPUT);
  pinMode(turnoff_pin, OUTPUT);
  digitalWrite(turnoff_pin, HIGH);
  gpio_hold_en(GPIO_NUM_32);
  riempi_memoria_tasti();
  delay(200);

}
int counter=0;
void loop() {
  if (bleKeyboard.isConnected()){
    
    if (digitalRead(clock_pin)==LOW){//la tastiera invia dati con il falling clock
      codice_carattere[contatore]=digitalRead(data_pin);
    
      contatore++;
    
      while (digitalRead(clock_pin)==LOW){}//finchè rimane low aspetto
    
    }
    if (contatore==lung_bit){//ho letto uno scancode completo
    
    
      valore_carattere_2=valore_carattere_1;
      valore_carattere_1=valore_carattere_0;
      valore_carattere_0=codice_carattere[1]*1+codice_carattere[2]*2+codice_carattere[3]*4+codice_carattere[4]*8+codice_carattere[5]*16+codice_carattere[6]*32+codice_carattere[7]*64+codice_carattere[8]*128;

    
    
    
      
      stampa_tasto(valore_carattere_0, valore_carattere_1, valore_carattere_2);
      ora=millis();//tengo traccia di quando è stata eseguita l'ultima lettura
    
    
    
      contatore=0;
    }
    if(((millis()-ora)/60000)>15)//se sono passati + di 15 minuti dall'ultima lettura vado in sleep
      sleep_();


  }
  else{//se non sono collegato ad un pc
    contatore=0;
    if(((millis()-ora)/60000)>2)// se sono passati + di due minuti senza collegamento spengo tastiera
      sleep_();
  } 
  

  

}