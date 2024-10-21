#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const uint8_t ROWS = 4;
const uint8_t COLS = 4;
int cnt=0;
int tecla=0;
char keys[ROWS][COLS] = {
  { '1', '4', '7', ':' },
  { '2', '5', '8', '0' },
  { '3', '6', '9', ';' },
  { '<', '=', '>', '?' }
};
uint8_t colPins[COLS] = { 23, 19, 18, 17 };
uint8_t rowPins[ROWS] = { 16, 4, 2, 15 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
int numero,num=0,opera=0,aux,error,clave=1234,armado=0,salir=0;
int led_armado=25;
int led_desarmado=26;
int sirena=14;
int s1=34,s2=35,s3=32,s4=33,s11,s22,s33,s44,alarma=0;

void setup() {
  
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  lcd.init();
  lcd.setCursor(0,0);
  lcd.print("Hola mundo!");
  lcd.backlight();
  pinMode(led_armado, OUTPUT);
  pinMode(led_desarmado, OUTPUT);
  pinMode(sirena, OUTPUT);
  pinMode(s1, INPUT_PULLUP);
  pinMode(s2, INPUT_PULLUP);
  pinMode(s3, INPUT_PULLUP);
  pinMode(s4, INPUT_PULLUP);
  digitalWrite(led_armado,LOW);
  digitalWrite(led_desarmado,HIGH);
  digitalWrite(sirena,LOW);
  menu();

}

void loop() { 
  tecla=16;
  int key = keypad.getKey();
  if(key>0){tecla=key -'0';}
  switch(tecla){
    case 1:seguridad();salir=0;numero=0;num=0;
          while(salir==0){
               tecla=16;
               int key = keypad.getKey();
               if(key>0){tecla=key -'0';}
               if(tecla==10){salir=1;}
               seguridad_code();
 
          }
          menu();delay(1000);tecla=16;

    case 2:calculadora();salir=0;numero=0;num=0;
          while(salir==0){
               tecla=16;
               int key = keypad.getKey();
               if(key>0){tecla=key -'0';}
               if(tecla==10){salir=1;}
                 //--------------------------------------
                if(tecla<10){
                  num++;
                  if(num==1){numero=tecla;}
                  if(num==2){numero=numero*10+tecla;}
                  if(num==3){numero=numero*10+tecla;}
                  if(num==4){numero=numero*10+tecla;num=0;}
                }
                if(tecla==12){
                  opera=1;
                  aux=numero;
                  lcd.setCursor(15,1);lcd.print("+");
                }
                if(tecla==11){
                  if(opera==1){
                    numero=aux+numero;
                    lcd.setCursor(15,1);lcd.print("=");
                  }
                }
                lcd.setCursor(0,1);lcd.print(numero);  lcd.print("   ");
            
          }
          menu();delay(1000);tecla=16;

    
  }
  //
}
void menu(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" menu principal ");
  lcd.setCursor(0,1);
  lcd.print("1.Se 2.Cal 3.Pr");

}

void calculadora(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("calculadora  ");
  lcd.setCursor(0,1);
  lcd.print("             ");
}
void seguridad(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" acceso - alarma  ");
  lcd.setCursor(0,1);
  lcd.print("             ");
}
void seguridad_code(){
  //--------------------------------------
  if(tecla<10){
    num++;
    if(num==1){numero=tecla;}
    if(num==2){numero=numero*10+tecla;}
    if(num==3){numero=numero*10+tecla;}
    if(num==4){numero=numero*10+tecla;num=0;}

  }
  //----------------------------------------------------
  if(tecla==11){
    if(clave==numero){
      if(armado==0){
        armado=1;
      }else{
        armado=0;
      }
    }
    num=0;numero=0;
  }
  //----------------------------------------------------
  if(armado==1){
    lcd.setCursor(15,1);lcd.print("A");
    digitalWrite(led_armado, HIGH); 
    digitalWrite(led_desarmado, LOW); 
  }else{
    lcd.setCursor(15,1);lcd.print("D");
    digitalWrite(led_armado, LOW); 
    digitalWrite(led_desarmado, HIGH); 
    alarma=0;
    digitalWrite(sirena, LOW);
  }
//-------------------------------------------------------
s11=digitalRead(s1);
s22=digitalRead(s2);
s33=digitalRead(s3);
s44=digitalRead(s4);
if((s11==0 || s22==0 || s33==0 || s44==0)& armado==1){
  alarma=1;
}
if(alarma==1){
  digitalWrite(sirena,HIGH);
}
//------------------------------------------------------
lcd.setCursor(0,1);lcd.print(numero);  lcd.print("   ");
}
