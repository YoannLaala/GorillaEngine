/******************************************************************************
**	Includes
******************************************************************************/
#include "ClangCooker.hpp"
#include <Core/File/FileManager.hpp>
#include <Core/Container/Dictionary.hpp>
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <clang-c/Index.h>
#include <functional>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		GetFullClassName
	//!	@date		2015-11-22
	static void GetFullClassName(CXCursor kClangCursor, String& _sNameOut)
	{
		_sNameOut.Set("");
		CXCursor kClangParentSemantic = clang_getCursorLexicalParent(kClangCursor);
		while(!clang_Cursor_isNull(kClangParentSemantic) && clang_getCursorKind(kClangParentSemantic) == CXCursor_Namespace)
		{
			_sNameOut.Prepend("::");
			_sNameOut.Prepend((const char*)clang_getCursorSpelling(kClangParentSemantic).data);
			kClangParentSemantic = clang_getCursorLexicalParent(kClangParentSemantic);
		}
		_sNameOut.Append((const char*)clang_getCursorSpelling(kClangCursor).data);
		_sNameOut.Remove("class ");
	}

	//!	@brief		ClassHandle
	//!	@date		2015-11-22
	struct ClassHandle
	{
		ClassHandle() : Description(nullptr), CompilerOffset(0) { }

		String			Name;
		Node			PropertyNode;
		Dictionary*		Description;
		Vector<String>	DependencyNames;
		int64			CompilerOffset;
	};
	void CreateClassHandle(CXCursor _kClangCursor, Dictionary* _pDescription, const char* _szclassName);
	
	//int num = clang_Cursor_getNumTemplateArguments(kClangCursor);
						//num;
						////clang_getPointeeType
						//

						//clang_visitChildren(fieldnewParent, test, nullptr);
						//CXCursorKind eClangKind2 = clang_getCursorKind(fieldnew);
						//eClangKind2;
						//CXType type2 = clang_getCursorType(fieldnew);
						//type2;
						//num = clang_Cursor_getNumTemplateArguments(fieldnew);
						//clang_visitChildren(fieldnew, test, pClangClientData);


	//!	@brief		ClangExtractFieldFromComponent
	//!	@date		2015-11-22
	CXChildVisitResult ClangExtractFieldFromComponent(CXCursor kClangCursor, CXCursor kClangCursorParent, CXClientData pClangClientData)
	{
		ClassHandle* pClass = static_cast<ClassHandle*>(pClangClientData);

		//Check parent before continuing
		String sClassName;
		CXCursor eClangClassCursorParent = clang_getTypeDeclaration(clang_getCursorType(kClangCursorParent));
		GetFullClassName(eClangClassCursorParent, sClassName);
		if(pClass->Name != sClassName && pClass->DependencyNames.Find(sClassName) == (uint32)-1) return CXChildVisit_Continue;

		CXCursorKind eClangKind = clang_getCursorKind(kClangCursor);		
		switch(eClangKind)
		{		
			/*case CXCursor_ClassDecl: 
			{	
				clang_visitChildren(kClangCursor, ClangExtractFieldFromComponent, pClangClientData);
				break;
			}*/

			// Check the class name to try recursive
			case CXCursor_CXXBaseSpecifier: 
			{	
				String& sDependencyName = pClass->DependencyNames.Add();
				CXCursor eClangClassCursor = clang_getTypeDeclaration(clang_getCursorType(kClangCursor));	
				GetFullClassName(eClangClassCursor, sDependencyName);	

				Node kClassNode = pClass->Description->Get(sDependencyName.GetBuffer());
				if(kClassNode.IsValid()) break;

				CreateClassHandle(eClangClassCursor, pClass->Description, sDependencyName.GetBuffer());
				break;
			}

			// Field to extract
			case CXCursor_FieldDecl:
			{
				if(pClass->Name != sClassName) break;

				CXType eClangType = clang_getCanonicalType(clang_getCursorType(kClangCursor));
				CX_CXXAccessSpecifier eClangScope = clang_getCXXAccessSpecifier(kClangCursor);
				int64 iSize = (int64)clang_Type_getSizeOf(eClangType);
				switch(eClangType.kind)
				{
					// Not supported
					case CXType_Unexposed: break;

					default:
					{
						// Format the display name
						char* szFieldName = (char*)clang_getCursorSpelling(kClangCursor).data;
						char* szDisplayName = szFieldName;
						if(eClangScope == CX_CXXPublic)
						{
							while(*szDisplayName != 0 && !isupper(*szDisplayName)) ++szDisplayName;
							if(*szDisplayName == 0) szDisplayName = szFieldName;
							szDisplayName[0] = (char)toupper(szDisplayName[0]);
						}

						static String sTypeName;
						if(eClangType.kind != CXType_Record) sTypeName = (const char*)clang_getTypeKindSpelling(eClangType.kind).data;
						else GetFullClassName(clang_getTypeDeclaration(eClangType), sTypeName);
						int64 iOffset = (clang_Cursor_getOffsetOfField(kClangCursor) / 8) - pClass->CompilerOffset;

						// Create the node
						Node kFieldNode = pClass->PropertyNode.Add(szDisplayName);
						kFieldNode["visible"] =  eClangScope == CX_CXXPublic;
						kFieldNode["type"] = sTypeName.GetBuffer();
						kFieldNode["offset"] = iOffset;

						// Specific complex class
						switch(eClangType.kind)
						{
							case CXType_ConstantArray:
							{
								CXType eClangElementType = clang_getArrayElementType(eClangType);
								int64 iElementSize = (int64)clang_Type_getSizeOf(eClangElementType);
								kFieldNode["size"] = iSize / iElementSize;
								kFieldNode["element_type"] = (const char*)clang_getTypeKindSpelling(eClangElementType.kind).data;
								break;
							}

							case CXType_Record:
							{
								static String sElementType;
								CXType eClangTemplateType = clang_Type_getTemplateArgumentAsType(eClangType, 0);
								CXCursor kClangClassCursor = clang_getTypeDeclaration(eClangTemplateType);
								GetFullClassName(kClangClassCursor, sElementType);
								kFieldNode["element_type"] = sElementType.GetBuffer();
								break;
							}
						}
						

						//int num = clang_Cursor_getNumTemplateArguments(kClangCursor);
						//num;
						////clang_getPointeeType
						//

						//clang_visitChildren(fieldnewParent, test, nullptr);
						//CXCursorKind eClangKind2 = clang_getCursorKind(fieldnew);
						//eClangKind2;
						//CXType type2 = clang_getCursorType(fieldnew);
						//type2;
						//num = clang_Cursor_getNumTemplateArguments(fieldnew);
						//clang_visitChildren(fieldnew, test, pClangClientData);

						break;
					}
				}

				// Workaround this the parser is using clang and we are compilating with msvc
				if(eClangType.kind == CXType_Bool) pClass->CompilerOffset += 3;

				break;
			}
		}			

		return CXChildVisit_Recurse;
	}

	//!	@brief		ClangCheckComponentBased
	//!	@date		2015-11-22
	CXChildVisitResult ClangCheckComponentBased(CXCursor kClangCursor, CXCursor /*kClangCursorParent*/, CXClientData pClangClientData)
	{
		static String sClassName;

		bool& bIsComponent = *static_cast<bool*>(pClangClientData);
		CXCursorKind eClangKind = clang_getCursorKind(kClangCursor);		
		switch(eClangKind)
		{		
			case CXCursor_ClassDecl: 
			{	
				GetFullClassName(kClangCursor, sClassName);
				if(sClassName == "Gorilla::Engine::Component")
				{
					bIsComponent = true;
					return CXChildVisit_Break;
				}	
				break;
			}

			// Check the class name to try recursive
			case CXCursor_CXXBaseSpecifier: 
			{		
				CXCursor eClangClassCursor = clang_getTypeDeclaration(clang_getCursorType(kClangCursor));
				GetFullClassName(eClangClassCursor, sClassName);

				// Check if we need to stop recursion
				if(sClassName == "Gorilla::Engine::Component") bIsComponent = true;
				else clang_visitChildren(eClangClassCursor, ClangCheckComponentBased, pClangClientData);
				break;
			}
		}

		return bIsComponent ? CXChildVisit_Break : CXChildVisit_Recurse;
	}

	//!	@brief		ClangFindComponentBased
	//!	@date		2015-11-22
	CXChildVisitResult ClangFindComponentBased(CXCursor kClangCursor, CXCursor /*kClangCursorParent*/, CXClientData pClangClientData)
	{
		Dictionary* pDescription = static_cast<Dictionary*>(pClangClientData);
		CXCursorKind eClangKind = clang_getCursorKind(kClangCursor);		
		switch(eClangKind)
		{		
			case CXCursor_ClassDecl: 
			{	
				static String sClassName;
				CXCursor eClangClassCursor = clang_getTypeDeclaration(clang_getCursorType(kClangCursor));
				GetFullClassName(eClangClassCursor, sClassName);

				// Extract information only from Component
				bool bIsComponent = false;
				clang_visitChildren(kClangCursor, ClangCheckComponentBased, &bIsComponent);
				if(bIsComponent)
				{
					CreateClassHandle(kClangCursor, pDescription, sClassName.GetBuffer());
					return CXChildVisit_Continue;
				}
			}
		}

		return CXChildVisit_Recurse;
	}

	//!	@brief		CreateClassHandle
	//!	@date		2015-11-22
	void CreateClassHandle(CXCursor _kClangCursor, Dictionary* _pDescription, const char* _szclassName)
	{
		Node kClassNode = _pDescription->Add(_szclassName);

		ClassHandle hClass;
		hClass.Description = _pDescription;
		hClass.Name = _szclassName;
		hClass.PropertyNode = kClassNode.Add("fields");
		clang_visitChildren(_kClangCursor, ClangExtractFieldFromComponent, &hClass);
		kClassNode["dependencies"] = hClass.DependencyNames;
	}

	//!	@brief		Serialize
	//!	@date		2015-11-22
	bool ClangCooker::Serialize(const char* _szInputPath, FileWriter& _kOutputStream)
	{
		Gorilla::Vector<Gorilla::String> vFile;

		// Check if there file to extract information from module
		String sDirectory;
		if(FileManager::IsFileExist(_szInputPath))
		{
			Gorilla::Dictionary dModule;
			if(!dModule.Read<Gorilla::DictionaryStreamJson>(_szInputPath))
			{
				printf("Failed to read module %s", _szInputPath);
				return false;
			}
			dModule.GetValue("files", vFile);
			sDirectory = GetInputPath().GetDirectory();
		}
		// Check if there file to extract information from directory
		else
		{
			FileManager::GetAllFiles(_szInputPath, vFile, true, "hpp;cpp");
			const Gorilla::uint32 uiFileCount = vFile.GetSize();
			for(Gorilla::uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile) 
			{
				vFile[uiFile].Remove(_szInputPath);
			}
			sDirectory = _szInputPath;
		}
		if(vFile.IsEmpty()) return true;

		// Build generic path
		Gorilla::String sCookerPath, sEditorPath;
		Gorilla::FileManager::GetDirectory(Gorilla::FileManager::Directory::Executable, sCookerPath);
		sEditorPath.Set(sCookerPath).Append("..\\..\\");
		Gorilla::Path::Format(sEditorPath);

		// Fill clang argument
		String sArgument;
		sArgument.Set("-isystem").Append(sEditorPath).Append("..\\..\\Sources\\Engine\\");
		const char* argv[] = 
		{
			sArgument.GetBuffer()
		};

		// Extract Field declaration
		String sFilePath;
		Gorilla::Dictionary dDescription;
		const Gorilla::uint32 uiFileCount = vFile.GetSize();
		for(Gorilla::uint32 uiFile = 0; uiFile < uiFileCount; ++uiFile) 
		{
			// build file path
			String& sRelativePath = vFile[uiFile];
			if(sRelativePath.Find(".cpp") != (uint32)-1) continue;
			sFilePath.Set(sDirectory).Append(sRelativePath);

			// Parse tranlation unit
			CXIndex pClangIndex = clang_createIndex(0, 0);
			CXTranslationUnit pClangUnit = clang_parseTranslationUnit(pClangIndex, sFilePath.GetBuffer(), argv, 1, nullptr, 0,  CXTranslationUnit_SkipFunctionBodies);
			if(!pClangUnit) return false;

			CXCursor kClangCursor = clang_getTranslationUnitCursor(pClangUnit);
			clang_visitChildren(kClangCursor, ClangFindComponentBased, &dDescription);
			clang_disposeTranslationUnit(pClangUnit);
			clang_disposeIndex(pClangIndex);
		}
		dDescription.Write<DictionaryStreamJson>(&_kOutputStream);

		return true;
	}
}