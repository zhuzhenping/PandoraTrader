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
#include <memory>
#include <vector>
#include <deque>
#include "cwTradeCommonDefine.h"
#include "cwProductTradeTime.h"
#include "cwKindleStick.h"

#define KINDLE_MULTI_THREAD

#ifdef KINDLE_MULTI_THREAD
#include "cwMutex.h"
#endif // KINDLE_MULTI_THREAD


typedef std::shared_ptr<cwKindleStick> cwKindleStickPtr;

class cwKindleStickSeries
{
public:
	enum cwKindleSeriesType
	{
		cwKindleTypeMinute = 0,							//����k�ߣ�������k����k�����ڼ�����������
		cwKindleTypeDaily,								//���ߣ�				������������k��
	};

public:
	cwKindleStickSeries();
	~cwKindleStickSeries();

	//��ʼ��K��  �������Ʒ�ֽ���ʱ�����
	bool InitialKindleStickSeries(const char * szInstrumentID, cwKindleSeriesType type = cwKindleTypeMinute, uint32_t m_iTimeScale = 60);
	//��ʼ��K��  �����Ʒ�ֽ���ʱ�����
	bool InitialKindleStickSeries(const char * szInstrumentID, const char * szProductID, cwKindleSeriesType type = cwKindleTypeMinute, uint32_t iTimeScale = 60);

	//������£����ú���Զ��γ�k��
	void PriceUpdate(cwMarketDataPtr pPriceData);

	//����k��������K�����С���Ҫ��������k�����ϲ�����,Ҫ��k��������Ҫ��˳����и���
	void UpdateKindle(cwKindleStickPtr pKindle);
	//�滻����k������
	void ReplaceKindle(cwKindleStickPtr pKindle);
	//��ȡk������
	inline uint32_t GetTimeScale() { return m_iTimeScale; }
	//����K�����鴦������ݾ��ȣ�Ĭ��ֵΪ0.00001
	void SetPrecision(double precision);
public:
	cwMarketDataPtr					m_PrePriceData;
	cwKindleStickPtr				m_pCurrentKindleStick;
	cwRangeOpenClose				m_cwRangeOCMode;
	uint32_t						m_iCurrentKindleLeftTime;

	bool							m_bIsNewKindle;
	bool							m_bThisFinished;

public:
	//��ȡK������
	bool			 GetKindleSerise(std::deque<cwKindleStickPtr> &	KindleStickDeque);

	//��ʱ��˳���ȡk�ߣ�nCountΪk�����У������k��nCountΪ0
	cwKindleStickPtr GetKindleStick(unsigned int nCount = 0);
	//��ʱ�������ȡk�ߣ�nCountΪk�����У������k��nCountΪ0
	cwKindleStickPtr GetLastKindleStick(unsigned int nCount = 0);

	/*�������ܣ���ȡK�����г���
	* ����������
	*     NULL
	* ����ֵ��
	*     k�����г���
	*
	*/
	inline size_t			GetKindleSize() { return m_KindleStickDeque.size(); }

	/*�������ܣ���ȡK����ߵ�k�ߣ�����ж��һ���ߵģ����������һ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	* ����ֵ��
	*     ���K�ߵı�ţ����-1.��ʧ�ܣ���������
	*/
	int GetKindleStickHighest();
	int GetKindleStickHighest(unsigned int nBegin, unsigned int nEnd);
	int GetKindleStickHighest(unsigned int nCount);

	/*�������ܣ���ȡK����ߵ�k�ߣ�����ж��һ���ߵģ��������ȣ��磩��һ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	* ����ֵ��
	*     ���K�ߵı�ţ����-1.��ʧ�ܣ���������
	*/
	int GetKindleStickHighestFirstOne();
	int GetKindleStickHighestFirstOne(unsigned int nBegin, unsigned int nEnd);
	int GetKindleStickHighestFirstOne(unsigned int nCount);

	/*�������ܣ���ȡK����ߵ�k�ߵ����ֵ
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	*	  [in,out] High - K�����ֵ
	* ����ֵ��
	*     true��Highֵ��Ч������Highֵ��Ч��
	*/
	bool GetHighest(double &High);
	bool GetHighest(unsigned int nBegin, unsigned int nEnd, double &High);
	bool GetHighest(unsigned int nCount, double &High);

	/*�������ܣ���ȡ��һ�������һ��,����nBegin����ָ��ֵ���ߵ�K��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	* ����ֵ��
	*     ����Ҫ���K�߱�ţ����-1������޴�k�ߡ�
	*/
	int GetKindleStickNextHigher(double High, unsigned int nBegin);
	int GetKindleStickNextHigher(double High, unsigned int nBegin, unsigned int nEnd);

