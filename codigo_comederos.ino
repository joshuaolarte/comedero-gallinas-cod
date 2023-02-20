#include <EEPROM.h>

#include <virtuabotixRTC.h>

#include <LiquidCrystal.h>


const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//inicializamos y configuramos el rtc
virtuabotixRTC myRTC(11, 12, 13);
int seconds = 0, minutes = 50, hours = 9, day_of_the_week = 1, day_of_the_month = 20, month = 2, year = 2023;
int limpiador = 1;
int boton_lcd;
int boton_sin_oprimir = 1023, up = 130, down = 306, left = 480, right = 0, select = 721;
int partes = 1;
int primerAlimentoh = 0,segundoAlimentoh = 0, primerAlimentom = 0,segundoAlimentom = 0;

struct Horario_Dispensado{
  //estructura usada para guardar las horas en que dispensa y el tiempo que dispensa
  unsigned int tiempo_R1;
  byte horasAl;
  byte minutosAl;

  unsigned int tiempo_R2;
  byte horasAl2;
  byte minutosAl2;

  
   
}Dispensado;


struct Timer{
  //estructura para reemplazar y funcion para reemplazar el delay por un timer
  
 unsigned long t_anterior = 0;
 unsigned int umbral = 0;
 
 
   bool is_the_time(){

    if( millis() - t_anterior > umbral){
    
      t_anterior =  millis();
      
      return true;
    }
    return false;
  }
};


void guardar_eeprom(){
  Horario_Dispensado x;
  
  x = Dispensado;
   EEPROM.put(0,x);
}


void leer_eeprom(){
  Horario_Dispensado x;

  EEPROM.get(0,x);

  Dispensado = x;
}



void setup() {
  //inicializamos la pantalla lcd y el puerto serial
   lcd.begin(16, 2);
   Serial.begin(9600);

  //esta funcion hace que apenas inicie el arduino lea la memoria eeprom y la guarde en Dispensado
   leer_eeprom();
   

  
   

   

  //configuramos y ponemos la hora y la fecha actual
//myRTC.setDS1302Time(seconds, minutes, hours, day_of_the_week, day_of_the_month, month, year);
}

//variables y objetos definidos para que el timer funcione correctamente

