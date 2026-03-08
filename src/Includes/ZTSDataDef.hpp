/*!
 * \file ZTSDataDef.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt行情数据定义文件,包括tick、bar、orderqueue、orderdetail、transaction等数据
 */
#pragma once
#include <stdlib.h>
#include <vector>
#include <deque>
#include <string.h>
#include <chrono>

#include "ZTSObject.hpp"

#include "ZTSTypes.h"
#include "ZTSMarcos.h"
#include "ZTSStruct.h"
#include "ZTSCollection.hpp"

using namespace std;

#pragma warning(disable:4267)


NS_ZTP_BEGIN
class ZTSContractInfo;
/*
 *	数值数组的内部封装
 *	采用std::vector实现
 *	包含数据格式化字符串
 *	数值的数据类型为double
 */
class ZTSValueArray : public ZTSObject
{
protected:
	vector<double>	m_vecData;

public:
	/*
	 *	创建一个数值数组对象
	 *	@decimal 保留的小数点位数
	 */
	static ZTSValueArray* create()
	{
		ZTSValueArray* pRet = new ZTSValueArray;
		pRet->m_vecData.clear();
		return pRet;
	}

	/*
	 *	读取数组的长度
	 */
	inline uint32_t	size() const{ return m_vecData.size(); }
	inline bool		empty() const{ return m_vecData.empty(); }

	/*
	 *	读取指定位置的数据
	 *	如果超出范围,则返回INVALID_VALUE
	 */
	inline double		at(uint32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= m_vecData.size())
			return INVALID_DOUBLE;

		return m_vecData[idx];
	}

	inline int32_t		translateIdx(int32_t idx) const
	{
		if(idx < 0)
		{
			return m_vecData.size()+idx;
		}

		return idx;
	}

	/*
	 *	找到指定范围内的最大值
	 *	如果超出范围,则返回INVALID_VALUE
	 */
	double		maxvalue(int32_t head, int32_t tail, bool isAbs = false) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		uint32_t begin = min(head, tail);
		uint32_t end = max(head, tail);

		if(begin <0 || begin >= m_vecData.size() || end < 0 || end > m_vecData.size())
			return INVALID_DOUBLE;

		double maxValue = INVALID_DOUBLE;
		for(uint32_t i = begin; i <= end; i++)
		{
			if(m_vecData[i] == INVALID_DOUBLE)
				continue;

			if(maxValue == INVALID_DOUBLE)
				maxValue = isAbs?abs(m_vecData[i]):m_vecData[i];
			else
				maxValue = max(maxValue, isAbs?abs(m_vecData[i]):m_vecData[i]);
		}

		//if (maxValue == INVALID_DOUBLE)
		//	maxValue = 0.0;

		return maxValue;
	}

	/*
	 *	找到指定范围内的最小值
	 *	如果超出范围,则返回INVALID_VALUE
	 */
	double		minvalue(int32_t head, int32_t tail, bool isAbs = false) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		uint32_t begin = min(head, tail);
		uint32_t end = max(head, tail);

		if(begin <0 || begin >= m_vecData.size() || end < 0 || end > m_vecData.size())
			return INVALID_DOUBLE;

		double minValue = INVALID_DOUBLE;
		for(uint32_t i = begin; i <= end; i++)
		{
			if (m_vecData[i] == INVALID_DOUBLE)
				continue;

			if(minValue == INVALID_DOUBLE)
				minValue = isAbs?abs(m_vecData[i]):m_vecData[i];
			else
				minValue = min(minValue, isAbs?abs(m_vecData[i]):m_vecData[i]);
		}

		//if (minValue == INVALID_DOUBLE)
		//	minValue = 0.0;

		return minValue;
	}

	/*
	 *	在数组末尾添加数据
	 */
	inline void		append(double val)
	{
		m_vecData.emplace_back(val);
	}

	/*
	 *	设置指定位置的数据
	 */
	inline void		set(uint32_t idx, double val)
	{
		if(idx < 0 || idx >= m_vecData.size())
			return;

		m_vecData[idx] = val;
	}

	/*
	 *	重新分配数组大小,并设置默认值
	 */
	inline void		resize(uint32_t uSize, double val = INVALID_DOUBLE)
	{
		m_vecData.resize(uSize, val);
	}

	/*
	 *	重载操作符[]
	 *	用法同getValue接口
	 */
	inline double&		operator[](uint32_t idx)
	{
		return m_vecData[idx];
	}

	inline double		operator[](uint32_t idx) const
	{
		return m_vecData[idx];
	}

	inline std::vector<double>& getDataRef()
	{
		return m_vecData;
	}
};

/*
 *	K线数据切片
 *	这个比较特殊,因为要拼接当日和历史的
 *	所以有两个开始地址
 */
