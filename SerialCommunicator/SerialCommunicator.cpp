//-----------------------------------------------------------------------
//
//   KdsLockEngine is written by Wiphoo (Terng) Methachawalit
//       for DSP :: KDS Lock - created on 2014/09/09
//   Copyright Wiphoo Methachawalit, 2014. All rights reserved.
//
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//	Standard Includes
//
//-----------------------------------------------------------------------

#include <assert.h>

//-----------------------------------------------------------------------
//
//	Non-Standard Includes
//
//-----------------------------------------------------------------------

#include "SerialCommunicator.h"

//-----------------------------------------------------------------------
//
//	Defines
//
//-----------------------------------------------------------------------

//	Default terminated serial command character
#define DEFAULT_TERMINATED_SERIAL_COMMAND_CHAR '\r'

//-----------------------------------------------------------------------
//
//	Inline Functions
//
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//	Global Variables
//
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//	Class SerialCommunicator Static Members Functions
//
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//	Class SerialCommunicator Members Functions
//
//-----------------------------------------------------------------------

SerialCommunicator::SerialCommunicator( SoftwareSerial &serial ) 
										: serial( &serial ),
											currentSerialBufferPosition( 0 ),
												terminatedCommandChar( DEFAULT_TERMINATED_SERIAL_COMMAND_CHAR ), 
												delimieterChar( DEFAULT_DELIMITER_CHAR ),
												numCommandCallbacks( 0 )
{

}
SerialCommunicator::~SerialCommunicator()
{

}

//-----------------------------------------------------------------------
//	Public Members Class
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//	Public Access Functions
//-----------------------------------------------------------------------

//	Add command callback function
//		RETURN - true if added success, otherwise false
bool SerialCommunicator::addCommandCallbackFunction( const char *command, void (*callbackFunc)() )
{
	bool result;
	
	//	Check if number of commands are full or not?
	if( this->numCommandCallbacks > MAX_SERIAL_COMMAND_CALLBACK_FUNCTION )
	//	Command in array is full, so set result false
		result = false;
	else
	{
		//	Copy command string and callback function into array.
		CommandCallback *newCommandCallback = &this->commandCallbackArray[this->numCommandCallbacks++];
		strncpy( newCommandCallback->command, command, MAX_SERIAL_BUFFER_SIZE );
		newCommandCallback->callbackFunc = callbackFunc;	
		
		//	Success set result to true
		result = true;
	}
	
	//	return result
	return result;
}

//	Read serial communication and check end of serial command
//		if it received complete command then call a callback for given command.
//		After processed callback function, sent back a completed processed command
void SerialCommunicator::readSerialCommunicator()
{
	//	Initialize a serial char buffer
	char buffer;
	
	//	loop
	while( this->serial->available() )
	{
		//	Read char from serial
		buffer = this->serial->read();
		
		//	Check for end of command
		if( buffer == this->terminatedCommandChar )
		//	This is a end of string
		{
			//	Store end of string in command array
			this->serialBuffer[this->currentSerialBufferPosition++] = '\0';
			
			//	Reset current serial buffer position
			this->currentSerialBufferPosition = 0;
			
			//
			//	Search for matched command string
			//
			{				
				//	Loop over all command array and store matched index
				bool isMatchedCommandCallback = false;
				for( unsigned int i = 0 ; i < this->numCommandCallbacks ; i ++ )
				{
					//	Get command callback at current index
					CommandCallback currentCommandCallback = this->commandCallbackArray[i];
					//	Compare command string
					if( strcmp( *this->serialBuffer, currentCommandCallback.command ) == 0 )
					//	Matched command
					{
						//	Call a callback function
						( currentCommandCallback.callbackFunc )();
						
						//	Clear serial buffer
						this->clearSerialSerialCommunicatorBuffer();
						
						//	Send acknowledge to a caller
					}
				}
			}
		}
		else
		//	This is not a terminated command char, so check it for only printable character
			if( isprint( buffer) )
			//	New character is printable, so add it into serial buffer
			{
				//	Store in serial buffer array
				( *this->serialBuffer[this->currentSerialBufferPosition++] ) = buffer;
				//	Check serial buffer doesn't full yet?
				if( this->currentSerialBufferPosition > MAX_SERIAL_BUFFER_SIZE )
				//	Buffer is full, so warp around
					//	Warp around buffer
					this->currentSerialBufferPosition = 0;
			}
	}
}

//-----------------------------------------------------------------------
//	Public Member Functions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//	Public Operators Overload
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//	Protected Member Functions
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//	Private Member Functions
//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
//
//	End SerialCommunicator
//
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//	EOF
//
//-----------------------------------------------------------------------

