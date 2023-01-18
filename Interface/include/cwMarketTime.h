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

//��������ʱ�䣬��ȷ����������0:0:0.000����ǰ�ĺ�������
//Ĭ��������ʼʱ��Ϊ6�㣬����ͨ��SetStartTime���������޸ģ������𳬹�24
//���ʱ��С��6�㣬���24��ʾ����ʵ��ʱ��ȡֵΪ��[6:0:0.000-30:0:0.000)
//Ҳ����˵0���Ӧ24�㣬1��25�㣬5��Ϊ29�㣬����ȡֵ���䣬

#pragma once
#include <string>
#include <memory>

class cwMarketTime
{
public:
	//UpdateTime Format: hh:mm:ss 09:10:11
	cwMarketTime(int StartTime = 6);
	cwMarketTime(int hour, int minute, int second, int MilliSecond = 0, int StartTime = 6);
	cwMarketTime(std::string updatetime, int MilliSecond = 0, int StartTime = 6);
	~cwMarketTime();

	void		SetStartTime(int iStartTime = 6) 
	{ 
		if (iStartTime < 24)
		{
			m_iStartTime = iStartTime;
		}
	}

	inline bool Reset(int hour, int minute, int second, int MilliSecond = 0);
	bool Reset(std::string updatetime, int MilliSecond = 0);
	bool Reset(int64_t iTotalMilliSecond);
	
	inline int64_t	GetTotalSecond() { return m_iTotalMilliSecond / 1000; }
	inline int64_t	GetTotalMilliSecond() { return m_iTotalMilliSecond; }

	inline int		GetHour() { return (m_iHour >= 24) ? m_iHour - 24 : m_iHour; }
	inline int		GetMinute() { return m_iMinute; }
	inline int		GetSecond() { return m_iSecond; }
	inline int		GetMilliSecond() { return m_iMilliSecond; }

	bool operator > (cwMarketTime const& right) const;
	bool operator < (cwMarketTime const& right) const;
	bool operator >= (cwMarketTime const& right) const;
	bool operator <= (cwMarketTime const& right) const;
	bool operator == (cwMarketTime const& right) const;
	bool operator != (cwMarketTime const& right) const { return !operator ==(right); }

private:
	int64_t m_iTotalMilliSecond;
	int		m_iHour, m_iMinute, m_iSecond, m_iMilliSecond;

	int		m_iStartTime;
};
typedef std::shared_ptr<cwMarketTime> cwMarketTimePtr;