class ZTSKlineSlice : public ZTSObject
{
private:
	char			_code[MAX_INSTRUMENT_LENGTH];
	ZTSKlinePeriod	_period;
	uint32_t		_times;
	typedef std::pair<ZTSBarStruct*, uint32_t> BarBlock;
	std::vector<BarBlock> _blocks;
	uint32_t		_count;

protected:
	ZTSKlineSlice()
		: _period(KP_Minute1)
		, _times(1)
		, _count(0)
	{

	}

	inline int32_t		translateIdx(int32_t idx) const
	{
		int32_t totalCnt = _count;
		if (idx < 0)
		{
			return max(0, totalCnt + idx);
		}

		return idx;
	}


public:
	static ZTSKlineSlice* create(const char* code, ZTSKlinePeriod period, uint32_t times, ZTSBarStruct* bars = NULL, int32_t count = 0)
	{
		ZTSKlineSlice *pRet = new ZTSKlineSlice;
		vvt_strcpy(pRet->_code, code);
		pRet->_period = period;
		pRet->_times = times;
		if(bars)
			pRet->_blocks.emplace_back(BarBlock(bars, count));
		pRet->_count = count;

		return pRet;
	}

	inline bool appendBlock(ZTSBarStruct* bars, uint32_t count)
	{
		if (bars == NULL || count == 0)
			return false;

		_count += count;
		_blocks.emplace_back(BarBlock(bars, count));
		return true;
	}

	inline std::size_t	get_block_counts() const
	{
		return _blocks.size();
	}

	inline ZTSBarStruct*	get_block_addr(std::size_t blkIdx)
	{
		if (blkIdx >= _blocks.size())
			return NULL;

		return _blocks[blkIdx].first;
	}

	inline uint32_t get_block_size(std::size_t blkIdx)
	{
		if (blkIdx >= _blocks.size())
			return 0;

		return _blocks[blkIdx].second;
	}

	inline ZTSBarStruct*	at(int32_t idx)
	{
		if (_count == 0)
			return NULL;

		idx = translateIdx(idx);
		do
		{
			for (auto& item : _blocks)
			{
				if ((uint32_t)idx >= item.second)
					idx -= item.second;
				else
					return item.first + idx;
			}
		} while (false);

		return NULL;
	}

	inline const ZTSBarStruct*	at(int32_t idx) const
	{
		if (_count == 0)
			return NULL;

		idx = translateIdx(idx);
		do
		{
			for (auto& item : _blocks)
			{
				if ((uint32_t)idx >= item.second)
					idx -= item.second;
				else
					return item.first + idx;
			}
		} while (false);
		return NULL;
	}


	/*
	*	查找指定范围内的最大价格
	*	@head 起始位置
	*	@tail 结束位置
	*	如果位置超出范围,返回INVALID_VALUE
	*/
	double		maxprice(int32_t head, int32_t tail) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		int32_t begin = max(0,min(head, tail));
		int32_t end = min(max(head, tail), size() - 1);

		double maxValue = this->at(begin)->high;
		for (int32_t i = begin; i <= end; i++)
		{
			maxValue = max(maxValue, at(i)->high);
		}
		return maxValue;
	}

	/*
	*	查找指定范围内的最小价格
	*	@head 起始位置
	*	@tail 结束位置
	*	如果位置超出范围,返回INVALID_VALUE
	*/
	double		minprice(int32_t head, int32_t tail) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		int32_t begin = max(0, min(head, tail));
		int32_t end = min(max(head, tail), size() - 1);

		double minValue = at(begin)->low;
		for (int32_t i = begin; i <= end; i++)
		{
			minValue = min(minValue, at(i)->low);
		}

		return minValue;
	}

	/*
	*	返回K线的大小
	*/
	inline int32_t	size() const{ return _count; }
	inline bool	empty() const{ return _count == 0; }

	/*
	*	返回K线对象的合约代码
	*/
	inline const char*	code() const{ return _code; }
	inline void		setCode(const char* code){ vvt_strcpy(_code, code); }


	/*
	*	将指定范围内的某个特定字段的数据全部抓取出来
	*	并保存的一个数值数组中
	*	如果超出范围,则返回NULL
	*	@type 支持的类型有KT_OPEN、KT_HIGH、KT_LOW、KT_CLOSE,KFT_VOLUME、KT_DATE
	*/
	ZTSValueArray*	extractData(ZTSKlineFieldType type, int32_t head = 0, int32_t tail = -1) const
	{
		if (_count == 0)
			return NULL;

		head = translateIdx(head);
		tail = translateIdx(tail);

		int32_t begin = max(0, min(head, tail));
		int32_t end = min(max(head, tail), size() - 1);

		ZTSValueArray *vArray = NULL;

		vArray = ZTSValueArray::create();

		for (int32_t i = begin; i <= end; i++)
		{
			const ZTSBarStruct& day = *at(i);
			switch (type)
			{
			case KFT_OPEN:
				vArray->append(day.open);
				break;
			case KFT_HIGH:
				vArray->append(day.high);
				break;
			case KFT_LOW:
				vArray->append(day.low);
				break;
			case KFT_CLOSE:
				vArray->append(day.close);
				break;
			case KFT_VOLUME:
				vArray->append(day.vol);
				break;
			case KFT_SVOLUME:
				if (day.vol > INT_MAX)
					vArray->append(1 * ((day.close > day.open) ? 1 : -1));
				else
					vArray->append((int32_t)day.vol * ((day.close > day.open) ? 1 : -1));
				break;
			case KFT_DATE:
				vArray->append(day.date);
				break;
			case KFT_TIME:
				vArray->append((double)day.time);
			}
		}

		return vArray;
	}
};

