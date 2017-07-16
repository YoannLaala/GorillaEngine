#include "Hpp2TypeInfo.hpp"

int main(int argc, const char** argv)
{
	Common::FileSystem kFileSystem;

	Common::ArgumentList kArgumentList;
	kArgumentList.Parse(argc, argv);

	// Initialize the MeshWriter
	MeshWriter* pMeshWriter = new MeshWriter();
	if(!pMeshWriter->Initialize())
	{
		exit(-1);
	}

	// Check the type of file
	DWORD dwAttribute = kFileSystem.GetAttribute(kArgumentList[0].GetString());
	switch(dwAttribute)
	{
		// Handle directory
		case FILE_ATTRIBUTE_DIRECTORY:
		{
			if(!pMeshWriter->SaveAllFile(kArgumentList[0].GetString(), true))
			{
				exit(-1);
			}
			break;
		}

		// Handle single file
		case FILE_ATTRIBUTE_ARCHIVE:
		case FILE_ATTRIBUTE_NORMAL:
		{
			if(!pMeshWriter->SaveOneFile(kArgumentList[0].GetString(), kArgumentList[1].GetString(), false))
			{
				exit(-1);
			}
			break;
		}
	}

	SAFE_DELETE(pMeshWriter)

	return 0;
}