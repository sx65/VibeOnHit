

//

#include "stdafx.h"
#include <iostream>
#include <lmshare.h>
#include <thread>
#include <string>

#include "SDKdemo.h"
#include "SDKdemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace CLovenseToy;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    


protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


#define APP_WM_LOG_MESSAGE        (WM_APP + 1)
#define APP_WM_DEVICE_STATE       (WM_APP + 2)
#define APP_WM_SEARCH_START       (WM_APP + 3)
#define APP_WM_SEARCH_END         (WM_APP + 4)
#define APP_WM_SEARCH_TOY         (WM_APP + 5)
#define APP_WM_TOY_STATUS         (WM_APP + 6)
#define APP_WM_TOY_SUPPORT_CMDS   (WM_APP + 7)
#define APP_WM_CMD_RESULT         (WM_APP + 10)



CSDKdemoDlg::CSDKdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SDKDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSDKdemoDlg::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_toyListInfo);
	DDX_Control(pDX, IDC_COMBO2, m_cmdList);
	DDX_Control(pDX, IDC_COMBO3, m_paramList);
	DDX_Control(pDX, IDC_COMBO5, m_paramList_1);
	DDX_Control(pDX, IDC_COMBO4, m_combToyName);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON2, m_btnStop);
	DDX_Control(pDX, IDC_EDIT1, m_editToken);
	DDX_Control(pDX, IDC_TEXT_BETTARY, m_txBetarry);
	DDX_Control(pDX, IDC_RICHEDIT_OUTPUT_APP_LOG, m_richedit_app_output);
}

BEGIN_MESSAGE_MAP(CSDKdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1,             &CSDKdemoDlg::OnBnClickedsearchButton)
	ON_BN_CLICKED(IDC_BUTTON2,             &CSDKdemoDlg::OnBnClickedStopSearchButton)
	ON_BN_CLICKED(IDC_SEND_BUTTON,         &CSDKdemoDlg::OnBnClickedSendButton)
	ON_BN_CLICKED(IDC_LIGHT_OPEN_BUTTON,   &CSDKdemoDlg::OnBnClickedLightOpenButton)
	ON_BN_CLICKED(IDC_LIGHT_CLOSE_BUTTON,  &CSDKdemoDlg::OnBnClickedLightCloseButton)
	ON_BN_CLICKED(IDC_LIGHT_FLASH_BUTTON,  &CSDKdemoDlg::OnBnClickedLightFlashButton)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTED,    &CSDKdemoDlg::OnBnClickedButtonConnected)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECTED, &CSDKdemoDlg::OnBnClickedButtonDisconnected)
	ON_CBN_SELCHANGE(IDC_COMBO1,           &CSDKdemoDlg::OnCbnSelchangeToyID)
	ON_EN_CHANGE(IDC_EDIT1,                &CSDKdemoDlg::OnEnChangeEdit1)
	ON_CBN_SELCHANGE(IDC_COMBO2,           &CSDKdemoDlg::OnCbnSelchangeOnCommand)
	ON_BN_CLICKED(IDC_BUTTON3,             &CSDKdemoDlg::OnBnClickedButton3)

	ON_MESSAGE(APP_WM_LOG_MESSAGE,         &CSDKdemoDlg::OnLogMessage)
	ON_MESSAGE(APP_WM_DEVICE_STATE,        &CSDKdemoDlg::OnDeviceStatusMessage)
	ON_MESSAGE(APP_WM_SEARCH_START,        &CSDKdemoDlg::OnSearchStart)
	ON_MESSAGE(APP_WM_SEARCH_END,          &CSDKdemoDlg::OnSearchEnd)
	ON_MESSAGE(APP_WM_SEARCH_TOY,          &CSDKdemoDlg::OnSearchToy)
	ON_MESSAGE(APP_WM_TOY_STATUS,          &CSDKdemoDlg::OnToyStatus)
	ON_MESSAGE(APP_WM_TOY_SUPPORT_CMDS,    &CSDKdemoDlg::OnToySupportedCmdsResult)	
	ON_MESSAGE(APP_WM_CMD_RESULT,          &CSDKdemoDlg::OnToyCmdResult)
END_MESSAGE_MAP()