/*
 *	K线数据
 *	K线数据的内部数据使用ZTSBarStruct
 *	ZTSBarStruct是一个结构体
 *	因为K线数据单独使用的可能性较低
 *	所以不做ZTSObject派生类的封装
 */
class ZTSKlineData : public ZTSObject
{
public:
	typedef std::vector<ZTSBarStruct> ZTSBarList;

protected:
	char			m_strCode[32];
	ZTSKlinePeriod	m_kpPeriod;
	uint32_t		m_uTimes;
	bool			m_bUnixTime;	//是否是时间戳格式,目前只在秒线上有效
	ZTSBarList		m_vecBarData;
	bool			m_bClosed;		//是否是闭合K线

protected:
	ZTSKlineData()
		:m_kpPeriod(KP_Minute1)
		,m_uTimes(1)
		,m_bUnixTime(false)
		,m_bClosed(true)
	{

	}

	inline int32_t		translateIdx(int32_t idx) const
	{
		if(idx < 0)
		{
			return max(0, (int32_t)m_vecBarData.size() + idx);
		}

		return idx;
	}

public:
	/*
	 *	创建一个K线数据对象
	 *	@code 要创建的合约代码
	 *	@size 初始分配的数据长度
	 */
	static ZTSKlineData* create(const char* code, uint32_t size)
	{
		ZTSKlineData *pRet = new ZTSKlineData;
		pRet->m_vecBarData.resize(size);
		vvt_strcpy(pRet->m_strCode, code);

		return pRet;
	}

	inline void setClosed(bool bClosed){ m_bClosed = bClosed; }
	inline bool isClosed() const{ return m_bClosed; }

	/*
	 *	设置周期和步长
	 *	@period	基础周期
	 *	@times 倍数
	 */
	inline void	setPeriod(ZTSKlinePeriod period, uint32_t times = 1){ m_kpPeriod = period; m_uTimes = times; }

	inline void	setUnixTime(bool bEnabled = true){ m_bUnixTime = bEnabled; }

	inline ZTSKlinePeriod	period() const{ return m_kpPeriod; }
	inline uint32_t		times() const{ return m_uTimes; }
	inline bool			isUnixTime() const{ return m_bUnixTime; }

	/*
	 *	查找指定范围内的最大价格
	 *	@head 起始位置
	 *	@tail 结束位置
	 *	如果位置超出范围,返回INVALID_VALUE
	 */
	inline double		maxprice(int32_t head, int32_t tail) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		uint32_t begin = min(head, tail);
		uint32_t end = max(head, tail);

		if(begin >= m_vecBarData.size() || end > m_vecBarData.size())
			return INVALID_DOUBLE;

		double maxValue = m_vecBarData[begin].high;
		for(uint32_t i = begin; i <= end; i++)
		{
			maxValue = max(maxValue, m_vecBarData[i].high);
		}