bool animation= 0;
Timer TiempoAnimacion;


 
void loop() {
  
 //esta funcion ayuda a actualizar todo y a acceder a cada elemento individualmente
   myRTC.updateTime();
  
//imprimimos hora y fecha en el serial
  Serial.print("Current Date / Time: ");                                                                  
  Serial.print(myRTC.dayofmonth);                                                                         
  Serial.print("/");                                                                                      
  Serial.print(myRTC.month);                                                                              
  Serial.print("/");                                                                                      
  Serial.print(myRTC.year);                                                                               
  Serial.print("  ");                                                                                     
  Serial.print(myRTC.hours);                                                                              
  Serial.print(":");                                                                                      
  Serial.print(myRTC.minutes);                                                                            
  Serial.print(":");                                                                                      
  Serial.print(myRTC.seconds);                                                                          

                          

 //tambien imprimimos los valores de los botones de la lcd
  boton_lcd = analogRead(A0);
  Serial.print(" boton ");
  Serial.println(boton_lcd);
  
 //empezamos la estructura del codigo el cual esta dividido en casos
  switch (partes){

  case 1:
    
  //primer caso es la bienvenida a la interfaz
   
 if(!animation and TiempoAnimacion.is_the_time()){
    TiempoAnimacion.umbral = 2000;
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Bienvenido");
    lcd.setCursor(7,1);
    lcd.print(":)");
    animation = 1;
 
  }else if( animation and TiempoAnimacion.is_the_time()){
      TiempoAnimacion.umbral = 3000;
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Presione select");
      animation = 0;
  }
 
   if(boton_lcd == select){
    delay(300);
    partes ++;
   
    }
    
   
    
  break;
  

case 2:
//el segundo caso se utliza para seleccionar la hora en la que quiere que salga la primera parte del alimento del dia, este caso se divide en dos partes uno para la hora y el otro para los minutos
if(limpiador == 1){
  lcd.clear();
  limpiador ++;
}
      lcd.setCursor(0,0);
      lcd.print("1er alimento");
      lcd.setCursor(5,1);
      lcd.print("|");
      lcd.setCursor(7,1);
      lcd.print(Dispensado.horasAl);
      lcd.setCursor(8,1);
      lcd.print(":");
      lcd.setCursor(10,1);
      lcd.print(Dispensado.minutosAl);
      lcd.setCursor(11,1);
      lcd.print("|");
      
      if(Dispensado.horasAl < 10){
        lcd.setCursor(6,1);
        lcd.print(0);
        
      }else if(Dispensado.horasAl >= 10){
        lcd.setCursor(6,1);
        lcd.print(Dispensado.horasAl);

      }

     if(Dispensado.minutosAl < 10){
       lcd.setCursor(9,1);
       lcd.print(0);

     }else if(Dispensado.minutosAl >= 10){
       lcd.setCursor(9,1);
       lcd.print(Dispensado.minutosAl);

       
      }

    if(boton_lcd == up){
    Dispensado.horasAl ++;
    delay(200);
    
    
    }else if(boton_lcd == down){
      Dispensado.horasAl --;
      delay(200);
    }
    
  if(Dispensado.horasAl >= 24){
    Dispensado.horasAl = 0;
  }
  
  if(boton_lcd == right){
   partes ++;
  }
  
  if(boton_lcd == select){
    partes += 2;
    guardar_eeprom();
    delay(200);
  }
 

  
  break;
  
 
  case 3:
 //esta es la segunda parte del caso anterior para poder cambiar los minutos

 if(limpiador == 2){
  lcd.clear();
  limpiador ++;
 }
 
  lcd.setCursor(0,0);
  lcd.print("1er alimento");
  lcd.setCursor(5,1);
  lcd.print("|");
  lcd.setCursor(7,1);
  lcd.print(Dispensado.horasAl);
  lcd.setCursor(8,1);
  lcd.print(":");
  lcd.setCursor(10,1);
  lcd.print(Dispensado.minutosAl);
  lcd.setCursor(11,1);
  lcd.print("|");


  if(Dispensado.horasAl < 10){
        lcd.setCursor(6,1);
        lcd.print("0");
        
      }else if(Dispensado.horasAl >= 10){
        lcd.setCursor(6,1);
        lcd.print(Dispensado.horasAl);
       
      }

  

  if(Dispensado.minutosAl < 10){
     lcd.setCursor(9,1);
     lcd.print("0");

    }else if(Dispensado.minutosAl >= 10){
        lcd.setCursor(9,1);
        lcd.print(Dispensado.minutosAl);


      }
   
   if(boton_lcd == up){
    Dispensado.minutosAl ++;
    delay(200);
    
    }else if(boton_lcd == down){
      Dispensado.minutosAl --;
      delay(200);
    }
  
  if(Dispensado.minutosAl >= 60 and Dispensado.minutosAl < 62){
    Dispensado.horasAl ++;
    Dispensado.minutosAl = 0;
  }

   if(Dispensado.minutosAl > 62){
    Dispensado.minutosAl = 0;
  }
   
  if(boton_lcd == left){
   partes --;
  limpiador --;
  }
  
  if(boton_lcd == select){
    partes ++;
    guardar_eeprom();
    delay(200);
  }
  
  break;
  case 4:
//este caso es usado para poner la hora de la caida del segundo alimento, al igual que las otras sera divida en dos partes una para las horas y la otra para los minutos

  if(limpiador == 3){
  lcd.clear();
  limpiador ++;
 }
  
  lcd.setCursor(0,0);
  lcd.print("2do alimento");
  lcd.setCursor(5,1);
  lcd.print("|");
  lcd.setCursor(7,1);
  lcd.print(Dispensado.horasAl2);
  lcd.setCursor(8,1);
  lcd.print(":");
  lcd.setCursor(10,1);
  lcd.print(Dispensado.minutosAl2);
  lcd.setCursor(11,1);
  lcd.print("|");


if(Dispensado.horasAl2 < 10){
        lcd.setCursor(6,1);
        lcd.print("0");
        
      }else if(Dispensado.horasAl2 >= 10){
        lcd.setCursor(6,1);
        lcd.print(Dispensado.horasAl2);
       
      }

  

  if(Dispensado.minutosAl2 < 10){
     lcd.setCursor(9,1);
     lcd.print("0");

    }else if(Dispensado.minutosAl2 >= 10){
        lcd.setCursor(9,1);
        lcd.print(Dispensado.minutosAl2);


    }

      
  if(boton_lcd == up){
    Dispensado.horasAl2 ++;
    delay(200);
    
  }else if(boton_lcd == down){
    Dispensado.horasAl2 --;
    delay(200);
  }

  
  if(Dispensado.horasAl2 >= 24){
    Dispensado.horasAl2 = 0;
  }
    
  
  if(boton_lcd == right){
   partes ++;
  }
  
  if(boton_lcd == select){
    partes +=2 ;
    guardar_eeprom();
    delay(200);
  }
  
  break;
  
  case 5:
 //esta es la segunda parte del anterior caso para los minutos
 
 if(limpiador == 4){
  lcd.clear();
  limpiador ++;
 }
  
  lcd.setCursor(0,0);
  lcd.print("2do alimento");
  lcd.setCursor(5,1);
  lcd.print("|");
  lcd.setCursor(7,1);
  lcd.print(Dispensado.horasAl2);
  lcd.setCursor(8,1);
  lcd.print(":");
  lcd.setCursor(10,1);
  lcd.print(Dispensado.minutosAl2);
  lcd.setCursor(11,1);
  lcd.print("|");


if(Dispensado.horasAl2 < 10){
        lcd.setCursor(6,1);
        lcd.print("0");
        
      }else if(Dispensado.horasAl2 >= 10){
        lcd.setCursor(6,1);
        lcd.print(Dispensado.horasAl2);
       
      }

  

  if(Dispensado.minutosAl2 < 10){
     lcd.setCursor(9,1);
     lcd.print("0");

    }else if(Dispensado.minutosAl2 >= 10){
        lcd.setCursor(9,1);
        lcd.print(Dispensado.minutosAl2);


    }

   if(boton_lcd == up){
    Dispensado.minutosAl2 ++;
    delay(200);
    
  }else if(boton_lcd == down){
    Dispensado.minutosAl2 --;
    delay(200);
  }
  if(Dispensado.minutosAl2 >= 60 and Dispensado.minutosAl2 <= 62 ){
    Dispensado.horasAl ++;
    Dispensado.minutosAl2 = 0;
  }
  if(Dispensado.minutosAl2 > 62){
      Dispensado.minutosAl2 = 0;
    }
   
  if(boton_lcd == left){
   partes --;
   limpiador --;
   
  }
  
  if(boton_lcd == select){
    partes ++;
    guardar_eeprom();
    delay(200);
  }
  
  break;
  
  case 6:
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Ok en proceso");
  delay(2000);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print(myRTC.hours);
  lcd.setCursor(7,0);
  lcd.print(":");
  lcd.setCursor(8,0);
  lcd.print(myRTC.minutes);
  delay(10000);
 

  if(Dispensado.horasAl == myRTC.hours and Dispensado.minutosAl == myRTC.minutes and myRTC.seconds == 0){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Descargando");
    lcd.setCursor(5,1);
    lcd.print("comida");
    digitalWrite(A1,0);
    delay(5000);
    digitalWrite(A1,1);
    
  } if(Dispensado.horasAl2 == myRTC.hours and Dispensado.minutosAl2 == myRTC.minutes and myRTC.seconds == 0 ){
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Descargando");
    lcd.setCursor(5,1);
    lcd.print("comida");
    digitalWrite(A1,0);
    delay(5000);
    digitalWrite(A1,1);
    
  }
 
  break;
  
 
 
 }
}