LRESULT CSDKdemoDlg::OnLogMessage(WPARAM wParam, LPARAM lParam)
{
	CString* pText = reinterpret_cast<CString*>(wParam);
	if (pText) {
		OutPutAppLog(*pText);
		delete pText;
	}
	return 0;
}


BOOL CSDKdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_main_hwnd = AfxGetMainWnd()->GetSafeHwnd();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	//AfxInitRichEdit2();

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	
	const std::vector<CmdConfig> TestCmdConfig = {
		{CLovenseToy::COMMAND_GET_DEVICE_TYPE      ,_T("DeviceType"),          0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_GET_LIGHT_STATUS     ,_T("Light Status"),        0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_GET_BATTERY          ,_T("Battery"),             0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_VIBRATE              ,_T("Vibrate"),             1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_LIGHT_ON             ,_T("Light On"),            0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_LIGHT_OFF            ,_T("Light Off"),           0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_POWER_OFF            ,_T("Power Off"),           0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_FLASH                ,_T("Flash"),               0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_ROTATE               ,_T("Rotate"),              1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_ROTATE_CLOCKWISE     ,_T("Clockwise Rotate"),    1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_ROTATE_ANTI_CLOCKWISE,_T("Anticlockwise Rotate"),1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_ROTATE_CHANGE        ,_T("Change Rotate"),       0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_VIBRATE1             ,_T("Vibrate1"),            1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_VIBRATE2             ,_T("Vibrate2"),            1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_VIBRATE3             ,_T("Vibrate3"),            1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_AVIBRATE             ,_T("Auto Vibrate"),        2, _T("Param(0~20)-Param2(0~300)"), },
		{CLovenseToy::COMMAND_AVIBRATE1            ,_T("Auto Vibrate 1"),      2, _T("Param(0~20)-Param2(0~300)"), },
		{CLovenseToy::COMMAND_AVIBRATE2            ,_T("Auto Vibrate 2"),      2, _T("Param(0~20)-Param2(0~300)"), },
		{CLovenseToy::COMMAND_AVIBRATE3            ,_T("Auto Vibrate 3"),      2, _T("Param(0~20)-Param2(0~300)"), },
		{CLovenseToy::COMMAND_ALIGHT_OFF           ,_T("ALight Off"),          0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_ALIGHT_ON            ,_T("ALight On"),           0, _T("No Param"),                  },
		{CLovenseToy::COMMAND_AIR_IN               ,_T("Air In"),              1, _T("Param(1~3)"),                },
		{CLovenseToy::COMMAND_AIR_OUT              ,_T("Air Out"),             1, _T("Param(1~3)"),                },
		{CLovenseToy::COMMAND_AIR_AUTO             ,_T("Air Auto"),            1, _T("Param(0~3)"),                },
		{CLovenseToy::COMMAND_FINGER               ,_T("Finger"),              1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_THRUSTING            ,_T("Thrusting"),           1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_DEPTH_CONTROL        ,_T("Depth"),               1, _T("Param(0~20)"),               },
		{CLovenseToy::COMMAND_MULTIPLE_CONTROL     ,_T("Multiply 3"),          3, _T("Param1(-1~20)-Param2(-1~20)-Param3(-1~20)"), },
		{CLovenseToy::COMMAND_MULTIPLE_2_CONTROL   ,_T("Multiply 2"),          2, _T("Param1(-1~20)-Param2(-1~20)"), },
		{CLovenseToy::COMMAND_GET_TOUCH_MODE       ,_T("GetTouchMode"),        0, _T("No Param")                   },
		{CLovenseToy::COMMAND_SET_TOUCH_MODE	   ,_T("SetTouchMode"),        1, _T("Param(0,1,2,3,5)")           },
		{CLovenseToy::COMMAND_SET_TOUCH_VALUE	   ,_T("SetTouchValue"),       2, _T("Param1(1-3)-Param2(0-100)")  },
		{CLovenseToy::COMMAND_GET_TOUCH_VALUE	   ,_T("GetTouchValue"),       0, _T("No Param")                   },
		{CLovenseToy::COMMAND_SET_TOUCH_LEVEL	   ,_T("SetTouchLevel"),       1, _T("Param1(1-3)")                },
		{CLovenseToy::COMMAND_GET_TOUCH_LEVEL	   ,_T("GetTouchLevel"),       0, _T("No Param")                   },
	};																            


	for (auto it : TestCmdConfig) {
		CString cmd_text;
		cmd_text.Format(L"%s(%s)", it.name.c_str(), it.describe.c_str());
		//m_cmdList.AddString(cmd_text);
		m_mapCmd[cmd_text.GetBuffer()] = it.cmd_type;

		m_cmd_config[it.cmd_type] = it;
	}

	for (int i = -1; i <= 20; i++)
	{
		auto s = std::to_wstring(i);
		m_paramList.AddString(s.c_str());
		m_paramList_1.AddString(s.c_str());
	}

	//m_paramList_1.ShowWindow(FALSE);
	m_paramList_1.EnableWindow(FALSE);

	m_cmdList.SetCurSel(0);
	m_paramList.SetCurSel(5);	
	m_paramList_1.SetCurSel(0);

	m_editToken.SetWindowTextW(_T(""));

	m_btnSearch.EnableWindow(FALSE);
	m_btnStop.EnableWindow(FALSE);

	m_toy_manager = GetLovenseToyManager();
	m_toy_manager->RegisterEventCallBack(this);
	
	return TRUE;  // 
}

void CSDKdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_CLOSE)
		{
			ReleaseLovenseToyManager();
			m_main_hwnd = nullptr;
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}



void CSDKdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); //

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		//
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CSDKdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

#define TAP_LINE L"-------------------------------------------------------------------------"
void CSDKdemoDlg::OnBnClickedsearchButton()
{
	this->OutPutAppLog(TAP_LINE);

	this->OutPutAppLog(L"[App] OnClick Start Search!");

	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL,path,MAX_PATH);
		
	CString token;
	m_editToken.GetWindowText(token);
	token = token.TrimLeft();
	token = token.TrimRight();
	std::string strToken = CW2A(token.GetString());  
	
	//todo token
	if (m_toy_manager->SetDeveloperToken(strToken.c_str()))
	{		
		int ret = m_toy_manager->StartSearchToy();
		if (ret == LOVENSE_ERR_SUCCESS)
		{
			m_btnSearch.EnableWindow(FALSE);
			m_btnStop.EnableWindow(TRUE);
		}
		else if (ret == LOVENSE_ERR_DEVICE_IS_SEARCHING)
		{
			this->OutPutAppLog(_T("Device is searching..."));
		}
		else {
			CString text;
			text.Format(_T("StartSearchToy failed. code: %d"), ret);
			this->OutPutAppLog(text);
		}
	}
	else {
		this->OutPutAppLog(_T("Check token invalid"));
	}
}


void CSDKdemoDlg::OnBnClickedStopSearchButton()
{
	this->OutPutAppLog(TAP_LINE);
	this->OutPutAppLog(L"[App] OnClick Stop Search");
	CLovenseToyManager *manager = GetLovenseToyManager();
	manager->StopSearchToy();
}

void CSDKdemoDlg::AddToyInfo(LPCTSTR toyID, LPCTSTR toyName)
{
	std::wstring info = toyID;
	m_toyListInfo.AddString(info.c_str());
	m_toyListInfo.SetCurSel(0);
}


void CSDKdemoDlg::get_params(CLovenseToy::CmdId cmd, std::vector<int>& v_params)
{
	v_params.clear();

	std::vector<CComboBox*> CComboBoxPrams = {&m_paramList, &m_paramList_1};

	for (auto it : CComboBoxPrams)
	{
		CString strParam;
		int idx = it->GetCurSel();
		if (idx < 0)
		{
			idx = 0;
		}

		it->GetLBText(idx, strParam);
		std::string param = CW2A(strParam.GetString());
		auto param_int = atoi(param.c_str());
		if (param_int >= 0)
		{
			v_params.push_back(param_int);
		}
		else
		{
			return;
		}
	}

	auto it = m_cmd_config.find(cmd);
	if (it != m_cmd_config.end() && v_params.size() > 0) {
		while (v_params.size() < it->second.param_count) {
			v_params.push_back(v_params[0]);
		}
	}
}

