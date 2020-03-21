//Librerias
#include "U8glib.h"
#include "LowPower.h"
#include <TimerOne.h>

U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0); // I2C / TWI

//Declarar Puertos de entrada y salida
//ASCII
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
#define ReducterVal 1
//OLED Color
#define BLACK 0
#define WHITE 1
#define NONE 100

uint8_t NumeroDePantalla = 1;
uint8_t UltimaPantalla = 1;
//Varaibles Pantalla2
int ValorStep = 0;
uint8_t ValorRepeticiones = 0;
//Varaible Pantalla 4
uint8_t ContadorPantalla4 = 0;
uint8_t MostrarConteo = 0;
//Varaible Pantalla 3
uint8_t ValorCantidadAngulos = 0;
int ValorDeAngulosPantalla3[10];
uint8_t ContadorAngulosP3 = 0;
//Variables Pantalla 5
int AngulosPantalla5[10];
uint8_t CantidadTotalAngulos = 0;
int AnguloActual = 0;
uint8_t NumeroAnguloActual = 0;
boolean PermitirMensajeExtra = false;
long ContadorP5Ang = 0;

uint8_t OpcionesPantalla[4][4] =
    {
        {WHITE, BLACK, NONE, NONE},
        {WHITE, BLACK, BLACK, BLACK},
        {WHITE, BLACK, BLACK, NONE},
        {WHITE, NONE, NONE, NONE},
};

//*************Variables Pantalla 1************//
uint8_t ContadorSeleccionOpcion[5] = {3, 3, 3, 3, 3};
uint8_t P1LimiteInf[5] = {3, 9, 6, 3, 3};
uint8_t P1LimiteSup[5] = {6, 12, 9, 3, 3};

uint8_t Contador1 = 0;

uint8_t PosicionPanel[5][4] =
    {
        {2, 3, 0, 0},
        {4, 5, 7, 9},
        {4, 6, 8, 0},
        {2, 0, 0, 0},
        {0, 0, 0, 0}};

