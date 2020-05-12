#ifndef __PCM_MEM_H
#define __PCM_MEM_H

/*
 * speaker_pcm
 *
 * Plays 8-bit PCM audio on pin 11/3 using pulse-width modulation (PWM).
 *
 * Uses two timers. The first changes the sample value near 8000 times a second.
 * The second holds pin 11/3 high for 0-255 ticks out of a 256-tick cycle,
 * depending on sample value. The second timer repeats 62500 times per second
 * (16000000 / 256), much faster than the playback rate (near 8000 Hz), so
 * it almost sounds halfway decent, just really quiet on a PC speaker.
 *
 * Takes over Timer 1 (16-bit) or Timer 0 (8-bit) for the 8000 Hz or 7812.5 Hz timer. 
 * If using Timer 1, it breaks PWM (analogWrite()) for Arduino pins 9 and 10. 
 * If using Timer 0, delay(), delayMicroseconds(), micros(), millis() cannot be used, while this library 
 * prepared alternative functions: altDelay(), altDelayMicroseconds(), altMicros(), altMillis() instead.
 * Takes Timer 2 (8-bit) for the pulse width modulation, breaking PWM for pins 11 & 3.
 *
 * References:
 *     http://www.uchobby.com/index.php/2007/11/11/arduino-sound-part-1/
 *     http://www.atmel.com/dyn/resources/prod_documents/doc2542.pdf
 *     http://www.evilmadscientist.com/article.php/avrdac
 *     http://gonium.net/md/2006/12/27/i-will-think-before-i-code/
 *     http://fly.cc.fer.hr/GDM/articles/sndmus/speaker2.html
 *     http://www.gamedev.net/reference/articles/article442.asp
 *
 * Michael Smith <michael@hurts.ca>
 * Programus <programus@gmail.com>
 */

/*
 * The audio data needs to be unsigned, 8-bit, 8000 Hz, and small enough
 * to fit in flash. 10000-13000 samples is about the limit.
 *
 * sounddata.h should look like this:
 *     const int sounddata_length=10000;
 *     const unsigned char sounddata_data[] PROGMEM = { ..... };
 *
 * You can use wav2c from GBA CSS:
 *     http://thieumsweb.free.fr/english/gbacss.html
 * Then add "PROGMEM" in the right place. I hacked it up to dump the samples
 * as unsigned rather than signed, but it shouldn't matter.
 *
 * http://musicthing.blogspot.com/2005/05/tiny-music-makers-pt-4-mac-startup.html
 * mplayer -ao pcm macstartup.mp3
 * sox audiodump.wav -v 1.32 -c 1 -r 8000 -u -1 macstartup-8000.wav
 * sox macstartup-8000.wav macstartup-cut.wav trim 0 10000s
 * wav2c macstartup-cut.wav sounddata.h sounddata
 *
 * (starfox) nb. under sox 12.18 (distributed in CentOS 5), i needed to run
 * the following command to convert my wav file to the appropriate format:
 * sox audiodump.wav -c 1 -r 8000 -u -b macstartup-8000.wav
 */

#include <avr/pgmspace.h>

/**
 * Set whether use timer0.
 * 
 * Use timer0 will free timer1 which is used by Servo motor, 
 * but the audio playing frequency would be 7812.5 Hz which is a little lower than 8000 Hz, 
 * and delay(), micro() and millis() functions could not be used, 
 * you must use altDelay(), altMicro, altMillis() instead.
 * 
 * @param use true to use timer0 or false to use timer1
 */
void useTimer0(bool use = true);

/**
 * Set the pin tha connect the speaker. 
 * Pin 11 is the default one.
 * 
 * @param pin Only pin 3 and pin 11 could be used. Use default pin 11 if others is specified.
 * @return The pin number actually be used. 
 */
uint8_t setSpeakerPin(uint8_t pin);

/**
 * Get the pin tha connect the speaker. 
 * 
 * @return The pin number be used. 
 */
uint8_t getSpeakerPin();

/**
 * Alternative of delay(ms).
 * You can always use it no matter you are using timer0 or not.
 */
void altDelay(unsigned long ms);

/**
 * Alternative of delayMicroseconds(us).
 * You can always use it no matter you are using timer0 or not.
 */
void altDelayMicroseconds(unsigned int us);
/**
 * Alternative of micros().
 * You can always use it no matter you are using timer0 or not.
 */
unsigned long altMicros();
/**
 * Alternative of millis().
 * You can always use it no matter you are using timer0 or not.
 */
unsigned long altMillis();

void startPlayback(unsigned char const *data, int length, bool loop = false);
void stopPlayback();

#endif