int CSDKdemoDlg::send_command(std::string szToyID, CLovenseToy::CmdId cmd)
{
	std::vector<int> params;
	this->get_params(cmd, params);

	int error = 0;
	int params_cnt = (int)params.size();
	if (params_cnt == 0) {
		error = m_toy_manager->SendCommand(szToyID.c_str(), cmd, -1);
	}
	else if (params_cnt == 1)
	{
		error = m_toy_manager->SendCommand(szToyID.c_str(), cmd, params[0]);
	}
	else if (params_cnt == 2)
	{
		error = m_toy_manager->SendCommand(szToyID.c_str(), cmd, params[0], params[1]);
	}
	else 
	{
		error = m_toy_manager->SendCommand(szToyID.c_str(), cmd, params[0], params[1], params[2]);
	}

	return error;
}

//send command to toy
void CSDKdemoDlg::OnBnClickedSendButton()
{
	CString strCmd;
	m_cmdList.GetLBText(m_cmdList.GetCurSel(), strCmd);

	auto cmdType = CLovenseToy::CmdId::COMMAND_VIBRATE;
	int curlCmd  = m_cmdList.GetCurSel();
	if (curlCmd < 0) {
		return;
	}

	m_cmdList.GetLBText(m_cmdList.GetCurSel(), strCmd);
	cmdType = (CLovenseToy::CmdId)m_mapCmd[strCmd.GetString()];

	std::vector<int> params;
	this->get_params(cmdType, params);

	int param_count = 0;
	auto it = m_cmd_config.find(cmdType);
	if (it != m_cmd_config.end()) {
		param_count = it->second.param_count;
	}


	std::wstring str_params;
	if (param_count > 0) {
		for (int i = 0; i < (int)params.size(); i++)
		{
			str_params += std::to_wstring(params[i]);
			if (i != params.size() - 1)
			{
				str_params += L",";
			}
		}
	}
	else {
		str_params = L"no params";
	}

	this->OutPutAppLog(TAP_LINE);
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);

	std::string toyID;
	toyID = CW2A(strID.GetString());

	CString log;
	log.Format(_T("[App] OnClick Send Cmd: id=%s type=%d param=%s")
		, strID.GetBuffer(), (int)cmdType, str_params.c_str());
	this->OutPutAppLog(log);
	clock_t startTime = clock();
	int error = this->send_command(toyID, cmdType);	
	clock_t endTime = clock();
	log.Format(_T("[App] Result: ret = %d (time = %dms)"), error, endTime-startTime);
	this->OutPutAppLog(log);
}

void CSDKdemoDlg::LovenseErrorEvent(int errorCode, const char *errorMsg)
{
	if (errorCode == LOVENSE_ERR_INVALIDTOKEN)
	{
		MessageBox(_T("Invalid token!"), _T("Error"), MB_OK);
	}

	CString log;
	log.Format(_T("**[SDK Callback] Error Code=%d %s"), errorCode, errorMsg?CString(errorMsg).GetBuffer():_T(""));
	this->OutPutAppLog(log);
}


LRESULT CSDKdemoDlg::OnDeviceStatusMessage(WPARAM wParam, LPARAM lParam)
{
	//todo
	return 0;
}

/*Bluetooth device status notification*/
void CSDKdemoDlg::LovenseDeviceStatus(enum LovenseDeviceState state, const char* device)
{
	::PostMessage(m_main_hwnd, APP_WM_DEVICE_STATE, state, 0);

	bool ok = false;

	const wchar_t* label = L"unkonwn";
	switch (state)
	{	
	case LOVENSE_DEVICE_OFF:
		label = L"off";
		break;
	case LOVENSE_DEVICE_ON:
		label = L"on";
		ok    = true;
		break;
	case LOVENSE_DEVICE_DISABLED:
		label = L"disabled";
		break;
	default:
		break;
	}

	if (ok) {
		m_btnSearch.EnableWindow(TRUE);
		m_btnStop.EnableWindow(FALSE);
	}

	CString text;
	text.Format(L"Bluetooth device status: %d(%s)", state, label);
	PostAppLog(text);
}


LRESULT CSDKdemoDlg::OnSearchStart(WPARAM wParam, LPARAM lParam)
{
	m_btnSearch.EnableWindow(FALSE);
	m_btnStop.EnableWindow(TRUE);
	this->OutPutAppLog(_T("**[SDK Callback] SDK Did Search Start"));

	return 0;
}