boolean Pantalla3a6 = false;
boolean Pantalla1a3 = false;
//*********VariablesDeLaPantalla*********//
uint8_t ColorFondoPanel[5][10] =
    {
        //Pantalla 1
        {BLACK, WHITE, WHITE, BLACK, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {BLACK, WHITE, BLACK, BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, BLACK},
        //Pantalla 3
        {BLACK, WHITE, BLACK, BLACK, WHITE, WHITE, BLACK, WHITE, BLACK, NONE},
        //Pantalla 4
        {WHITE, BLACK, WHITE, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {WHITE, BLACK, BLACK, BLACK, NONE, NONE, NONE, NONE, NONE, NONE},
};

uint8_t PosXPanel[5][10] =
    {
        //Pantalla 1
        {4, 4, 4, 68, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {4, 4, 4, 4, 80, 74, 4, 6, 92, 94},
        //Pantalla 3
        {4, 4, 14, 14, 84, 4, 6, 92, 94, NONE},
        //Pantalla 4
        {4, 50, 25, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {4, 15, 15, 15, NONE, NONE, NONE, NONE, NONE, NONE},
};

uint8_t PosYPanel[5][10] =
    {
        //Pantalla 1
        {2, 14, 28, 28, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {2, 14, 28, 40, 28, 40, 52, 53, 52, 53},
        //Pantalla 3
        {2, 14, 28, 40, 40, 52, 53, 52, 53, NONE},
        //Pantalla 4
        {2, 28, 52, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {2, 28, 40, 52, NONE, NONE, NONE, NONE, NONE, NONE},
};

const String StrLabel[4][10] =
    {
        //Pantalla 1
        {"CONFIGURACION", "OPCIONES", "Modo ", "Modo ", "", "", "", "", "", ""},
        //Pantalla 2
        {"PASO A PASO", "DEFINIR VALORES", "Valor de Paso:", "Repeticiones", "0", "0", "", "Menu", "", "Start"},
        //Pantalla 3
        {"SECUENCIA ANGULOS", "Definir Valores", "Cantidad de", "Ángulos", "0", "", "Menu", "", "Ang", ""},
        //Pantalla 4
        {"INICIO ROTACION", "5", "Sec de Ang", "", "", "", "", "", "", ""}
        /*// Pantalla 5
  {"ROTACION", "Pos Angular", "Numero de", "Movimiento", "", "", "", "", "", ""},
  // Pantalla 6
  {"a", "a", "a", "a", "a", "a", "a", "", "", ""}*/
};

const String StrLabel2[10] = {"SECUENCIA ANGULOS", "Definir Angulos", "Valor de", "Ángulo", "0", "", "Cant", "", "Ang", ""};

uint8_t AnchoPanel[5][10] =
    {
        //Pantalla 1
        {120, 120, 56, 56, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {120, 120, 70, 70, 20, 20, 30, 26, 30, 26},
        //Pantalla 3
        {120, 120, 70, 70, 20, 30, 26, 30, 26, NONE},
        //Pantalla 4
        {120, 10, 70, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {120, 70, 70, 70, NONE, NONE, NONE, NONE, NONE},
};

uint8_t LargoPanel[6][10] =
    {
        //Pantalla 1
        {12, 12, 36, 36, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {12, 12, 10, 10, 10, 10, 10, 8, 10, 8},
        //Pantalla 3
        {12, 12, 10, 10, 10, 10, 8, 10, 8, NONE},
        //Pantalla 4
        {22, 20, 10, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {22, 10, 10, 10, NONE, NONE, NONE, NONE, NONE, NONE},
};

uint8_t TipoLetra[6][10] =
    {
        //Pantalla 1
        {1, 2, 2, 2, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {1, 2, 2, 2, 2, 2, 2, 3, 2, 3},
        //Pantalla 3
        {1, 2, 2, 2, 2, 2, 3, 2, 3, NONE},
        //Pantalla 4
        {1, 1, 2, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {1, 2, 2, 2, NONE, NONE, NONE, NONE, NONE, NONE},
};

uint8_t PosXLabel[6][10] =
    {
        //Pantalla 1
        {5, 5, 5, 69, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {4 + 4, 4 + 10, 4 + 2, 4 + 2, 80 + 2, 74 + 2, 4, 6 + 2, 92, 94 + 2},
        //Pantalla 3
        {8, 14, 14, 14, 86, 4, 8, 92, 96, NONE},
        //Pantalla 4
        {8, 52, 30, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {8, 17, 17, 17, BLACK, WHITE, BLACK, NONE, NONE, BLACK},
};

uint8_t PosYLabel[6][10] =
    {
        //Pantalla 1
        {13, 23, 36, 36, NONE, NONE, NONE, NONE, NONE, NONE},
        //Pantalla 2
        {12, 24, 36, 48, 36, 48, 52, 59, 52, 59},
        //Pantalla 3
        {12, 24, 36, 48, 48, 52, 59, 52, 59, NONE},
        //Pantalla 4
        {20, 46, 61, NONE, NONE, NONE, NONE, NONE, NONE, NONE},
        // Pantalla 5
        {20, 36, 48, 60, NONE, NONE, NONE, NONE, NONE, NONE},
};

uint8_t ArrayLimits[6] = {4, 10, 9, 3, 4, 1};

//Varaible para el timer Interrupt
boolean AutorizarGiro = false;
boolean UltimoEstado = false;
int ContadorGiro = 0;
#define TiempoStep 50000

//Variables de OLED
#define WHITE 1
#define BLACK 0

int ContadorInt = 0;

//Timer Interrupt***********
void timerInterupt()
{
  if (AutorizarGiro == true)
  {
    //ContadorGiro = 0;
    if (digitalRead(SWITCH) == 0)
    {
      AutorizarGiro = false;
    }
  }
  if (NumeroDePantalla == 4)
  {
    ContadorPantalla4++;
    MostrarConteo = ContadorPantalla4 / 20;
  }
  if (NumeroDePantalla == 5)
  {
    if (!PermitirMensajeExtra)
    {
      int DirG = 0;
      if (AnguloActual > AngulosPantalla5[NumeroAnguloActual-1])
      {
        ContadorP5Ang--;
        DirG = 1;
        RotarMotor(DirG);
      }
      else if (AnguloActual < AngulosPantalla5[NumeroAnguloActual-1])
      {
        ContadorP5Ang++;
        DirG = 2;
        RotarMotor(DirG);
      }
      AnguloActual = ContadorP5Ang*ReducterVal;
      if (AnguloActual > 359)
      {
        AnguloActual = 0;
        ContadorP5Ang = 0;
      }
      else if (AnguloActual < 0)
      {
        AnguloActual = 359;
        ContadorP5Ang = 359;
      }
      if (AnguloActual == AngulosPantalla5[NumeroAnguloActual-1])
      {
        PermitirMensajeExtra = true;
        RotarMotor(DirG);
      }
    }
    
  }
}

void RotarMotor(int a)
{
    if (a == 1)
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
    else if (a == 2)
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
    else if (a == 0)
    {
      ApagarSalidas();
      //AutorizarGiro = false;
    }
}

void setup()
{
  // put your setup code here, to run once:
  Initialize();
}

void loop()
{
  // put your main code here, to run repeatedly:
  //Leer y Mostrar Grados de Movimiento
  display.firstPage();
  do
  {
    PantallaMonitor();
  } while (display.nextPage());
  //Verificar Si el buton Switch fue encendido
  if (digitalRead(SWITCH) == 1 && !AutorizarGiro)
  {
    if (NumeroDePantalla != 5)
    {
      AutorizarGiro = true;
      SeleccionPantallas();
    }
    else if (NumeroDePantalla == 5 && PermitirMensajeExtra)
    {
      PermitirMensajeExtra = false;
      NumeroAnguloActual++;
      if (NumeroAnguloActual-1 == CantidadTotalAngulos)
      {
        SeleccionPantallas();
      }
    }
  }
  if (NumeroDePantalla == 4)
  {
    if (int(5 - MostrarConteo) < 0)
    {
      SeleccionPantallas();
    }
  }
  delay(10);
}

void Initialize()
{
  Serial.begin(9600);
  // Rotary encoder pins
  pinMode(OUTB, INPUT);
  pinMode(OUTA, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), ContadorEncoder, RISING);
  pinMode(SWITCH, INPUT_PULLUP);
  //Stepper Motor Pins
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  //
  ApagarSalidas();
  Timer1.initialize(TiempoStep);
  Timer1.attachInterrupt(timerInterupt);
  if (display.getMode() == U8G_MODE_R3G3B2)
    display.setColorIndex(255); // white
  else if (display.getMode() == U8G_MODE_GRAY2BIT)
    display.setColorIndex(3); // max intensity
  else if (display.getMode() == U8G_MODE_BW)
    display.setColorIndex(1); // pixel on
  else if (display.getMode() == U8G_MODE_HICOLOR)
    display.setHiColorByRGB(255, 255, 255);
}

//***************Funciones

void SeleccionPantallas()
{
  //Verificar pantalla Actual
  switch (NumeroDePantalla)
  {
  case 1:
    if (ContadorSeleccionOpcion[NumeroDePantalla - 1] >= 3 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 9)
    {
      int SavedVal = ContadorSeleccionOpcion[NumeroDePantalla - 1];
      ContadorSeleccionOpcion[NumeroDePantalla - 1] = 3;
      ActPantalla();
      if (SavedVal < 6)
      {
        UltimaPantalla = NumeroDePantalla;
        NumeroDePantalla = 2;
      }
      else if (SavedVal == 6)
      {
        UltimaPantalla = NumeroDePantalla;
        NumeroDePantalla = 3;
      }
      ResetPantalla();
    }
    break;
  case 2:
    if (ContadorSeleccionOpcion[NumeroDePantalla - 1] >= 3 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 9)
    {
      ContadorSeleccionOpcion[NumeroDePantalla - 1] = ContadorSeleccionOpcion[NumeroDePantalla - 1] + 3;
      ActPantalla();
    }
    else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] >= 9 && ContadorSeleccionOpcion[NumeroDePantalla - 1] <= 12)
    {
      int SavedVal = ContadorSeleccionOpcion[NumeroDePantalla - 1];
      ContadorSeleccionOpcion[NumeroDePantalla - 1] = 3;
      ActPantalla();
      if (SavedVal < 12)
      {
        UltimaPantalla = NumeroDePantalla;
        NumeroDePantalla = 1;
        ValorStep = 0;
        ValorRepeticiones = 0;
      }
      else if (SavedVal == 12)
      {
        if (ValorRepeticiones > 0)
        {
          UltimaPantalla = 2;
          NumeroDePantalla = 4;
        }
      }
      ResetPantalla();
    }
    break;
  case 3:
    if (ContadorSeleccionOpcion[NumeroDePantalla - 1] >= 3 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 6)
    {
      ContadorSeleccionOpcion[NumeroDePantalla - 1] = ContadorSeleccionOpcion[NumeroDePantalla - 1] + 3;
      ActPantalla();
    }
    else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] >= 6 && ContadorSeleccionOpcion[NumeroDePantalla - 1] <= 9)
    {
      int SavedVal = ContadorSeleccionOpcion[NumeroDePantalla - 1];
      ContadorSeleccionOpcion[NumeroDePantalla - 1] = 3;
      ActPantalla();
      if (SavedVal < 9)
      {
        if (ContadorAngulosP3 == 0)
        {
          UltimaPantalla = NumeroDePantalla;
          NumeroDePantalla = 1;
          ValorCantidadAngulos = 0;
        }
        else
        {
          UltimaPantalla = NumeroDePantalla;
          NumeroDePantalla = 3;
          ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] = 0;
          ContadorAngulosP3--;
        }
      }
      else if (SavedVal == 9 && ValorCantidadAngulos != 0)
      {
        if (ContadorAngulosP3 < ValorCantidadAngulos)
        {
          UltimaPantalla = NumeroDePantalla;
          NumeroDePantalla = 3;
          ContadorAngulosP3++;
        }
        else if (ContadorAngulosP3 == ValorCantidadAngulos)
        {
          UltimaPantalla = 3;
          NumeroDePantalla = 4;
        }
      }
      ResetPantalla();
    }
    break;
  case 4:
    if (int(5 - MostrarConteo) < 0)
    {
      if (UltimaPantalla == 2)
      {
        CantidadTotalAngulos = ValorRepeticiones;
        for (int a = 0; a < CantidadTotalAngulos; a++)
        {
          AngulosPantalla5[a] = ((a + 1) * ValorStep) % 360;
        }
        ValorRepeticiones=0;
        ValorStep=0;
      }
      else if (UltimaPantalla == 3)
      {
        CantidadTotalAngulos = ValorCantidadAngulos;
        for (int a = 0; a < CantidadTotalAngulos; a++)
        {
          AngulosPantalla5[a] = ValorDeAngulosPantalla3[a];
          ValorDeAngulosPantalla3[a] =0;
        }
        ValorCantidadAngulos=0;
      }
      NumeroDePantalla = 5;
      UltimaPantalla = 4;
      ContadorPantalla4 = 0;
      MostrarConteo = 0;
      PermitirMensajeExtra = false;
      NumeroAnguloActual = 1;
      ResetPantalla();
    }
    else
    {
      NumeroDePantalla = UltimaPantalla;
      UltimaPantalla = 4;
      ContadorPantalla4 = 0;
      MostrarConteo = 0;
      ResetPantalla();
    }

    break;
  case 5:
    for (int t = 0; t< 10; t++)
    {
      AngulosPantalla5[t] = 0;
    }
    CantidadTotalAngulos = 0;
    AnguloActual = 0;
    NumeroAnguloActual = 0;
    PermitirMensajeExtra = false;
    ContadorP5Ang = 0;
    NumeroDePantalla = 1;
    UltimaPantalla = 5;
    ResetPantalla();
    break;
  default:
    break;
  }
}

void ResetPantalla()
{
  display.firstPage();
  do
  {
    display.setColorIndex(BLACK);
    display.drawBox(0, 0, 128, 64);
  } while (display.nextPage());
}

// Pantalla del Sitema
void PantallaMonitor()
{
  //*****************Establecer Pantalla *************************//
  //[NumeroDePantalla - 1][K];
  for (int K = 0; K < ArrayLimits[NumeroDePantalla - 1]; K++)
  {
    display.setColorIndex(ColorFondoPanel[NumeroDePantalla - 1][K]);
    display.drawBox(PosXPanel[NumeroDePantalla - 1][K], PosYPanel[NumeroDePantalla - 1][K], AnchoPanel[NumeroDePantalla - 1][K], LargoPanel[NumeroDePantalla - 1][K]);
    display.setColorIndex(CambiarColor(ColorFondoPanel[NumeroDePantalla - 1][K]));
    /*StrLabel[NumeroDePantalla - 1][K]*/
    MostrarString(TipoLetra[NumeroDePantalla - 1][K], K, PosXLabel[NumeroDePantalla - 1][K], PosYLabel[NumeroDePantalla - 1][K], 0);
    if ((NumeroDePantalla == 1 && (K == 2 || K == 3)))
    {
      if (NumeroDePantalla == 1)
      {
        if (K == 2)
        {
          for (int k1 = 1; k1 <= 2; k1++)
          {
            MostrarString(TipoLetra[NumeroDePantalla - 1][K], k1 - 1, PosXLabel[NumeroDePantalla - 1][K], PosYLabel[NumeroDePantalla - 1][K] + 12 * k1, 1);
          }
        }
        else if (K == 3)
        {
          for (int k1 = 1; k1 <= 2; k1++)
          {
            MostrarString(TipoLetra[NumeroDePantalla - 1][K], k1 + 1, PosXLabel[NumeroDePantalla - 1][K], PosYLabel[NumeroDePantalla - 1][K] + 12 * k1, 1);
          }
        }
      }
    }
    if (NumeroDePantalla == 5 && K == 3)
    {
      display.setColorIndex(WHITE);
      display.drawDisc(127 - 15, 63 - 15, 10);
      if (PermitirMensajeExtra)
      {
        display.setColorIndex(BLACK);
        display.drawDisc(127 - 15, 63 - 15, 7);
        /*for (int k1 = 1; k1 <= 1; k1++)
        {
          display.setColorIndex(WHITE);
          //display.drawBox(127 - 15 - 10 - 4 * k1 - 2, 63 - 15 - (k1 + 1) * 10 - 5 - 8, 25 + 4 * k1, 10);
          MostrarString(3, k1, 127 - 15 - 10 - 4 * k1, 63 - 15 - (k1 + 1) * 10 - 5, 1);
        }*/
      }
      else if (!PermitirMensajeExtra)
      {
        /*for (int k1 = 0; k1 <= 1; k1++)
        {
          display.setColorIndex(BLACK);
          display.drawBox(127 - 15 - 10 - 4 * k1 - 2, 63 - 15 - (k1 + 1) * 10 - 5 - 8, 25 + 4 * k1, 10);
          //MostrarString(3, k1 , 127-15-10 - 4*k1, 63-15-(k1+1)*10-5, 1);
        }*/
      }
    }
  }
}

void MostrarString(int a, int k1, int Px, int Py, int b)
{
  if (a == 1)
  {
    display.setFont(u8g_font_7x14B);
  }
  else if (a == 2)
  {
    display.setFont(u8g_font_helvB08);
  }
  else if (a == 3)
  {
    display.setFont(u8g_font_5x7);
  }
  //Serial.println(DataToShow);
  char ValueOfChar[20];
  if (b == 0)
  {
    if (NumeroDePantalla == 1)
    {
      StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
    }
    else if (NumeroDePantalla == 2)
    {
      String DataShow;
      if (k1 == 4)
      {
        int abc = ValorStep;
        itoa(abc, ValueOfChar, 10);
      }
      else if (k1 == 5)
      {
        int abc = ValorRepeticiones;
        itoa(abc, ValueOfChar, 10);
      }
      else
      {
        StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
      }
    }
    else if (NumeroDePantalla == 4)
    {
      if (k1 == 1)
      {
        int abc = 5 - MostrarConteo;
        if (abc < 0)
        {
          abc = 0;
        }
        itoa(abc, ValueOfChar, 10);
      }
      else if (k1 == 2)
      {
        int Data[20];
        for (int t = 0; t < 20; t++)
        {
          Data[t] = ' ';
        }
        if (UltimaPantalla == 2)
        {
          Data[0] = 'P';
          Data[1] = 'a';
          Data[2] = 's';
          Data[3] = 'o';
          Data[4] = ' ';
          Data[5] = 'a';
          Data[6] = ' ';
          Data[7] = 'P';
          Data[8] = 'a';
          Data[9] = 's';
          Data[10] = 'o';
        }
        else if (UltimaPantalla == 3)
        {
          Data[0] = 'S';
          Data[1] = 'e';
          Data[2] = 'c';
          Data[3] = ' ';
          Data[4] = 'd';
          Data[5] = 'e';
          Data[6] = ' ';
          Data[7] = 'A';
          Data[8] = 'n';
          Data[9] = 'g';
        }
        for (int t = 0; t < 20; t++)
        {
          //Serial.println(Data[t]);
          ValueOfChar[t] = Data[t];
        }
      }
      else
      {
        StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
      }
    }
    else if (NumeroDePantalla == 5)
    {
      int Data[20];
      for (int t = 0; t < 20; t++)
      {
        Data[t] = ' ';
      }
      if (k1 == 0 )
      {
        Data[0] = 'R';
        Data[1] = 'O';
        Data[2] = 'T';
        Data[3] = 'A';
        Data[4] = 'C';
        Data[5] = 'I';
        Data[6] = 'O';
        Data[7] = 'N';
        
      }
      else if (k1 == 1)
      {
        int D[12] = {'P', 'o', 's', ' ', 'a', 'n', 'g', ':', ' '};
        int DDD = AnguloActual;
        for (int y = 9; y <= 11; y++)
        {
          if (DDD / (pow(10, (11 - y))) >= 1)
          {
            D[y] = DDD / (pow(10, (11 - y))) + '0';
            if (DDD/100 >= 1 || DDD/10 >= 1 && DDD/10 < 10 || DDD/1 >= 1 && DDD/1 < 10)
            {
              int sddd = (pow(10, (11 - y)));
              DDD = DDD % sddd;
            }
            
          }
          else if (DDD / (pow(10, (11 - y))) == 0 )
          {
            D[y] = '0';
          }
          else
          {
            D[y] = ' ';
          }
        }
        for (int t = 0; t < 12; t++)
        {
          Data[t] = D[t];
        }
      }
      else if (k1 == 2)
      {
        int D[9] = {'N', 'u', 'm', 'e', 'r', 'o', ' ', 'd', 'e'};
        for (int t = 0; t < 9; t++)
        {
          Data[t] = D[t];
        }
      }
      else if (k1 == 3)
      {
        int D[14] = {'M', 'o', 'v', 'i', 'm', 'i', 'e', 'n', 't', 'o', ':', ' '};
        int DDD = NumeroAnguloActual;
        for (int y = 12; y <= 13; y++)
        {
          if (DDD / (pow(10, (13 - y))) >= 1)
          {
            D[y] = DDD / (pow(10, (13 - y))) + '0';
            if (DDD/10 >= 1 && DDD/10 < 10 || DDD/1 >= 1 && DDD/1 < 10)
            {
              int sddd = (pow(10, (11 - y)));
              DDD = DDD % sddd;
            }
          }
          else if (DDD / (pow(10, (11 - y))) == 0 && y == 13)
          {
            D[y] = '0';
          }
          else
          {
            D[y] = ' ';
          }
        }
        for (int t = 0; t < 14; t++)
        {
          Data[t] = D[t];
        }
      }
      for (int t = 0; t < 20; t++)
        {
          //Serial.println(Data[t]);
          ValueOfChar[t] = Data[t];
        }
      //DataShow.toCharArray(ValueOfChar, 20);
    }
    else if (NumeroDePantalla == 3)
    {
      /*String DataShow;
      if (ContadorAngulosP3 == 0)
      {
        if (k1 == 4)
        {
          int abc = ValorCantidadAngulos;
          itoa(abc, ValueOfChar, 10);
        }
        else
        {
          StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
        }
      }
      else */
      if (ContadorAngulosP3 >= 0)
      {
        if (k1 == 1)
        {
          if (ContadorAngulosP3 > 0)
          {
            int Data[20] = {'D', 'e', 'f', 'i', 'n', 'i', 'r', ' ', 'V', 'a', 'l', 'o', 'r', 'e', 's'};
            for (int t = 0; t < 20; t++)
            {
              //Serial.println(Data[t]);
              ValueOfChar[t] = Data[t];
            }
          }
          else if (ContadorAngulosP3 == 0)
          {
            StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
          }
        }
        else if (k1 == 3)
        {
          int Data[20] = {'A', 'n', 'g', 'u', 'l', 'o', '1', '1', '1', ':'};
          if (ContadorAngulosP3 == 0)
          {
            Data[6] = 's';
            Data[7] = ':';
            Data[8] = ' ';
            Data[9] = ' ';
          }
          else if (ContadorAngulosP3 == 10)
          {
            Data[6] = '1';
            Data[7] = '0';
            Data[8] = ' ';
          }
          else if (ContadorAngulosP3 < 10)
          {
            Data[6] = ' ';
            Data[7] = ContadorAngulosP3 + '0';
            ;
            Data[8] = ' ';
          }
          for (int t = 0; t < 20; t++)
          {
            //Serial.println(Data[t]);
            ValueOfChar[t] = Data[t];
          }
        }
        else if (k1 == 4)
        {
          if (ContadorAngulosP3 > 0)
          {
            int abc = ValorDeAngulosPantalla3[ContadorAngulosP3 - 1];
            itoa(abc, ValueOfChar, 10);
          }
          else if (ContadorAngulosP3 == 0)
          {
            int abc = ValorCantidadAngulos;
            itoa(abc, ValueOfChar, 10);
          }
        }
        else if (k1 == 6)
        {
          int Data[20];
          for (int t = 0; t < 20; t++)
          {
            Data[t] = ' ';
          }
          if (ContadorAngulosP3 == 0)
          {
            Data[0] = 'M';
            Data[1] = 'e';
            Data[2] = 'n';
            Data[3] = 'u';
            Data[4] = ' ';
            Data[5] = ' ';
            Data[6] = ' ';
            Data[7] = ' ';
          }
          else if (ContadorAngulosP3 == 1)
          {
            Data[0] = 'S';
            Data[1] = 'e';
            Data[2] = 'c';
            Data[3] = ' ';
            Data[4] = ' ';
            Data[5] = ' ';
            Data[6] = ' ';
            Data[7] = ' ';
          }
          else if (ContadorAngulosP3 > 1)
          {
            Data[0] = 'A';
            Data[1] = 'n';
            Data[2] = 'g';
            Data[3] = (ContadorAngulosP3 - 1) + '0';
            Data[4] = ' ';
            Data[5] = ' ';
            Data[6] = ' ';
            Data[7] = ' ';
          }
          for (int t = 0; t < 20; t++)
          {
            //Serial.println(Data[t]);
            ValueOfChar[t] = Data[t];
          }
        }
        else if (k1 == 8)
        {
          int Data[20];
          for (int t = 0; t < 20; t++)
          {
            Data[t] = ' ';
          }
          if (ContadorAngulosP3 == 0)
          {
            Data[0] = 'I';
            Data[1] = 'n';
            Data[2] = 'i';
            Data[3] = 't';
            Data[4] = ' ';
            Data[5] = ' ';
            Data[6] = ' ';
            Data[7] = ' ';
          }
          else if (ContadorAngulosP3 == ValorCantidadAngulos)
          {
            Data[0] = 'S';
            Data[1] = 't';
            Data[2] = 'a';
            Data[3] = 'r';
            Data[4] = 't';
            Data[5] = ' ';
            Data[6] = ' ';
            Data[7] = ' ';
          }
          else if (ContadorAngulosP3 < ValorCantidadAngulos)
          {
            Data[0] = 'A';
            Data[1] = 'n';
            Data[2] = 'g';
            if ((ContadorAngulosP3 + 1) < 10)
            {
              Data[3] = (ContadorAngulosP3 + 1) + '0';
              Data[4] = ' ';
              Data[5] = ' ';
              Data[6] = ' ';
              Data[7] = ' ';
              Data[8] = ' ';
            }
            else if ((ContadorAngulosP3 + 1) == 10)
            {
              Data[3] = '1';
              Data[4] = '0';
              Data[5] = ' ';
              Data[6] = ' ';
              Data[7] = ' ';
              Data[8] = ' ';
            }
          }
          for (int t = 0; t < 20; t++)
          {
            //Serial.println(Data[t]);
            ValueOfChar[t] = Data[t];
          }
        }
        else if (k1 == 2 || k1 == 0)
        {
          if (ContadorAngulosP3 > 0)
          {
            StrLabel2[k1].toCharArray(ValueOfChar, 20);
          }
          else if (ContadorAngulosP3 == 0)
          {
            StrLabel[NumeroDePantalla - 1][k1].toCharArray(ValueOfChar, 20);
          }
        }
        else
        {
          int Data[20];
          for (int t = 0; t < 20; t++)
          {
            //Serial.println(Data[t]);
            ValueOfChar[t] = 0;
          }
        }
      }
    }
  }
  else if (b == 1)
  {
    int Data[20];
    for (int t = 0; t < 20; t++)
    {
      Data[t] = ' ';
    }
    if (NumeroDePantalla == 1)
    {
      if (k1 == 0)
      {
        Data[0] = 'P';
        Data[1] = 'a';
        Data[2] = 's';
        Data[3] = 'o';
        Data[4] = ' ';
        Data[5] = 'a';
      }
      else if (k1 == 1)
      {
        Data[0] = ' ';
        Data[1] = 'P';
        Data[2] = 'a';
        Data[3] = 's';
        Data[4] = 'o';
      }
      else if (k1 == 2)
      {
        Data[0] = 'S';
        Data[1] = 'e';
        Data[2] = 'c';
        Data[3] = 'u';
        Data[4] = 'e';
        Data[5] = 'n';
        Data[6] = 'c';
        Data[7] = 'i';
        Data[8] = 'a';
        Data[9] = ' ';
      }
      else if (k1 == 3)
      {
        Data[0] = 'D';
        Data[1] = 'e';
        Data[2] = ' ';
        Data[3] = 'A';
        Data[4] = 'n';
        Data[5] = 'g';
        Data[6] = 'u';
        Data[7] = 'l';
        Data[8] = 'o';
        Data[9] = 's';
      }
      for (int t = 0; t < 20; t++)
      {
        //Serial.println(Data[t]);
        ValueOfChar[t] = Data[t];
      }
    }
    if (NumeroDePantalla == 5)
    {
      if (k1 == 0)
      {
        Data[0] = 'P';
        Data[1] = 'r';
        Data[2] = 'e';
        Data[3] = 's';
        Data[4] = 'i';
        Data[5] = 'o';
        Data[6] = 'n';
        Data[7] = 'a';
        Data[8] = 'r';
      }
      else if (k1 == 1)
      {
        Data[0] = 'B';
        Data[1] = 'o';
        Data[2] = 't';
        Data[3] = 'o';
        Data[4] = 'n';
      }
    }
  }
  display.drawStr(Px, Py, ValueOfChar);
}

//Cambiar Color
int CambiarColor(uint8_t a)
{
  if (a == WHITE)
  {
    return BLACK;
  }
  else if (a == BLACK)
  {
    return WHITE;
  }
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
  if (AutorizarGiro == false && NumeroDePantalla != 5)
  {
    if (digitalRead(OUTB) == 1)
    {
      if (NumeroDePantalla == 2 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 9)
      {
        if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
        {
          if (ValorStep > 0)
          {
            ValorStep--;
          }
          else if (ValorStep == 0)
          {
            ValorStep = 359;
          }
        }
        else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 6)
        {
          if (ValorRepeticiones > 0)
          {
            ValorRepeticiones--;
          }
          else if (ValorRepeticiones == 0)
          {
            ValorRepeticiones = 10;
          }
        }
      }
      else if (NumeroDePantalla == 3 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 6)
      {
        if (ContadorAngulosP3 == 0)
        {
          if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
          {
            if (ValorCantidadAngulos > 0)
            {
              ValorCantidadAngulos--;
            }
            else if (ValorCantidadAngulos == 0)
            {
              ValorCantidadAngulos = 10;
            }
          }
        }
        else if (ContadorAngulosP3 > 0)
        {
          if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
          {
            if (ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] > 0)
            {
              ValorDeAngulosPantalla3[ContadorAngulosP3 - 1]--;
            }
            else if (ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] == 0)
            {
              ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] = 359;
            }
          }
        }
      }
      else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] > P1LimiteInf[NumeroDePantalla - 1])
      {
        ContadorSeleccionOpcion[NumeroDePantalla - 1]--;
      }
    }
    else if (digitalRead(OUTB) == 0)
    {
      if (NumeroDePantalla == 2 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 9)
      {
        if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
        {
          if (ValorStep < 359)
          {
            ValorStep++;
          }
          else if (ValorStep == 359)
          {
            ValorStep = 0;
          }
        }
        else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 6)
        {
          if (ValorRepeticiones < 10)
          {
            ValorRepeticiones++;
          }
          else if (ValorRepeticiones == 10)
          {
            ValorRepeticiones = 0;
          }
        }
      }
      else if (NumeroDePantalla == 3 && ContadorSeleccionOpcion[NumeroDePantalla - 1] < 6)
      {
        if (ContadorAngulosP3 == 0)
        {
          if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
          {
            if (ValorCantidadAngulos < 10)
            {
              ValorCantidadAngulos++;
            }
            else if (ValorCantidadAngulos == 10)
            {
              ValorCantidadAngulos = 0;
            }
          }
        }
        else if (ContadorAngulosP3 > 0)
        {
          if (ContadorSeleccionOpcion[NumeroDePantalla - 1] == 3)
          {
            if (ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] < 359)
            {
              ValorDeAngulosPantalla3[ContadorAngulosP3 - 1]++;
            }
            else if (ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] == 359)
            {
              ValorDeAngulosPantalla3[ContadorAngulosP3 - 1] = 0;
            }
          }
        }
      }
      else if (ContadorSeleccionOpcion[NumeroDePantalla - 1] < P1LimiteSup[NumeroDePantalla - 1])
      {
        ContadorSeleccionOpcion[NumeroDePantalla - 1]++;
      }
    }
    if (NumeroDePantalla <= 4)
    {
      ActPantalla();
    }
  }
}
void ActPantalla()
{
  Contador1 = 0;
  int k2 = 0;
  while (PosicionPanel[NumeroDePantalla - 1][k2] != 0 && Contador1 <= 3)
  {
    if (ContadorSeleccionOpcion[NumeroDePantalla - 1] - (3 * (k2 + 1)) >= 0 && ContadorSeleccionOpcion[NumeroDePantalla - 1] - (3 * (k2 + 1)) <= 2)
    {
      ColorFondoPanel[NumeroDePantalla - 1][PosicionPanel[NumeroDePantalla - 1][k2]] = WHITE;
    }
    else
    {
      ColorFondoPanel[NumeroDePantalla - 1][PosicionPanel[NumeroDePantalla - 1][k2]] = BLACK;
    }
    if (NumeroDePantalla == 2)
    {
      Serial.println("Pos:" + String(Contador1) + " " + String(ColorFondoPanel[NumeroDePantalla - 1][PosicionPanel[NumeroDePantalla - 1][k2]]));
    }
    k2++;
    Contador1++;
  }
}
