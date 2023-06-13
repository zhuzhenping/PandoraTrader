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
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <thread>
#include "cwProductTradeTime.h"
#include "cwChinaTradingCalendar.h"
#include "cwTradeCommonDefine.h"
#include "cwStrategyLog.h"
#include "cwBasicCout.h"

class cwBasicStrategy
{
public:
	enum cwOpenCloseMode :int
	{
		CloseTodayThenYd = 0,		//��ƽ����ƽ��,�ɿ�������ƽ���⣨��������ν����Ʒ��
		OpenOnly = 1,				//ֻ��
		CloseOnly = 2,				//ֻƽ ֻ�ᱨ��ƽ�ֲ��ֱ�����������������ڳֲ�����Ҳֻ�����ֲ���
		CloseYdThenOpen = 3,		//��ƽ�򣬺󿪲֣���ƽ������ƽ��ܹ��Ʒ�֣��ײ���Ҫ��ȫ��ƽ���ٿ���,�����µ�������С��ʵ�ʱ�����
		CloseYdOrOpen = 4,			//�ݲ�֧�֣�����ƽ�򣬿ɿ��֣����ֺ���ƽ�֣�����ƽ��ܹ��Ʒ�֣��ֲ����󿪲֣��ײ�����һ����ֿ���ûƽ
		CloseYdThenToday = 5		//�ݲ�֧�֣���ƽ����ƽ��,�ɿ�������ƽ����ˣ�ƽ��Ϳ��ֲ���Ʒ��
	};
	const char * GetOpenCloseModeString(cwOpenCloseMode openclose);

	enum cwInstrumentTradeDateSpace:int
	{
		cwFinishDeliver = 0,				//��ɽ���
		cwDeliverMonth = 1,					//������
		cwFirstMonthBeforeDeliverMonth,		//������ǰ��һ����
		cwSecondMonthBeforeDeliverMonth,	//������ǰ�ڶ�����
		cwRegularTradingDateSpace				//��ͨ��������ʱ���
	};

public:
	cwBasicStrategy();
	~cwBasicStrategy();

	//��ʾϵͳ�Ƿ��ʼ����ɣ������������б���������
	bool	m_bStrategyReady;

	/*
	PriceUpdate��OnRtnTrade��OnOrderCanceled��OnRspOrderInsert��OnRspOrderCancel�⼸��������ϵͳ�ص������������Ե�ͬ���麯��
	������Ӧ������±�ϵͳ���ã���ִ������ʵ�ֲ��Թ��ܡ�
	*/
	///MarketData SPI
	//�������
	virtual void PriceUpdate(cwMarketDataPtr pPriceData) = 0;
	
