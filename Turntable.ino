//Librerias
#include "U8glib.h"
#include "LowPower.h"
#include <TimerOne.h>

U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE|U8G_I2C_OPT_DEV_0);  // I2C / TWI

//Declarar Puertos de entrada y salida
//Salidas para el motor
#define ENA 8
#define IN1 9
#define IN2 10
#define IN3 11
#define IN4 12
#define ENB 13

//Salidas para el OLED 
#define SDA A4
#define SDL A5

//Entradas de Encoder rotatorio
#define OUTB 4
#define OUTA 2
#define SWITCH 3

//Variables del sistema
int ContadorDePulsosEncoder = 0;
int UltimoValorDePulsos = 0;
int SeguimientoMov = 0;
int ContadorRotacion = 1;
#define ValorMinPulsos 0
#define ValorMaxPulsos 200

//Varaible para el timer Interrupt
boolean AutorizarGiro = false;
boolean UltimoEstado = false;
int ContadorGiro = 0;

//Variables de OLED
#define WHITE 1
#define BLACK 0

//Timer Interrupt***********
void timerInterupt()
{
  if (AutorizarGiro == true)
  {
    if (UltimoValorDePulsos > SeguimientoMov)
    {
      if (ContadorRotacion == 1)
      {
        Secuencia1N();
      }
      else if (ContadorRotacion == 2)
      {
        Secuencia2N();
      }
      else if (ContadorRotacion == 3)
      {
        Secuencia3N();
      }
      else if (ContadorRotacion == 4)
      {
        Secuencia4N();
      }
      ContadorRotacion++;
      if (ContadorRotacion > 4)
      {
        ContadorRotacion = 1;
      }
      UltimoValorDePulsos--;
    }
    else if (UltimoValorDePulsos < SeguimientoMov)
    {
      if (ContadorRotacion == 1)
      {
        Secuencia1();
      }
      else if (ContadorRotacion == 2)
      {
        Secuencia2();
      }
      else if (ContadorRotacion == 3)
      {
        Secuencia3();
      }
      else if (ContadorRotacion == 4)
      {
        Secuencia4();
      }
      ContadorRotacion++;
      if (ContadorRotacion > 4)
      {
        ContadorRotacion = 1;
      }
      UltimoValorDePulsos++;
      //Serial.println("Cont:" + String(UltimoValorDePulsos));
    }
    else if (UltimoValorDePulsos == SeguimientoMov)
    {
      ApagarSalidas();
      AutorizarGiro = false;
    }
    //ContadorGiro = 0;
  }
}

void setup() {
  // put your setup code here, to run once:
  Initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Leer y Mostrar Grados de Movimiento
  display.firstPage();
  //long to = millis();
  do {
    PantallaMonitor();
  } while ( display.nextPage() );
  //Verificar Si el buton Switch fue encendido
  if (digitalRead(SWITCH) == 1 && !AutorizarGiro)
  {
    AutorizarGiro = true;
    ContadorGiro = 500;
  }
  delay(10);
}

void Initialize()
{
  Serial.begin(9600);
  // Rotary encoder pins
  pinMode(OUTB,INPUT);
  pinMode(OUTA,INPUT);
  attachInterrupt(digitalPinToInterrupt(2), ContadorEncoder, RISING);
  pinMode(SWITCH,INPUT_PULLUP);
  //Stepper Motor Pins
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENB,OUTPUT);
  //
  ApagarSalidas();
  Timer1.initialize(50000);
  Timer1.attachInterrupt(timerInterupt);
  if (display.getMode() == U8G_MODE_R3G3B2 ) display.setColorIndex(255);  // white
  else if (display.getMode() == U8G_MODE_GRAY2BIT) display.setColorIndex(3);         // max intensity
  else if (display.getMode() == U8G_MODE_BW) display.setColorIndex(1);         // pixel on
  else if (display.getMode() == U8G_MODE_HICOLOR) display.setHiColorByRGB(255, 255, 255);
}

//***************Funciones
// Pantalla del Sitema
void PantallaMonitor()
{
  //Colocar Titulo
  display.setColorIndex(WHITE);
  display.drawBox(5, 5, 115, 20);
  display.setColorIndex(BLACK);
  display.setFont(u8g_font_helvB14);
  String DataToShow = "Rotacion";
  char ValueOfChar[20];
  DataToShow.toCharArray(ValueOfChar,20);
  display.drawStr(7,18, ValueOfChar);
  // Colocar Valor En Degree
  double Deg = 0;
  Deg = ContadorDePulsosEncoder*1.8;
  char ValueOfChar2[10];
  dtostrf(Deg,3,1,ValueOfChar2);
  display.setColorIndex(BLACK);
  display.drawBox(5, 22, 115, 40);
  display.setColorIndex(WHITE);
  display.setFont(u8g_font_helvB14);
  display.drawStr(5,50, "D: ");
  display.drawStr(35,50, ValueOfChar2);
}

//Secuencias del sistema
void ApagarSalidas()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}


void Secuencia1()
{
  digitalWrite(IN4, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
}

void Secuencia2()
{
  digitalWrite(IN1, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, HIGH);
}

void Secuencia3()
{
  digitalWrite(IN3, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN2, HIGH);
}

void Secuencia4()
{
  digitalWrite(IN2, LOW); 
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN4, HIGH);
}


void Secuencia4N()
{
  digitalWrite(IN3, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
}

void Secuencia3N()
{
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, HIGH);
}

void Secuencia2N()
{
  digitalWrite(IN4, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(ENA, HIGH);
  digitalWrite(IN2, HIGH);
}

void Secuencia1N()
{
  digitalWrite(IN1, LOW); 
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN4, HIGH);
}

//****************Interrupcion
void ContadorEncoder()
{
  if (AutorizarGiro == false)
  {
    if (digitalRead(OUTB) == 1)
    {
      ContadorDePulsosEncoder--;
      SeguimientoMov--;
    }
    else if (digitalRead(OUTB) == 0 )
    {
      ContadorDePulsosEncoder++;
      SeguimientoMov++;
    }
    if (ContadorDePulsosEncoder >= ValorMaxPulsos)
    {
      ContadorDePulsosEncoder = ValorMinPulsos + 1;
    }
    else if (ContadorDePulsosEncoder <= ValorMinPulsos)
    {
      ContadorDePulsosEncoder = ValorMaxPulsos - 1;
    }
    //Serial.println("Cont:" + String(ContadorDePulsosEncoder));
  }
}