LRESULT CSDKdemoDlg::OnSearchEnd(WPARAM wParam, LPARAM lParam)
{
	m_btnSearch.EnableWindow(TRUE);
	m_btnStop.EnableWindow(FALSE);
	this->OutPutAppLog(_T("**[SDK Callback] SDK Did Search End"));

	return 0;
}

void CSDKdemoDlg::LovenseSearchStart()
{
	std::cout << "LovenseDidSearchStart"<< std::endl;
	::PostMessage(m_main_hwnd, APP_WM_SEARCH_START, 0, 0);
}

void CSDKdemoDlg::LovenseSearchEnd()
{
	std::cout << "LovenseDidSearchEnd" << std::endl;
	::PostMessage(m_main_hwnd, APP_WM_SEARCH_END, 0, 0);
}

LRESULT CSDKdemoDlg::OnSearchToy(WPARAM wParam, LPARAM lParam)
{
	auto info = reinterpret_cast<MyToyInfo*>(wParam);
	if (info == nullptr) {
		return 0;
	}
	CString log;
	log.Format(_T("**[SDK Callback] Found Toy Name=%s Id=%s"), info->toy_name.GetBuffer(), info->toy_id.GetBuffer());
	this->OutPutAppLog(log);

	TMap_Iterator iter = m_search_toy.find(CString(info->toy_id));
	if (iter == m_search_toy.end())
	{
		std::shared_ptr<MyToyInfo> pMyToy(info);

		m_search_toy.insert(std::make_pair(pMyToy->toy_id, pMyToy));
		m_toyListInfo.AddString(pMyToy->toy_id);

		m_combToyName.AddString(pMyToy->toy_name);

		if (m_toyListInfo.GetCurSel() < 0) {
			m_toyListInfo.SetCurSel(0);
			m_combToyName.SetCurSel(0);
		}
		
	}
	else
	{
		iter->second = std::shared_ptr<MyToyInfo>(info);
	}

	return 0;
}

void CSDKdemoDlg::LovenseSearchingToys(const LovenseToyItem* item)
{
	auto toy = new MyToyInfo();
	toy->toy_id = item->id;
	toy->toy_name = item->name;
	toy->toy_type = item->type;

	::PostMessage(m_main_hwnd, APP_WM_SEARCH_TOY, (WPARAM)toy, 0);
}

LRESULT CSDKdemoDlg::OnToyCmdResult(WPARAM wParam, LPARAM lParam)
{
	auto data = reinterpret_cast<ToyCmdResult*>(wParam);
	if (data == nullptr) {		
		return 0;
	}

	CString cmd_name = L"unknown";
	auto it = m_cmd_config.find(data->cmd);
	if (it != m_cmd_config.end()) {
		cmd_name = it->second.name.c_str();
	}

	CString log;
	log.Format(L"**[SDK Callback] Send Cmd Result Id=%s cmd=%d(%s) result=%s erorcode=%d"
		, CString(data->toy_id.c_str()).GetBuffer()
		, (int)(data->cmd)
		, cmd_name.GetBuffer()
		, CString(data->result.c_str()).GetBuffer()
		, (int)data->code);
	this->OutPutAppLog(log);

	delete data;

	return 0;
}

void CSDKdemoDlg::LovenseSendCmdResult(const char * szToyID, CLovenseToy::CmdId cmd, const char *result, CLovenseToy::ErrorCode errorCode)
{
	auto data = new ToyCmdResult();
	data->toy_id = szToyID;
	data->cmd    = cmd;
	data->result = result;
	data->code   = (int)errorCode;

	::PostMessage(m_main_hwnd, APP_WM_CMD_RESULT, (WPARAM)data, 0);

}

