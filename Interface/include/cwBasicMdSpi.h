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
#include <deque>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <float.h>
#include <unordered_map>

#include "cwMutex.h"
#include "cwBasicStrategy.h"
#include "cwBasicTradeSpi.h"

//#define USING_CW_MEMORY_POOL

#ifdef CW_USING_TBB_LIB
#include "atomicops.h"
#include "spscqueue.h"
//#include "oneapi/tbb/concurrent_queue.h"
#include "oneapi/tbb/concurrent_unordered_map.h"
#endif // CW_USING_TBB_LIB

#ifdef USING_CW_MEMORY_POOL
#endif

//#define TIME_LICENCE_LIMIT
#define TIME_LIMIT 20221031

#ifdef CWCOUTINFO
#include "cwBasicCout.h"
#endif

#define CV_NOTIFY

class cwBasicMdSpi
{
public:
	enum PriceServerStatus
	{
		Status_UnConnected = 0
		, Status_Connected
		, Status_Logined
	};
public:
	cwBasicMdSpi(cwMDAPIType apiType);
	virtual ~cwBasicMdSpi();

	virtual void SubscribeMarketData(std::vector<std::string>& SubscribeInstrument) = 0;
	virtual void UnSubscribeMarketData(std::vector<std::string>& SubscribeInstrument) = 0;

	//��������API֧�ֶ������У��ʲ���֤�ýӿ���Ч�����ע��Ӧ������ĺ������иú�������
	virtual void SubscribeMarketDataAll(bool bAll);

	inline PriceServerStatus GetCurrentStatus()
	{
		return m_CurrentStatus;
	}

	inline const char * GetCurrentStatusString()
	{
		switch (m_CurrentStatus)
		{
		case cwBasicMdSpi::Status_UnConnected:
			return " UnConnect ";
			break;
		case cwBasicMdSpi::Status_Connected:
			return " Connecting ";
			break;
		case cwBasicMdSpi::Status_Logined:
			return " Working ";
			break;
		default:
			break;
		}
		return " UnConnect ";
	}

	inline int			GetMarketDataDequeSize() { return (int)m_DepthMarketDataDeque.size(); }

	inline cwMarketDataPtr	GetLastestMarketData(std::string InstrumentID)
	{
#ifndef CW_USING_TBB_LIB
		cwAUTOMUTEX mt(m_MarketDataUpdateMutex, true);
#endif // CW_USING_TBB_LIB
		auto it = m_LastestMarketDataMap.find(InstrumentID);
		if (it != m_LastestMarketDataMap.end()
			&& it->second.get() != NULL)
		{
			return it->second;
		}

		return cwMarketDataPtr();
	}

	//User Setting Method
	void		RegisterTradeSPI(cwBasicTradeSpi * pTradeSpi);
	

	virtual void	RegisterStrategy(cwBasicStrategy * pBasicStrategy) = 0;

	void			SetMdInfo(const char * pszInfo);

	const cwMDAPIType								m_cwMdAPIType;
	char											m_szMdInfo[128];
	volatile bool									m_MdDequeDone;			//dequeue data work done
	bool											m_bNoUseBasicMdUpdate;

#ifdef CW_USING_TBB_LIB
	spsc_queue<cwMarketDataPtr>						m_DepthMarketDataDeque;
#else
	cwMUTEX											m_MarketDataUpdateMutex;
	std::deque <cwMarketDataPtr>					m_DepthMarketDataDeque;
#endif // CW_USING_TBB_LIB


	cwBasicStrategy*								m_pBasicStrategy;

	cwMarketDataPtr									CreateMarketData();

	cwMarketDataPtr									m_cwLastestMarketData;
protected:
	PriceServerStatus								m_CurrentStatus;

#ifdef _MSC_VER
#pragma region CommenDefine
#endif // _MSC_VER
#define RESET_INVALIDLEVEL(ORIGIN, SIDE, LEVEL) if (DBL_MAX - ORIGIN->SIDE##Price##LEVEL\
<= std::numeric_limits<double>::epsilon())\
{\
ORIGIN->SIDE##Price##LEVEL = 0;\
}

#ifndef RESET_INVALIDDATA
#define RESET_INVALIDDATA(ORIGIN, MEMBER) if (DBL_MAX - ORIGIN->MEMBER\
<= std::numeric_limits<double>::epsilon())\
{\
ORIGIN->MEMBER = 0;\
}
#endif

#ifdef _MSC_VER
#pragma endregion
#endif

	inline void			ResetInvalidLevel(cwMarketDataPtr pData)
	{
		RESET_INVALIDLEVEL(pData, Ask, 1);
		RESET_INVALIDLEVEL(pData, Ask, 2);
		RESET_INVALIDLEVEL(pData, Ask, 3);
		RESET_INVALIDLEVEL(pData, Ask, 4);
		RESET_INVALIDLEVEL(pData, Ask, 5);

		RESET_INVALIDLEVEL(pData, Bid, 1);
		RESET_INVALIDLEVEL(pData, Bid, 2);
		RESET_INVALIDLEVEL(pData, Bid, 3);
		RESET_INVALIDLEVEL(pData, Bid, 4);
		RESET_INVALIDLEVEL(pData, Bid, 5);

		RESET_INVALIDDATA(pData, LastPrice);
		RESET_INVALIDDATA(pData, Turnover);
		RESET_INVALIDDATA(pData, PreSettlementPrice);
		RESET_INVALIDDATA(pData, PreClosePrice);
		RESET_INVALIDDATA(pData, PreOpenInterest);
		RESET_INVALIDDATA(pData, PreDelta);
		RESET_INVALIDDATA(pData, OpenInterest);

		RESET_INVALIDDATA(pData, OpenPrice);
		RESET_INVALIDDATA(pData, HighestPrice);
		RESET_INVALIDDATA(pData, LowestPrice);
		RESET_INVALIDDATA(pData, ClosePrice);
		RESET_INVALIDDATA(pData, SettlementPrice);
		RESET_INVALIDDATA(pData, UpperLimitPrice);
		RESET_INVALIDDATA(pData, LowerLimitPrice);
		RESET_INVALIDDATA(pData, CurrDelta);

		//if (DBL_MAX - pData->UpperLimitPrice <= std::numeric_limits<double>::epsilon())
		//{
		//	pData->UpperLimitPrice = 0;
		//}

	}

#ifdef  CV_NOTIFY
	inline void			NotifyMDUpdateThread() { m_MDUpdateMutexCv.notify_one(); };
#else
	inline void			NotifyMDUpdateThread() { };
#endif //  CV_NOTIFY


	std::condition_variable	m_MDUpdateMutexCv;

	std::thread			m_MarketDataUpdateThread;
	volatile bool		m_bMarketDataUpdateThreadRun;
	void				MarketDataUpdate();

	cwBasicTradeSpi*	m_pTradeSpi;

#ifdef CW_USING_TBB_LIB
	tbb::concurrent_unordered_map<std::string, cwMarketDataPtr>		m_LastestMarketDataMap;
#else
	std::unordered_map<std::string, cwMarketDataPtr>				m_LastestMarketDataMap;
#endif // CW_USING_TBB_LIB

	static	int			m_iMdApiCount;

#ifdef CWCOUTINFO
	cwBasicCout			m_cwShow;
#endif

#ifdef USING_CW_MEMORY_POOL
#endif // USING_CW_MEMORY_POOL

};

