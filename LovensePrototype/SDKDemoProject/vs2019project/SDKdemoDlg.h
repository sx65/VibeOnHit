
#pragma once
#include "afxwin.h"
#include <map>
#include <unordered_map>

#include "Lovense.h"


#include <memory>
using namespace std;

class MyToyInfo {
public:
	CString                         toy_id;
	CString                         toy_name;
	int32_t                         toy_battery{-1};
	uint32_t                        toy_type{0};
	bool                            toy_connected{false};
	std::vector<CLovenseToy::CmdId> supported_cmds;
};

struct ToyCmdResult
{
	std::string        toy_id;
	CLovenseToy::CmdId cmd;
	std::string        result;
	int                code;
};


struct CmdConfig
{
	int          cmd_type;
	std::wstring name;
	int          param_count;
	std::wstring describe;
	
};


typedef std::map < CString/*toy id*/, std::shared_ptr<MyToyInfo>> TMap;
typedef std::map < CString/*toy id*/, std::shared_ptr<MyToyInfo>>::iterator TMap_Iterator;

class CSDKdemoDlg : public CDialogEx , ILovenseSDKNotify
{
public:
	CSDKdemoDlg(CWnd* pParent = NULL);	

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SDKDEMO_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedsearchButton();
	afx_msg void OnBnClickedStopSearchButton();

	afx_msg void OnBnClickedSendButton();
	afx_msg void OnCbnSelchangeToyID();
	afx_msg void OnCbnSelchangeOnCommand();
	//QueryToySupportedCommands
	afx_msg void OnBnClickedButton3();

	afx_msg void OnBnClickedLightOpenButton();
	afx_msg void OnBnClickedLightCloseButton();
	afx_msg void OnBnClickedLightFlashButton();
	afx_msg void OnBnClickedButtonConnected();
	afx_msg void OnBnClickedButtonDisconnected();
	afx_msg void OnEnChangeEdit1();

	afx_msg LRESULT OnDeviceStatusMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSearchToy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToyStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToyCmdResult(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToySupportedCmdsResult(WPARAM wParam, LPARAM lParam);
protected:
	void AddToyInfo(LPCTSTR toyID, LPCTSTR toyName);

	void get_params(CLovenseToy::CmdId cmd, std::vector<int>& params);
	int  send_command(std::string szToyID, CLovenseToy::CmdId cmd);
public:
	/*Bluetooth device status notification*/
	virtual void LovenseDeviceStatus(enum LovenseDeviceState state, const char* device);

	virtual	void LovenseSearchStart() ;
	virtual void LovenseSearchEnd() ;

	virtual void LovenseSearchingToys(const LovenseToyItem* item);

	virtual void LovenseSendCmdResult(const char * szToyID, CLovenseToy::CmdId cmd, const char *result, CLovenseToy::ErrorCode errorCode) ;

	/*Toy status change*/
	virtual void LovenseToyStatus(const struct LovenseToyItem* item, enum LovenseToyState state);
	virtual void LovenseErrorEvent(int errorCode, const char *errorMsg);
	
	virtual void LovenseToySupportedCommands(const char* szToyID, const CLovenseToy::CmdId* commands, int length);

	void OutPutAppLog(CString log);
	void PostAppLog(CString log);
private:
	HICON                                 m_hIcon;
	CComboBox                             m_combToyName;	
	CButton                               m_btnSearch;
	CButton                               m_btnStop;
	CStatic                               m_txBetarry;
	CEdit                                 m_editToken;
	CRichEditCtrl                         m_richedit_app_output;
	CComboBox                             m_toyListInfo;
	CComboBox                             m_cmdList;
	CComboBox                             m_paramList;
	CComboBox                             m_paramList_1;

	std::unordered_map<std::wstring, int> m_mapCmd;
	TMap                                  m_search_toy;

	std::unordered_map<int, CmdConfig>    m_cmd_config;

	CLovenseToyManager*                   m_toy_manager{nullptr};
	HWND                                  m_main_hwnd{nullptr};
};
