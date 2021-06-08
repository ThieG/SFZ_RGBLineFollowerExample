/*!
****************************************************************************************************
* \file      SFZ_RGBLineFollowerExample.ino
*
* \ingroup   RESCUE_LINE
*
*            Arduino Software als Beispiel, wie so ein Programm funktionieren und aussehen kann.
* 
*            Hier ist die Dokumentation von Makeblock zu finden: 
*            http://docs.makeblock.com/diy-platform/en/electronic-modules/sensors/rgb-line-follower.html
*
* \note      Die beiden Files MeRGBLineFollower.cpp und MeRGBLineFollower.h gehoeren zu diesem 
*            Projekt duerfen hier nicht fehlen, sonst klappt das nicht!
*            
* \note      Fuer den Auriga muss das "Arduino Mega2560" Board eingestellt und
*            das "MeAuriga.h" File includiert werden.
****************************************************************************************************
*/
/*!
****************************************************************************************************
* \defgroup RESCUE_LINE     Robocup Rescue Line
****************************************************************************************************
*/
#include "MeAuriga.h"
#include "MeRGBLineFollower.h"


#define RGB_RING_NUM_OF_LEDS  12u
#define RGB_RING_SET_ALL_LEDS  0u
#define RGB_RING_LED_PIN      44u /* 12 LED Ring controller is on Auriga D44/PWM */

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief    Globale Variablen und Funktionsprototypen die man hier halt so braucht!
*/
/*------------------------------------------------------------------------------------------------*/
MeRGBLineFollower RGBLineFollower(PORT_7, ADDRESS2);
MeRGBLed RGBLedRing(0, RGB_RING_NUM_OF_LEDS); /* LED Ring auf dem Auriga ist an Port 0 mit 12 LEDs */