		return maxValue;
	}

	/*
	 *	查找指定范围内的最小价格
	 *	@head 起始位置
	 *	@tail 结束位置
	 *	如果位置超出范围,返回INVALID_VALUE
	 */
	inline double		minprice(int32_t head, int32_t tail) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		uint32_t begin = min(head, tail);
		uint32_t end = max(head, tail);

		if(begin >= m_vecBarData.size() || end > m_vecBarData.size())
			return INVALID_DOUBLE;

		double minValue = m_vecBarData[begin].low;
		for(uint32_t i = begin; i <= end; i++)
		{
			minValue = min(minValue, m_vecBarData[i].low);
		}

		return minValue;
	}
	
	/*
	 *	返回K线的大小
	 */
	inline uint32_t	size() const{return m_vecBarData.size();}
	inline bool IsEmpty() const{ return m_vecBarData.empty(); }

	/*
	 *	返回K线对象的合约代码
	 */
	inline const char*	code() const{ return m_strCode; }
	inline void		setCode(const char* code){ vvt_strcpy(m_strCode, code); }

	/*
	 *	读取指定位置的开盘价
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	open(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].open;
	}

	/*
	 *	读取指定位置的最高价
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	high(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].high;
	}

	/*
	 *	读取指定位置的最低价
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	low(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].low;
	}

	/*
	 *	读取指定位置的收盘价
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	close(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].close;
	}

	/*
	 *	读取指定位置的成交量
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	volume(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].vol;
	}

	/*
	 *	读取指定位置的总持
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	openinterest(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_UINT32;

		return m_vecBarData[idx].hold;
	}

	/*
	 *	读取指定位置的增仓
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	additional(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].add;
	}	

	/*
	 *	读取指定位置的总持
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	bidprice(int32_t idx) const
	{
		idx = translateIdx(idx);

		if (idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_UINT32;

		return m_vecBarData[idx].bid;
	}

	/*
	 *	读取指定位置的增仓
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	askprice(int32_t idx) const
	{
		idx = translateIdx(idx);

		if (idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].ask;
	}

	/*
	 *	读取指定位置的成交额
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline double	money(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_DOUBLE;

		return m_vecBarData[idx].money;
	}

	/*
	 *	读取指定位置的日期
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline uint32_t	date(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_UINT32;

		return m_vecBarData[idx].date;
	}

	/*
	 *	读取指定位置的时间
	 *	如果超出范围则返回INVALID_VALUE
	 */
	inline uint64_t	time(int32_t idx) const
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return INVALID_UINT32;

		return m_vecBarData[idx].time;
	}

	/*
	 *	将指定范围内的某个特定字段的数据全部抓取出来
	 *	并保存的一个数值数组中
	 *	如果超出范围,则返回NULL
	 *	@type 支持的类型有KT_OPEN、KT_HIGH、KT_LOW、KT_CLOSE,KFT_VOLUME、KT_DATE
	 */
	ZTSValueArray*	extractData(ZTSKlineFieldType type, int32_t head = 0, int32_t tail = -1) const
	{
		head = translateIdx(head);
		tail = translateIdx(tail);

		uint32_t begin = min(head, tail);
		uint32_t end = max(head, tail);

		if(begin >= m_vecBarData.size() || end >= (int32_t)m_vecBarData.size())
			return NULL;

		ZTSValueArray *vArray = NULL;

		vArray = ZTSValueArray::create();

		for(uint32_t i = 0; i < m_vecBarData.size(); i++)
		{
			const ZTSBarStruct& day = m_vecBarData.at(i);
			switch(type)
			{
			case KFT_OPEN:
				vArray->append(day.open);
				break;
			case KFT_HIGH:
				vArray->append(day.high);
				break;
			case KFT_LOW:
				vArray->append(day.low);
				break;
			case KFT_CLOSE:
				vArray->append(day.close);
				break;
			case KFT_VOLUME:
				vArray->append(day.vol);
				break;
			case KFT_SVOLUME:
				if(day.vol > INT_MAX)
					vArray->append(1 * ((day.close > day.open) ? 1 : -1));
				else
					vArray->append((int32_t)day.vol * ((day.close > day.open)?1:-1));
				break;
			case KFT_DATE:
				vArray->append(day.date);
				break;
			case KFT_TIME:
				vArray->append((double)day.time);
			}
		}

		return vArray;
	}

public:
	/*
	 *	获取K线内部vector的引用
	 */
	inline ZTSBarList& getDataRef(){ return m_vecBarData; }

	inline ZTSBarStruct*	at(int32_t idx)
	{
		idx = translateIdx(idx);

		if(idx < 0 || idx >= (int32_t)m_vecBarData.size())
			return NULL;
		return &m_vecBarData[idx];
	}

	/*
	 *	释放K线数据
	 *	并delete所有的日线数据,清空vector
	 */
	virtual void release()
	{
		if(isSingleRefs())
		{
			m_vecBarData.clear();
		}		

		ZTSObject::release();
	}

	/*
	 *	追加一条K线
	 */
	inline void	appendBar(const ZTSBarStruct& bar)
	{
		if(m_vecBarData.empty())
		{
			m_vecBarData.emplace_back(bar);
		}
		else
		{
			ZTSBarStruct* lastBar = at(-1);
			if(lastBar->date==bar.date && lastBar->time==bar.time)
			{
				memcpy(lastBar, &bar, sizeof(ZTSBarStruct));
			}
			else
			{
				m_vecBarData.emplace_back(bar);
			}
		}
	}
};



/*
 *	Tick数据对象
 *	内部封装ZTSTickStruct
 *	封装的主要目的是出于跨语言的考虑
 */
class ZTSTickData : public ZTSPoolObject<ZTSTickData>
{
public:
	ZTSTickData() :m_pContract(NULL) {}

