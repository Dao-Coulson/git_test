// MainTest_Console.cpp : Defines the entry point for the console application.
//���ڽӿ�ʹ��Demo

#include "stdafx.h"
#include "windows.h"
#include "tsiTcp.h"
#include "Shlwapi.h"
#include "Pathcch.h"
#pragma comment (lib,"Shlwapi.lib")
#define DF_FAKE_UIN 999999999
#define TSI_DNF_CONN_SVR_DOMAIN "tgscp.mainconn.gamesafe.qq.com"
using namespace tscNet;

HMODULE g_hModule;
/* ֧��΢����ϵ�ʺŽṹ */
typedef struct
{
	unsigned char account[64];
	unsigned short account_type;
}Accountid;

typedef struct
{
	Accountid account_id;
	unsigned char plat_id;
}UserAccountInfo, *pUserAccountInfo;

// dwCmdIDΪ����ID���ɸ����Լ�������net_desc.h�в�ѯ�õ�����SafeLogin�����ϱ���dwCmdIDΪCS_NET_CLIENT_SYSTEM_CHECK_RESULT
typedef struct tagCSManagerInit
{
	ULONG   uGameID;
	ULONG   uUin;
	UserAccountInfo stUserAccountInfo; //������Ա����֧�����˻���Ϣ��ʼ�� ChangeByGaeawang
} CSMANAGERINIT, *PCSMANAGERINIT;

void GetTersafePath(DWORD gameId, TCHAR (&terSafeDllFilePath)[MAX_PATH])
{
	ZeroMemory(terSafeDllFilePath, sizeof(terSafeDllFilePath));

	// ��ȡ��ǰTASChannel.dll��·��, ����Ĭ�ϵ�TerSafe.dll��TASChannel.dll·����ͬ.
	// �Դ˼���ƴ�ӳ�Ĭ�ϵ�TerSafe.dll��·��.
	TCHAR defaultTerSafeDllFilePath[MAX_PATH] = { 0 };
	GetModuleFileNameW(g_hModule, defaultTerSafeDllFilePath, MAX_PATH);
	PathRemoveFileSpecW(defaultTerSafeDllFilePath);
	PathAppendW(defaultTerSafeDllFilePath, CS_CHANNEL_DLL_NAME);
	// ͨ����Ϊ��ϷĿ¼��ʹ�õ�TerSafe.dll�������Ƶ�ʸ���, ���Ǳ�����Ŀ¼�µ�Ҫ��.
	// �ڴ˴��Ƚ������µ�TerSafe.dll��·��������Ŀ¼������ͬ, �����Ƚ�ʱ, �ɸ���
	// ������·������ͬ, �ж��Ƿ���ݲ�ͬ��Ϸ�����˲�ͬ����ϷĿ¼TerSafe.dll·��.
	// ����Ϸ��TerSafe.dll·����Ĭ�ϵ�·����ͬ, �����û��ƥ�䵽����TerSafe.dll�������Ϸ,
	// ��ô�򷵻�Ĭ�ϵ�TerSafe.dll·��, ����ͬ, ��֤��ĳЩ��Ϸ�Դ���TerSafe.dll���,
	// ������Ҫ����Ϸ�ڵ������������Ĭ��·����, ������Ĭ��·��.
	TCHAR newerTerSafeDllFilePath[MAX_PATH] = { 0 };
	_tcsncpy_s(newerTerSafeDllFilePath, defaultTerSafeDllFilePath, _TRUNCATE);

	{
		TCHAR innerPath[] = { 'G', 'a', 'm', 'e', 0 };

		for (int i = 0; i < 3; i++)
		{
			PathRemoveFileSpec(newerTerSafeDllFilePath);
		}

		PathAppendW(newerTerSafeDllFilePath, innerPath);
		PathAppendW(newerTerSafeDllFilePath, CS_CHANNEL_DLL_NAME);
	}


	// �˴��ж���ϷTerSafe.dll·���Ƿ��Ĭ�ϵ�TerSafe.dll·����ͬ, ����ͬ, ����Ϸ������TerSafe.dll�������,
	// ���俽����Ĭ��·����.
	if (_tcsicmp(newerTerSafeDllFilePath, defaultTerSafeDllFilePath) && PathFileExists(newerTerSafeDllFilePath))
	{
		CopyFile(newerTerSafeDllFilePath, defaultTerSafeDllFilePath, FALSE);
	}

	// ��Ĭ��·���´���TerSafe.dll���, �򽫸����·�����Ʒ���.
	if (PathFileExists(defaultTerSafeDllFilePath))
	{
		_tcsncpy_s(terSafeDllFilePath, defaultTerSafeDllFilePath, _TRUNCATE);
		// TerSafe�����߷ǳ���, ���յĴ���·�����벻�����ļ���, ������������ķ�б��.
		// ������Ҫ����Ĵ���.
		PathRemoveFileSpec(terSafeDllFilePath);
		PathAddBackslashW(terSafeDllFilePath);
	}
}