	///Trade SPI
	//�ɽ��ر�
	virtual void OnRtnTrade(cwTradePtr pTrade) = 0;
	//�����ر�, pOrderΪ���±�����pOriginOrderΪ��һ�θ��±����ṹ�壬�п���ΪNULL
	virtual void OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) = 0;
	//�����ɹ�
	virtual void OnOrderCanceled(cwOrderPtr pOrder) = 0;
	//����¼��������Ӧ
	virtual void OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};
	//��������������Ӧ
	virtual void OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};

	///Strategy Timer
	virtual void OnStrategyTimer(int iTimerId, const char * szInstrumentID) {};
	//�����Խ��׳�ʼ�����ʱ�����OnReady, �����ڴ˺��������Եĳ�ʼ������
	virtual void OnReady() {};

	///Special For Simulation 
	///These functions will NOT be called in normal mode
	//�زⲿ�ֽ�����ҹ�̽��������̽�������������
	virtual void OnSimulationPartEnd() {};
	//�����ز������������
	virtual void OnSimulationFinished() {};

	virtual void InitialStrategy(const char * pConfigFilePath);
	std::string			m_strConfigFileFullPath;

	//��Trade SPI׼������ǰ��������Ҫ�õ���Լ��Ϣ���������øú����ȴ��ļ��л�ȡ��Լ��Ϣ������ΪNULLʱ��Ĭ�Ϻͳ�����ͬһ·��
	virtual bool InitalInstrumentData(const char * pInstrumentDataFilePath = NULL);

	///Action  Function
	//��ʾ��������
	virtual std::string  GetStrategyName() { return "BasicStrategy"; }
	//��ȡ���԰汾��
	virtual std::string  GetStrategyVersion() { return "V1.0.0_20220820"; }

	//��ȡ���µ�����
	cwMarketDataPtr	GetLastestMarketData(std::string InstrumentID);
	//��ȡ�˻���Ϣ
	cwAccountPtr GetAccount();
	//��ȡ�ҵ��б�����map���ڷ�����Ϣ�����ر������(OrderRef)ΪKey
	bool GetActiveOrders(std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);		///key OrderRef
	//��ȡ�ҵ��б������Լ��map���ڷ�����Ϣ�����ر������(OrderRef)ΪKey
	bool GetActiveOrders(std::string InstrumentID, std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);		///key OrderRef
	//��ȡ��ͷ�ҵ��������������������Լ
	int GetActiveOrdersLong(std::string InstrumentID);		///key OrderRef
	//��ȡ��ͷ�ҵ��������������������Լ
	int GetActiveOrdersShort(std::string InstrumentID);		///key OrderRef
	//��ȡ���б����б�����map���ڷ�����Ϣ���������������(sysOrderID)ΪKey
	bool GetAllOrders(std::map<cwSysOrderKey, cwOrderPtr>& Orders);				///Key OrderSysID
	//��ȡ���гɽ��б�����map���ڷ�����Ϣ���ɽ���ţ�TradeID��ΪKey
	bool GetTrades(std::map<std::string, cwTradePtr>& trades);					///Key TradeID
	//��ȡ�ֲ��б�����map���ڷ�����Ϣ����ԼIDΪKey
	bool GetPositions(std::map<std::string, cwPositionPtr>& PositionMap);		///Key InstrumentID
	//��ȡ��Լ�ľ��ֲ֣�
	int	 GetNetPosition(std::string InstrumentID);
	//��ȡ�ֲֺ͹ҵ��б�
	bool GetPositionsAndActiveOrders(std::map<std::string, cwPositionPtr>& PositionMap,
		std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);
	//��ȡָ����Լ�ֲֺ͹ҵ��б�
	bool GetPositionsAndActiveOrders(std::string InstrumentID, cwPositionPtr& pPosition, std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);
	//��ȡָ����Լ���ֲֺ͹ҵ��б�
	bool GetNetPositionAndActiveOrders(std::string InstrumentID, int & iPosition, std::map<cwActiveOrderKey, cwOrderPtr> & ActiveOrders);
	//��ȡ��Լ��Ϣ
	cwInstrumentDataPtr GetInstrumentData(std::string InstrumentID);


	//���ĺ�Լ
	//ͬʱ���Ķ����Լ
	void	   SubScribePrice(std::vector<std::string>& SubscribeInstrument);
	//һ��ֻ�趩��һ����Լ
	void	   SubScribePrice(const char * pInstrumentID);
	//ȡ�����ĺ�Լ
	void	   UnSubScribePrice(std::vector<std::string>& UnSubscribeInstrument);

	//��Լ��Ϣ�б���Ҫ��С�䶯����Լ��������Ϣ����ͨ�����map��ȡ����ԼIDΪKey
	std::unordered_map<std::string, cwInstrumentDataPtr> m_InstrumentMap;
	//��ȡ��Լ��С�䶯�������ȡʧ�ܷ���-1
	double    GetTickSize(const char * szInstrumentID);
	//��ȡ��Լ�����������ȡʧ�ܷ���-1
	double	  GetMultiplier(const char * szInstrumentID);
	//��ȡ��֤���ʣ���ӹ�̨�в�ѯ����δ��ѯ��֮ǰĬ�Ϸ���1�����ٷְٱ�֤��ռ��
	double	  GetMarginRate(const char * szInstrumentID);
	//��ȡ��ƷID 
	char *    GetProductID(const char * szInstrumentID);

	//��ȡ����ʱ��Σ��࿪�̶�����;����̶�����
	//��������Լ��������ʱ�䣨102835->10:28:35),���׽׶Σ� ��ý���ʱ�ο��̶����룬�����̶�����
	bool	  GetTradeTimeSpace(const char * szInstrumentID, const char * updatetime,
		cwProductTradeTime::cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	//��ȡǰһ������ʱ�ε���ǰ����ʱ�ο���ʱ����
	int		  GetPreTimeSpaceInterval(const char * szInstrumentID, cwProductTradeTime::cwTradeTimeSpace iTradeIndex);
	//��ȡָ������ʱ��
	cwProductTradeTime::TradeTimePtr GetTradeTime(const char * szInstrumentID, cwProductTradeTime::cwTradeTimeSpace iTradeIndex);

	//��ȡ��ǰ������
	cwPandoraTrader::cwDate	  GetTradingDay();
	const char *		      GetTradingDayStr();

	bool					  GetInstrumentDateSpace(const char* szInstrumentID, cwPandoraTrader::cwDate date,
		cwInstrumentTradeDateSpace& iTradeDateSpace, int &iRemain);

	bool					  GetBuisnessDayRemain(const char* szInstrumentID,
		cwInstrumentTradeDateSpace& iTradeDateSpace, int& iRemain);

	//��ȡ��Լ��ǰ��������
	int		  GetInstrumentCancelCount(std::string InstrumentID);
	//��ȡ��Լ�Ƿ��Ƕ���
	bool	  IsThisStrategySubScribed(std::string InstrumentID);
	//��ȡ��ǰ״̬�Ƿ�Ϊ�ز�ģ����� ����ز�ģʽ�·���true������false
	inline bool GetIsSimulation() { return m_bIsSimulation; }

	//���ö�ʱ�� iTimerId��ʱ��id����OnStrategyTimer�ص����ݴ�id�ж����ĸ���ʱ������, iElapse ������������룩
	//Ŀǰ��֧��100����ʱ������ʱ���ڻص��������������߼������ж�ʱ���ص�����һ���̡߳�
	//ͬ��id�£�����������ᱻ����
	//InstrumentID �Ƕ�ʱ�������ĺ�Լ��������������������NULL
	bool	  SetTimer(int iTimerId, int iElapse, const char * szInstrumentID = nullptr);
	void	  RemoveTimer(int iTimerId);

	///ϵͳ���ýӿ���Ϣ����
	void					_SetMdSpi(cwMDAPIType apiType, void * pSpi);
	void					_SetTradeSpi(cwTradeAPIType apiType, void *pSpi);
	void					_SetIsSimulation(bool IsSimulation = false) { m_bIsSimulation = IsSimulation; };

	virtual void			_SetReady() = 0;
	virtual void			_PriceUpdate(cwMarketDataPtr pPriceData) = 0;
	virtual void			_OnRtnTrade(cwTradePtr pTrade) = 0;
	virtual void			_OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) = 0;
	virtual void			_OnOrderCanceled(cwOrderPtr pOrder) = 0;
	virtual void			_OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) = 0;
	virtual void			_OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) = 0;
	virtual void			_OnTimer(int iTimerId, const char * szInstrumentID) = 0;
