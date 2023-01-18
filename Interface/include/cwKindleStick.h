#pragma once
#include <cstdint>
#include <float.h>
#include <string.h>

class cwKindleStick
{
public:
	cwKindleStick();
	~cwKindleStick();

	std::uint64_t		StartTime;			//��ʼʱ���
	std::uint64_t		EndTime;			//����ʱ���
	std::uint64_t		HighTime;			//��߼�ʱ��
	std::uint64_t		LowTime;			//��ͼ�ʱ��

	char				szStartTime[16];
	char				szEndTime[16];


	double				Open;				//���̼�
	double				Close;				//������
	double				High;				//��߼�
	double				Low;				//��ͼ�

	std::uint64_t		TotalVolume;		//�ܳɽ�
	std::uint64_t		LastVolume;			//K�߳ɽ���
	double				OpenInterest;		//�ֲ���
	double				TotalTurnOver;		//�ܳɽ���
	double				LastTurnOver;		//K�߳ɽ���

#ifdef WIN32
	static size_t GetLength()
	{
		return sizeof(std::uint64_t) * 4 + sizeof(double) * 7;
	}
#else
	static std::size_t GetLength()
	{
		return sizeof(std::uint64_t) * 4 + sizeof(double) * 7;
	}
#endif // WIN32

	void clear()
	{
		memset(szStartTime, 0, sizeof(szStartTime));
		memset(szEndTime, 0, sizeof(szEndTime));

		StartTime = EndTime = 0;
		HighTime = LowTime = 0;
		Open = Close = 0.0;
		TotalVolume = LastVolume = 0;
		OpenInterest = 0;
		Low = DBL_MAX;
		High = -1;
		TotalTurnOver = LastTurnOver = 0.0;
	}

	cwKindleStick(std::uint64_t start, std::uint64_t end, double low, double high, double open, double close,
		std::uint64_t volume, std::uint64_t lastVolume, uint32_t interest, double turnover, double lastturnover)
		: StartTime(start)
		, EndTime(end)
		, Open(open)
		, Close(close)
		, High(high)
		, Low(low)
		, TotalVolume(volume)
		, LastVolume(lastVolume)
		, OpenInterest(interest)
		, TotalTurnOver(turnover)
		, LastTurnOver(lastturnover)
	{};
};