LRESULT CSDKdemoDlg::OnToyStatus(WPARAM wParam, LPARAM lParam)
{
	auto toy = reinterpret_cast<MyToyInfo*>(wParam);
	if (toy == nullptr) {
		return 0;
	}

	auto state = (int)(lParam);

	auto it = m_search_toy.find(toy->toy_id);
	if (it == m_search_toy.end()) {
		delete toy;
		return 0;
	}

	const wchar_t* label = L"unknown";
	switch (state)
	{
	case LOVENSE_TOY_CONNECTED:
		label = L"connected";
		it->second->toy_name = toy->toy_name;
		it->second->toy_type = toy->toy_type;
		it->second->toy_connected = true;

		m_toy_manager->QueryToySupportedCommands(CT2A(toy->toy_id));

		break;
	case LOVENSE_TOY_DISCONNECTED:
		label = L"disconnected";
		it->second->toy_connected = false;
		break;
	case LOVENSE_TOY_DISABLED:
	{
		label = L"disabled";
		{
			int nIndex = m_toyListInfo.FindStringExact(-1, toy->toy_id);
			if (nIndex != CB_ERR) {

				bool is_remove_current = nIndex == m_toyListInfo.GetCurSel();
				if (is_remove_current) {
					m_toyListInfo.SetWindowTextW(L"");
					m_combToyName.SetWindowTextW(L"");
				}

				m_toyListInfo.DeleteString(nIndex);
				m_combToyName.DeleteString(nIndex);

				if (is_remove_current && m_toyListInfo.GetCount() > 0) {
					m_toyListInfo.SetCurSel(0);
					m_combToyName.SetCurSel(0);
				}
			}
		}
		
		m_search_toy.erase(it);

		break;
	}
	default:
		break;
	}

	CString log;
	log.Format(_T("**[SDK Callback] ToyId=%s Name=%s Type=%d State=%d(%s)")
		, toy->toy_id.GetBuffer(), toy->toy_name.GetBuffer(), (int)toy->toy_type, state, label);
	this->OutPutAppLog(log);

	delete toy;

	return 0;
}

void CSDKdemoDlg::LovenseToyStatus(const struct LovenseToyItem* item, enum LovenseToyState state)
{
	auto toy = new MyToyInfo();
	toy->toy_id = item->id;
	toy->toy_name = item->name;
	toy->toy_type = item->type;

	//auto toy = new CString(szToyID);
	::PostMessage(m_main_hwnd, APP_WM_TOY_STATUS, (WPARAM)toy, state);
}

LRESULT CSDKdemoDlg::OnToySupportedCmdsResult(WPARAM wParam, LPARAM lParam)
{
	auto commands = std::unique_ptr<std::vector<CLovenseToy::CmdId>>(reinterpret_cast<std::vector<CLovenseToy::CmdId>*>(wParam));
	auto toy_id   = std::unique_ptr<CString>(reinterpret_cast<CString*>(lParam));

	if (commands == nullptr || toy_id == nullptr) {
		return 0;
	}

	auto it = m_search_toy.find(*toy_id);
	if (it == m_search_toy.end()) {
		return 0;
	}

	it->second->supported_cmds = *commands;


	m_cmdList.ResetContent();

	CString log_text;
	log_text.Format(L"Toy: %s supported cmd:\n", toy_id->GetBuffer());

	//SetCurSel
	int index = -1;
	for (const auto& it : it->second->supported_cmds) {

		auto cmd_it = m_cmd_config.find(it);

		if (cmd_it != m_cmd_config.end()) {
			const auto& cmd_config = cmd_it->second;
			CString cmd_text;
			cmd_text.Format(L"%s(%s)", cmd_config.name.c_str(), cmd_config.describe.c_str());
			m_cmdList.AddString(cmd_text);
			log_text.AppendFormat(L"\t cmd: %d name: %s\n", it, cmd_config.name.c_str());


			if (it == CLovenseToy::CmdId::COMMAND_VIBRATE || (index < 0)) {
				index = m_cmdList.GetCount() - 1;
			}
		}
		else {
			log_text.AppendFormat(L"\t cmd: %d name: %s\n", it, L"unkonwn");
		}		
	}

	if (index >= 0) {
		m_cmdList.SetCurSel(index);
	}

	OnCbnSelchangeOnCommand();

	this->OutPutAppLog(TAP_LINE);
	this->OutPutAppLog(log_text);
	
	return 0;
}

void CSDKdemoDlg::LovenseToySupportedCommands(const char* szToyID, const CLovenseToy::CmdId* commands, int length)
{
	auto commdands = new std::vector<CLovenseToy::CmdId>;
	auto toy_id    = new CString(szToyID);

	for (int i = 0; i < length; i++) {
		commdands->push_back(commands[i]);
	}

	::PostMessage(m_main_hwnd, APP_WM_TOY_SUPPORT_CMDS, (WPARAM)commdands, (LPARAM)toy_id);
}