	/*
	 *	创建一个tick数据对象
	 *	@stdCode 合约代码
	 */
	static inline ZTSTickData* create(const char* stdCode)
	{
		ZTSTickData* pRet = ZTSTickData::allocate();
		auto len = strlen(stdCode);
		memcpy(pRet->m_tickStruct.code, stdCode, len);
		pRet->m_tickStruct.code[len] = 0;

		return pRet;
	}

	/*
	 *	根据tick结构体创建一个tick数据对象
	 *	@tickData tick结构体
	 */
	static inline ZTSTickData* create(ZTSTickStruct& tickData)
	{
		ZTSTickData* pRet = allocate();
		memcpy(&pRet->m_tickStruct, &tickData, sizeof(ZTSTickStruct));

		return pRet;
	}

	inline void setCode(const char* code, std::size_t len = 0)
	{
		len = (len == 0) ? strlen(code) : len;

		memcpy(m_tickStruct.code, code, len);
		m_tickStruct.code[len] = '\0';
	}

	/*
	 *	读取合约代码
	 */
	inline const char* code() const{ return m_tickStruct.code; }

	/*
	 *	读取市场代码
	 */
	inline const char*	exchg() const{ return m_tickStruct.exchg; }

	/*
	 *	读取最新价
	 */
	inline double	price() const{ return m_tickStruct.price; }

	inline double	open() const{ return m_tickStruct.open; }

	/*
	 *	最高价
	 */
	inline double	high() const{ return m_tickStruct.high; }

	/*
	 *	最低价
	 */
	inline double	low() const{ return m_tickStruct.low; }

	//昨收价,如果是期货则是昨结算
	inline double	preclose() const{ return m_tickStruct.pre_close; }
	inline double	presettle() const{ return m_tickStruct.pre_settle; }
	inline double	preinterest() const{ return m_tickStruct.pre_interest; }

	inline double	upperlimit() const{ return m_tickStruct.upper_limit; }
	inline double	lowerlimit() const{ return m_tickStruct.lower_limit; }
	//成交量
	inline double	totalvolume() const{ return m_tickStruct.total_volume; }

	//成交量
	inline double	volume() const{ return m_tickStruct.volume; }

	//结算价
	inline double	settlepx() const{ return m_tickStruct.settle_price; }

	//总持
	inline double	openinterest() const{ return m_tickStruct.open_interest; }

	inline double	additional() const{ return m_tickStruct.diff_interest; }

	//成交额
	inline double	totalturnover() const{ return m_tickStruct.total_turnover; }

	//成交额
	inline double	turnover() const{ return m_tickStruct.turn_over; }

	//交易日
	inline uint32_t	tradingdate() const{ return m_tickStruct.trading_date; }

	//数据发生日期
	inline uint32_t	actiondate() const{ return m_tickStruct.action_date; }

	//数据发生时间
	inline uint32_t	actiontime() const{ return m_tickStruct.action_time; }


	/*
	 *	读取指定档位的委买价
	 *	@idx 0-9
	 */
	inline double		bidprice(int idx) const
	{
		if(idx < 0 || idx >= 10) 
			return -1;

		return m_tickStruct.bid_prices[idx];
	}

	/*
	 *	读取指定档位的委卖价
	 *	@idx 0-9
	 */
	inline double		askprice(int idx) const
	{
		if(idx < 0 || idx >= 10) 
			return -1;

		return m_tickStruct.ask_prices[idx];
	}

	/*
	 *	读取指定档位的委买量
	 *	@idx 0-9
	 */
	inline double	bidqty(int idx) const
	{
		if(idx < 0 || idx >= 10) 
			return -1;

		return m_tickStruct.bid_qty[idx];
	}

	/*
	 *	读取指定档位的委卖量
	 *	@idx 0-9
	 */
	inline double	askqty(int idx) const
	{
		if(idx < 0 || idx >= 10) 
			return -1;

		return m_tickStruct.ask_qty[idx];
	}

	/*
	 *	返回tick结构体的引用
	 */
	inline ZTSTickStruct&	getTickStruct(){ return m_tickStruct; }

	inline void setContractInfo(ZTSContractInfo* cInfo) { m_pContract = cInfo; }
	inline ZTSContractInfo* getContractInfo() const { return m_pContract; }

private:
	ZTSTickStruct		m_tickStruct;
	ZTSContractInfo*	m_pContract;
};

class ZTSOrdQueData : public ZTSObject
{
public:
	static inline ZTSOrdQueData* create(const char* code)
	{
		ZTSOrdQueData* pRet = new ZTSOrdQueData;
		vvt_strcpy(pRet->m_oqStruct.code, code);
		return pRet;
	}

	static inline ZTSOrdQueData* create(ZTSOrdQueStruct& ordQueData)
	{
		ZTSOrdQueData* pRet = new ZTSOrdQueData;
		memcpy(&pRet->m_oqStruct, &ordQueData, sizeof(ZTSOrdQueStruct));

		return pRet;
	}

