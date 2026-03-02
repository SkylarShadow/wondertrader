/*!
 * \file VvTSVariant.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt通用变量对象定义
 */
#pragma once

#include "VvTSTypes.h"
#include "VvTSObject.hpp"
#include "VvTSCollection.hpp"

#include <string>
#include <string.h>
#include <vector>
#include <map>

#ifdef _MSC_VER
#define INT64_FMT	"%I64d"
#define UINT64_FMT	"%I64u"
#else
#define INT64_FMT	"%ld"
#define UINT64_FMT	"%lu"
#endif


NS_VVTP_BEGIN

/*
 *	VvTSVariant是一个通用数据容器,设计目标是Json的Value类
 *	和Json不同的地方在于,VvTSVariant满足WT系统内的派生关系
 *	可以通过引用计数管理数据,从而减少数据复制
 */
class VvTSVariant : public VvTSObject
{
public:
	typedef VvTSArray					ChildrenArray;
	typedef VvTSHashMap<std::string>		ChildrenMap;
	typedef std::vector<std::string>	MemberNames;

	typedef enum
	{
		VT_Null,
		VT_Array,
		VT_Int32,
		VT_Uint32,
		VT_Int64,
		VT_Uint64,
		VT_String,
		VT_Real,
		VT_Boolean,
		VT_Object
	}ValueType;


protected:
	VvTSVariant() :_type(VT_Null){}

private:
	static inline VvTSVariant* create(int32_t i32)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Int32;
		char s[32] = { 0 };
		sprintf(s, "%d", i32);
		ret->_value._string = new std::string(s);
		return ret;
	}

	static inline VvTSVariant* create(uint32_t u32)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Uint32;
		char s[32] = { 0 };
		sprintf(s, "%u", u32);
		ret->_value._string = new std::string(s);
		return ret;
	}

	static inline VvTSVariant* create(int64_t i64)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Int64;
		char s[32] = { 0 };
		sprintf(s, INT64_FMT, i64);
		ret->_value._string = new std::string(s);
		return ret;
	}

	static inline VvTSVariant* create(uint64_t u64)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Uint64;
		char s[32] = { 0 };
		sprintf(s, UINT64_FMT, u64);
		ret->_value._string = new std::string(s);
		return ret;
	}

	static inline VvTSVariant* create(double _real)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Real;
		char s[32] = { 0 };
		sprintf(s, "%.10f", _real);
		ret->_value._string = new std::string(s);
		return ret;
	}

	static inline VvTSVariant* create(const char* _string)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_String;
		ret->_value._string = new std::string(_string);
		return ret;
	}

	static inline VvTSVariant* create(bool _bool)
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Boolean;
		ret->_value._string = new std::string(_bool ? "true" : "false");
		return ret;
	}