void CSDKdemoDlg::OnBnClickedLightOpenButton()
{
	this->OutPutAppLog(TAP_LINE);
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);
	this->OutPutAppLog(_T("[App] OnClick Did Send Light On Cmd!"));

	CLovenseToyManager *manager = GetLovenseToyManager();
	std::string toyID;
	toyID = CW2A(strID.GetString());
	clock_t startTime = clock();
	int error = manager->SendCommand(toyID.c_str(), CLovenseToy::CmdId::COMMAND_LIGHT_ON, 0);
	clock_t endTime = clock();
	CString log;
	log.Format(_T("[App] Result: ret = %d (time = %dms)"), error, endTime - startTime);
	this->OutPutAppLog(log);
}


void CSDKdemoDlg::OnBnClickedLightCloseButton()
{
	this->OutPutAppLog(TAP_LINE);
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);
	this->OutPutAppLog(_T("[App] OnClick Did Send Light Off Cmd!"));

	CLovenseToyManager *manager = GetLovenseToyManager();
	std::string toyID;
	toyID = CW2A(strID.GetString());
	clock_t startTime = clock();
	int error = manager->SendCommand(toyID.c_str(), CLovenseToy::CmdId::COMMAND_LIGHT_OFF, 0);
	clock_t endTime = clock();
	CString log;
	log.Format(_T("[App] Result: ret = %d (time = %dms)"), error, endTime - startTime);
	this->OutPutAppLog(log);
	
}

void CSDKdemoDlg::OnBnClickedLightFlashButton()
{
	
	this->OutPutAppLog(TAP_LINE);
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);
	this->OutPutAppLog(_T("[App] OnClick Did Send Get Battery Cmd!"));

	std::string toyID;
	toyID = CW2A(strID.GetString());

	//CLovenseToyManager *manager = GetLovenseToyManager();
	//int battery = 0;
	//int ret = manager->GetToyBattery(toyID.c_str(), &battery);
	m_toy_manager->SendCommand(toyID.c_str(), CLovenseToy::CmdId::COMMAND_GET_BATTERY);
	
	/*CString log;
	log.Format(_T("**[SDK Callback] Error Code:%d ToyId=%s Battery=%d"),ret, strID, battery);
	this->OutPutAppLog(log);*/

}


void CSDKdemoDlg::OnBnClickedButtonConnected()
{
	auto row = m_toyListInfo.GetCurSel();
	if (row < 0) {
		return;
	}

	this->OutPutAppLog(TAP_LINE);

	CString strID;
	m_toyListInfo.GetLBText(row, strID);
	CString log;
	log.Format(_T("[App] OnClick Connect: toy=%s"), strID.GetBuffer());
	this->OutPutAppLog(log);

	std::string toyID;
	toyID = CW2A(strID.GetString());
	
	clock_t startTime = clock();
	int error = m_toy_manager->ConnectToToy(toyID.c_str());
	clock_t endTime = clock();
	log.Format(_T("[App] Result: ret = %d (time = %dms)"), error, endTime - startTime);
	this->OutPutAppLog(log);
}


void CSDKdemoDlg::OnBnClickedButtonDisconnected()
{
	this->OutPutAppLog(TAP_LINE);
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);
	std::string toyID;
	toyID = CW2A(strID.GetString());
	CString log;
	log.Format(_T("[App] OnClick DisConnect: toy=%s"), strID);
	this->OutPutAppLog(log);

	clock_t startTime = clock();
	CLovenseToyManager *manager = GetLovenseToyManager();
	int error = manager->DisConnectedToy(toyID.c_str());
	clock_t endTime = clock();
	log.Format(_T("[App] Result: ret = %d (time = %dms)"), error, endTime - startTime);
	this->OutPutAppLog(log);
}