	inline ZTSOrdQueStruct& getOrdQueStruct(){return m_oqStruct;}

	inline const char* exchg() const{ return m_oqStruct.exchg; }
	inline const char* code() const{ return m_oqStruct.code; }
	inline uint32_t tradingdate() const{ return m_oqStruct.trading_date; }
	inline uint32_t actiondate() const{ return m_oqStruct.action_date; }
	inline uint32_t actiontime() const { return m_oqStruct.action_time; }

	inline void		setCode(const char* code) { vvt_strcpy(m_oqStruct.code, code); }

	inline void setContractInfo(ZTSContractInfo* cInfo) { m_pContract = cInfo; }
	inline ZTSContractInfo* getContractInfo() const { return m_pContract; }

private:
	ZTSOrdQueStruct		m_oqStruct;
	ZTSContractInfo*	m_pContract;
};

class ZTSOrdDtlData : public ZTSObject
{
public:
	static inline ZTSOrdDtlData* create(const char* code)
	{
		ZTSOrdDtlData* pRet = new ZTSOrdDtlData;
		vvt_strcpy(pRet->m_odStruct.code, code);
		return pRet;
	}

	static inline ZTSOrdDtlData* create(ZTSOrdDtlStruct& odData)
	{
		ZTSOrdDtlData* pRet = new ZTSOrdDtlData;
		memcpy(&pRet->m_odStruct, &odData, sizeof(ZTSOrdDtlStruct));

		return pRet;
	}

	inline ZTSOrdDtlStruct& getOrdDtlStruct(){ return m_odStruct; }

	inline const char* exchg() const{ return m_odStruct.exchg; }
	inline const char* code() const{ return m_odStruct.code; }
	inline uint32_t tradingdate() const{ return m_odStruct.trading_date; }
	inline uint32_t actiondate() const{ return m_odStruct.action_date; }
	inline uint32_t actiontime() const { return m_odStruct.action_time; }

	inline void		setCode(const char* code) { vvt_strcpy(m_odStruct.code, code); }

	inline void setContractInfo(ZTSContractInfo* cInfo) { m_pContract = cInfo; }
	inline ZTSContractInfo* getContractInfo() const { return m_pContract; }


private:
	ZTSOrdDtlStruct		m_odStruct;
	ZTSContractInfo*	m_pContract;
};

class ZTSTransData : public ZTSObject
{
public:
	static inline ZTSTransData* create(const char* code)
	{
		ZTSTransData* pRet = new ZTSTransData;
		vvt_strcpy(pRet->m_tsStruct.code, code);
		return pRet;
	}

	static inline ZTSTransData* create(ZTSTransStruct& transData)
	{
		ZTSTransData* pRet = new ZTSTransData;
		memcpy(&pRet->m_tsStruct, &transData, sizeof(ZTSTransStruct));

		return pRet;
	}

	inline const char* exchg() const{ return m_tsStruct.exchg; }
	inline const char* code() const{ return m_tsStruct.code; }
	inline uint32_t tradingdate() const{ return m_tsStruct.trading_date; }
	inline uint32_t actiondate() const{ return m_tsStruct.action_date; }
	inline uint32_t actiontime() const { return m_tsStruct.action_time; }

	inline ZTSTransStruct& getTransStruct(){ return m_tsStruct; }

	inline void		setCode(const char* code) { vvt_strcpy(m_tsStruct.code, code); }

	inline void setContractInfo(ZTSContractInfo* cInfo) { m_pContract = cInfo; }
	inline ZTSContractInfo* getContractInfo() const { return m_pContract; }

private:
	ZTSTransStruct		m_tsStruct;
	ZTSContractInfo*	m_pContract;
};

/*
 *	@brief 历史Tick数据数组
 *	@details 内部使用ZTSArray作为容器
 */
class ZTSHisTickData : public ZTSObject
{
protected:
	char						m_strCode[32];
	std::vector<ZTSTickStruct>	m_ayTicks;
	bool						m_bValidOnly;
	double						m_dFactor;

	ZTSHisTickData() :m_bValidOnly(false), m_dFactor(1.0){}

public:
	/*
	 *	@brief 创建指定大小的tick数组对象
	 *	@details 内部的数组预先分配大小
	 *
	 *	@param stdCode 合约代码
	 *	@param nSize 预先分配的大小
	 */
	static inline ZTSHisTickData* create(const char* stdCode, unsigned int nSize = 0, bool bValidOnly = false, double factor = 1.0)
	{
		ZTSHisTickData *pRet = new ZTSHisTickData;
		vvt_strcpy(pRet->m_strCode, stdCode);
		pRet->m_ayTicks.resize(nSize);
		pRet->m_bValidOnly = bValidOnly;
		pRet->m_dFactor = factor;

		return pRet;
	}

