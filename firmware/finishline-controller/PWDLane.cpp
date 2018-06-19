#include "PWDLane.h"
#include <Arduino.h>

static const byte digit[32] = {
  // bitfield with LSB = first segment and MSB = last segment
  // 0x0 up to 0xf = hexadecimal characters
  // Special characters, see constants in .h file
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, // 0-9
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x74, 0x76, 0x0e, 0x38, // A-L
  0x54, 0x5c, 0x73, 0x50, 0x78, 0x1c, 0x3e, 0x6e, 0x40, 0x30, // n-
  0x49, 0x0
};

PWDLane::PWDLane( const uint8_t deviceAddress, const uint8_t clockpin, 
    const uint8_t datapin, CRGB * leds ) : _pcf( deviceAddress ), _tm( clockpin, datapin )
{
  _address = deviceAddress;
  _avgScanInterval = 0;
  _triggerActive = false;
  _triggered = false;
  _leds = leds;
}


void PWDLane::begin()
{
  _pcf.begin();
}

void PWDLane::select( bool on )
{
  Serial.print( F("Lane ") );
  Serial.print( _address );
  Serial.println( on ? F("selected") : F("unselected") );
  _pcf.write8( on ? 7 : 0 );
}

void PWDLane::showNumber( uint32_t number )
{
  _tm.DigitDisplayWrite( number );
}

void PWDLane::setBigDigit( uint8_t rank )
{
  // we can use the _address variable here, it is not entirely correct,
  // because someone could set up the lanes starting not from zero...
  // we are modifying the global array of LEDs here!
  for( int i=0; i<7; i++ ) {
    _leds[i+_address*7] = ( ( digit[rank] & (1 << i) ) ? CRGB( 20, 20, 20) : CRGB::Black );
  }
}

uint8_t PWDLane::getAddress()
{
  return _address;
}

bool PWDLane::hasTriggered()
{
  return _triggered;
}

// vim:si:sw=2
