#pragma once

#ifdef  __cplusplus
extern "C" {
#endif

namespace CLovenseToy
{
	enum CmdId
	{
		COMMAND_UNKNOWN                   = -1,

		/**
		-Power off!
		- param Key = no parameter
		*/
		COMMAND_POWER_OFF                 = 100,

		/**
		- Vibrate the toy .The parameter must be between 0 and 20!
		- Supported toys = all
		*/
		COMMAND_VIBRATE                   = 101,

		/**
		- Rotate the toy .The parameter must be between 0 and 20!
		- Supported toys = Nora
		*/
		COMMAND_ROTATE                    = 102,

		/**
		- Rotate clockwise .The parameter must be between 0 and 20!
		- Supported toys = Nora
		*/
		COMMAND_ROTATE_CLOCKWISE          = 103,

		/**
		- Rotate anti-clockwise .The parameter must be between 0 and 20!
		- Supported toys = Nora
		*/
		COMMAND_ROTATE_ANTI_CLOCKWISE     = 104,

		/**
		- Change the rotation direction
		- param Key = no parameter
		- Supported toys = Nora
		*/
		COMMAND_ROTATE_CHANGE             = 105,

		/**
		- Activate the first vibrator at level n .The parameter must be between 0 and 20!
		- param Key = kSendCommandParamKey_VibrateLevel
		- Supported toys = Edge
		*/
		COMMAND_VIBRATE1                  = 113,

		/**
		- Activate the second vibrator at level n .The parameter must be between 0 and 20!
		- param Key = kSendCommandParamKey_VibrateLevel
		- Supported toys = Edge
		*/
		COMMAND_VIBRATE2                  = 114,

		/**
		- Activate the third vibrator at level n .The parameter must be between 0 and 20!
		- param Key = kSendCommandParamKey_VibrateLevel
		- Supported toys = Lapis
		*/
		COMMAND_VIBRATE3                  = 115,

		/**
		- The motor vibrates at gear x and stops after t seconds.
		- param x: (0,20)
		- param t: (0, 300)
		*/
		COMMAND_AVIBRATE                  = 116,

		/**
		- The first motor vibrates at gear x and stops after t seconds.
		- param x: (0,20)
		- param t: (0, 300)
		*/
		COMMAND_AVIBRATE1                 = 117,

		/**
		- The second motor vibrates at gear x and stops after t seconds.
		- param x: (0,20)
		- param t: (0, 300)
		*/
		COMMAND_AVIBRATE2                 = 118,

		/**
		- The third motor vibrates at gear x and stops after t seconds.
		- param x: (0,20)
		- param t: (0, 300)
		*/
		COMMAND_AVIBRATE3                 = 119,

		/**
		- Flash the light 3 times
		- param Key = no parameter
		- Supported toys = all
		*/
		COMMAND_FLASH                     = 201,

		/**
		- Turn off the light (saved permanently)
		- param Key = no parameter
		- Supported toys = Lush / Hush / Edge
		*/
		COMMAND_LIGHT_OFF                = 210,

		/**
		- Turn on the light (saved permanently)
		- param Key = no parameter
		- Supported toys = Lush / Hush / Edge
		*/
		COMMAND_LIGHT_ON                 = 211,

		/**
		Get the light status (1: on, 0:off)
		- param Key = no parameter
		* Supported toys = Lush  Hush  Edge
		*/
		COMMAND_GET_LIGHT_STATUS         = 212,

		/**
		- Turn off the AID light (saved permanently)
		- param Key = no parameter
		- Supported toys = Domi
		*/
		COMMAND_ALIGHT_OFF               = 220,

		/**
		- Turn on the AID light (saved permanently)
		- param Key = no parameter
		- Supported toys = Domi
		*/
		COMMAND_ALIGHT_ON                = 221,

		/**
		Get the AID light status (1: on, 0:off)
		- param Key = no parameter
		* Supported toys = Domi
		*/
		COMMAND_GET_ALIGHT_STATUS        = 222,

		/**
		- Get battery status,
		- param Key = no parameter
		* Supported toys = all
		*/
		COMMAND_GET_BATTERY              = 300,

		/**
		Get device/toy information
		- param Key = no parameter
		* Supported toys = all
		*/
		COMMAND_GET_DEVICE_TYPE          = 310,

		/**
		Inflatable n seconds. The parameter must be between (1-3)
		* Supported toys = Max
		*/
		COMMAND_AIR_IN                   = 330,

		/**
		Deflation n seconds. The parameter must be between (1-3)
		* Supported toys = Max
		*/
		COMMAND_AIR_OUT                  = 331,

		/**
		Cycle inflation for n seconds, deflation for n seconds, 0 for stop!
		The parameter must be between (0-3)
		* Supported toys = Max
		*/
		COMMAND_AIR_AUTO                 = 332,

		/**
		Start tracking the toy movement (0-4)
		- param Key = no parameter
		* Supported toys = Max,Nora
		*/
		COMMAND_START_MOVE               = 400,

		/*
		Stop tracking the toy movement
		- param Key = no parameter
		* Supported toys = Max,Nora
		*/
		COMMAND_STOP_MOVE                = 401,

		/**
		- Control the toy .The parameter must be between 0 and 20!
		- Supported toys = flexer
		*/
		COMMAND_FINGER                   = 402,

		/**
		- Control the toy, stretch out and draw back.The parameter must be between 0 and 20!
		- Supported toys = gravity
		*/
		COMMAND_THRUSTING                = 403,

		/**
		- Control the three motors of the toy simultaneously.
		- parameters: params1[-1,-20], params2[-1,-20], params3[-1,-20]!
		- -1: keep
		- 0:  stop		
		*/
		COMMAND_MULTIPLE_CONTROL         = 404,

		/**
		- Control the toy, depth.
		- parameters: params[depth: 0-20]
		- Supported toys = soloce,soloce pro
		*/
		COMMAND_DEPTH_CONTROL            = 405,

		/**
		- Set/get the toy's current mode. TouchMode.
		- parameters: params[mode: 0-4]
		- empty: params[mode: 0-4]
		- 0: Key mode, controlled by the key vibration
		- 1: Touch mode. The deeper the insert, the stronger the vibration
		- 2: Speed mode. The faster the speed, the stronger the vibration
		- 3: Interactive mode, returns the current insertion direction, speed, position. The format is as follows:
			■ dir: Direction, 0 unknown, 1 in, 2 out
			■ speed: Speed. The value ranges from 0 to 10
			■ site: Location information, a total of 4 locations uploaded
		- 5: The automatic interactive mode, while controlling its own vibration according to the depth,
		-    returns the same data format as 3
		- Supported toys = Mission2
		*/
		COMMAND_SET_TOUCH_MODE           = 406,

		/**
		- Set/get the toy's current mode. TouchMode.
		- parameters: params[mode: 0-4]
		- empty: params[mode: 0-4]
		- 0: Key mode, controlled by the key vibration
		- 1: Touch mode. The deeper the insert, the stronger the vibration
		- 2: Speed mode. The faster the speed, the stronger the vibration
		- 3: Interactive mode, returns the current insertion direction, speed, position. The format is as follows:
			■ dir: Direction, 0 unknown, 1 in, 2 out
			■ speed: Speed. The value ranges from 0 to 10
			■ site: Location information, a total of 4 locations uploaded
		- 5: The automatic interactive mode, while controlling its own vibration according to the depth,
		-    returns the same data format as 3
		- Supported toys = Mission2
		*/
		COMMAND_GET_TOUCH_MODE          = 407,

		/**
		- Set the three touch intensities corresponding to the touch keys，SetTouchV.
		- parameters: params1[x: 1-3], Indicates the corresponding gear of the touch key
		- parameters: params2[y: 0-100], Represents the attenuation value corresponding to the gear position
		- Supported toys = Mission2
		*/
		COMMAND_SET_TOUCH_VALUE         = 408,

		/**
		- Gets the decay value of the current toy and returns the corresponding
		- values for the 3 gears，GetTouchV.
		- return: (example)TV:40,80,100
		- Supported toys = Mission2
		*/
		COMMAND_GET_TOUCH_VALUE         = 409,

		/**
		- Setting the toy's decay level is the same as the click function of the touch button，SetTouchL.
		- parameters: params1[x: 1-3], Indicates the corresponding gear of the touch key
		- Supported toys = Mission2
		*/
		COMMAND_SET_TOUCH_LEVEL         = 410,

		/**
		- Gets the decay level of the current toy
		- values for the 3 gears，GetTouchL.
		- return: (example)TL:3
		- Supported toys = Mission2
		*/
		COMMAND_GET_TOUCH_LEVEL         = 411,

		/**
		- Control the two motors of the toy simultaneously.
		- parameters: params1[-1,-20], params2[-1,-20]
		*/
		COMMAND_MULTIPLE_2_CONTROL      = 412,
	};

	enum ErrorCode
	{
		LOVENSE_ERR_UNKNOWN                      = -1,
							                     
		LOVENSE_ERR_SUCCESS                      = 0,

		//system error. example:winrt::hresult_error
		LOVENSE_ERR_SYSTEM                       = 100,
							                     
		LOVENSE_ERR_RUNTIME                      = 200,
							                     
		LOVENSE_ERR_ABORT                        = 500,
		//Invalid argument
		LOVENSE_ERR_PARAM_ERROR                  = 501,

		//Invalid token
		LOVENSE_ERR_INVALIDTOKEN                 = 1001,

		//HID interface initialization failed!
		LOVENSE_ERR_HID_INIT_FAILED              = 1002,
										         
		//No HID USB Dongle found!		         
		LOVENSE_ERR_HID_DONGLE_NOT_FOUND         = 1003,

		//Unable to get HID Dongle information!
		LOVENSE_ERR_HID_GET_DONGLE_ID_ERROR      = 1004,


		//COM READ ERROR
		LOVENSE_ERR_COM_READ_ERROR               = 1009,

		//COM WRITE ERROR
		LOVENSE_ERR_COM_WRITE_ERROR              = 1010,

		//COM NOT FOUND ERROR
		LOVENSE_ERR_COM_NOT_FOUND_ERROR          = 1011,
		//HID READ ERROR				         
		LOVENSE_ERR_HID_READ_ERROR               = 1012,
										         
		//HID WRITE ERROR				         
		LOVENSE_ERR_HID_WRITE_ERROR              = 1013,
										         
		//DEVICE NOT FOUND				         
		LOVENSE_ERR_DONGLE_NO_FOUND_ERROR        = 1014,
										         
		//HID DEVICE OPEN FAILED		         
		LOVENSE_ERR_HID_OPEN_FAILED_ERROR        = 1015,
										         
		//COM DEVICE OPEN FAILED		         
		LOVENSE_ERR_COM_OPEN_FAILED_ERROR        = 1016,

		//device can not be write
		LOVENSE_ERR_DEVICE_WRITE_FAILED          = 1017,
										         
		//device can not be read		         
		LOVENSE_ERR_DEVICE_READ_FAILED           = 1018,

		//device is searching, You can't send any commands
		LOVENSE_ERR_DEVICE_IS_SEARCHING          = 1019,

		LOVENSE_ERR_CMD_NOT_MATCHING             = 1020,

		//send cmd too more.
		LOVENSE_ERR_HANDLE_BUSYSING              = 1021,
									             
		LOVENSE_ERR_START_SEARCHING              = 1022,

		//--------------------------------------------------------
		LOVENSE_ERR_ENTER_BOOTLOADER_SEND_FAILED = 1100,
		LOVENSE_ERR_ENTER_BOOTLOADER_RECV_FAILED = 1101,
		LOVENSE_ERR_EXIT_BREAK_MODE_SEND_FAILED  = 1102,
		LOVENSE_ERR_BOOTLOADER_RESTART_FAILED    = 1103,

		LOVENSE_ERR_DFU_CMD_SEND_ERROR           = 1112,
		LOVENSE_ERR_DFU_CMD_RECV_ERROR           = 1113,
		LOVENSE_ERR_DFU_CMD_RESP_ERROR           = 1114,

		LOVENSE_ERR_UPDATE_FILE_OPEN_ERROR       = 1120,
		LOVENSE_ERR_UPDATE_FILE_READ_ERROR       = 1121,
		LOVENSE_ERR_UPDATE_FILE_COPY_ERROR       = 1122,
										         
		LOVENSE_ERR_NOTIFY_UPDATED_FAILED        = 1130,
		LOVENSE_ERR_NOTIFY_RESTART_FAILED        = 1131,

		//--------------------------------------------------
		//dongle or bluetooth device invalid
		LOVENSE_ERR_DEVICE_INVALID               = 1500,

		//--------------------------------------------------
		//cmd args count not match
		LOVENSE_ERR_CMD_ARGS_SIZE_ERROR          = 2000,
		//cmd any value not valid		                 
		LOVENSE_ERR_CMD_ARGS_VALUE_ERROR         = 2001,
		//cmd not exist			                 
		LOVENSE_ERR_CMD_NOT_EXIST                = 2002,
		//cmd params type error			                 
		LOVENSE_ERR_CMD_ARGS_TYPE_ERROR          = 2003,
		//Unrecognized command			         
		LOVENSE_ERR_INVALID_CMD                  = 2004,

		//The toy does not exist.
		LOVENSE_ERR_TOY_NOT_EXIST                = 3001,
		//Toys are not connected	             
		LOVENSE_ERR_TOY_NOT_CONNECTED            = 3002,
		//Toy connect failed		             
		LOVENSE_ERR_TOY_CONNECT_FAILED           = 3003,
	};

	/*Lovense Toy Type*/
	enum ToyType {
		LVS_UNKNOWN      = 0,
		LVS_MAX          = 0x01,
		LVS_NORA         = 0x02,
		LVS_HUSH         = 0x03,
		LVS_LUSH         = 0x04,
		LVS_AMBI         = 0x05,
		LVS_DOMI         = 0x06,
		LVS_SECRET       = 0x07,
		LVS_EDGE         = 0x08,
		LVS_OSCI         = 0x09,
		LVS_OSCI3        = 0X0A,
		LVS_DIAMO        = 0x0B,
		LVS_MISSION      = 0x0C,		
		LVS_DOLCE        = 0x0E,
		LVS_XMACHINE     = 0x0F,
		LVS_HYPHY        = 0x10,
		LVS_CALOR        = 0x11,
		LVS_FERRI        = 0x12,
		LVS_EXOMOON      = 0x13,
		LVS_GUSH         = 0x14,
		LVS_TENERA       = 0x15,
		LVS_FLEXER       = 0x16,
		LVS_GRAVITY      = 0x17,
		LVS_GEMINI       = 0x18,
		LVS_SOLACE       = 0X19,
		LVS_RIDGE        = 0X20,
		LVS_VULSE        = 0X21,
		LVS_LAPIS        = 0X22,
		LVS_MINIXMACHINE = 0X23,  //Mini XMachine
		LVS_MISSION2     = 0X24,
		LVS_SOLACEPRO    = 0X25,
		LVS_GUSH2        = 0x26,
		LVS_GRAVITY2     = 0x27,
		LVS_SPINEL       = 0x28,
	};
}

#ifdef  __cplusplus
}
#endif
