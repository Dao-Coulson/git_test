#ifndef _tsi_tcp_h_
#define _tsi_tcp_h_

#include "tsPubDef.h"
#include "tsiTcpError.h"

#define CS_MAX_SKEY_LEN 256

ns_begin_tscNet;

//////////////////////////////////////////////////////////////////////////
class ItnTcpEvent;

//////////////////////////////////////////////////////////////////////////
// PREDEFINED DOMAIN
#define DNF_CONN_SVR_DOMAIN     ("conn.gamesafe.qq.com")
#define CF_CONN_SVR_DOMAIN      ("conncf.gamesafe.qq.com")

typedef enum tagGameID 
{
    GAME_ID_OTHER           = 0,
    GAME_ID_QQFFO           = 1,
    GAME_ID_QQFO            = 2,
    GAME_ID_CROSSFIRE       = 3,
    GAME_ID_GAMEAPP         = 4,
    GAME_ID_DNF             = 5,
    GAME_ID_UI_LAUNCH_DX    = 6,
    GAME_ID_QY              = 7,
    GAME_ID_QQSG            = 8,
    GAME_ID_DP              = 9,
    GAME_ID_AVA             = 10,
    GAME_ID_QQPETS3D        = 11,
    GAME_ID_CLIENT          = 12,
    GAME_ID_GAME            = 13,
    GAME_ID_NCLIENT         = 14,
    GAME_ID_TTY3D           = 15,
    GAME_ID_QQHXSJ          = 16,
    GAME_ID_LAUNCHER        = 17,  
    GAME_ID_XYCLIENT        = 18,
    GAME_ID_QQGAMEHALL      = 19,   
    GAME_ID_QQMINIGAME      = 20,
    GAME_ID_C9              = 21,
    GAME_ID_H2              = 22,
    GAME_ID_LEAGUEOFLEGENDS = 23,
    GAME_ID_NBA2KONLINE     = 24,
    GAME_ID_GAME_NX         = 25,   
    GAME_ID_QQHX            = 26,
    GAME_ID_XXZSHELL        = 27,
    GAME_ID_TGAME           = 28,
    GAME_ID_ASURA           = 29,  
    GAME_ID_WAROFZOMBIE     = 30,
    GAME_ID_XCLIENT         = 31,
    GAME_ID_DJ2             = 32,
	GAME_ID_ARCHEAGE		= 44,
    GAME_ID_X52             = 45,
	GAME_ID_CODOL			= 52,
	GAME_ID_TPHELPER		= 86,
	GAME_ID_LOLCLIENT		= 89,
    //...
    GAME_ID_QQLOGIN         = 200,
    GAME_ID_QQGameProtect   = 201,
} EGameID;

typedef enum tagSkeyVerifyResult
{
	SKEY_VERIFY_NOT_NEED	= 0,
	SKEY_VERIFY_INIT		= 1,
	SKEY_VERIFY_SUCC		= 2,
	SKEY_VERIFY_FAIL		= 3
}SkeyVerifyResult;

typedef struct tagTS_TCPINIT_PARAM
{
    USHORT uGameId;
    UINT   uUin;
    CHAR  *pszDomain;   //�˴�����ָ������
    DWORD  dwWorldId;
} TS_TCPINIT_PARAM;

#pragma pack(1)
typedef struct tagTSConnSkey
{
	DWORD PtloginAppId;
	USHORT uServiceId;
	BYTE SkeyBuffLen;                             
	BYTE SkeyBuff[CS_MAX_SKEY_LEN];          
	BYTE SkeyBuffID;                               	/*    Bind Macrosgroup:TCSConnIdentifyReqID,*/
	BYTE Reserved[8];                              	/*   �����ֶ� */
}TSConnSkey;
#pragma pack()

typedef struct tagTS_TCPREG_PARAM
{
    UINT         uServiceId;
    ItnTcpEvent *pTcpEnvent;
} TS_TCPREG_PARAM;

