//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	author: Wu Chang Sheng
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>
#include <vector>
#include <string>

#include "cwCommonUtility.h"
#include "cwBasicMdSpi.h"

#ifdef _WIN64
//define something for Windows (64-bit)
#include "FemasTradeApi64\USTPFtdcMduserApi.h"
#elif _WIN32
//define something for Windows (32-bit)
#include "FemasTradeApi32\USTPFtdcMduserApi.h"
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
// define something for simulator   
#elif TARGET_OS_IPHONE
// define something for iphone  
#else
#define TARGET_OS_OSX 1
// define something for OSX
#endif
#elif __linux__ or _linux
// linux
#include "FemasTradeApiLinux/USTPFtdcMduserApi.h"
#elif __unix // all unices not caught above
// Unix
#elif __posix
// POSIX
#endif


#ifdef WIN32
#pragma comment(lib, "USTPmduserapiAF.lib")
#endif

class cwFemasMdSpi
	: public CUstpFtdcMduserSpi
	, public cwBasicMdSpi
{

public:
	cwFemasMdSpi(void);
	virtual ~cwFemasMdSpi(void);

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���Ļص���ʼ֪ͨ����API�յ�һ�����ĺ����ȵ��ñ�������Ȼ���Ǹ�������Ļص�������Ǳ��Ļص�����֪ͨ��
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	virtual void OnPackageStart(int nTopicID, int nSequenceNo) {};

	///���Ļص�����֪ͨ����API�յ�һ�����ĺ����ȵ��ñ��Ļص���ʼ֪ͨ��Ȼ���Ǹ�������Ļص��������ñ�������
	///@param nTopicID ������루��˽���������������������ȣ�
	///@param nSequenceNo �������
	virtual void OnPackageEnd(int nTopicID, int nSequenceNo) {};


	///����Ӧ��
	virtual void OnRspError(CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ǰ��ϵͳ�û���¼Ӧ��
	virtual void OnRspUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�û��˳�Ӧ��
	virtual void OnRspUserLogout(CUstpFtdcRspUserLogoutField *pRspUserLogout, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CUstpFtdcDepthMarketDataField *pDepthMarketData);

	///���ĺ�Լ�������Ϣ
	virtual void OnRspSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�˶���Լ�������Ϣ
	virtual void OnRspUnSubMarketData(CUstpFtdcSpecificInstrumentField *pSpecificInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��ȡ����������ϢӦ��
	virtual void OnRspGetMarketTopic(CUstpFtdcRspMarketTopicField *pRspMarketTopic, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ȡ�������Ӧ��
	virtual void OnRspGetMarketData(CUstpFtdcRspDepthMarketDataField *pRspDepthMarketData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	inline CUstpFtdcMduserApi * GetUserThostFtdcMdApi()
	{
		return m_pMarketDataUserApi;
	}

	virtual void RegisterStrategy(cwBasicStrategy * pBasicStrategy);

	void Connect(const char * pszFrontAddress, const char * pszLocalAddr = NULL);
	void DisConnect();

	void WaitForFinish();

	void SetUserLoginField(const char * szBrokerID, const char * szUserID, const char * szPassword);
	void SetUserLoginField(CUstpFtdcReqUserLoginField& reqUserLoginField);

	virtual void SubscribeMarketData(std::vector<std::string>& SubscribeInstrument);
	virtual void UnSubscribeMarketData(std::vector<std::string>& SubscribeInstrument);

private:

	CUstpFtdcMduserApi *			m_pMarketDataUserApi;

	//User Config Data
	CUstpFtdcReqUserLoginField		m_ReqUserLoginField;
	char							m_szMDFrount[1024];

	//һ�ζ��ĺ�Լ����
	const int						m_iSubscribeCountOnce;

	//
	int								m_iRequestId;

	std::map<std::string, bool>		m_SubscribeInstrumentMap;

	int								m_iMdAPIIndex;

	CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(cwFemasMdSpi);
}; 

