#ifndef _WEB_WEB_OBJECT_HPP_
#define _WEB_WEB_OBJECT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Web/WebFunction.hpp>
#include <functional>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{	
	class WebBrowser;
	class WebBrowserHandle;
	class WebPage;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{	
	class WebArgument
	{
	public:
		WebArgument()
		{
		
		}

		~WebArgument()
		{
			const uint32 uiStringCount = m_vString.GetSize();
			for(uint32 uiString = 0; uiString < uiStringCount; ++uiString)
			{
				delete reinterpret_cast<String*>(m_vValue[m_vString[uiString]]);
			}
		}

	public:

		inline void				Clear() { m_vValue.Clear(); m_vString.Clear(); }

		inline uint32			GetSize() const { return m_vValue.GetSize(); }
		inline bool				GetBool(uint32 _uiIndex) const { return m_vValue[_uiIndex] != 0; }
		inline uint32			GetUInt32(uint32 _uiIndex) const { return static_cast<uint32>(m_vValue[_uiIndex]); }
		inline uint64			GetUInt64(uint32 _uiIndex) const { return m_vValue[_uiIndex]; }
		inline float32			GetFloat32(uint32 _uiIndex) const { return static_cast<float32>(m_vValue[_uiIndex]); }
		inline const String&	GetString(uint32 _uiIndex) const { return *reinterpret_cast<String*>(m_vValue[_uiIndex]); }
		inline void*			GetCallback(uint32 _uiIndex) const { return reinterpret_cast<void*>(m_vValue[_uiIndex]); }

		inline void				SetBool(uint32 _uiIndex, bool _bValue) { m_vValue.ExpandTo(_uiIndex, static_cast<uint64>(_bValue)); }
		/*void					Set(uint32 _uiIndex, uint8 _uiValue);
		void					Set(uint32 _uiIndex, uint16 _uiValue);*/
		inline void				SetUInt32(uint32 _uiIndex, uint32 _uiValue) { m_vValue.ExpandTo(_uiIndex, static_cast<uint64>(_uiValue)); }

		inline void				SetInt32(uint32 _uiIndex, int32 _iValue) { m_vValue.ExpandTo(_uiIndex, static_cast<uint64>(_iValue)); }

		inline void				SetFloat64(uint32 _uiIndex, float64 _fValue) { m_vValue.ExpandTo(_uiIndex, static_cast<uint64>(_fValue)); }
		inline void				SetCallback(uint32 _uiIndex, void* _pCallback) { m_vValue.ExpandTo(_uiIndex, reinterpret_cast<uint64>(_pCallback)); }


		/*void					Set(uint32 _uiIndex, uint64 _uiValue);
		void					Set(uint32 _uiIndex, int8 _iValue);
		void					Set(uint32 _uiIndex, int16 _iValue);
		void					Set(uint32 _uiIndex, int32 _iValue);
		void					Set(uint32 _uiIndex, int64 _iValue);
		void					Set(uint32 _uiIndex, float32 _fValue);
		void					Set(uint32 _uiIndex, float64 _fValue);
		void					Set(uint32 _uiIndex, float64 _fValue);
		void					Set(uint32 _uiIndex, float64 _fValue);*/
		inline void				SetString(uint32 _uiIndex, const char* _szValue) 
		{ 
			// Create String
			if(m_vString.Find(_uiIndex) == (uint32)-1)
			{
				m_vString.Add(_uiIndex);
				m_vValue.ExpandTo(_uiIndex, reinterpret_cast<uint64>(new String()));
			}
			
			reinterpret_cast<String*>(m_vValue[_uiIndex])->Set(_szValue);
		}

	private:
		Vector<uint64>	m_vValue;
		Vector<uint32>	m_vString;
	};

	class WebValueList
	{
	public:
		void		SetBool(uint32 _uiIndex, bool _bValue);
		/*void		Set(uint32 _uiIndex, uint8 _uiValue);
		void		Set(uint32 _uiIndex, uint16 _uiValue);*/
		void		SetUInt32(uint32 _uiIndex, uint32 _uiValue);
		/*void		Set(uint32 _uiIndex, uint64 _uiValue);
		void		Set(uint32 _uiIndex, int8 _iValue);
		void		Set(uint32 _uiIndex, int16 _iValue);
		void		Set(uint32 _uiIndex, int32 _iValue);
		void		Set(uint32 _uiIndex, int64 _iValue);
		void		Set(uint32 _uiIndex, float32 _fValue);
		void		Set(uint32 _uiIndex, float64 _fValue);
		void		Set(uint32 _uiIndex, float64 _fValue);
		void		Set(uint32 _uiIndex, float64 _fValue);*/
		void		SetString(uint32 _uiIndex, const char* _szValue);
							   
		uint32		GetSize() const;
		bool		GetBool(uint32 _uiIndex) const;
		uint32		GetUInt32(uint32 _uiIndex) const;
		String		GetString(uint32 _uiIndex) const;
	};

	class WebValue
	{
	public:
		void	AddReference();
		void	RemoveReference();

		bool	GetBool() const;
		uint32	GetUInt32() const;
		uint64	GetUInt64() const;
		float32	GetFloat32() const;
		String	GetString() const;
	};
}}				

#endif