	/*�������ܣ���ȡK����͵�k�ߣ�����ж��һ���͵ģ����������һ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	* ����ֵ��
	*     ���K�ߵı�ţ����-1.��ʧ�ܣ���������
	*/
	int GetKindleStickLowest();
	int GetKindleStickLowest(unsigned int nBegin, unsigned int nEnd);
	int GetKindleStickLowest(unsigned int nCount);

	/*�������ܣ���ȡK����͵�k�ߣ�����ж��һ���͵ģ��������ȣ��磩��һ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	* ����ֵ��
	*     ���K�ߵı�ţ����-1.��ʧ�ܣ���������
	*/
	int GetKindleStickLowestFirstOne();
	int GetKindleStickLowestFirstOne(unsigned int nBegin, unsigned int nEnd);
	int GetKindleStickLowestFirstOne(unsigned int nCount);

	/*�������ܣ���ȡK����ߵ�k�ߵ���Сֵ
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	*	  [in,out] Low  - K����Сֵ
	* ����ֵ��
	*     true��Lowֵ��Ч������Lowֵ��Ч��
	*/
	bool GetLowest(double &Low);
	bool GetLowest(unsigned int nBegin, unsigned int nEnd, double &Low);
	bool GetLowest(unsigned int nCount, double &Low);

	/*�������ܣ���ȡ��һ�������һ��,����nBegin����ָ��ֵ���͵�K��
* ����������
*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
*     [in] nEnd		- K�߷�Χ������ţ�
* ����ֵ��
*     ����Ҫ���K�߱�ţ����-1������޴�k�ߡ�
*/
	int GetKindleStickNextLower(double Low, unsigned int nBegin);
	int GetKindleStickNextLower(double Low, unsigned int nBegin, unsigned int nEnd);

	/*�������ܣ���ȡָ����ΧK�ߵĲ���λ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	*	  [in] nUnilateralCompareNum - ���߱Ƚ���������2�����ʾ������2��K�߶��ߣ��ұ���������K�߶���Ϊ���壬���Ϊ�㣬���������K��
	*     [in, out] nIndexVector - ����Ҫ���K�߱��(��ʱ��˳���г���
	*	  [in, out] nIndexHighestPeak - ��߲����λ��
	* ����ֵ��
	*     true �ҵ�����Ҫ���K�ߣ�false δ�ҵ�����Ҫ���K��
	*/
	bool GetKindleStickPeak(unsigned int nBegin, unsigned int nEnd,
		unsigned int nUnilateralCompareNum, std::vector<unsigned int>& nIndexVector, unsigned int& nIndexHighestPeak);
	
	/*�������ܣ���ȡָ����ΧK�ߵĲ���λ��
	* ����������
	*     [in] nBegin	- K�߷�Χ��ʼ��ţ�
	*     [in] nEnd		- K�߷�Χ������ţ�
	*	  [in] nUnilateralCompareNum - ���߱Ƚ���������2�����ʾ������2��K�߶��ͣ��ұ���������K�߶���Ϊ���ȣ����Ϊ�㣬���������K��
	*     [in, out] nIndexVector - ����Ҫ���K�߱��(��ʱ��˳���г���
	*	  [in, out] nIndexLowestTrough - ��Ͳ��ȵ�λ��
	* ����ֵ��
	*     true �ҵ�����Ҫ���K�ߣ�false δ�ҵ�����Ҫ���K��
	*/
	bool GetKindleStickTrough(unsigned int nBegin, unsigned int nEnd,
		unsigned int nUnilateralCompareNum, std::vector<unsigned int>& nIndexVector, unsigned int& nIndexLowestTrough);

	const char* GetInstrumentID() { return m_strInstrumentID.c_str(); }
private:
	std::string								m_strInstrumentID;
	std::string								m_strProductID;

	cwKindleSeriesType						m_cwKindleSeriesType;
	//K�����ڣ���Ϊ��λ
	uint32_t								m_iTimeScale;
	bool									m_bIsInitialed;
	std::uint64_t							m_LastestUpdateTime;

	bool									m_bUsingProductTradeTime;
	cwProductTradeTime						m_ProductTradeTime;

#ifdef KINDLE_MULTI_THREAD
	cwMUTEX									m_KindleQueueMutex;
#endif // KINDLE_MULTI_THREAD
	std::deque<cwKindleStickPtr>			m_KindleStickDeque;

	double									m_dInsEQ;
};

