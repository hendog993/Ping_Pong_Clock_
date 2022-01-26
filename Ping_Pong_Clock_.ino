/* Ping Pong Clock
 * Author : Henry Gilbert
 * Date 10-22-2021
 */

// TODO ensure things are updating on the right cycle
// TODO are the color structs necessary? A global macro/const int works just as fine, they aren't changing-yet.
// TODO figure out whether or not 60 second loop w/ += 60 is good, or 1 second loop with ++

#include <FastLED.h>
#include "Globals.h"

/* Create LED array */
#ifdef _cplusplus 
extern "C" {
#endif 

CRGB leds [ NUM_LEDS ] ; // todo should this be volatile? 

typedef struct Time_t
{
    byte digit1 ;
    byte digit2 ;
    byte digit3 ;
    byte digit4 ;
} Time ;

volatile Time masterTime ;

 /* For Digits 1-4, if the digit's number  */
int32_t writeTimeToClock ( const Time * const timePointer )
{
    int32_t success = EXIT_FAILURE ;

    if ( NULL == timePointer )
    {
        return success ;
    }

    /* Store each time's digit numbers into an array for iteration */
    byte time_values[ 4 ] = { timePointer->digit1,
                              timePointer->digit2,
                              timePointer->digit3,
                              timePointer->digit4 } ;

    /* For digits 1-4,For the respective LED of each digit, if the number's digit should be on,
     * write the respective digit's LED.
     */
    for ( int i = 0; i < 4 ; i++ )
    {
        for ( int j = 0 ; j < 12 ; j++ )
        {
            if ( nums [ time_values[ i ]][ j ] == 1 )
            {
                leds[digits[i][j]] = CRGB( NUMBER_RED_COLOR, NUMBER_GREEN_COLOR, NUMBER_BLUE_COLOR );
            }
        }
    }

    success = EXIT_SUCCESS ;
    return success  ;
}

int32_t handleMasterTimeReset ( int32_t * const masterTimeInSeconds )
{   // TODO can this be a ternary?
    if (*masterTimeInSeconds >= NUM_SECONDS_IN_DAY )
    {
        *masterTimeInSeconds = 0 ;
    }

    // *masterTimeInSeconds = ( *masterTimeInSeconds >= NUM_SECONDS_IN_DAY ) ? 0 : *masterTimeInSeconds ;
    return EXIT_SUCCESS ;
}

int32_t convertSecondsToFourDigitTime ( Time * const timePointer ,  int32_t timeValueInSeconds )
{
    int32_t success = EXIT_FAILURE ;

    if ( ( NULL == timePointer) ||
         (timeValueInSeconds >= NUM_SECONDS_IN_DAY ) )
    {
        return success ; // Error , these should be handled before entry
    }

    /* Convert Seconds to Minutes and Hours */
    byte hours , minutes ;
    hours = timeValueInSeconds / NUM_SECONDS_IN_HOUR ;
    minutes = ( timeValueInSeconds % NUM_SECONDS_IN_HOUR ) / NUM_SECONDS_IN_MINUTE ;

    if ( ( hours >= NUM_HOURS_IN_DAY ) ||
         ( minutes >= NUM_MINUTES_IN_HOUR ))
     {
        return success ; // Error - hours can't be greater than = 24 or min greater than = 60
     }

    /* Convert Minutes and Hours into individual dights */
    /* TODO are these magic numbers ? */
    byte tensDigitOfHours = ( hours/10 ) % 10 ;
    byte onesDigitOfHours =  hours - ( tensDigitOfHours * 10 ) ;
    byte tensDigitOfMinutes = ( minutes/10 ) % 10 ;
    byte onesDigitOfMinutes = minutes - ( tensDigitOfMinutes * 10 ) ;

    /* Cerify calculation */
    if ( ( tensDigitOfHours > MAX_VAL_DIGIT_1 ) ||
         ( onesDigitOfHours > MAX_VAL_DIGIT_2 ) ||
         ( tensDigitOfMinutes > MAX_VAL_DIGIT_3 ) ||
         ( onesDigitOfMinutes > MAX_VAL_DIGIT_4 ))
    {
        return success ; // Error , exceeds limits of 24 hour time
    }

    /* Construct the mastertime pointer  */
    timePointer->digit1 = tensDigitOfHours ;
    timePointer->digit2 = onesDigitOfHours ;
    timePointer->digit3 = tensDigitOfMinutes ;
    timePointer->digit4 = onesDigitOfMinutes ;

    success = EXIT_SUCCESS ;
    return success ;
}

/* Write All Values in matrix to value in background color */
void writeBackgroundColor ( )
{
    for ( int i = 0; i < NUM_LEDS ; i++ )
    {
        leds[i] = CRGB ( BACKGROUND_RED_COLOR ,
                         BACKGROUND_GREEN_COLOR ,
                         BACKGROUND_BLUE_COLOR ) ;
    }
}

void writeColonColor ( )
{
    leds[COLON_TOP] = CRGB ( COLON_RED_COLOR,
                             COLON_GREEN_COLOR,
                             COLON_BLUE_COLOR) ;
    leds[COLON_BOT] = CRGB ( COLON_RED_COLOR,
                             COLON_GREEN_COLOR ,
                             COLON_BLUE_COLOR ) ;
}

int32_t writeFullMatrix ( const Time * const timePointer  )
{
    int32_t success = EXIT_FAILURE ;

    /* Write Background */
    writeBackgroundColor () ;
    
    /* Write Colon */
    writeColonColor () ;

    /* Write Time  */
    success = writeTimeToClock ( timePointer ) ; 

    /* Show LEDs */
    FastLED.show () ;

    return success ;
}

/* Global Variables */
uint32_t startMillis ;
uint32_t currentMillis ;
const uint16_t period = 1000 ; // Milliseconds
volatile int32_t timeInSeconds = 0 ;  // TODO make this configurable with encoder once debounce is figured out.

/* Rotary Encoder Portion */
uint32_t rotVector;
uint16_t rotCounter;

void rotISR()
{
    noInterrupts();
    rotVector = ( rotVector << 2u ) & 0xCu ; // Preserve old A,B as A',B'
     // Read encoder inputs 
    rotVector |= ( digitalRead ( 2 ) ) ? 2 : 0 ;
    rotVector |= ( digitalRead ( 3 ) ) ? 1 : 0 ;
 
    switch ( rotVector )
    {
        case 11u:      
            (timeInSeconds == 0)? timeInSeconds = 0 : timeInSeconds-= 60;
            convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ;
            writeFullMatrix ( &masterTime ) ;
            break;
        case 7u:
            timeInSeconds+= 60;
            convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ;
            writeFullMatrix ( &masterTime ) ;
            break;
            
        default:
            break;
    }
    
    interrupts();
}
/************ end rotary encoder section  *************/

#ifdef _cpluscplus
}
#endif 
void setup()
{
//    Serial.begin ( 115200 ) ;
    FastLED.addLeds<WS2812, LED_PIN, GRB>( leds , NUM_LEDS );
    FastLED.setMaxPowerInVoltsAndMilliamps( 5, 500 );
    FastLED.clear( );
    FastLED.show( ) ;

    /* Attach Interrupt */
    attachInterrupt(digitalPinToInterrupt(2), rotISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(3), rotISR, CHANGE);
    
    /* Initialize Time Struct  */
    convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ;

    /* Initialize Timer  */
    startMillis = millis () ;

    /* Write initial time, since the first mainloop call won't occur until period is met */
    writeFullMatrix ( &masterTime ) ;
}

/* Main loop logic:
 * If the period has passed ( 1 second ) ,
 * 1. convert the time in seconds into digits in the time poiner,
 * 2. Write the full LED matrix with background, colon, then time.
 * 3. Increment the time value in seconds
 * 4. Handle the time rollover in seconds
 * 5. Set the start time to the current time for the next period
 */
void loop()
{
    currentMillis = millis () ;
    if (currentMillis - startMillis >= period )
        {
           /* Inside time controlled loop */
            convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ;
            writeFullMatrix ( &masterTime ) ;
            timeInSeconds ++ ;
            handleMasterTimeReset ( &timeInSeconds ) ;
            startMillis = currentMillis ;
         }
     /* Enter input handling here, outside of main time-based loop */
}
