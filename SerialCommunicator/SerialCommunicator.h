//-----------------------------------------------------------------------
//
//   KdsLockEngine is written by Wiphoo (Terng) Methachawalit
//       for Wiphoo - Arduino Libraries - created on 2014/09/09
//   Copyright Wiphoo Methachawalit, 2014. All rights reserved.
//
//-----------------------------------------------------------------------

#if !defined(___SerialCommunicator_h)
#define ___SerialCommunicator_h

//-----------------------------------------------------------------------
//
//	Standard Imports
//
//-----------------------------------------------------------------------

#include <SoftwareSerial.h>

//-----------------------------------------------------------------------
//
//	Non-Standard Imports
//
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//
//	Defines
//
//-----------------------------------------------------------------------

//	Maximum buffer size
#define MAX_SERIAL_BUFFER_SIZE 100

//	Maximum serial command callback function
#define MAX_SERIAL_COMMAND_CALLBACK_FUNCTION 20

//	Default delimiter in serial communication
#define DEFAULT_DELIMITER_CHAR ':'

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
//	Class SerialCommunicator
//
//-----------------------------------------------------------------------
class SerialCommunicator
{

	//-----------------------------------------------------------------------
	//	Public Members Class
	//-----------------------------------------------------------------------
	public:
		
		class CommandCallback
		{
			public:
				//	Command String
				char command[MAX_SERIAL_BUFFER_SIZE];
				
				//	Callback function	
				void (*callbackFunc)();
		};
	
	//-----------------------------------------------------------------------
	//	Protected Members Class
	//-----------------------------------------------------------------------
	protected:
		
		//	Arduino software serial
		SoftwareSerial *serial;
		
		//	Serial buffer
		char *serialBuffer[MAX_SERIAL_BUFFER_SIZE];
		
		//	Current position in serial buffer
		unsigned int currentSerialBufferPosition;
		
		//	Terminated serial command char
		char terminatedCommandChar;
		
		//	Delimiter in serial communication
		char delimieterChar;
		
		//	Array of command string and callback function
		CommandCallback commandCallbackArray[MAX_SERIAL_COMMAND_CALLBACK_FUNCTION];
		
		//	Number of current command and callback function in array
		unsigned int numCommandCallbacks;
		
		//	Default callback function when command doesn't match in commandCallbackArray
		void (*defaultCallbackFunc)();
		
	//-----------------------------------------------------------------------
	//	Private Members Class
	//-----------------------------------------------------------------------
	private:
	
	//-----------------------------------------------------------------------
	//	Public Constructors / Destructors
	//-----------------------------------------------------------------------
	public:
	
	SerialCommunicator( SoftwareSerial &serial );
	
	~SerialCommunicator();
	
	//-----------------------------------------------------------------------
	//	Public Access Functions
	//-----------------------------------------------------------------------
	public:
	
		//	Add command callback function
		//		RETURN - true if added success, otherwise false
		bool addCommandCallbackFunction( const char *command, void (*callbackFunc)() );
		
		//	Set default callback function handler
		//		This will called when doesn't have valid command in command callback function array
		void setDefaultCallbackFunction( void (*callbackFunc)() )
		{ this->defaultCallbackFunc = callbackFunc; }
			
		//	Clear serial communication buffer
		void clearSerialSerialCommunicationBuffer()
		{ this->currentSerialBufferPosition = 0; }
		
		//	Read serial communication and check end of serial command
		//		if it received complete command then call a callback for given command.
		//		After processed callback function, sent completed processed command acknowledge.
		void readSerailCommunication();
		
		//	Sent a acknowledge string
		//		FORMAT - acknowledge + delimiter char + result str + terminated command char
		void sendSerialAcknowledge( const char *acknowledgeStr, 
										const char *resultStr, 
										const char delimiter = DEFAULT_DELIMITER_CHAR )
		{
			//	Write serial acknowledge
			this->serial->print( acknowledgeStr );
			this->serial->print( delimiter );
			this->serial->print( resultStr );
			this->serial->println( this->terminatedCommandChar );
		}
		
	//-----------------------------------------------------------------------
	//	Public Member Functions
	//-----------------------------------------------------------------------
	public:
		
	//-----------------------------------------------------------------------
	//	Public Operators Overload
	//-----------------------------------------------------------------------
	public:
	
	//-----------------------------------------------------------------------
	//	Protected Member Functions
	//-----------------------------------------------------------------------
	protected:
	
	//-----------------------------------------------------------------------
	//	Private Member Functions
	//-----------------------------------------------------------------------
	private:
};

//-----------------------------------------------------------------------
//
//	End SerialCommunicator
//
//-----------------------------------------------------------------------

#endif

//-----------------------------------------------------------------------
//
//	EOF
//
//-----------------------------------------------------------------------