typedef struct tagTS_TCPREG_PARAM_EX
{
	TS_TCPREG_PARAM tcpParam;
	TSConnSkey		Skey;
	UINT			uStates;
}TS_TCPREG_PARAM_EX;

//c# sdk��֧��open_id��ʼ��csͨ��
#pragma pack(1)
typedef struct tagAccountId
{
	unsigned char account_[65];
	unsigned short account_type_;
}AccountId;

typedef struct tagAccountInfo
{
	AccountId account_id_;
	unsigned short plat_id_;
	unsigned int game_id_;
}AccountInfo;

typedef struct tagTS_TCPINIT_PARAM_EX
{
	TS_TCPINIT_PARAM tcpInitParam;
	AccountInfo accountInfo;
}TS_TCPINIT_PARAM_EX;
#pragma pack()

//�������շ��ӿ�
class ItnBigDataRecvHandler
{
public:
	virtual void OnRcvData(BYTE *pbyData, DWORD dwDataSize) = 0;
	virtual void OnError(DWORD dwErrorId) = 0;
	//�ļ��ϱ���ɴ����ص�������Ƕ���������û����첽�����õ�
	virtual void OnDataSendCompeled(DWORD dwCrc) = 0;
};

typedef struct tagTS_BIG_PACKET_PARAM
{
	UINT	uServiceId;
	UINT	uCmd;
	ItnBigDataRecvHandler* pBigPacketEvent;
}TS_BIG_PACKET_PARAM;

typedef enum enmCsChannelSvrId
{
    CS_CHANNEL_SVRID_DEFAULT       = 0,    //default server
    CS_CHANNEL_SVRID_TCJ           = 1,    //TCJģ��
    CS_CHANNEL_SVRID_TEST          = 2,    //���Է�����
    CS_CHANNEL_SVRID_TSI           = 3,    //TSI������
    CS_CHANNEL_SVRID_TSU           = 4,    //TSU������
    CS_CHANNEL_SVRID_ANTIBOT       = 5,    //antibotʹ��
    CS_CNANNEL_SVRID_SPIDER        = 6,
    CS_CHANNEL_SVRID_SOFTKEYBOARD  = 7,    //�ֿ���
    CS_CHANNEL_SVRID_RPCODE        = 8,    //RPCODE
    CS_CHANNEL_SVRID_CSCHANNEL     = 9,    //�ڶ�ͨ��
    CSCHANNEL_SERVICE_CTRL         = 10,   // CSͨ�����ư�
    CS_CHANNEL_SVRID_TCJ2_UPLOAD   = 11,   //TCJ2.0 Uploadģ��
    CS_CHANNEL_SVRID_TCJ2_UPLOAD_ENCRYPT = 12, //TCJ2.0 Uploadģ��, ����
    CSCHANNEL_SERVICE_UPGRADE_SVR  = 500, // �����������
	CS_CHANNEL_SERVICE_WUKONG  = 71,//wukong
	CS_CHANNEL_SERVICE_BIG_PACKET  = 72, // ���ת������
    CS_CHANNEL_SVRID_END           = 1024  //end
} ENM_CS_CHANNEL_SVR_ID;

typedef enum enmSendFlags
{
    SEND_FLAGS_NONE           = 0x00,
    SEND_FLAGS_ENCRYPT        = 0x01,
    SEND_FLAGS_COMPRESS       = 0x02,
    SEND_FLAGS_ENCRYPT_QUICK  = 0x04, //�����ܽ����㷨
    SEND_FLAGS_SYNC           = 0xF4,
} ENM_SEND_FLAGS;

//���������������, ��ʵ��ҵ���߼���Ӧ��ǡ��
enum EEncryptFunction
{
    EENCRYPT_TenTean  = 0,
    EENCRYPT_TenRijnDael2  = 1,
    EENCRYPT_TenRijnDael3  = 2,
    EENCRYPT_TenRijnDael4  = 3,
    EENCRYPT_TenRijnDael5  = 4,
};

