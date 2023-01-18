//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	Create by Wu Chang Sheng on May. 10th 2020
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

//��ʾ���»��߿�ͷ�ĺ�����_PriceUpdateΪϵͳ���ã�������á�

#pragma once
#include "cwCommonUtility.h"
#include "cwBasicStrategy.h"
#include "cwKindleStickSeries.h"
#include <condition_variable>
#include <atomic>

#define		CW_CORRECT_TRADINGDAY
//#define		CW_USING_MYSQL_LIB


class cwBasicKindleStrategy :
	public cwBasicStrategy
{
public:
	typedef std::shared_ptr<cwKindleStickSeries>				cwKindleSeriesPtr;

public:
	cwBasicKindleStrategy();
	virtual ~cwBasicKindleStrategy();

	///MarketData SPI
	//������£�OnBar������PriceUpdate�ص��� ��PriceUpdate�Ѿ����Ի�ȡ���ºõ�K�ߣ�
	virtual void			PriceUpdate(cwMarketDataPtr pPriceData) {};
	//������һ����K�ߵ�ʱ�򣬻���øûص�
	virtual void			OnBar(cwMarketDataPtr pPriceData, int iTimeScale, cwBasicKindleStrategy::cwKindleSeriesPtr pKindleSeries) {};

	///Trade SPI
	//�ɽ��ر�
	virtual void			OnRtnTrade(cwTradePtr pTrade) {};
	//�����ر�, pOrderΪ���±�����pOriginOrderΪ��һ�θ��±����ṹ�壬�п���ΪNULL
	virtual void			OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) {};
	//�����ɹ�
	virtual void			OnOrderCanceled(cwOrderPtr pOrder) {};
	//����¼��������Ӧ
	virtual void			OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};
	//��������������Ӧ
	virtual void			OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};

	///System Call Back
	//��ʱ����Ӧ
	//��ʱ��ID, ��SetTimer��ʱ�򴫸�ϵͳ�����InstrumentID��NULL,�ڻص���ʱ��szInstrumentIDΪ���ַ�������������
	//����ʲô��Լ��TimerId��OnStrategyTimer��szInstrumentID�����Ǹ���Լ��Ϣ
	virtual void			OnStrategyTimer(int iTimerId, const char * szInstrumentID) {};
	//�����Խ��׳�ʼ�����ʱ�����OnReady, �����ڴ˺��������Եĳ�ʼ������
	virtual void			OnReady() {};


	//����k�ߣ� iTimeScale��k�����ڣ���������5����Ϊ300��
	cwKindleSeriesPtr		SubcribeKindle(const char * szInstrumentID, int iTimeScale, int HisKindleCount = 0);
	cwKindleSeriesPtr		SubcribeDailyKindle(const char * szInstrumentID);
	//��Ҫ��Լ��Ϣ֧��
	cwKindleSeriesPtr		SubcribeIndexKindle(const char* szProductId, int iTimeScale, int HisKindleCount = 0);

	std::string				GetIndexName(const char* szProductId);

	//��tick���ݹ�����ʷ����
	bool					InitialHisKindleFromKinldeFile(const char * szFilePath);
	bool					InitialHisKindleFromIndexFile(const char * szTickFile);

	bool					InitialHisKindleFromHisKindleFolder(const char* szHisFolder);
	//
	void					GetKindleFromPublicBus();

	bool					InitialHisKindleFromDB();

	//��ȡ�Ѿ����ĵ�k��
	cwKindleSeriesPtr		GetKindleSeries(const char * szInstrumentID, int iTimeScale);

	//��������--�޼۵�
	cwOrderPtr				InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr				InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)
	cwOrderPtr				InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	cwOrderPtr				EasyInputOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	//�ú�����Զ����������µ�ģʽ�ͽ�������Լ��Ϣ���ã����в𵥲�����
	std::deque<cwOrderPtr>	EasyInputMultiOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//����
	bool					CancelOrder(cwOrderPtr pOrder);
	//ȫ������
	int						CancelAll();
	//��ָ����Լȫ������
	int						CancelAll(const char * szInstrumentID);
	//��ָ����Լ�ͷ���ȫ������
	int						CancelAll(const char * szInstrumentID, cwFtdcDirectionType direction);

	//ί�н��ף�PositionAgency����������ᰴ�������óֲ�
	//ע�⣬������PositionAgency����֮���������µ����߳������������������ͻ��
	virtual void			SetAgentManager(void * pAgentMgr);

	//���ú�Լ�����ʲ����ID, ����û�����õĺ�Լ��Ĭ�����ʲ���ϣ�portfolio)IDΪ0���ʲ�����С�
	//����ͬ��portfolio�µĺ�Լ������ͬ���߳�����������ÿ���ʲ���϶����Լ��Ĵ����߳�
	void					SetPortfolioId(const char * szInstrumentID, unsigned int iPortfolioId);

	//����ͬ��ģʽ
	//true:ͬ��, false:�첽
	//�����λ�͹ҵ���ص���Ϣ����Ҫ���ݻص��ӿ�������ͳ�ƵĻ�����ʹ��ͬ��ģʽ
	//�����λ�͹ҵ���ص���Ϣ��ֻ��ƽ̨�ص��ӿ�����ȡ���ɣ������ʲ���ϵĻ����������첽ģʽ����
	//�����ڻز��ʱ��ʹ��ͬ��ģʽ
	void					SetSynchronizeMode(bool bSynchronous);

	void					SetWriteIndexInfoCacheToFile(bool bNeedWriteToFile) { m_bNeedWriteCacheToFile = bNeedWriteToFile; };
	///ϵͳ���ýӿ���Ϣ����
	virtual void			_SetReady();
	virtual void			_PriceUpdate(cwMarketDataPtr pPriceData);
	virtual void			_OnRtnTrade(cwTradePtr pTrade);
	virtual void			_OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr());
	virtual void			_OnOrderCanceled(cwOrderPtr pOrder);
	virtual void			_OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);
	virtual void			_OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);
	virtual void			_OnTimer(int iTimerId, const char * szInstrumentID);

	enum cwKINDLE_TIMESCALE:int
	{
		cwKINDLE_TIMESCALE_1MIN = 60,
		cwKINDLE_TIMESCALE_3MIN = cwKINDLE_TIMESCALE_1MIN * 3,
		cwKINDLE_TIMESCALE_5MIN = cwKINDLE_TIMESCALE_1MIN * 5,
		cwKINDLE_TIMESCALE_15MIN = cwKINDLE_TIMESCALE_1MIN * 15,
		cwKINDLE_TIMESCALE_30MIN = cwKINDLE_TIMESCALE_1MIN * 30,
		cwKINDLE_TIMESCALE_1HOUR = cwKINDLE_TIMESCALE_1MIN * 60,
		cwKINDLE_TIMESCALE_DAILY
	};
