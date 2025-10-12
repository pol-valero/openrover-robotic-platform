// RcTrainer.cpp
//
// Copyright (C) 2013 Mike McCauley
// $Id:  $

#include <RcTrainer.h>

// Interrupt vectors for the Arduino interrupt pins
// Each interrupt can be handled by a different instance of RcTrainer, allowing you to have
// up to  RcTrainers per Arduino, provided that many interrupt pins are available
#define MAX_INTERRUPTS 6
RcTrainer* RcTrainer::_RcTrainerForInterrupt[MAX_INTERRUPTS];

RcTrainer::RcTrainer(uint8_t interrupt)
{
    if (interrupt < MAX_INTERRUPTS)
    {
	_RcTrainerForInterrupt[interrupt] = this;
	switch (interrupt)
	{
	    case 0:
		attachInterrupt(interrupt, interruptHandler0, RISING);
		break;
	    case 1:
		attachInterrupt(interrupt, interruptHandler1, RISING);
		break;
	    case 2:
		attachInterrupt(interrupt, interruptHandler2, RISING);
		break;
	    case 3:
		attachInterrupt(interrupt, interruptHandler3, RISING);
		break;
	    case 4:
		attachInterrupt(interrupt, interruptHandler4, RISING);
		break;
	    case 5:
		attachInterrupt(interrupt, interruptHandler5, RISING);
		break;
	}
    }
}

int16_t RcTrainer::getChannelRaw(uint16_t channel)
{
    if (channel >= RCTRAINER_MAX_CHANNELS)
	return 0;
    return  _channels[channel];
}
int16_t RcTrainer::getChannel(int16_t channel, int16_t mapFromLow, int16_t mapFromHigh, int16_t mapToLow, int16_t mapToHigh)
{
    int16_t val = getChannelRaw(channel);
    val = map(val, mapFromLow, mapFromHigh, mapToLow, mapToHigh);
    val = constrain(val, mapToLow, mapToHigh);
    return val;
}

void RcTrainer::interruptHandler()
{  
    uint32_t interruptTime = micros();
    uint32_t pulse_width =  interruptTime - _lastInterruptTime;
    
    if (pulse_width > RCTRAINER_MIN_INTERFRAME_INTERVAL) 
    {
	// Start of a new frame of channels
	_nextChannelNumber = 0;
    }
    else
    {
	// End of a variable width channel value, use the elapsed time
	// since the last transition, which is a measurement of the analog
	// channel value
	// For my Spektrum DX6i, the min is about 1096, and the max is about 1916
	// Centre of range is about 1512
	// gear is 1096 to 1932
	// If flap is configured, it goes between 1096 and 1512
	// Channels are:
	// 0 throttle
	// 1 aileron
	// 2 elevator
	// 3 rudder
	// 4 gear
	// 5 flap/gyro
	if (_nextChannelNumber < RCTRAINER_MAX_CHANNELS)
	    _channels[_nextChannelNumber++] = pulse_width;
    }
    _lastInterruptTime = interruptTime;
}

void RcTrainer::interruptHandler0()
{
    _RcTrainerForInterrupt[0]->interruptHandler();
}
void RcTrainer::interruptHandler1()
{
    _RcTrainerForInterrupt[1]->interruptHandler();
}
void RcTrainer::interruptHandler2()
{
    _RcTrainerForInterrupt[2]->interruptHandler();
}
void RcTrainer::interruptHandler3()
{
    _RcTrainerForInterrupt[3]->interruptHandler();
}
void RcTrainer::interruptHandler4()
{
    _RcTrainerForInterrupt[4]->interruptHandler();
}
void  RcTrainer::interruptHandler5()
{
    _RcTrainerForInterrupt[5]->interruptHandler();
}