protected:
	///�����ʾ
	cwBasicCout				m_cwShow;						



	///ϵͳ���ýӿ���Ϣ����
	std::set<std::string>	m_SubscribedInstrumentSet;

	cwOrderPtr				_InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);		///��������--�޼۵�
	cwOrderPtr				_InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);		///��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr				_InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);		///��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)

	cwOrderPtr				_EasyInputOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);																				///�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���

	//�ú�����Զ����������µ�ģʽ�ͽ�������Լ��Ϣ���ã����в𵥲�����
	std::deque<cwOrderPtr>	_EasyInputMultiOrder(const char * szInstrumentID, int volume, double price,
		cwOpenCloseMode openclosemode = cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	bool					_CancelOrder(cwOrderPtr pOrder);

private:	
	///ϵͳ���ýӿ���Ϣ����
	bool									m_bIsSimulation;

	void *									m_pTradeSpi;
	cwTradeAPIType							m_TradeApiType;

	void *									m_pMdSpi;
	cwMDAPIType								m_MdApiType;

	cwProductTradeTime						m_ProductTradeTime;
	cwStrategyLog							m_BasicStrategyLog;
	cwPandoraTrader::cwChinaTradingCalendar	m_TradingCalendar;

	//Timer	key:TimerID, value:Elapse in ms
	std::unordered_map<int, int>			m_cwTimerElapseMap;
	//Timer Key:TimerID, value:InstrumentID
	std::unordered_map<int, std::string>	m_cwTimerId2InstrumentMap;

	std::thread								m_StrategyTimerWorkingThread;
	volatile bool							m_bStrategyTimerWorkingThreadRun;
	void									StrategyTimerWorkingThread();
};