public:
	static inline VvTSVariant* createObject()
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Object;
		ret->_value._map = ChildrenMap::create();
		return ret;
	}

	static inline VvTSVariant* createArray()
	{
		VvTSVariant* ret = new VvTSVariant();
		ret->_type = VT_Array;
		ret->_value._array = ChildrenArray::create();
		return ret;
	}

	inline bool has(const char* key) const
	{
		if (_type != VT_Object)
			return false;

		auto it = _value._map->find(key);
		if (it == _value._map->end())
			return false;

		return true;
	}

	inline int32_t asInt32() const
	{
		switch (_type)
		{
		case VT_Null:
			return 0;
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
			return _value._string ? (int32_t)atof(_value._string->c_str()) : 0;
		default:
			return 0;
		}
	}

	inline uint32_t asUInt32() const
	{
		switch (_type)
		{
		case VT_Null:
			return 0;
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
			return _value._string ? (uint32_t)atof(_value._string->c_str()) : 0;
		default:
			return 0;
		}
	}

	inline int64_t asInt64() const
	{
		switch (_type)
		{
		case VT_Null:
			return 0;
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
			return _value._string ? strtoll(_value._string->c_str(), NULL, 10) : 0;
		default:
			return 0;
		}
	}

	inline uint64_t asUInt64() const
	{
		switch (_type)
		{
		case VT_Null:
			return 0;
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
			return _value._string ? strtoull(_value._string->c_str(), NULL, 10) : 0;
		default:
			return 0;
		}
	}

	inline double asDouble() const
	{
		switch (_type)
		{
		case VT_Null:
			return 0.0;
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
			return _value._string ? strtod(_value._string->c_str(), NULL) : 0.0;
		default:
			return 0.0;
		}
	}

	inline std::string	asString() const
	{
		switch (_type)
		{
		case VT_Null:
			return "";
		case VT_Int32:
		case VT_Uint32:
		case VT_Int64:
		case VT_Uint64:
		case VT_Real:
		case VT_String:
		case VT_Boolean:
			return _value._string ? *_value._string : "";
		default:
			return "";
		}
	}

	inline const char* asCString() const
	{
		if (_type != VT_Object && _type != VT_Array && _value._string != NULL)
			return _value._string->c_str();

		return "";
	}

	inline bool asBoolean() const
	{
		if (_value._string)
		{
			return vvt_stricmp(_value._string->c_str(), "true") == 0 || vvt_stricmp(_value._string->c_str(), "yes") == 0;
		}

		return false;
	}

	inline int32_t getInt32(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asInt32();

		return 0;
	}

	inline uint32_t getUInt32(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asUInt32();

		return 0;
	}

	inline int64_t getInt64(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asInt64();

		return 0;
	}

	inline uint64_t getUInt64(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asUInt64();

		return 0;
	}

	inline double getDouble(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asDouble();

		return 0.0;
	}

	inline std::string getString(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asString();

		return "";
	}

	inline const char* getCString(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asCString();

		return "";
	}

	inline bool getBoolean(const char* name) const
	{
		VvTSVariant* p = get(name);
		if (p)
			return p->asBoolean();

		return false;
	}

	inline VvTSVariant* get(const char* name) const
	{
		if (_type != VT_Object)
			return NULL;

		if (_value._map == NULL)
			return NULL;

		VvTSVariant* ret = static_cast<VvTSVariant*>(_value._map->get(name));
		return ret;
	}

	inline VvTSVariant* get(const std::string& name) const
	{
		if (_type != VT_Object)
			return NULL;

		if (_value._map == NULL)
			return NULL;

		VvTSVariant* ret = static_cast<VvTSVariant*>(_value._map->get(name));
		return ret;
	}

	inline VvTSVariant* get(uint32_t idx) const
	{
		if (_type != VT_Array)
			return NULL;

		if (_value._array == NULL)
			return NULL;

		VvTSVariant* ret = static_cast<VvTSVariant*>(_value._array->at(idx));
		return ret;
	}

	inline bool append(const char* _name, const char* _string)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_string);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, int32_t _i32)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_i32);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, uint32_t _u32)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_u32);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, int64_t _i64)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_i64);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, uint64_t _u64)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_u64);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, double _real)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_real);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, bool _bool)
	{
		if (_type != VT_Object)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		VvTSVariant* item = VvTSVariant::create(_bool);
		_value._map->add(_name, item, false);
		//item->release();

		return true;
	}

	inline bool append(const char* _name, VvTSVariant *item, bool bAutoRetain = true)
	{
		if (_type != VT_Object || NULL == item)
			return false;

		if (_value._map == NULL)
		{
			_value._map = ChildrenMap::create();
		}

		_value._map->add(_name, item, bAutoRetain);

		return true;
	}

	inline bool append(const char* _str)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_str);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(int32_t _i32)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_i32);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(uint32_t _u32)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_u32);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(int64_t _i64)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_i64);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(uint64_t _u64)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_u64);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(double _real)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_real);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(bool _bool)
	{
		if (_type != VT_Array)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		VvTSVariant* item = VvTSVariant::create(_bool);
		_value._array->append(item, false);
		//item->release();

		return true;
	}

	inline bool append(VvTSVariant *item, bool bAutoRetain = true)
	{
		if (_type != VT_Array || NULL == item)
			return false;

		if (_value._array == NULL)
		{
			_value._array = ChildrenArray::create();
		}

		_value._array->append(item, bAutoRetain);

		return true;
	}

	inline uint32_t size() const
	{
		if (_type != VT_Array && _type != VT_Object)
			return 0;

		else if (_type == VT_Array)
		{
			return (_value._array == NULL) ? 0 : _value._array->size();
		}
		else
		{
			return (_value._map == NULL) ? 0 : _value._map->size();
		}
	}

	inline MemberNames memberNames() const
	{
		MemberNames names;
		if (_type == VT_Object && _value._map != NULL)
		{
			auto it = _value._map->begin();
			for (; it != _value._map->end(); it++)
			{
				names.emplace_back(it->first);
			}
		}

		return std::move(names);
	}

	virtual void release()
	{
		if (isSingleRefs())
		{
			switch (_type)
			{
			case VT_Array:
				if (NULL != _value._array)
				{
					_value._array->release();
				}
				break;
			case VT_Object:
				if (NULL != _value._map)
				{
					_value._map->release();
				}
				break;
			default:
				if (NULL != _value._string)
				{
					delete _value._string;
				}
				break;
			}
		}
		VvTSObject::release();
	}

	inline ValueType type() const{ return _type; }

	inline bool isArray() const{ return _type == VT_Array; }
	inline bool isObject() const{ return _type == VT_Object; }

private:
	union ValueHolder
	{
		std::string*	_string;

		ChildrenMap*	_map;
		ChildrenArray*	_array;
	};
	ValueHolder	_value;
	ValueType	_type;
};

NS_VVTP_END