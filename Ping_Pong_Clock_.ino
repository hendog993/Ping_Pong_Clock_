/* Ping Pong Clock
 * Author : Henry Gilbert
 * Date 10-22-2021
 */

// TODO ensure things are updating on the right cycle 
// TODO are the color structs necessary? A global macro/const int works just as fine, they aren't changing-yet.
// TODO figure out whether or not 60 second loop w/ += 60 is good, or 1 second loop with ++

#include <FastLED.h>
#include "Globals.h"
CRGB leds [ NUM_LEDS ] ; 

/* Struct Definitions  */ 

typedef struct Color_t 
{
    byte red_value ; 
    byte green_value ; 
    byte blue_value ;     
} Color ; 

typedef struct Time_t 
{
    byte digit1 ; 
    byte digit2 ; 
    byte digit3 ; 
    byte digit4 ;    
} Time ; 

/* Struct Declarations */
//Color numberColor ; 
//Color backgroundColor ; 
//Color colonColor ; 

Time masterTime ; 

 /* For Digits 1-4, if the digit's number  */
int32_t writeTimeToClock ( Time * timePointer ) 
{   
    int32_t success = EXIT_FAILURE ;
     
    if (timePointer == NULL ) 
    {
        return success ; 
    }
    
    /* Store each time's digit numbers into an array for iteration */
    byte time_values[ 4 ] = { timePointer->digit1,
                              timePointer->digit2,
                              timePointer->digit3,
                              timePointer->digit4 } ;
                               
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

int32_t handleMasterTimeReset ( int32_t & masterTimeInSeconds ) 
{
    if (masterTimeInSeconds >= NUM_SECONDS_IN_DAY ) 
    {
        masterTimeInSeconds = 0 ;
    }
    return EXIT_SUCCESS ;
}

int32_t convertSecondsToFourDigitTime ( Time * timePointer , int32_t timeValueInSeconds ) 
{
    int32_t success = EXIT_FAILURE ; 
    
    if ( ( timePointer == NULL ) ||
         timeValueInSeconds >= NUM_SECONDS_IN_DAY )
    {
        return success ; // Error    
    }

    /* Convert Seconds to Minutes and Hours */
    byte hours , minutes ; 
    hours = timeValueInSeconds / NUM_SEC_IN_HOUR ; 
    minutes = (timeValueInSeconds % NUM_SEC_IN_HOUR ) / NUM_SECONDS_IN_MINUTE ; 
    
    if ( ( hours >= NUM_HOURS_IN_DAY ) ||
         ( minutes >= NUM_MINUTES_IN_HOUR ))
     {
        return success ; // Error - hours can't be greater than = 24 or min greater than = 60
     }
     
    /* Convert Minutes and Hours into individual dights */  
    /* TODO are these magic numbers ? */
    byte tensDigitOfHours = ( hours/10 ) % 10 ; 
    byte onesDigitOfHours =  hours - ( tensDigitOfHours * 10 ) ;
    byte tensDigitOfMinutes = ( minutes/10) % 10 ;
    byte onesDigitOfMinutes = minutes - ( tensDigitOfMinutes * 10 ) ; 

    /* TODO are these magic numberrs */
    if ( ( tensDigitOfHours > 2 ) ||    
         ( onesDigitOfHours > 4 ) ||
         ( tensDigitOfMinutes > 5 ) ||
         ( onesDigitOfMinutes > 10 ))
    {
        return success ; // Error , exceeds limits of 24 hour time 
    }

    /* Construct the master time in the time pointer struct TODO add some error checking here */ 
    timePointer->digit1 = tensDigitOfHours ; 
    timePointer->digit2 = onesDigitOfHours ; 
    timePointer->digit3 = tensDigitOfMinutes ; 
    timePointer->digit4 = onesDigitOfMinutes ; 
    
    success = EXIT_SUCCESS ; 
    return success ; 
}

/* Write All Values in matrix to value in background color */
int32_t writeBackgroundColor ( ) 
{  
    for ( int i = 0; i < NUM_LEDS ; i++ ) 
    {
        leds[i] = CRGB ( BACKGROUND_RED_COLOR , 
                         BACKGROUND_GREEN_COLOR ,
                         BACKGROUND_BLUE_COLOR ) ;  
    }
    return EXIT_SUCCESS ; 
}

int32_t writeColonColor ( ) 
{
    leds[COLON_TOP] = CRGB ( COLON_RED_COLOR, 
                             COLON_GREEN_COLOR,
                             COLON_BLUE_COLOR) ;
    leds[COLON_BOT] = CRGB ( COLON_RED_COLOR, 
                             COLON_GREEN_COLOR ,
                             COLON_BLUE_COLOR ) ;
    return EXIT_SUCCESS ;
}

int32_t writeFullMatrix ( Time * timePointer  ) 
{
    int32_t success = EXIT_FAILURE ; 
    /* Write Background */
    success = writeBackgroundColor () ;
    
    /* Write Colon */
    success = writeColonColor () ;
    
    /* Write Time  */
    success = writeTimeToClock (timePointer) ;
    /* Show LEDs */

    FastLED.show () ;
    return success ; 
}

/* Global Variables */
uint32_t startMillis ; 
uint32_t currentMillis ; 
const uint16_t period = 60000 ; // Milliseconds 
int32_t timeInSeconds = 45240 ; 
int32_t success = EXIT_SUCCESS ; 

void setup() 
{
    Serial.begin ( 9600 ) ;
    FastLED.addLeds<WS2812, LED_PIN, GRB>( leds , NUM_LEDS );
    FastLED.setMaxPowerInVoltsAndMilliamps( 5, 500 );
    FastLED.clear( );
    FastLED.show( ) ; 

    /* Initialize Time Struct  */
    convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ;
    /* Write Initial Time and Background Color */
    
    /* Initialize Timer  */
    startMillis = millis () ; 
    
    /* Write initial time, since the first mainloop call won't occur until period is met */
    writeFullMatrix ( &masterTime ) ;
}

void loop() 
{
    while (success == EXIT_SUCCESS ) 
    {
    currentMillis = millis () ; 
    if (currentMillis - startMillis >= period ) 
        {
            /* Inside time controlled loop */
            success = convertSecondsToFourDigitTime ( &masterTime , timeInSeconds ) ; 
            success = writeFullMatrix ( &masterTime ) ;
            timeInSeconds += 60 ; 
            success = handleMasterTimeReset ( timeInSeconds ) ;
            startMillis = currentMillis ;
        }
     /* Enter input handling here, outside of main time-based loop */   
    }
    /* Error Segment - Should never reach here */
    Serial.println ( "Exit failure " );
}
