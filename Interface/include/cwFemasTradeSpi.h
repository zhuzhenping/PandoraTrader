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
#include <string>
#include <map>
#include <thread>

#include "cwCommonUtility.h"
#include "cwBasicStrategy.h"
#include "cwBasicTradeSpi.h"
#include "cwBasicCout.h"

#ifdef _WIN64
//define something for Windows (64-bit)
#include "FemasTradeApi64\USTPFtdcTraderApi.h"
#elif _WIN32
//define something for Windows (32-bit)
#include "FemasTradeApi32\USTPFtdcTraderApi.h"
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
#include "FemasTradeApiLinux/USTPFtdcTraderApi.h"
#elif __unix // all unices not caught above
// Unix
#elif __posix
// POSIX
#endif

#define CW_USING_AUTHCODE

#ifdef _MSC_VER
#ifdef CW_USING_AUTHCODE
#pragma comment(lib, "USTPtraderapiAF.lib")
#else
#pragma comment(lib, "USTPtraderapiAF.lib")
#endif // CW_USING_AUTHCODE

#endif // _MSC_VER



class cwFemasTradeSpi 
	: public CUstpFtdcTraderSpi
	, public cwBasicTradeSpi
{
public:
	friend class cwBasicStrategy;

	enum cwReqType
	{
		cwReqAuthenticate = 0
		, cwReqUserLogin
		, cwReqQryInvestor
		, cwReqQryInstrument
		, cwReqQryTradingAccount
		, cwRspQryInvestorPosition
		, cwReqQryOrder
		, cwReqQryTrade
	};

public:
	cwFemasTradeSpi();
	~cwFemasTradeSpi();

public:
	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();
	virtual void OnQryFrontConnected() {};
	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnQryFrontDisconnected(int nReason) {};

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse) {};

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

	///�û������޸�Ӧ��
	virtual void OnRspUserPasswordUpdate(CUstpFtdcUserPasswordUpdateField *pUserPasswordUpdate, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����¼��Ӧ��
	virtual void OnRspOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����¼��Ӧ��
	virtual void OnRspQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���۲���Ӧ��
	virtual void OnRspQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///ѯ������Ӧ��
	virtual void OnRspForQuote(CUstpFtdcReqForQuoteField *pReqForQuote, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�ͻ��������Ӧ��
	virtual void OnRspMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�û���������Ӧ��
	virtual void OnRspUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///�û�����ϯ�����Ӧ��
	virtual void OnRspTransferMoney(CUstpFtdcstpTransferMoneyField *pstpTransferMoney, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///����������֪ͨ
	virtual void OnRtnFlowMessageCancel(CUstpFtdcFlowMessageCancelField *pFlowMessageCancel) {};

	///����¼�����ر�
	virtual void OnErrRtnOrderInsert(CUstpFtdcInputOrderField *pInputOrder, CUstpFtdcRspInfoField *pRspInfo);

	///������������ر�
	virtual void OnErrRtnOrderAction(CUstpFtdcOrderActionField *pOrderAction, CUstpFtdcRspInfoField *pRspInfo);

	///��Լ����״̬֪ͨ
	virtual void OnRtnInstrumentStatus(CUstpFtdcInstrumentStatusField *pInstrumentStatus);

	///�˻������ر�
	virtual void OnRtnInvestorAccountDeposit(CUstpFtdcInvestorAccountDepositResField *pInvestorAccountDepositRes) {};

	///���ۻر�
	virtual void OnRtnQuote(CUstpFtdcRtnQuoteField *pRtnQuote) {};

	///����¼�����ر�
	virtual void OnErrRtnQuoteInsert(CUstpFtdcInputQuoteField *pInputQuote, CUstpFtdcRspInfoField *pRspInfo) {};

	///���۳�������ر�
	virtual void OnErrRtnQuoteAction(CUstpFtdcQuoteActionField *pQuoteAction, CUstpFtdcRspInfoField *pRspInfo) {};

	///ѯ�ۻر�
	virtual void OnRtnForQuote(CUstpFtdcReqForQuoteField *pReqForQuote) {};

	///������Ϲ���֪ͨ
	virtual void OnRtnMarginCombinationLeg(CUstpFtdcMarginCombinationLegField *pMarginCombinationLeg) {};

	///�ͻ��������ȷ��
	virtual void OnRtnMarginCombAction(CUstpFtdcInputMarginCombActionField *pInputMarginCombAction) {};

	///�û���������
	virtual void OnRtnUserDeposit(CUstpFtdcstpUserDepositField *pstpUserDeposit) {};

	///��ѯǰ��ϵͳ�û���¼Ӧ��
	virtual void OnRspQueryUserLogin(CUstpFtdcRspUserLoginField *pRspUserLogin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///������ѯӦ��
	virtual void OnRspQryOrder(CUstpFtdcOrderField *pOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�ɽ�����ѯӦ��
	virtual void OnRspQryTrade(CUstpFtdcTradeField *pTrade, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///����Ͷ�����˻���ѯӦ��
	virtual void OnRspQryUserInvestor(CUstpFtdcRspUserInvestorField *pRspUserInvestor, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///���ױ����ѯӦ��
	virtual void OnRspQryTradingCode(CUstpFtdcRspTradingCodeField *pRspTradingCode, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ�����ʽ��˻���ѯӦ��
	virtual void OnRspQryInvestorAccount(CUstpFtdcRspInvestorAccountField *pRspInvestorAccount, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��Լ��ѯӦ��
	virtual void OnRspQryInstrument(CUstpFtdcRspInstrumentField *pRspInstrument, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///��������ѯӦ��
	virtual void OnRspQryExchange(CUstpFtdcRspExchangeField *pRspExchange, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���ֲֲ߳�ѯӦ��
	virtual void OnRspQryInvestorPosition(CUstpFtdcRspInvestorPositionField *pRspInvestorPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///�Ϲ������ѯӦ��
	virtual void OnRspQryComplianceParam(CUstpFtdcRspComplianceParamField *pRspComplianceParam, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ�����������ʲ�ѯӦ��
	virtual void OnRspQryInvestorFee(CUstpFtdcInvestorFeeField *pInvestorFee, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///Ͷ���߱�֤���ʲ�ѯӦ��
	virtual void OnRspQryInvestorMargin(CUstpFtdcInvestorMarginField *pInvestorMargin, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ױ�����ϳֲֲ�ѯӦ��
	virtual void OnRspQryInvestorCombPosition(CUstpFtdcRspInvestorCombPositionField *pRspInvestorCombPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///���ױ��뵥�ȳֲֲ�ѯӦ��
	virtual void OnRspQryInvestorLegPosition(CUstpFtdcRspInvestorLegPositionField *pRspInvestorLegPosition, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Լ����Ϣ��ѯӦ��
	virtual void OnRspQryInstrumentGroup(CUstpFtdcRspInstrumentGroupField *pRspInstrumentGroup, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��ϱ�֤�����Ͳ�ѯӦ��
	virtual void OnRspQryClientMarginCombType(CUstpFtdcRspClientMarginCombTypeField *pRspClientMarginCombType, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Ȩ¼��Ӧ��
	virtual void OnRspExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Ȩ����Ӧ��
	virtual void OnRspExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///��Ȩ֪ͨ
	virtual void OnRtnExecOrder(CUstpFtdcExecOrderField *pExecOrder) {};

	///��Ȩ¼�����ر�
	virtual void OnErrRtnExecOrderInsert(CUstpFtdcInputExecOrderField *pInputExecOrder, CUstpFtdcRspInfoField *pRspInfo) {};

	///��Ȩ��������ر�
	virtual void OnErrRtnExecOrderAction(CUstpFtdcInputExecOrderActionField *pInputExecOrderAction, CUstpFtdcRspInfoField *pRspInfo) {};

	///����ϯ�ʽ�ͬ��֪ͨ
	virtual void OnRtnTransferMoney(CUstpFtdcSyncMoneyTransferField *pSyncMoneyTransfer) {};

	///ϵͳʱ���ѯӦ��
	virtual void OnRspQrySystemTime(CUstpFtdcRspQrySystemTimeField *pRspQrySystemTime, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};
#ifdef CW_USING_AUTHCODE
	///��͸��ܿͻ���֤Ӧ��
	virtual void OnRspDSUserCertification(CUstpFtdcDSUserCertRspDataField *pDSUserCertRspData, CUstpFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
#endif

public:
	//User Setting Method
	inline CUstpFtdcTraderApi * GetUserThostFtdcMdApi()
	{
		return m_pTraderUserApi;
	}

	virtual void RegisterBasicStrategy(cwBasicStrategy * pBasicStrategy, void * pSpi = NULL);

	void Connect(const char * pszFrontAddress, const char * pszLocalAddr = NULL);
	void DisConnect();

	void WaitForFinish();

	void SetUserLoginField(const char * szBrokerID, const char * szUserID, const char * szPassword, const char * szUserProductInfo = INTERFACENAME);
	void SetUserLoginField(CUstpFtdcReqUserLoginField& reqUserLoginField);
	void SetAuthenticateInfo(const char * szAppID, const char * szAuthCode);

	//User Trader Method
	//�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData);


	///�����ر�
	virtual void OnRtnOrder(CUstpFtdcOrderField *pOrder);

	///�ɽ��ر�
	virtual void OnRtnTrade(CUstpFtdcTradeField *pTrade);

	//virtual void Input Order
	virtual	cwOrderPtr InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
	virtual cwOrderPtr InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
	virtual cwOrderPtr InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction,
		cwOpenClose openclose, int volume, double price);
#if 0
	virtual void InputOrder(cwOrderPtr pOrder);
#endif

	virtual void CancelOrder(const char * szLocalOrderID);
	virtual void CancelOrder(cwOrderPtr pOrder);

	//��ѯ��֤����
	virtual double		GetMarginRate(std::string InstrumentID);

protected:
	bool		MyReqFunction(cwReqType nType, void * pData);

	bool		AddMyReqToList(cwReqType nType);

#ifdef _MSC_VER
#pragma region CTPDefine2CWDefine
#endif // _MSC_VER
	cwFtdcDirectionType			GetFemas2CwDirectionType(TUstpFtdcDirectionType direction);
	cwFtdcHedgeFlagType			GetFemas2CwHedgeFlagType(TUstpFtdcHedgeFlagType hedge);
	cwFtdcOffsetFlagType		GetFemas2CwOffsetFlagType(TUstpFtdcOffsetFlagType Offset);
	cwFtdcOrderPriceType		GetFemas2CwOrderPriceType(TUstpFtdcOrderPriceTypeType orderpricetype);
	cwFtdcTimeConditionType		GetFemas2CwTimeConditionType(TUstpFtdcTimeConditionType timeconditiontype);
	cwFtdcVolumeConditionType	GetFemas2CwVolumeConditionType(TUstpFtdcVolumeConditionType volumeconditiontype);
	cwFtdcForceCloseReasonType	GetFemas2CwForceCloseReasonType(TUstpFtdcForceCloseReasonType forceclosereasontype);
	cwFtdcOrderSourceType		GetFemas2CwOrderSourceType(TUstpFtdcOrderSourceType ordersource);
	cwFtdcOrderStatusType		GetFemas2CwOrderStatusType(TUstpFtdcOrderStatusType orderstatustype);
	cwFtdcOrderTypeType			GetFemas2CwOrderTypetype(TUstpFtdcOrderTypeType ordertype);
	cwFtdcInstrumentStatusType	GetFemas2CwInstrumentStatusType(TUstpFtdcInstrumentStatusType intrumentstatustype);

	TUstpFtdcDirectionType		GetCw2FemasDirectionType(cwFtdcDirectionType direction);

	cwOrderPtr					GetcwOrderPtr(CUstpFtdcOrderField * pOrder);
	cwOrderPtr					GetcwOrderPtr(CUstpFtdcInputOrderField * pInputOrder);
	cwTradePtr					GetcwTradePtr(CUstpFtdcTradeField * pTrade);
#ifdef _MSC_VER
#pragma endregion
#endif // _MSC_VER

private:
	CUstpFtdcTraderApi *		m_pTraderUserApi;

	int							m_iRequestId;

	void						LoopReqQryThread();
protected:
	std::thread					m_MyReqQryThread;
	volatile bool				m_bReqQryThreadRun;
	std::deque<cwReqType>		m_PrioritizedReqList;
	cwMUTEX						m_PrioritizedReqListMutex;

	//Investor Data
	CUstpFtdcReqUserLoginField  m_ReqUserLoginField;
	std::string					m_strInvestorID;

	//TThostFtdcSessionIDType		m_SessionID;
	//TThostFtdcFrontIDType		m_FrontID;
#ifdef CW_USING_AUTHCODE
		///�û�AppID
	TUstpFtdcDSAppIDType				m_AppID;
	///�û���Ȩ��
	TUstpFtdcDSAuthorizationCodeType	m_AuthCode;
#endif

	char						m_szTradeFrount[1024];

#ifdef NoCancelTooMuchPerTick
	uint32_t					m_iLatestUpdateTime;
#endif // NoCancelTooMuchPerTick

	int m_iTradeAPIIndex;

#ifdef CWCOUTINFO
	cwBasicCout					m_cwShow;
#endif

	CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(cwFemasTradeSpi);
};

