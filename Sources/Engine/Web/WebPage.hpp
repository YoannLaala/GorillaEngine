#ifndef _WEB_WEB_PAGE_HPP_
#define _WEB_WEB_PAGE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Thread/Mutex.hpp>
#include <Web/WebFunction.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{
	class WebBrowser;
	class WebPageHandle;
	class WebArgument;
	class WebValueList;
}}

namespace Gorilla { namespace Renderer
{
	class Renderer;
	class Texture2D;
}}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla { namespace Web
{	
	class WebPage
	{
		friend class WebPageHandle;
		friend class WebBrowser;

	public:
		struct EThread
		{
			enum Type : uint8
			{
				Main = 0,
				Javascript,
				Count,
			};
		};

	public:
		struct EFlag
		{
			enum Type : uint8
			{
				Changed = 1 << 0,
			};
		};

		struct EDialog
		{
			enum Type : uint8
			{
				File = 0,
				FileMultiple,
				Folder,
				Save,

				Count,
			};
		};

	private:
		WebPage();
		~WebPage();

	public:
		void	Open(const char* _szUrl);

		const char* m_szUrl;
		inline const char* GetUrl() const { return m_szUrl;}

		void OpenFileDialog(EDialog::Type _eDialog, std::function<void (const Vector<String>&)> _pCallback, const char* _szTitle = nullptr, const char* _szFilter = nullptr, const char* _szDefaultPath = nullptr);

	private:
		void	Initialize(Renderer::Texture2D* _pTexture, bool _bTransparent);
		void	Release();

	public:
		inline bool		HasFlag		(EFlag::Type _eFlag) const { return (m_eFlag & _eFlag) != 0; }
		inline void		SetFlag		(EFlag::Type _eFlag) { m_eFlag |= _eFlag; }
		inline void		RemoveFlag	(EFlag::Type _eFlag) { m_eFlag &= ~_eFlag; }

		void Update();
		
		void CreateCallback(const char* _szPath, std::function<void (const WebArgument&, WebValueList&)> _pCallback, bool _bSynchronous = false);
		void ExecuteFunction(const char* _szPath, const char* _szValue);

		class ArgumentList
		{
		public:
			template<typename T> inline void Add(T _kValue);
			template<> inline void Add(bool _bValue) { SetBool(GetSize(), _bValue); }

			template<typename T> inline void Add(const Vector<T>& _kValue);
			template<> inline void Add(const Vector<String>& _vValue) { SetArrayString(GetSize(), _vValue); }

		private:
			uint32 GetSize() const;
			void SetBool(uint32 _uiIndex, bool _bValue);
			void SetArrayString(uint32 _uiIndex, const Vector<String>& _vValue);
		};



		void PrepareArgument(ArgumentList* /*_pArgumentList*/) {}
		template <typename T, typename... ARGUMENTS>
		void PrepareArgument(ArgumentList* _pArgumentList, T _kValue, ARGUMENTS... _vArgument)
		{
			_pArgumentList->Add(_kValue);
			 PrepareArgument(_pArgumentList, _vArgument...);
		}

		ArgumentList* ExecuteFunctionInternal(const char* _szPath);
		ArgumentList* ExecuteCallbackInternal(void* _pCallback);

		template <typename... Cdr>
		void ExecuteFunction2(const char* _szPath, Cdr... cdr)
		{
		  ArgumentList* pArgumentList = ExecuteFunctionInternal(_szPath);
		  PrepareArgument(pArgumentList, cdr...);
		}



		template <typename... ARGUMENTS>
		void ExecuteCallback(void* _pCallback, ARGUMENTS... _vArgument)
		{
		  ArgumentList* pArgumentList = ExecuteCallbackInternal(_pCallback);
		  PrepareArgument(pArgumentList, _vArgument...);
		}

		

		//template<typename... ARGUMENTS> void PrepareArgument(const char* _szValue, ARGUMENTS... _vArgument);
		//template<typename... ARGUMENTS> void PrepareArgument(bool _bValue, ARGUMENTS... _vArgument);
		//template<typename... ARGUMENTS> void ExecuteFunction(const char* /*_szPath*/, ARGUMENTS... _vArgument)
		//{
		//	
		//	PrepareArgument(_vArgument...);
		//}

		void SwitchFunctionThread(WebFunction* _pFunction, EThread::Type _eSource, EThread::Type _eDestination)
		{
			m_mFunction.Lock();
			m_vFunction[_eSource].Remove(_pFunction);
			m_vFunction[_eDestination].Add(_pFunction);
			m_mFunction.Unlock();
		}

		inline const void* GetBuffer() const { return m_pBuffer; }
		inline Renderer::Texture2D* GetTexture() { return m_pTexture; }
		void Resize(uint32 _uiWidth, uint32 _uiHeight);

	private:
		Vector<WebFunction*>	m_vFunction[EThread::Count];
		Mutex					m_mFunction;
		Renderer::Texture2D*	m_pTexture;
		const void*				m_pBuffer;
		WebPageHandle*			m_pHandle;
		uint8					m_eFlag;
	};

	/*template<typename... ARGUMENTS>
	void WebPage::PrepareArgument(const char* _szValue, ARGUMENTS... _vArgument) 
	{
		bar(_szValue);
		PrepareArgument(_vArgument...);
	}

	template<typename... ARGUMENTS>
	void WebPage::PrepareArgument(bool _bValue, ARGUMENTS... _vArgument) 
	{
		bar(_bValue);
		PrepareArgument(_vArgument...);
	}*/
}}				

#endif