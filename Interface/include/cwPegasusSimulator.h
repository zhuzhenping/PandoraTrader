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
#include <thread>
#include <atomic>

#include "cwBasicSimulator.h"
#include "cwTickTradeManager.h"
#include "cwProductTradeTime.h"
#include "cwSettlement.h"

#include "tinyxml.h"

#include "cwBasicCout.h"

class cwPegasusSimulator :
	public cwBasicSimulator
{

public:
	cwPegasusSimulator();
	~cwPegasusSimulator();

	//��ʼ��ģ��������ȡ���úͺ�Լ��Ϣ
	virtual void		InitialSimulator(const char * pConfigFilePath);
	//��ȡ������Ϣ
	bool				ReadXmlConfigFile();

	//����ģ����
	bool				SimulationStart();
	//�����������
	virtual bool		StartMarketDataServer();

	//����
	//Md
	virtual int			ReqUserMdLogin();
	
	//Trade
	virtual int			ReqQryInstrument();
	virtual int			ReqQryPosition();
	virtual int			ReqQryOrders();
	virtual int			ReqQryTrades();


	virtual int			ReqOrderInsert(cwOrderPtr pOrder);
	virtual int			CancelOrder(cwOrderPtr pOrder);

	virtual cwPandoraTrader::cwDate GetTradingDay();

	cwOrderPtr			GetOrder(cwOrderPtr pOrder);
	cwTradePtr			GetTrade(cwOrderPtr pOrder, double dTradePrice, int iTradeCnt = 1);

	cwFtdcDateType								m_CurrentTradingDay;					//�ز����� ������
	cwFtdcDateType								m_CurrentActionDay;						//�ز����� ��Ȼ��
	cwFtdcTimeType								m_CurrentSimulationTime;				//�ز����� ʱ��

	volatile bool								m_bSimulationFinished;					//�ز����
	cwSettlement								m_cwSettlement;							//�ز����� ����ģ��

	//Custom Data interface return Data List Size
	int					AddCustomData(cwMarketDataPtr pData, bool bSimulationPartEnd = false, bool bSimulationFinish = false);
	int					GetCustomDataDequeSize() { return m_iCustomDataDequeSize; }

	std::string									m_strSimulatorName;
private:
	enum SIMTYPE:int
	{
		type_CSV_file = 0,				//CSV�ļ�
		type_BIN_file,					//bin�������ļ�
		type_CSV_List_file,				//CSV�ļ�����
		type_BIN_List_file,				//bin�������ļ�����
		type_DB,						//���ݿ�
		type_REAL_Time_Quote,			//ʵʱ����
		type_Custom_Quote				//�û��Զ�������
	};

	SIMTYPE				m_SimType;
	int					m_iInterval;

	std::string			m_strFrontAddr;
	std::string			m_strInstrumentFile;

	std::thread			m_SimulatorProcessorThread;
	volatile bool		m_bMarketDataUpdateThreadRun;
	void				SimulatorProcessor();

	std::thread			m_MarketDataUpdateThread;
	void				CsvMarketDataUpdate();
	void				BinMarketDataUpdate();
	void				RealTimeMarketDataUpdate();
	void				CustomMarketDataUpdate();

	std::map<std::string, std::string>			m_MarketDataFileMap;

	std::unordered_map<std::string, cwInstrumentDataPtr>	m_InstrumentMap;

	//ϵͳ�������
	int					m_iSysOrderID;
	//ϵͳ�ɽ����
	int					m_iSysTradeID;

	cwTickTradeManager	m_cwTickManager;

	//���µ��������ݣ�key:InstrumentID
	std::map<std::string, cwMarketDataPtr>								m_LastestMarketDataMap;

	//���еĶ����� key:SysOrderId
	std::map<std::string, cwOrderPtr>									m_TotalOrderMap;

	//δʹ��
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_TotalLongOrderMap;
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_TotalShortOrderMap;

	//��϶�����	key:InstrumentID, key:price*1000 value: OrderList
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_LongWaitOrderListMap;
	std::map<std::string, std::map<int64_t, std::deque<cwOrderPtr>>>	m_ShortWaitOrderListMap;

	enum UserActionType :int
	{
		UAT_IO = 0,
		UAT_CO
	};
	struct cwSimulationUserAction
	{
		UserActionType	Actiontype;
		cwOrderPtr		pOrder;
	};

	cwProductTradeTime										m_ProductTradeTime;

	std::deque<cwSimulationUserAction>						m_UndealOrderDeque;
	
	std::deque<cwTradePtr>									m_TradeDeque;

	cwBasicCout												m_cwShow;

	cwMUTEX													m_ProcessMutex;

	std::deque<cwMarketDataPtr>								m_MDCacheDeque;							//������������
	cwMUTEX													m_MDCacheMutex;
	volatile std::atomic<bool>								m_bMDCacheMutexReady;
	volatile std::atomic<bool>								m_bSimulationPartEnd;

	cwAccountPtr											m_pAccount;

	double													m_dDeposit;

	//CacheFile
	// 
	bool													m_bNeedCacheFile;
	std::string												m_strCacheFilePath;

	cwMUTEX													m_CacheWorkingMutex;
	std::deque<std::string>									m_CacheWorkingList;

	std::map<std::string, std::string>						m_MarketDataCacheFileMap;

	//Result 
	//Balance Data
	struct TimeBalanceData
	{
		std::string		strDateTime;
		double			dBalance;
	};
	bool													m_bSaveAccountResult;
	int														m_iAccountResultInterval;
	std::deque<TimeBalanceData>								m_dTimeBalanceDQ;

	std::map<std::string, bool>								m_bSaveInsResultMap;
	std::map<std::string, int>								m_iInsResultInterval;
	std::map<std::string, std::deque<TimeBalanceData>>		m_dInsTimeBalanceDQ;

	//Custom Data
	struct CustomDataStruct
	{
		cwMarketDataPtr pData;
		bool			bSimulationPartEnd;
		bool			bSimulationFinish;
	};
	typedef	std::shared_ptr<CustomDataStruct>				CustomDataPtr;
	std::deque<CustomDataPtr>								m_CustomDataDeque;
	volatile std::atomic<int>								m_iCustomDataDequeSize;
	cwMUTEX													m_CustomDataMutex;
};