typedef enum enmCsChannelErrId
{
    CS_CHANNEL_CONNECT_ERROR                    = 404,
    CS_CHANNEL_TICKET_VERIFY_ERROR              = 504, 
    CS_CHANNEL_TICKET_VERIFY_SUCCESS            = 604,
    CS_CHANNEL_RECONNECT_FAIL                   = 704,
	CS_CHANNEL_CHANGE_ANTIBOT					= 804,
	CS_CHANNEL_CHANGE_CS						= 904,
	CS_CHANNEL_SKEY_VERIFY_FAIL					= 1004,
	CS_CHANNEL_SKEY_VERIFY_SUCCESS				= 1104,

}ENM_CS_CHANNEL_ERR_ID;

//////////////////////////////////////////////////////////////////////////
static const unsigned int TCP_SEEION_ID_LEN = 16;
class ItnTcpEvent
{
public:
    virtual void OnRcvData(BYTE *pbyData, DWORD dwDataSize) = 0;
    virtual void OnError(DWORD dwErrorId) = 0;
    virtual UINT OnTcpSessionChange(unsigned char byNewSessionID[TCP_SEEION_ID_LEN])  = 0;
};

/* ʹ����ע��

1�����ͺ���Ϊ�첽�����ú��������ء����η�����������4MB��

2��CSͨ���ص����ƣ�

   1) ���ݻص�(OnRcvData)��ŵ�һ�����У�ͨ��������һ���̣߳����λص������еĺ�����
      ��ˣ����ĳ���ص�����ס������Ӱ�쵽�������ݻص�������ֻҪ�������˺������ɡ�

   2) ����ص�(OnError)���ڴ����߳��н��У����ԣ��벻Ҫ��OnError�в�Ҫ�����������
      �����Ӱ������ͨ���ĵĹ���Ч�ʡ�

3��ѹ��ѡ����δʵ�֡�
*/

typedef struct SCsChannelStatus
{
    UINT   uSendSuccPkgCount;                        	   /*   ����pkg�ɹ��ĸ������Ƿ�Ƭ */
    UINT   uSendFailPkgCount;                              /*   ����pkgʧ�ܵĸ������Ƿ�Ƭ*/
    UINT   uRecvSuccPkgCount;                        	   /*   ����pkg�ɹ��ĸ���, �Ƿ�Ƭ */
    UINT   uRecvFailPkgCount;                              /*   ����pkgʧ�ܵĸ���, �Ƿ�Ƭ */
    UINT   uLastRecvPkgSeq;                          	   /*   ���һ�ν��հ���pkg seq*/
    UINT   uLastSendPkgSeq;                          	   /*   ���һ�η��Ͱ���pkg seq*/
    UINT   uConnectFailCount;                        	   /*   ����ʧ�ܴ��� */
    UINT   uGetSessionFailCount;                     	   /*   ����getsessionʧ�ܵĴ��� */
    UINT   uSenderQueueSize;                               /*   ���Ͷ��еĳ���*/
    UINT   uSenderMapSize;                                 /*   ����map�ĳ���*/
    UINT   uRecverMapSize;                                 /*   ����map�ĳ���*/
    UINT   uCallbackQueueSize;                             /*   Callback���г���*/
    UINT   uClientIPAddress;                               /*   �ͻ���IP��ַ*/
    UINT   uServiceIPAddress;                              /*   ��������IP��ַ*/  
    UINT   uMainCtrlCount;                                 /*   MainCtrl���д���*/
    UINT   uCallbackCount;                                 /*   CallBack���д���*/
    UINT   uMainCtrlThreadId;                              /*   �߳�MainCtrlId*/
    UINT   uCallBackThreadId;                              /*   �߳�CallBackId*/
    BOOL   bIsConnectOK;                                   /*   �����Ƿ��Ѿ�����*/
}CS_CHANNEL_STATUS,*PCS_CHANNEL_STATUS;

//////////////////////////////////////////////////////////////////////////

