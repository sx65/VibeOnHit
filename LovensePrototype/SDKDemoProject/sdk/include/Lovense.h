#ifndef __LOVENSE_SDK_H__
#define __LOVENSE_SDK_H__


#ifdef LOVENSE_SDK_STATIC
#define LOVENSE_EXTERN
#else
#ifdef LOVENSE_SDK_EXPORTS
#define LOVENSE_EXTERN __declspec(dllexport)
#else
#define LOVENSE_EXTERN __declspec(dllimport)
#endif
#endif

#include "LovenseDef.h"

/* Compatibility for non-Clang compilers */
#ifndef __has_declspec_attribute
#define __has_declspec_attribute(x) 0
#endif

#ifdef  __cplusplus
extern "C" {
#endif


enum LovenseDeviceState  {
	/*unkonwn*/
	LOVENSE_DEVICE_UNKONWN  = 0,  // unkonwn
	/*off*/				    
	LOVENSE_DEVICE_OFF      = 1,
	/*on*/				    
	LOVENSE_DEVICE_ON       = 2,
	/*disabled*/
	LOVENSE_DEVICE_DISABLED = 3
};

enum LovenseToyState{
	LOVENSE_TOY_CONNECTED    = 1,
	LOVENSE_TOY_DISCONNECTED = 2,
	LOVENSE_TOY_DISABLED     = 3,
};

struct LovenseToyItem
{
	char*               id;
	char*               name;
	enum CLovenseToy::ToyType type;
};

/**Abstract class*/
class LOVENSE_EXTERN ILovenseSDKNotify
{
public:
	/*Bluetooth device status notification*/
	virtual void LovenseDeviceStatus(enum LovenseDeviceState state, const char* device) = 0;

	/*Call when toy search start*/
	virtual	void LovenseSearchStart() = 0;

	/*Call when toy searching toy*/
	virtual void LovenseSearchingToys(const struct LovenseToyItem* item) = 0;

	/*Call when Something went wrong*/
	virtual void LovenseErrorEvent(int errorCode, const char* errorMsg) = 0;

	/*Call when toy search end*/
	virtual void LovenseSearchEnd() = 0;

	/*Call when send cmd return*/
	virtual void LovenseSendCmdResult(const char* szToyID, CLovenseToy::CmdId cmd, const char* result, CLovenseToy::ErrorCode errorCode) = 0;

	/*Call when toy connected, or disconnected or others*/
	virtual void LovenseToyStatus(const struct LovenseToyItem* item, enum LovenseToyState state) = 0;

	/*Call the QueryToySupportCommand API to return the results.*/
	virtual void LovenseToySupportedCommands(const char* szToyID, const CLovenseToy::CmdId* commands, int length) = 0;
};


class LOVENSE_EXTERN CLovenseToyManager
{
public:
	/**
	* Pass your token into Lovense framework
	*/
	virtual bool SetDeveloperToken(const char* szToken) = 0;

	/**
	* Subscription notification
	*/
	virtual void RegisterEventCallBack(ILovenseSDKNotify* lovenseNotify) = 0;

	/**
	* Search for Lovense toys
	* return [CLovenseToy::ErrorCode]
	*/
	virtual int StartSearchToy() = 0;

	/**
	* Stop searching
	* return [CLovenseToy::ErrorCode]
	*/
	virtual int StopSearchToy() = 0;

	/**
	* Send a command to the toy. support number of cmd parameters is 0 or 1;
	* 
	* @param toyId toy ID
	* @param commandType command
	* @param paramDict command parameters
	* @return [CLovenseToy::ErrorCode]
	*/
	virtual int SendCommand(const char* szToyID, CLovenseToy::CmdId cmd) = 0;

	
	virtual int SendCommand(const char* szToyID, CLovenseToy::CmdId cmd, int nParam) = 0;
	/*
	* Send a command to the toy. support number of cmd parameters is 2;
	*/
	virtual int SendCommand(const char* szToyID, CLovenseToy::CmdId cmd, int nParam1, int nParam2) = 0;
	/*
	* Send a command to the toy. support number of cmd parameters is 3;
	*/
	virtual int SendCommand(const char* szToyID, CLovenseToy::CmdId cmd, int nParam1, int nParam2, int nParam3) = 0;


	/**
	* Connect a toy
	* 
	* @param toyId toy ID
	* If Successful connection successfully, will notify by LovenseToyStatus;
	* @return [CLovenseToy::ErrorCode]
	*/
	virtual int ConnectToToy(const char* szToyID) = 0;

	/**
	* DisConnect a toy
	* 
	* @param toyId toy ID
	* If Successfully disconnected, will notify by LovenseToyStatus;
	* @return [CLovenseToy::ErrorCode]
	*/
	virtual int DisConnectedToy(const char* szToyID) = 0;

	/**
	* Check what commands the toy supports
	* @param toy type
	* @return [CLovenseToy::ErrorCode] callback: LovenseToySupportedCommands
	* @brief: Call after the toy is connected
	*/
	virtual int QueryToySupportedCommands(const char* szToyID) = 0;
};

/**
* Get the class instance through this export interface!
*/
LOVENSE_EXTERN CLovenseToyManager* const GetLovenseToyManager();

/**
* Release worker context
*/
LOVENSE_EXTERN void                      ReleaseLovenseToyManager();
	

#ifdef  __cplusplus
}
#endif

#endif // __LOVENSE_SDK_H__
