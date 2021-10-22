#ifndef GLOBALS_H
#define GLOBALS_H

#define COLON_TOP 64
#define COLON_BOT 66

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1 
#define NUM_LEDS  128 
#define LED_PIN 2 
#define NUM_SECONDS_IN_DAY 86400
#define NUM_SECONDS_IN_HOUR 3600 
#define NUM_SECONDS_IN_MINUTE 60 
#define NUM_SEC_IN_HOUR 3600 
#define NUM_HOURS_IN_DAY 24 
#define NUM_MINUTES_IN_HOUR 60 
#define NUM_MINUTES_IN_DAY 1440 // TODO maybe remove this, it's unnecesssary 


/* Color Value Definitions */
#define BACKGROUND_RED_COLOR 255 
#define BACKGROUND_GREEN_COLOR 0 
#define BACKGROUND_BLUE_COLOR 0 

/* Colon Color Definitions */
#define COLON_RED_COLOR 255 
#define COLON_GREEN_COLOR 255 
#define COLON_BLUE_COLOR 255

/* Number Color Definitions */
#define NUMBER_RED_COLOR 0 
#define NUMBER_GREEN_COLOR 255
#define NUMBER_BLUE_COLOR 0

// TODO should these magic numbers be refactored ? 
const byte row1[ 17 ] =     {6,19,20,33,34,47,48,61,62,75,76,89,90,103,104,117,118}; 
const byte row2[ 18 ] =  {5,7,18,21,32,35,46,49,60,63,74,77,88,91,102,105,116,119};
const byte row3[ 19 ] =  {4,8,17,22,31,36,45,50,59,64,73,78,87,92,101,106,115,120,127};
const byte row4[ 20 ] = {0,3,9,16,23,30,37,44,51,58,65,72,79,86,93,100,107,114,121,126};
const byte row5[ 19 ] =  {2,10,15,24,29,38,43,52,57,66,71,80,85,94,99,108,113,122,125};
const byte row6[ 18 ] =   {1,11,14,25,28,39,42,53,56,67,70,81,84,95,98,109,112,123};
const byte row7[ 17 ] =    {12,13,26,27,40,41,54,55,68,69,82,83,96,97,110,111,124};

/* LED Assignments for digits  */
const byte nums [10][12] = 
{
    {1,1,0,1,1,1,0,0,0,1,1,1},      // zero 
    {0,0,0,0,0,1,0,1,0,1,1,1},      // one 
    {1,0,1,1,1,1,0,1,0,1,1,0},      // two
    {1,0,1,0,1,1,0,1,0,1,1,1},      // three
    {1,1,1,0,1,0,1,1,0,0,1,0},      // four 
    {1,1,1,0,1,1,0,1,0,1,0,1},      // five
    {0,0,1,1,1,1,0,1,1,1,0,1},      // six
    {1,0,0,0,1,0,1,1,0,1,1,0},      // seven 
    {1,1,1,1,1,1,0,1,0,1,1,1},      // eight    
    {1,1,1,0,1,0,1,1,0,1,1,0},      // nine 
};

/* LED Assignments for each respective digit */
const byte digits[4][12] = 
{
    {7,8,9,10,11,14,15,16,17,18,22,24},                 // leds in the 1st digit 
    {35,36,37,38,39, 42,43,44,45,46,50,52},             // leds in the 2nd digit 
    {77,78,79,80,81,84,85,86,87,88,92,94},              // leds in the 3rd digit 
    {105,106,107,108,109,112,113,114,115,116,120,122},  // leds in the 4th digit 
}  ;

/* LED assignment of the clock perimeter */
const byte perimeter[] = {0,2,1,12,13,26,27,40,41,54,55,68,69,82,83,96,97,110,111,124,123,125,126,127,119,118,117,
                  104,103,90,89,76,75,62,61,48,47,34,33,20,19,6,5,4};                   

#endif

/*
void debugDigits () 
{
    for (int i=0 ; i<10 ; i++) 
    {
        FastLED.clear();
        byte time_values[4] = {i,i,i,i};
                                   
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
        delay(1000);
        FastLED.show();
}*/
