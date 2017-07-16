/******************************************************************************
**	Includes
******************************************************************************/
#include <Web/WebValue.hpp>

#pragma warning(push)
#pragma warning(disable : 4100)
	#include <cef/include/cef_v8.h>
#pragma warning(pop)

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla { namespace Web
{
	inline const CefV8ValueList* GetData(const WebValueList* _pData) { return reinterpret_cast<const CefV8ValueList*>(_pData); }
	inline CefV8ValueList* GetData(WebValueList* _pData) { return reinterpret_cast<CefV8ValueList*>(_pData); }
	uint32	WebValueList::GetSize() const									{ return static_cast<uint32>(GetData(this)->size());	}
	bool	WebValueList::GetBool(uint32 _uiIndex) const					{ return GetData(this)->at(_uiIndex)->GetBoolValue(); }
	uint32	WebValueList::GetUInt32(uint32 _uiIndex) const					{ return GetData(this)->at(_uiIndex)->GetUIntValue(); }
	String	WebValueList::GetString(uint32 _uiIndex) const					{ return GetData(this)->at(_uiIndex)->GetStringValue().ToString().c_str(); }
	void	WebValueList::SetBool(uint32 _uiIndex, bool _bValue)			{ GetData(this)->insert(GetData(this)->begin() + _uiIndex, CefV8Value::CreateBool(_bValue));	}
	void	WebValueList::SetUInt32(uint32 _uiIndex, uint32 _uiValue)		{ GetData(this)->insert(GetData(this)->begin() + _uiIndex, CefV8Value::CreateUInt(_uiValue));	}
	void	WebValueList::SetString(uint32 _uiIndex, const char* _szValue)	{ GetData(this)->insert(GetData(this)->begin() + _uiIndex, CefV8Value::CreateString(_szValue));	}

	inline CefV8Value* GetData(const WebValue* _pData) { return reinterpret_cast<CefV8Value*>(const_cast<WebValue*>(_pData)); }
	inline CefV8Value* GetData(WebValue* _pData) { return reinterpret_cast<CefV8Value*>(_pData); }
	void	WebValue::AddReference() { GetData(this)->AddRef(); }
	void	WebValue::RemoveReference() { GetData(this)->Release(); }

	bool	WebValue::GetBool() const { return GetData(this)->GetBoolValue(); }
	uint32	WebValue::GetUInt32() const { return GetData(this)->GetUIntValue(); }
	uint64	WebValue::GetUInt64() const { return (uint64) GetData(this)->GetDoubleValue(); }
	float32	WebValue::GetFloat32() const { return static_cast<float32>(GetData(this)->GetDoubleValue()); }
	String	WebValue::GetString() const { return GetData(this)->GetStringValue().ToString().c_str(); }
}}
