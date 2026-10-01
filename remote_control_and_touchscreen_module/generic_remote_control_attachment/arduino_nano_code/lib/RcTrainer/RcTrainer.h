// RcTrainer.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2013 Mike McCauley
// $Id: $
//
/// \mainpage RcTrainer library for Arduino
///
/// This is the Arduino RcTrainer library.
/// It provides the ability to read the PPM encoded signal from an RC transmitter in trainer mode
/// containing the PPM encoded position of the servo channels.
///
/// Many RC transmitters provide a trainer output that is designed to allow 2 tranmsitters 
/// to be connected for training purposes. The transmitter has a socket for a training cable.
/// When configured for training, the tranmitter emits a signal on that socket that encodes the position
/// of all the transmitters control channels
///
/// This library allows you to connect that training output signal to an Arduino, and to read the
/// channel outputs to control the Arduino.
///
/// Example Arduino programs are included to show the main modes of use.
///
/// The version of the package that this documentation refers to can be downloaded 
/// from http://www.airspayce.com/mikem/arduino/RcTrainer/RcTrainer-1.0.zip
/// You can find the latest version at http://www.airspayce.com/mikem/arduino/RcTrainer
///
/// \par Transmitters
///
/// This library has been tested with the following transmitters:
///
/// -Spektrum DX6i. The DX6i has a 3.5mm mono phone socket on the back. The signal is 3.3V p-p, normally high.
/// When you plug into the 'TRAINER' socket on the back, the transmitter comes on and sends the PPM signal
/// on the TRAINER socket. No radio signal is transmitted. Tested with Arduino Uno.
///
/// \par Connecting to Arduino
///
/// The connection between the transmitter and the Arduino must be electrically compatible, 
/// and the connection must be made to one of the interrupt capable pins on the Arduino.
///
/// Some transmitters emit a 3.3V signal. This is compatible with a 3.3V Arduino, and may be compatible with
/// a 5V Arduino.
///
/// Some tramsmitters emit a 5V signal. This is compatible with a 5V Arduino, and may be compatible with
/// a 3.3V Arduino, provided the input is 5V tolerant.
///
/// The transmitter must be connected to one of the interrupt capable pins. On Arduino Uno for example, 
/// this means digital inputs D2 or D3 only. Other Arduinos may have the interrupts on other pins and 
/// there may be more interrupts.
///
/// Example for Spektrum DX6i and Arduino Uno:
/// \code
///                 Arduino      Spektrum DX6i
///                 D2-----------TIP
///                 GND----------RING
/// \endcode
///
/// \par Installation
///
/// Install in the usual way: unzip the distribution zip file to the libraries
/// sub-folder of your sketchbook. 
///
/// This software is Copyright (C) 2011 Mike McCauley. Use is subject to license
/// conditions. The main licensing options available are GPL V2 or Commercial:
/// 
/// \par Open Source Licensing GPL V2
///
/// This is the appropriate option if you want to share the source code of your
/// application with everyone you distribute it to, and you also want to give them
/// the right to share who uses it. If you wish to use this software under Open
/// Source Licensing, you must contribute all your source code to the open source
/// community in accordance with the GPL Version 2 when your application is
/// distributed. See http://www.gnu.org/copyleft/gpl.html
/// 
/// \par Commercial Licensing
///
/// This is the appropriate option if you are creating proprietary applications
/// and you are not prepared to distribute and share the source code of your
/// application. Contact info@airspayce.com for details.
///
/// \par Revision History
///
/// \version 1.0 Initial release

#ifndef RCTRAINER_h
#define RCTRAINER_h

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <wiring.h>
#endif

// These defs cause trouble on some versions of Arduino
#undef round
#undef double

/////////////////////////////////////////////////////////////////////
/// \class RcTrainer RcTrainer.h <RcTrainer.h>
/// \brief Read servo positions from RC Transmitter in Trainer mode
///
/// This class provides the ability to read servo positions from a PPM encoded
/// digital signal, such as the one emitted by an RC traansmitter in Trainer mode.
class RcTrainer
{
public:
/// Maximum number of permitted channels.
#define RCTRAINER_MAX_CHANNELS 10
/// Minimum interfame interval in microseconds
#define RCTRAINER_MIN_INTERFRAME_INTERVAL 3000

    /// Constructor
    /// Creates a new RcTrainer. You have multiple RcTrainer instances connected to
    /// different inputs. If the interrupt number is out of 
    /// range for your processor, RcTrainer will silently fail to work correctly. 
    /// \param[in] interrupt This is the number of the interrupt pin (not the digital input pin number)
    /// that is connected to the transmitter. The mapping from interrupt number to digital pin number 
    /// depends on your Arduino. See http://arduino.cc/en/Reference/attachInterrupt for details
    RcTrainer(uint8_t interrupt = 0);

    /// Read the raw channel value for the specified channel.
    /// The raw channel value is the length of the PPM pulse for that channel in microseconds
    /// The range of values seen will depend on your transmitter, and the position of the stick
    /// connected to that channel, and the configuration of the transmitter.
    /// On the DX6i, the range of raw channel values varies from about 1096 to 1916, with 
    /// the stick midpoints at about 1512.
    /// \param[in] channel The number of the channel to get.
    /// \return The raw channel value in microseconds. If the channel number is out of range for that
    /// transmitter, or if it is more than RCTRAINER_MAX_CHANNELS, returns 0.
    int16_t getChannelRaw(uint16_t channel);

    /// Reads a scaled channel value
    /// The raw channel value for the selected channel is mapped according to the specified parameters
    /// so that the result always lies in the range mapToLow to mapToHigh.
    /// Raw alues are scaled using the Arduino map() function, so that raw values in the range 
    /// mapFromLow to mapFromHigh are scaled to lie in the range mapToLow to mapToHigh. 
    /// The result is contrained to lie in the range  mapToLow to mapToHigh.
    /// The default values are suitable for use with the DX6i, and scale the channels to the range 0-1023.
    /// \param[in] channel The number of the channel to get.
    /// \param[in] mapFromLow
    /// \param[in] mapFromHigh
    /// \param[in] mapToLow
    /// \param[in] mapToHigh
    /// \return The raw channel value scaled according to the map arguments. 
    /// If the channel number is out of range for that
    /// transmitter, or if it is more than RCTRAINER_MAX_CHANNELS, returns 0 scaled as per the map arguments
    int16_t getChannel(int16_t channel, int16_t mapFromLow = 1096, int16_t mapFromHigh = 1916, int16_t mapToLow = 0, int16_t mapToHigh = 1023);

private:
    /// Array of instances connected to interrupts 0 to 6
    static RcTrainer*        _RcTrainerForInterrupt[];

    uint8_t  _nextChannelNumber;
    uint16_t _channels[RCTRAINER_MAX_CHANNELS];
    uint32_t _lastInterruptTime;

    void interruptHandler();
    static void interruptHandler0();
    static void interruptHandler1();
    static void interruptHandler2();
    static void interruptHandler3();
    static void interruptHandler4();
    static void interruptHandler5();
};

/// @example dx6i.ino 
/// Print out servo positions from a Spektrum DX6i in trainer mode

#endif
