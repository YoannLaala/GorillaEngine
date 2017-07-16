#ifndef _RENDERER_RENDER_BUFFER_HPP_
#define _RENDERER_RENDER_BUFFER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>


#define DECLARE_RENDER_BUFFER(_type_)															\
static uint32 GetType() { static uint32 uiId = Hash::Generate(#_type_); return uiId ; }			\

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Renderer
{
	class RenderBuffer
	{
	public:
		struct EFlag
		{
			enum Type : uint8
			{
				Static = 1 << 0,
			};
		};

	private:
		struct Section
		{
			Section() : Size(0) { }
			~Section() { }

			Vector<uint8>		Buffer;
			uint32				Size;		
		};

	public:
		RenderBuffer();
		~RenderBuffer();

		template <class T>
		bool Get(T** _ppArray, uint32& _uiSize);

		template <class T>
		T* Push(uint32 uiCount = 1);

		void Clear();

	private:
		HashMap<uint32, Section*> m_mSection;
	};

	//!	@brief		Get 
	//!	@date		2015-11-11
	template <class T>
	bool RenderBuffer::Get(T** _ppArray, uint32& _uiSize)
	{		
		// Find proper section
		uint32 uiTypeId = T::GetType();
		Section* pSection = m_mSection[uiTypeId];
		if(pSection)
		{
			*_ppArray  = reinterpret_cast<T*>(&pSection->Buffer[0]);
			_uiSize = pSection->Size;
			return _uiSize > 0;
		}

		*_ppArray = NULL;
		_uiSize = 0;

		return false;
	}

	//!	@brief		Push 
	//!	@date		2015-11-11
	template <class T>
	T* RenderBuffer::Push(uint32 uiCount /*= 1*/)
	{		
		// Find proper section
		uint32 uiTypeId = T::GetType();
		Section* pSection = m_mSection[uiTypeId];
		if(!pSection)
		{
			pSection = new Section();
			m_mSection.Add(uiTypeId, pSection);
		}

		// Allocate the command in the proper section
		uint32 uiOffset = pSection->Buffer.GetSize();
		pSection->Buffer.Expand(sizeof(T) * uiCount);
		pSection->Size += uiCount;

		return reinterpret_cast<T*>(&pSection->Buffer[uiOffset]);
	}
}}

#endif