private:
	///ϵͳ���ýӿ���Ϣ����
	//����K��
	void					_UpdateKindleSeries(cwMarketDataPtr pPriceData, std::map<int, cwKindleSeriesPtr> & OnBarMap);
	bool					_GetAgentWorking(std::string instrumentid);

protected:
	bool					m_bNightMode;						//�Ƿ�Ϊҹ��
	std::string				m_strAppStartDay;					//��ǰ����
	std::string				m_strAppStartNextDay;				//�ڶ�������
	std::string				m_strNextTradingDay;				//��һ�������գ��Ե�ǰ���ڼ��㣬��һ�������գ�
	std::string				m_strAppStartTime;					//������ʱ��

	const unsigned int		m_iDefaultWorkBenchId;				//Ĭ�Ϲ�����ID, Ϊ0���Զ��幤����ID,�����0.

private:
	bool					m_bSynchronizeMode;					//�Ƿ�ͬ��	true:ͬ���� false:�첽

	cwMUTEX																			m_cwDealKindleMutex;			//K�ߴ���ͬ��
	///K������ key:instrument key: TimeScale value :Kindle Series
	std::unordered_map<std::string, std::unordered_map<int, cwKindleSeriesPtr>>		m_KindleSeriesMap;
	///��ʷk������ Key:Instrument key: TimeScale value:HisKindle Count
	std::unordered_map<std::string, std::unordered_map<int, int>>					m_HisKindleCountMap;

	///Updating Thread 
	///�����¼�����
	enum StrategyEventType
	{
		EventType_OnReady = 0							//ϵͳReady�ص�
		, EventType_OnTimer								//��ʱ���ص�
		, EventType_PriceUpdate							//Tick�������
		, EventType_OnBar								//K�߸���
		, EventType_RtnTrade							//�ɽ��ر�
		, EventType_RtnOrder							//�����ر�
		, EventType_OnCanceled							//�����ر�
		, EventType_OnRspInsert							//����¼��ر���Ӧ
		, EventType_OnRspCancel							//��������������Ӧ
		, AgentType_PriceUpdate							//�������������
		, AgentType_RtnTrade							//������ �ɽ��ر�
		, AgentType_RtnOrder							//������ �����ر�
		, AgentType_OnCanceled							//������ �����ر�
		, AgentType_OnRspInsert							//������ ����¼��ر���Ӧ
		, AgentType_OnRspCancel							//������ ��������������Ӧ
	};

	///�����¼���Ϣ���ݣ� ��ͬ�¼������²�ͬ�������ֶ�������
	struct EventTypeStruct
	{
		StrategyEventType		EventType;				//�¼���Ϣ����
		cwMarketDataPtr			pPriceData;				//��������
		cwTradePtr				pTrade;					//�ɽ���Ϣ
		cwOrderPtr				pOrder;					//��ǰ������Ϣ
		cwOrderPtr				pOriginOrder;			//����ǰ������Ϣ����
		cwRspInfoPtr			pRspInfo;				//�ر���Ϣ

		std::string				strInstrumentID;		//��Լ
		int						iBarId;					//k�ߺ�
		cwKindleSeriesPtr		pKindle;				//K������
	};
	typedef std::shared_ptr<EventTypeStruct>					EventTypeStructPtr;

	//�ʲ���Ϲ�����
	struct PortfolioWorkBench
	{
		unsigned int											iWorkBenchId;					//������ID��������
		std::string												strWorkBenchName;				//���������ƣ��ɲ���ֵ

		std::atomic<int>										iTradeInfoCnt;					//��ǰ��Ҫ����Ľ�����Ϣ����
		std::condition_variable									TradeInfoDoneCv;				//

		std::deque<EventTypeStructPtr>							EventTypeStructDeque;			//�������¼���Ϣ����
		cwMUTEX													EventTypeDequeMutex;			//�¼���Ϣ����ͬ��
		std::condition_variable									EventWorkingMutexCv;			//�����������֪ͨ�����������߳�
		std::atomic<bool>										bEventFinished;

		std::thread												EventTypeWorkingThread;			//�����������߳�
		volatile std::atomic<bool>								bEventTypeWorkingThreadRun;		//�������߳�����״̬
	};
	typedef std::shared_ptr<PortfolioWorkBench>					PortfolioWorkBenchPtr;

	//֧�ָ����ʲ���ϣ�portfolio)���������趨�����߳�������
	std::unordered_map<std::string, unsigned int>				m_InstrumentToPortfolioMap;		//Key:InstrumentID�� value:WorkBenchID
	std::unordered_map<unsigned int, PortfolioWorkBenchPtr>		m_PortfolioMgrIntMap;			//key:WorkBenchID, value:WorkBench
	std::unordered_map<std::string, PortfolioWorkBenchPtr>		m_PortfolioMgrStrMap;			//Key:InstrumentID, value:WorkBench

	PortfolioWorkBenchPtr										m_pDefaultWorkBench;			//Ĭ�Ϲ�����

	//����������
	PortfolioWorkBenchPtr						CreateWorkBench(unsigned int iBenchId, const char * pBenchName = NULL);
	//��ȡ������
	PortfolioWorkBenchPtr						GetWorkBench(std::string instrumentid);

	//�����������߳�
	void										_EventTypeWorkingThread(PortfolioWorkBenchPtr pWorkBench);
	void										_AddEventType(PortfolioWorkBenchPtr pWorkBench, EventTypeStructPtr EventPtr);


	//std::deque<EventTypeStructPtr>				m_EventTypeStructDeque;
	//cwMUTEX										m_EventTypeDequeMutex;
	//std::condition_variable						m_EventWorkingMutexCv;

	//std::thread									m_EventTypeWorkingThread;
	//volatile bool								m_bEventTypeWorkingThreadRun;

	//void										_EventTypeWorkingThread();
	//void										_AddEventType(EventTypeStructPtr EventPtr);

	CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(cwBasicKindleStrategy);

	void *										m_pAgentManager;


	///Index Price and Kindle Update;
	std::string									m_strHisDataPath;
	bool										m_bNeedIndexKindle = false;

	std::unordered_map<std::string, cwMarketDataPtr>									m_FileLastMDCacheMap;
	//key Product, key InstrumentID
	std::unordered_map <std::string, std::unordered_map<std::string, cwMarketDataPtr>>	m_IndexCalcuteDataCache;

	//ָ�����㹤���߳�
	cwMUTEX										m_UpdateIndexPriceDequeMutex;
	bool										m_bUpdateIndexPriceThreadRun = false;
	bool										m_bNeedWriteCacheToFile = false;
	void										_UpdateIndexPriceWorkingThread();
	std::thread									m_UpdateIndexPriceWorkingThread;
};