void RGBLedSetStatus(uint8_t);
void RGBLineDelay(unsigned long);

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief     Arduino Setup
*
*            Wird beim Start des Programms (entweder nach dem uebertragen auf das Board oder 
*            nach Druecken des Reset-Tasters) einmalig aufgerufen, 
*            um z. B. Pins als Eingang oder Ausgang zu definieren.
*/
/*------------------------------------------------------------------------------------------------*/
void setup() {

  Serial.begin(115200);
  Serial.println(F(" "));
  Serial.println(F("------------------------------------"));
  Serial.println(F("    _____ ______ ______ "));
  Serial.println(F("   / ____|  ____|___  / "));
  Serial.println(F("  | (___ | |__     / /  "));
  Serial.println(F("   \\___ \\|  __|   / /   "));
  Serial.println(F("   ____) | |     / /__  "));
  Serial.println(F("  |_____/|_|    /_____| "));
  Serial.println(F(" "));
  Serial.println(F("------------------------------------"));

  Serial.println(F("INFO: RGB LED Ring initialisieren..."));
  RGBLedRing.setpin(RGB_RING_LED_PIN);
  RGBLedRing.setColor(RGB_RING_SET_ALL_LEDS, 0, 0, 0 ) ;
  RGBLedRing.show();
  
  Serial.println(F("INFO: RGB Linefollower startet..."));
  RGBLineFollower.begin();
  RGBLineFollower.updataAllSensorValue();

  /*------------------------------------------------------------------------------------------------
   * Beispiel, wie man die Messfarbe vom RGB Linefollower aendern kann.
   * Neue Farbe koennen sein: RGB_COLOUR_RED(1), RGB_COLOUR_GREEN(2) oder RGB_COLOUR_BLUE(3)          
   *------------------------------------------------------------------------------------------------*/
  RGBLineFollower.setRGBColour(RGB_COLOUR_BLUE);

  /*------------------------------------------------------------------------------------------------
   * Einstellen der Empfindlichkeit für die Linienfolge, mit der die Reaktionsgeschwindigkeit 
   * fuer die Linienfolge eingestellt wird. Je groeßer der Wert ist, desto empfindlicher wird er. 
   * Beachte, dies wird nur benoetigt, wenn getPositionState() zur Linienverfolgung benuetzt wird.
  *------------------------------------------------------------------------------------------------*/
  RGBLineFollower.setKp(0.3); 

  delay(100);
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief     Arduino Main Loop
*
*            Wird durchgehend immer wieder durchlaufen, solange das Arduino-Board eingeschaltet ist.
*/
/*------------------------------------------------------------------------------------------------*/
void loop() 
{ 

  /*------------------------------------------------------------------------------------------------
   * Beispiel, wie der aktuelle schwarz/weiß Zustand der einzelnen RGB Sensoren eingelesen wird. 
   *             |---------------------------|     
   *             |RGB1  |RGB2  |RGB3  |RGB4  |
   *             |---------------------------|                                                      
   *------------------------------------------------------------------------------------------------*/
  uint8_t aktuelleLinienPosition = RGBLineFollower.getPositionState();

  switch (aktuelleLinienPosition) {
    case B0000:
      /* Aktion auswaehlen */
    break;
    case B0001:
      /* ...eine andere Aktion auswaehlen... */
    break;

      /* ... */
  }
  
  /*------------------------------------------------------------------------------------------------
   * Beispiel, wie man die Abweichung von der schwarzen Linie auslesen kann.
   * wobei die Abweichung von -512 bis 512 gehen kann.  
   * Bei 0 steht der Sensor genau mitten ueber der Linie!
   *------------------------------------------------------------------------------------------------*/
  int16_t aktuelleLinienAbweichung = RGBLineFollower.getPositionOffset();

  /* Zustand der vier Sensoren am LED Ring anzeigen.... */
  RGBLedSetStatus (aktuelleLinienPosition);

  /* Hier wird dann zum Schluss 50ms gewartet und ist ein Ersatz zu delay(50); */
  RGBLineDelay(50); 

  /* und weiter gehts, wieder nach oben zum loop() Anfang */ 
}

/*------------------------------------------------------------------------------------------------*/
/*!
* \brief     Liniensensor Status LED setzen
*
*            Hiermit werden die RGB LED am LED Ring fuer die Anzeige der Linenensensor Werte gesetzt.
*              1 -> weiss -> weisser Untergrund (LED am Sensor ist an!)
*              0 -> blau  -> schwarze Linie (LED am Sensor ist aus!)
*/
/*------------------------------------------------------------------------------------------------*/
void RGBLedSetStatus (uint8_t linieZustand)
{
  if ((linieZustand & B0001) != 0) {
    RGBLedRing.setColor( 1, 55, 55, 55 ) ;
  } else {
    RGBLedRing.setColor( 1, 0, 0, 55 ) ;
  }
  if ((linieZustand & B0010) != 0) {
    RGBLedRing.setColor( 2, 55, 55, 55 ) ;
  } else {
    RGBLedRing.setColor( 2, 0, 0, 55 ) ;
  }
  if ((linieZustand & B0100) != 0) {
    RGBLedRing.setColor( 4, 55, 55, 55 ) ;
  } else {
    RGBLedRing.setColor( 4, 0, 0, 55 ) ;
  }
  if ((linieZustand & B1000) != 0) {
    RGBLedRing.setColor( 5, 55, 55, 55 ) ;
  } else {
    RGBLedRing.setColor( 5, 0, 0, 55 ) ;
  }
  
  RGBLedRing.show();
}


/*------------------------------------------------------------------------------------------------*/
/*!
* \brief     Delay mit RGB Sensor 
*
*            Hier wir die Wartezeit genuetzt um den RGB Sensor derweil aus zu lesen, 
*            damit die Werte von deinem Programm gelesen werden koennen.
*/
/*------------------------------------------------------------------------------------------------*/
void RGBLineDelay (unsigned long delay)
{
  unsigned long NaechsterLoop = millis() + delay;
  do {
    /* Wenn nichts mehr zu tun ist, dann wird eben der RGB Linefollower bedient. */
    RGBLineFollower.loop();    
  } while (NaechsterLoop >= millis());  
}

