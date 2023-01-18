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
#include <memory>
#include <unordered_map>
#include <vector>
#include "cwMarketTime.h"

//Ʒ�ֽ���ʱ��
class cwProductTradeTime
{

public:
	cwProductTradeTime();
	~cwProductTradeTime();

	enum cwTradeTimeSpace
	{
		NoTrading = 0										//�ǽ���ʱ��
		, NightPartOne										//ҹ��
		, AMPartOne											//�����һ�׶�
		, AMPartTwo											//����ڶ��׶�
		, PMPartOne											//�����һ�׶�
		, AMCallAuctionOrderingOpen							//���Ͼ��۱��������翪�̣�
		, AMCallAuctionMatchOpen							//���Ͼ��۴�ϣ����翪�̣�
		, NightCallAuctionOrderingOpen						//���Ͼ��۱�����ҹ�̿��̣�
		, NightCallAuctionMatchOpen							//���Ͼ��۴�ϣ�ҹ�̿��̣�
		, CallAuctionOrderingClose							//���Ͼ��۱��������̣�
		, CallAuctionMatchClose								//���Ͼ��۴�ϣ����̣�
		, TradeTimeSpaceCnt
	};

	enum cwRangeOpenClose
	{
		cwLeftOpenRightOpen = 0,							//(a,b)�� ���ҿ�����
		cwLeftOpenRightClose,								//(a,b]�� ���ұ�����
		cwLeftCloseRightOpen,								//[a,b)�� ����ҿ�����
		cwLeftCloseRightClose								//[a,b]�� ����ұ�����
	};

	typedef struct tagProductTradeTime
	{
		cwTradeTimeSpace TradeTimeSpace;
		cwRangeOpenClose RangeOpenClose;

		cwMarketTimePtr BeginTime;
		cwMarketTimePtr EndTime;

	}ProductTradeTime;
	typedef std::shared_ptr<ProductTradeTime> TradeTimePtr;

	//����Ʒ��ID,��ʱ���ȡ����ʱ��
	bool GetTradeTimeSpace(std::string ProductId, std::string updatetime,
		cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);
	bool GetTradeTimeSpace(std::string ProductId, uint32_t hour, uint32_t minute, uint32_t second,
		cwTradeTimeSpace& iTradeIndex, int& iOpen, int& iClose);

	TradeTimePtr GetTradeTime(std::string ProductId, cwTradeTimeSpace iTradeIndex);

	//��ȡ��ǰһ������ʱ�ε�ʱ���
	int	 GetPreTimeSpaceInterval(std::string ProductId, cwTradeTimeSpace iTradeIndex);
	int	 GetTimeSpaceInterval(std::string productId, std::string starttime, std::string endTime);

private:
	std::unordered_map<std::string, std::vector<TradeTimePtr>> m_ProductTradeTimeMap;
	void InitialTradeTimeMap();
};

