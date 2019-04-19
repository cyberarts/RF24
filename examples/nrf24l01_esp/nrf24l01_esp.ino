/*
  Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  version 2 as published by the Free Software Foundation.
*/

/**
   Example LED Remote

   This is an example of how to use the RF24 class to control a remote
   bank of LED's using buttons on a remote control.

   On the 'remote', connect any number of buttons or switches from
   an arduino pin to ground.  Update 'button_pins' to reflect the
   pins used.

   On the 'led' board, connect the same number of LED's from an
   arduino pin to a resistor to ground.  Update 'led_pins' to reflect
   the pins used.  Also connect a separate pin to ground and change
   the 'role_pin'.  This tells the sketch it's running on the LED board.

   Every time the buttons change on the remote, the entire state of
   buttons is send to the led board, which displays the state.
*/

#include <SPI.h>
#include "nRF24L01_esp.h"
#include "RF24_esp.h"
//#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (CE & CS)

RF24_esp *radio;

// sets the role of this unit in hardware.  Connect to GND to be the 'led' board receiver
// Leave open to be the 'remote' transmitter
const int role_pin = 15;

// Pins on the remote for buttons
const uint8_t button_pins[] = { 2, 3, 4, 5, 6, 7 };
const uint8_t num_button_pins = sizeof(button_pins);

// Pins on the LED board for LED's
const uint8_t led_pins[] = { 2, 3, 4, 5, 6, 7 };
const uint8_t num_led_pins = sizeof(led_pins);

//
// Topology
//

// Single radio pipe_addr address for the 2 nodes to communicate.
const uint64_t pipe_addr = 0xE8E8F0F0E1LL;

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_remote = 1, role_led } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Remote", "LED Board"};

// The role of the current running sketch
role_e role;

//
// Payload
//

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];

//
// Setup
//
SPIClass * hspi = NULL;
void setup(void)
{
  Serial.begin(115200);
  Serial.println("start");
  //
  // Role
  //
  hspi = new SPIClass(HSPI);

  //ce,csn,spiclass
  radio = new RF24_esp(22, 23, hspi);
  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin, HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
  if ( digitalRead(role_pin) )
    role = role_remote;
  else
    role = role_led;
  //Serial.println("begin SPI");
  //radio->mSPI->begin();
  //Serial.println("using SPI");
  //
  // Print preamble
  //


  //printf_begin();
  //printf("\n\rRF24/examples/led_remote/\n\r");
  //printf("ROLE: %s\n\r",role_friendly_name[role]);

  //
  // Setup and configure rf radio
  //
  Serial.println("begin radio");
  //radio->mSPI->begin();

  radio->begin();
  radio->setPALevel(RF24_PA_LOW);
  radio->setChannel(80);
  Serial.println("using radio");
  radio->openWritingPipe(pipe_addr);
  Serial.println("pipe opened");
    //
    // Open pipe_addrs to other nodes for communication
    //

    // This simple sketch opens a single pipe_addrs for these two nodes to communicate
    // back and forth.  One listens on it, the other talks to it.

    if ( role == role_remote )
    {    
      //radio->openWritingPipe(pipe_addr);
    }
    else
    {
      //radio->openReadingPipe(1,pipe_addr);
    }

    //
    // Start listening
    //
/*
    if ( role == role_led )
      radio->startListening();

    //
    // Dump the configuration of the rf unit for debugging
    //

    radio->printDetails();

    //
    // Set up buttons / LED's
    //

    // Set pull-up resistors for all buttons
    if ( role == role_remote )
    {
      int i = num_button_pins;
      while(i--)
      {
        pinMode(button_pins[i],INPUT);
        digitalWrite(button_pins[i],HIGH);
      }
    }

    // Turn LED's ON until we start getting keys
    if ( role == role_led )
    {
      int i = num_led_pins;
      while(i--)
      {
        pinMode(led_pins[i],OUTPUT);
        led_states[i] = HIGH;
        digitalWrite(led_pins[i],led_states[i]);
      }
    }
  */
}

//
// Loop
//

void loop(void)
{
  radio->write( "hello", 5 );
  Serial.println("sent hello");
   delay(500);
  //
  // Remote role.  If the state of any button has changed, send the whole state of
  // all buttons.
  //
  /*
    if ( role == role_remote )
    {
      // Get the current state of buttons, and
      // Test if the current state is different from the last state we sent
      int i = num_button_pins;
      bool different = false;
      while(i--)
      {
        uint8_t state = ! digitalRead(button_pins[i]);
        if ( state != button_states[i] )
        {
          different = true;
          button_states[i] = state;
        }
      }

      // Send the state of the buttons to the LED board
      if ( different )
      {
        Serial.print("Now sending...");
        bool ok = radio->write( button_states, num_button_pins );
        if (ok)
          Serial.print("ok\n\r");
        else
          Serial.print("failed\n\r");
      }

      // Try again in a short while
      delay(20);
    }

    //
    // LED role.  Receive the state of all buttons, and reflect that in the LEDs
    //

    if ( role == role_led )
    {
      // if there is data ready
      if ( radio->available() )
      {
        // Dump the payloads until we've gotten everything
        while (radio->available())
        {
          // Fetch the payload, and see if this was the last one.
          radio->read( button_states, num_button_pins );

          // Spew it
          Serial.print("Got buttons\n\r");

          // For each button, if the button now on, then toggle the LED
          int i = num_led_pins;
          while(i--)
          {
            if ( button_states[i] )
            {
              led_states[i] ^= HIGH;
              digitalWrite(led_pins[i],led_states[i]);
            }
          }
        }
      }
    }*/
 
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
