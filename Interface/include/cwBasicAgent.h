//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	author: Wu Chang Sheng on May. 20th 2020
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <map>

#include "cwTradeCommonDefine.h"
#include "cwProductTradeTime.h"
#include "cwBasicStrategy.h"

class cwBasicAgent
{
public:
	friend class cwAgentManager;
public:
	cwBasicAgent(bool bMonopoly = false);
	virtual ~cwBasicAgent();

	virtual void			PriceUpdate(cwMarketDataPtr pPriceData) {};
	virtual void			OnRtnTrade(cwTradePtr pTrade) {};
	virtual void			OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr()) {};
	virtual void			OnOrderCanceled(cwOrderPtr pOrder) {};
	virtual void			OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};
	virtual void			OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo) {};

	//��������--�޼۵�
	cwOrderPtr				InputLimitOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr				InputFAKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)
	cwOrderPtr				InputFOKOrder(const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	cwOrderPtr				EasyInputOrder(const char * szInstrumentID, int volume, double price,
		cwBasicStrategy::cwOpenCloseMode openclosemode = cwBasicStrategy::cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	//�ú�����Զ����������µ�ģʽ�ͽ�������Լ��Ϣ���ã����в𵥲�����
	std::deque<cwOrderPtr>	EasyInputMultiOrder(const char * szInstrumentID, int volume, double price,
		cwBasicStrategy::cwOpenCloseMode openclosemode = cwBasicStrategy::cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//����
	bool					CancelOrder(cwOrderPtr pOrder);
	//���ô����˵Ĺ���״̬	
	void					SetAgentWorking(bool bWork) { m_bAgentWorking = bWork; }
	//��ȡ�����˵Ĺ���״̬
	bool					GetAgentWorking() { return m_bAgentWorking; }
	//ֹͣ������				
	//bool					StopAgent();

	//��ȡ���µ�����
	cwMarketDataPtr	GetLastestMarketData(std::string InstrumentID);

	//��ȡ�ֲֺ͹ҵ��б�
	bool GetPositionsAndActiveOrders(std::map<std::string, cwPositionPtr>& PositionMap,
		std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);
	//��ȡָ����Լ�ֲֺ͹ҵ��б�
	bool GetPositionsAndActiveOrders(std::string InstrumentID, cwPositionPtr& pPosition, std::map<cwActiveOrderKey, cwOrderPtr>& ActiveOrders);
	//��ȡָ����Լ���ֲֺ͹ҵ��б�
	bool GetNetPositionAndActiveOrders(std::string InstrumentID, int & iPosition, std::map<cwActiveOrderKey, cwOrderPtr> & ActiveOrders);


	//��ȡ����ʱ��Σ��࿪�̶�����;����̶�����
	//��������Լ��������ʱ�䣨102835->10:28:35),���׽׶Σ� ��ý���ʱ�ο��̶����룬�����̶�����
	bool		GetTradeTimeSpace(const char * szInstrumentID, const char * updatetime,
		cwProductTradeTime::cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	//��ȡ��Լ��С�䶯�������ȡʧ�ܷ���-1
	double		GetTickSize(const char * szInstrumentID);

protected:
	//�Ƿ�Ըú�Լ¢�ϣ�����ǵĻ��� �����µ����ᱻ�ܾ���
	bool					m_bMonopoly;
	//�������Ƿ��ڹ���
	bool					m_bAgentWorking;
private:
	virtual void			_PriceUpdate(cwMarketDataPtr pPriceData);
	virtual void			_OnRtnTrade(cwTradePtr pTrade);
	virtual void			_OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr());
	virtual void			_OnOrderCanceled(cwOrderPtr pOrder);
	virtual void			_OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);
	virtual void			_OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);

	//Agent ID
	int						m_iAgentID;
	void *					m_pAgentManager;
};