	/*
	 *	@brief 根据tick数组对象创建历史tick数据对象
	 *	@details 内部的tick数组不用再分配了

	 *	@param ayTicks tick数组对象指针
	 */
	static inline ZTSHisTickData* create(const char* stdCode, bool bValidOnly = false, double factor = 1.0)
	{
		ZTSHisTickData *pRet = new ZTSHisTickData;
		vvt_strcpy(pRet->m_strCode, stdCode);
		pRet->m_bValidOnly = bValidOnly;
		pRet->m_dFactor = factor;

		return pRet;
	}

	//读取tick数据的条数
	inline uint32_t	size() const{ return m_ayTicks.size(); }
	inline bool		empty() const{ return m_ayTicks.empty(); }

	//读取该数据对应的合约代码
	inline const char*		code() const{ return m_strCode; }

	/*
	 *	获取指定位置的tick数据
	 *	
	 */
	inline ZTSTickStruct*	at(uint32_t idx)
	{
		if (m_ayTicks.empty() || idx >= m_ayTicks.size())
			return NULL;

		return &m_ayTicks[idx];
	}

	inline std::vector<ZTSTickStruct>& getDataRef() { return m_ayTicks; }

	inline bool isValidOnly() const{ return m_bValidOnly; }

	/*
	*	追加一条Tick
	*/
	inline void	appendTick(const ZTSTickStruct& ts)
	{
		m_ayTicks.emplace_back(ts);
		//复权修正
		m_ayTicks.back().price *= m_dFactor;
		m_ayTicks.back().open *= m_dFactor;
		m_ayTicks.back().high *= m_dFactor;
		m_ayTicks.back().low *= m_dFactor;
	}
};

//////////////////////////////////////////////////////////////////////////
/*
 *	@brief Tick数据切片,从连续的tick缓存中做的切片
 *	@details 切片并没有真实的复制内存,而只是取了开始和结尾的下标
 *	这样使用虽然更快,但是使用场景要非常小心,因为他依赖于基础数据对象
 */
class ZTSTickSlice : public ZTSObject
{
private:
	char			_code[MAX_INSTRUMENT_LENGTH];
	typedef std::pair<ZTSTickStruct*, uint32_t> TickBlock;
	std::vector<TickBlock> _blocks;
	uint32_t		_count;

protected:
	ZTSTickSlice() { _blocks.clear(); }
	inline int32_t		translateIdx(int32_t idx) const
	{
		if (idx < 0)
		{
			return max(0, (int32_t)_count + idx);
		}

		return idx;
	}

public:
	static inline ZTSTickSlice* create(const char* code, ZTSTickStruct* ticks = NULL, uint32_t count = 0)
	{
		//if (ticks == NULL || count == 0)
		//	return NULL;

		ZTSTickSlice* slice = new ZTSTickSlice();
		vvt_strcpy(slice->_code, code);
		if(ticks != NULL)
		{
			slice->_blocks.emplace_back(TickBlock(ticks, count));
			slice->_count = count;
		}

		return slice;
	}

	inline bool appendBlock(ZTSTickStruct* ticks, uint32_t count)
	{
		if (ticks == NULL || count == 0)
			return false;

		_count += count;
		_blocks.emplace_back(TickBlock(ticks, count));
		return true;
	}

	inline bool insertBlock(std::size_t idx, ZTSTickStruct* ticks, uint32_t count)
	{
		if (ticks == NULL || count == 0)
			return false;

		_count += count;
		_blocks.insert(_blocks.begin()+idx, TickBlock(ticks, count));
		return true;
	}

	inline std::size_t	get_block_counts() const
	{
		return _blocks.size();
	}

	inline ZTSTickStruct*	get_block_addr(std::size_t blkIdx)
	{
		if (blkIdx >= _blocks.size())
			return NULL;

		return _blocks[blkIdx].first;
	}

	inline uint32_t get_block_size(std::size_t blkIdx)
	{
		if (blkIdx >= _blocks.size())
			return INVALID_UINT32;

		return _blocks[blkIdx].second;
	}

	inline uint32_t size() const{ return _count; }

	inline bool empty() const{ return (_count == 0); }

	inline const ZTSTickStruct* at(int32_t idx)
	{
		if (_count == 0)
			return NULL;

		idx = translateIdx(idx);
		do 
		{
			for(auto& item : _blocks)
			{
				if ((uint32_t)idx >= item.second)
					idx -= item.second;
				else
					return item.first + idx;
			}
		} while (false);
		return NULL;
	}
};

//////////////////////////////////////////////////////////////////////////
/*
 *	@brief 逐笔委托数据切片,从连续的逐笔委托缓存中做的切片
 *	@details 切片并没有真实的复制内存,而只是取了开始和结尾的下标
 *	这样使用虽然更快,但是使用场景要非常小心,因为他依赖于基础数据对象
 */
