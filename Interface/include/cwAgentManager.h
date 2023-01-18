//////////////////////////////////////////////////////////////////////////////////
//*******************************************************************************
//---
//---	Create by Wu Chang Sheng on May. 16th 2020
//---
//--	Copyright (c) by Wu Chang Sheng. All rights reserved.
//--    Consult your license regarding permissions and restrictions.
//--
//*******************************************************************************
//////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <atomic>

#include "cwCommonUtility.h"
#include "cwBasicAgent.h"
#include "cwTradeCommonDefine.h"
#include "cwBasicKindleStrategy.h"

class cwAgentManager
{
public:
	friend class cwBasicKindleStrategy;
public:
	cwAgentManager();
	~cwAgentManager();

	virtual void			_PriceUpdate(cwMarketDataPtr pPriceData);
	virtual void			_OnRtnTrade(cwTradePtr pTrade);
	virtual void			_OnRtnOrder(cwOrderPtr pOrder, cwOrderPtr pOriginOrder = cwOrderPtr());
	virtual void			_OnOrderCanceled(cwOrderPtr pOrder);
	virtual void			_OnRspOrderInsert(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);
	virtual void			_OnRspOrderCancel(cwOrderPtr pOrder, cwRspInfoPtr pRspInfo);


	//��������--�޼۵�
	cwOrderPtr				InputLimitOrder(int agentid, const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FAK����Filled And Kill �����ɽ�ʣ���Զ�����ָ�
	cwOrderPtr				InputFAKOrder(int agentid, const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);
	//��������--FOK��(FOK Filled Or Kill ����ȫ���ɽ������Զ�����ָ��)
	cwOrderPtr				InputFOKOrder(int agentid, const char * szInstrumentID, cwFtdcDirectionType direction, cwOpenClose openclose, int volume, double price);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	cwOrderPtr				EasyInputOrder(int agentid, const char * szInstrumentID, int volume, double price,
		cwBasicStrategy::cwOpenCloseMode openclosemode = cwBasicStrategy::cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//�򻯱��������� volume����ʾ�򣬸���ʾ�����Զ���ƽ���гֲ־�ƽ�֣�û�оͿ���
	//�ú�����Զ����������µ�ģʽ�ͽ�������Լ��Ϣ���ã����в𵥲�����
	std::deque<cwOrderPtr>	EasyInputMultiOrder(int agentid, const char * szInstrumentID, int volume, double price,
		cwBasicStrategy::cwOpenCloseMode openclosemode = cwBasicStrategy::cwOpenCloseMode::CloseTodayThenYd,
		cwInsertOrderType insertordertype = cwInsertOrderType::cwInsertLimitOrder);

	//����
	bool					CancelOrder(int agentid, cwOrderPtr pOrder);

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
	bool	  GetTradeTimeSpace(const char * szInstrumentID, const char * updatetime,
		cwProductTradeTime::cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	//��ȡ��Լ��С�䶯�������ȡʧ�ܷ���-1
	double    GetTickSize(const char * szInstrumentID);

	//Agent �Ƿ��д�����
	virtual bool			HasAgent(std::string instrumentid);

	//��Agent ָ��ע���������������˵���UnRegisterAgent��
	//��Ҫ��֤pAgent�������ڼ���Ч�������й���Agent�ڴ�ռ�������ͷš�
	int						RegisterAgent(std::string instrumentid, cwBasicAgent * pAgent, bool bMonopoly = false);
	int						UnRegisterAgent(std::string instrumentid, int iAgentId);
	int						UnRegisterAgent(int iAgentId);

	struct cwAgentContainer
	{
		//��ռ��Agent���
		int					MonopolyAgentID;
		//��ռ��Agent
		cwBasicAgent *		pMonopolyAgent;

		//Key: AgentID, agent
		std::unordered_map<int, cwBasicAgent *>	pAgentMap;
	};
	typedef std::shared_ptr<cwAgentContainer> cwAgentContainerPtr;

	//
protected:
	CW_DISALLOW_COPYCTOR_AND_ASSIGNMENT(cwAgentManager);

	cwBasicKindleStrategy *											m_pBasicStrategy;
	void					_SetBasicStrategy(cwBasicKindleStrategy * pBasicStrategy);

	std::unordered_map<int, cwBasicAgent *>							m_TotalAgentMap;			//key AgentID,
	std::unordered_map<std::string, cwAgentContainerPtr>			m_AgentContainerMap;		//key Instrument
	std::unordered_map<std::string, int>							m_OrderRefToAgentIDMap;		//key: Order Ref, value: AgentId

	std::atomic<int>												m_iAgentID;
};

