//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	Created by Wu Chang Sheng on Dec.12th, 2018
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <vector>
#include <map>

#include "cwTradeCommonDefine.h"
#include "cwDate.h"

class cwBasicSimulator
{
public:
	cwBasicSimulator();
	~cwBasicSimulator();

	virtual void InitialSimulator(const char * pConfigFilePath);
	std::string			m_strConfigFileFullPath;
	std::string			m_strExeFullPath;
	//Md
	virtual int ReqUserMdLogin() { return 0; }

	//Trade
	virtual int ReqQryInstrument() { return 0; }
	virtual int ReqQryPosition() { return 0; }
	virtual int ReqQryOrders() { return 0; }
	virtual int ReqQryTrades() { return 0; }


	virtual int ReqOrderInsert(cwOrderPtr pOrder) { return 0; }
	virtual int CancelOrder(cwOrderPtr pOrder) { return 0; }

	virtual cwPandoraTrader::cwDate GetTradingDay() = 0;

	/// MD
	//���ĺ�Լ
	void		SubScribePrice(std::vector<std::string>& SubscribeInstrument);
	//ȡ�����ĺ�Լ
	void		UnSubScribePrice(std::vector<std::string>& UnSubscribeInstrument);

	bool		GetIsSubscribedInstrument(const char * Ins);
	size_t		GetSubscribedInstrumentCount() { return m_SubscribeInstrumentMap.size(); };
	///Trade

	///ϵͳ���ýӿ���Ϣ����
	void			   SetMdSpi(void * pSpi);
	void			   SetTradeSpi(void *pSpi);

protected:
	void *		m_pMdSpi;
	void *		m_pTradeSpi;

	std::map<std::string, bool>	m_SubscribeInstrumentMap;
};