class ItnTcp
{
public:
    ~ItnTcp(){}
    virtual UINT Init(TS_TCPINIT_PARAM *pInitParam) = 0;
    virtual UINT Register(TS_TCPREG_PARAM *pRegParam) = 0;
    virtual UINT SendData(DWORD dwServiceId, BYTE *pbyData, DWORD dwDataSize,int nSendFlags = SEND_FLAGS_NONE) = 0;
    virtual UINT SetHeartBeat(BOOL bSet) = 0;
    virtual UINT Release() = 0;
    virtual void SetQosWork(bool bUseQos) = 0;
    virtual void SetHttpWork(bool bUseHttp) = 0;
    virtual void SetQosReportWork(bool bUseQosReport) = 0;//����tqos�ϱ�����
    virtual void GetCsChannelstatus(CS_CHANNEL_STATUS& stCsChannelStatus) = 0;
    virtual UINT SendTicketBuff(BYTE *pbyData, DWORD dwDataSize) = 0;
	virtual UINT GetCsChannelErrCode()=0;
	virtual UINT RegisterSafeChannelEx(TS_TCPREG_PARAM_EX *pInitParam)=0;	//��Ȩģʽ��CSͨ����ʼ��
	virtual void SendSkeyBuff(TSConnSkey* skey)=0;	//���������Ժ���Ҫ����Skey����Ȩ��OnTcpSessionChange������
	virtual UINT ResetCsChannel(TS_TCPINIT_PARAM *pInitParam)=0;
	virtual void StopCsChannel()=0;
	virtual void UnRegisterSessionChangeCallBack(UINT dwServiceId)=0;
	//֧��open_id��ʼ��csͨ��
	virtual void SetUserInfo(AccountInfo* pstAccountInfo, BYTE* pbyToken, DWORD dwTokenLen, DWORD dwGameVer)=0;
	virtual UINT InitEx(TS_TCPINIT_PARAM_EX *pInitParam)=0;
	virtual UINT RegisterBigDataService(TS_BIG_PACKET_PARAM *pRegParam) = 0;
	virtual UINT SendBigData(DWORD dwServiceId, BYTE* pbyData, DWORD dwDataSize) = 0;
};


ItnTcp *CreateTnTcp();

int ReportTransStatistic();

ns_end;

//tersafe DllEntry6 ���������ӿ�
void * WINAPI CreateCsChannelObj();

//��������dll��ʹ��tersafe.dll
#define CS_CHANNEL_DLL_NAME			    TEXT("tersafe.dll")

#define CS_CHANNEL_EXPORT_FUNCTION		"DllEntry"//GetProcAddress û��unicode �汾

//Dll ��������ʹ�÷�ʽ
typedef tscNet::ItnTcp *(WINAPI *PFNCreateCsChannel)();

inline tscNet::ItnTcp *CreateCsChannel(const TCHAR *pszSafeDllPath = NULL)
{
    HMODULE hModule = ::GetModuleHandle(CS_CHANNEL_DLL_NAME);
	if (hModule == NULL)
	{
		TCHAR szPath[MAX_PATH] = {0};

		if (pszSafeDllPath == NULL)
		{
			::GetModuleFileName(NULL, szPath, MAX_PATH);

			TCHAR *pszLastBackSlash = _tcsrchr(szPath, '\\');

			if (pszLastBackSlash != NULL)
			{
				*(pszLastBackSlash + 1) = 0;
			}
		}
		else
		{
			_tcsncpy(szPath, pszSafeDllPath, sizeof(szPath) / sizeof(szPath[0]) - 1);
		}

		size_t cRemaining = sizeof(szPath) / sizeof(szPath[0]) - _tcslen (szPath) - 1;

		_tcsncat(szPath, CS_CHANNEL_DLL_NAME, cRemaining);
		hModule = ::LoadLibrary(szPath);
		if (hModule == NULL)
		{
			return NULL;
		}
	}

	PFNCreateCsChannel pCreateCsChannel = (PFNCreateCsChannel)::GetProcAddress(hModule, CS_CHANNEL_EXPORT_FUNCTION);

	if (pCreateCsChannel == NULL)
	{
		return NULL;
	}    

	return (tscNet::ItnTcp *)pCreateCsChannel();
}

#endif //_tsi_tcp_h_