class ZTSOrdDtlSlice : public ZTSObject
{
private:
	char				m_strCode[MAX_INSTRUMENT_LENGTH];
	ZTSOrdDtlStruct*	m_ptrBegin;
	uint32_t			m_uCount;

protected:
	ZTSOrdDtlSlice() :m_ptrBegin(NULL), m_uCount(0) {}
	inline int32_t		translateIdx(int32_t idx) const
	{
		if (idx < 0)
		{
			return max(0, (int32_t)m_uCount + idx);
		}

		return idx;
	}

public:
	static inline ZTSOrdDtlSlice* create(const char* code, ZTSOrdDtlStruct* firstItem, uint32_t count)
	{
		if (count == 0 || firstItem == NULL)
			return NULL;

		ZTSOrdDtlSlice* slice = new ZTSOrdDtlSlice();
		vvt_strcpy(slice->m_strCode, code);
		slice->m_ptrBegin = firstItem;
		slice->m_uCount = count;

		return slice;
	}

	inline uint32_t size() const { return m_uCount; }

	inline bool empty() const { return (m_uCount == 0) || (m_ptrBegin == NULL); }

	inline const ZTSOrdDtlStruct* at(int32_t idx)
	{
		if (m_ptrBegin == NULL)
			return NULL;
		idx = translateIdx(idx);
		return m_ptrBegin + idx;
	}
};

//////////////////////////////////////////////////////////////////////////
/*
 *	@brief 委托队列数据切片,从连续的委托队列缓存中做的切片
 *	@details 切片并没有真实的复制内存,而只是取了开始和结尾的下标
 *	这样使用虽然更快,但是使用场景要非常小心,因为他依赖于基础数据对象
 */
class ZTSOrdQueSlice : public ZTSObject
{
private:
	char				m_strCode[MAX_INSTRUMENT_LENGTH];
	ZTSOrdQueStruct*	m_ptrBegin;
	uint32_t			m_uCount;

protected:
	ZTSOrdQueSlice() :m_ptrBegin(NULL), m_uCount(0) {}
	inline int32_t		translateIdx(int32_t idx) const
	{
		if (idx < 0)
		{
			return max(0, (int32_t)m_uCount + idx);
		}

		return idx;
	}

public:
	static inline ZTSOrdQueSlice* create(const char* code, ZTSOrdQueStruct* firstItem, uint32_t count)
	{
		if (count == 0 || firstItem == NULL)
			return NULL;

		ZTSOrdQueSlice* slice = new ZTSOrdQueSlice();
		vvt_strcpy(slice->m_strCode, code);
		slice->m_ptrBegin = firstItem;
		slice->m_uCount = count;

		return slice;
	}

	inline uint32_t size() const { return m_uCount; }

	inline bool empty() const { return (m_uCount == 0) || (m_ptrBegin == NULL); }

	inline const ZTSOrdQueStruct* at(int32_t idx)
	{
		if (m_ptrBegin == NULL)
			return NULL;
		idx = translateIdx(idx);
		return m_ptrBegin + idx;
	}
};

//////////////////////////////////////////////////////////////////////////
/*
 *	@brief 逐笔成交数据切片,从连续的逐笔成交缓存中做的切片
 *	@details 切片并没有真实的复制内存,而只是取了开始和结尾的下标
 *	这样使用虽然更快,但是使用场景要非常小心,因为他依赖于基础数据对象
 */
class ZTSTransSlice : public ZTSObject
{
private:
	char			m_strCode[MAX_INSTRUMENT_LENGTH];
	ZTSTransStruct*	m_ptrBegin;
	uint32_t		m_uCount;

protected:
	ZTSTransSlice() :m_ptrBegin(NULL), m_uCount(0) {}
	inline int32_t		translateIdx(int32_t idx) const
	{
		if (idx < 0)
		{
			return max(0, (int32_t)m_uCount + idx);
		}

		return idx;
	}

public:
	static inline ZTSTransSlice* create(const char* code, ZTSTransStruct* firstItem, uint32_t count)
	{
		if (count == 0 || firstItem == NULL)
			return NULL;

		ZTSTransSlice* slice = new ZTSTransSlice();
		vvt_strcpy(slice->m_strCode, code);
		slice->m_ptrBegin = firstItem;
		slice->m_uCount = count;

		return slice;
	}

	inline uint32_t size() const { return m_uCount; }

	inline bool empty() const { return (m_uCount == 0) || (m_ptrBegin == NULL); }

	inline const ZTSTransStruct* at(int32_t idx)
	{
		if (m_ptrBegin == NULL)
			return NULL;
		idx = translateIdx(idx);
		return m_ptrBegin + idx;
	}
};

NS_ZTP_END