BOOL Init(PCSMANAGERINIT pstCSManagerInit)
{
	BOOL bRet = FALSE;
	DWORD  dwTempCode = 0;
	TS_TCPINIT_PARAM  stInitParam = { 0 };
	TS_TCPINIT_PARAM_EX  stInitParamEx = { 0 };
	TS_TCPREG_PARAM  stTsiRegister = { 0 };

		//����Tersafe.dll�������µ�ͨ��
		TCHAR szModulePath[MAX_PATH] = { 0 };
		GetTersafePath(pstCSManagerInit->uGameID, szModulePath);
		ItnTcp *pTnTcp = CreateCsChannel(_tcslen(szModulePath) ? szModulePath : NULL);

		if (pTnTcp == NULL)
		{
			MessageBoxA(0, "CreateTnTcp Error", 0, 0);
			return bRet;
		}

		//��ʼ��ͨ��
		//����TP6�ӿڣ���Ϊ���ܳ�������Ϸ�л�ȡ���ɹ������
		/*sec_proto_tersafe::AccountInfo stAccountInfo = { 0 };
		CSManager::GetAccountInfo(&stAccountInfo); //��������Ϸ�⣬�˺������ܻ�ȡ��AccountInfo��Ϣ*/

		pTnTcp->SetQosWork(1);
		pTnTcp->SetHttpWork(1);
		pTnTcp->SetQosReportWork(1);

/*
		//if (pstCSManagerInit->stUserAccountInfo.account_id.account[0]=='\0')//��Ϸ�ڣ����ϵķ�ʽ��ʼ��CSͨ�� ChangeByGaeawang
		{
			stInitParam.uGameId = (USHORT)pstCSManagerInit->uGameID;
			stInitParam.uUin = pstCSManagerInit->uUin;
			stInitParam.pszDomain = TSI_DNF_CONN_SVR_DOMAIN;
			VDBGOUT_A(("####CSManager::Init stInitParam.uUin= %d", stInitParam.uUin));
			VDBGOUT_A(("####CSManager::Init stInitParam.uGameId= %d", stInitParam.uGameId));
			VDBGOUT_A(("####CSManager::Init stInitParam.pszDomain= %s", stInitParam.pszDomain));
			//MessageBoxA(0,0,0,0);
			dwTempCode = pTnTcp->Init(&stInitParam);
		}
*/

		//else //��Ϸ�⣬���ϲ㺯������������
		{
		stInitParamEx.tcpInitParam.uGameId = (USHORT)pstCSManagerInit->uGameID;
		if (pstCSManagerInit->uUin==0)
		{
		pstCSManagerInit->uUin = DF_FAKE_UIN ;//���uinΪ0ͨ�����ʼ��ʧ��
		}
		stInitParamEx.tcpInitParam.uUin = pstCSManagerInit->uUin;
		stInitParamEx.tcpInitParam.pszDomain = TSI_DNF_CONN_SVR_DOMAIN;
		if (stInitParamEx.accountInfo.account_id_.account_[0]=='\0')
		{
		strcpy_s((char*)stInitParamEx.accountInfo.account_id_.account_,sizeof(stInitParamEx.accountInfo.account_id_.account_),"999999999");
		}
		else
		{
		strcpy_s((char*)stInitParamEx.accountInfo.account_id_.account_,sizeof(stInitParamEx.accountInfo.account_id_.account_), (const char*)pstCSManagerInit->stUserAccountInfo.account_id.account);
		}
		stInitParamEx.accountInfo.game_id_ = pstCSManagerInit->uGameID;
		stInitParamEx.accountInfo.plat_id_ = 2;// atoi((char*)(&pstCSManagerInit->stUserAccountInfo.plat_id));
		stInitParamEx.accountInfo.account_id_.account_type_ = pstCSManagerInit->stUserAccountInfo.account_id.account_type;
		//MessageBoxA(0,0,0,0);
		dwTempCode = pTnTcp->InitEx(&stInitParamEx); //ʹ��TerSafe�ṩ���½ӿڳ�ʼ��CSͨ��������֧��΢�ŵ�¼
		}



		if (SUCCESS != dwTempCode)
		{
			MessageBoxA(0,"CSManager Init Error %u",0,0);
			return bRet;
		}

		//ע��ͨ���ص����Ȼص���CSManager��CSManager�ٻص�������ICSEvent
		/*if (bRegister)
		{
			stTsiRegister.uServiceId = dwServiceID;
			stTsiRegister.pTcpEnvent = (ItnTcpEvent *)this;
			dwTempCode = pTnTcp->Register(&stTsiRegister);

			if (SUCCESS != dwTempCode)
			{
				goto Exit0;
			}
		}*/
	
	Exit0:
		return bRet;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*CSMANAGERINIT stCSManagerInit = {0};
	stCSManagerInit.uGameID = 23;
	stCSManagerInit.uUin = DF_FAKE_UIN;
	BOOL bret = Init(&stCSManagerInit);*/
	DWORD dwUin= 4289999999;
	char qq[64] = {0};
	_ultoa_s(dwUin, qq, 64, 10);
	system("pause");
	return 0;
}