void CSDKdemoDlg::OnCbnSelchangeToyID()
{
	m_combToyName.SetCurSel(m_toyListInfo.GetCurSel());
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(),strID);

	auto it = m_search_toy.find(strID);
	if (it != m_search_toy.end())
	{
		auto info = m_search_toy[strID];
		CString battery;
		battery.Format(_T("%d"), info->toy_battery);
		m_txBetarry.SetWindowTextW(battery);
	}

	m_cmdList.ResetContent();

	if (it != m_search_toy.end() && it->second->toy_connected) {

		if (it->second->supported_cmds.empty()) {
			int code = m_toy_manager->QueryToySupportedCommands(CT2A(it->second->toy_id));
			if (code != 0) {
				CString text;
				text.Format(_T("QueryToySupportCommand failed. code: %d"), code);
				this->OutPutAppLog(text);
			}
		}
		else {
			int index = -1;
			for (const auto& it : it->second->supported_cmds) {

				auto cmd_it = m_cmd_config.find(it);

				if (cmd_it != m_cmd_config.end()) {
					const auto& cmd_config = cmd_it->second;
					CString cmd_text;
					cmd_text.Format(L"%s(%s)", cmd_config.name.c_str(), cmd_config.describe.c_str());
					m_cmdList.AddString(cmd_text);

					if (it == CLovenseToy::COMMAND_VIBRATE || (index < 0)) {
						index = m_cmdList.GetCount() - 1;
					}
				}
			}

			if (index >= 0) {
				m_cmdList.SetCurSel(index);
			}
		}
	}

	OnCbnSelchangeOnCommand();
}


void CSDKdemoDlg::OnEnChangeEdit1()
{

}

void CSDKdemoDlg::OutPutAppLog(CString log)
{
	log += _T("\r\n");

	int nBegin = this->m_richedit_app_output.GetTextLength();
	this->m_richedit_app_output.SetSel(nBegin, nBegin);   // Select last character
	this->m_richedit_app_output.ReplaceSel(log);          // Append, move cursor to end of text
	this->m_richedit_app_output.SetSel(-1, 0);             // Remove Black selection bars
	nBegin = this->m_richedit_app_output.GetTextLength(); // Get New Length
	this->m_richedit_app_output.SetSel(nBegin, nBegin);    // Cursor to End of new text
	this->m_richedit_app_output.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CSDKdemoDlg::PostAppLog(CString log)
{
	CString* pText = new CString(log);
	::PostMessage(m_main_hwnd, APP_WM_LOG_MESSAGE, (WPARAM)pText, 0);
}

void CSDKdemoDlg::OnCbnSelchangeOnCommand()
{	
	CString strCmd;
	auto cmdType = CLovenseToy::CmdId::COMMAND_VIBRATE;
	auto curlCmd = m_cmdList.GetCurSel();
	if (curlCmd < 0) {
		return;
	}
	m_cmdList.GetLBText(m_cmdList.GetCurSel(), strCmd);

	cmdType = (CLovenseToy::CmdId)m_mapCmd[strCmd.GetString()];

	auto it = (m_cmd_config.find(cmdType));
	if (it == m_cmd_config.end() || it->second.param_count == 0) {
		m_paramList.EnableWindow(FALSE);
		m_paramList.SetCurSel(0);

		m_paramList_1.EnableWindow(FALSE);
		m_paramList_1.SetCurSel(0);
	}
	else if (it->second.param_count == 1)
	{
		m_paramList.EnableWindow(TRUE);
		m_paramList.SetCurSel(2);

		m_paramList_1.EnableWindow(FALSE);
		m_paramList_1.SetCurSel(0);
	}
	else {
		m_paramList.EnableWindow(TRUE);
		m_paramList.SetCurSel(2);

		m_paramList_1.EnableWindow(TRUE);
		m_paramList_1.SetCurSel(2);
	}
}


void CSDKdemoDlg::OnBnClickedButton3()
{
	CString strID;
	m_toyListInfo.GetLBText(m_toyListInfo.GetCurSel(), strID);
	auto manager = GetLovenseToyManager();

	auto it = m_search_toy.find(strID);
	if (it != m_search_toy.end())
	{
		auto info = it->second;
		if (!info->toy_connected) {
			CString log;
			log.Format(_T("Call after the toy:%s is connected"), strID.GetBuffer());
			this->OutPutAppLog(log);
			return;
		}
		int code = manager->QueryToySupportedCommands(CT2A(strID));
		if (code != 0) {
			CString text;
			text.Format(_T("QueryToySupportCommand failed. code: %d"), code);
			this->OutPutAppLog(text);
		}
	}
}
