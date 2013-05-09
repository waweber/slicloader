#include "Boot.h"

#include <Protocol/LoadedImage.h>

EFI_STATUS Boot(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* systab)
{
	EFI_GUID LoadedImageProtocolGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	CHAR16 boot_filename[] = L"\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
	EFI_STATUS res;

	//load boot file
	VOID* data = NULL;
	UINTN data_size = 0;
	res = LoadFile(ImageHandle, systab, boot_filename, &data, &data_size);

	if (res == EFI_NOT_FOUND )
	{
		ErrorPrint(L"Could not find boot manager.\r\n");
		return EFI_NOT_FOUND ;
	}
	else if (res)
	{
		ErrorPrint(L"Loading boot manager image failed.\r\n");
		return EFI_LOAD_ERROR ;
	}

	//get this image's info
	EFI_LOADED_IMAGE_PROTOCOL* img_proto;
	res = systab->BootServices->HandleProtocol(ImageHandle,
			&LoadedImageProtocolGuid, (VOID**) &img_proto);

	if (res)
	{
		systab->BootServices->FreePool(data);
		ErrorPrint(L"Failed to get image info protocol. (Error %d)\r\n", res);
		return EFI_LOAD_ERROR ;
	}

	//Load the image
	EFI_HANDLE new_img_handle;
	res = systab->BootServices->LoadImage(FALSE, ImageHandle, NULL, data,
			data_size, &new_img_handle);

	systab->BootServices->FreePool(data);

	if (res)
	{
		ErrorPrint(L"Failed to load new image. (Error %d)\r\n", res);
		return EFI_LOAD_ERROR ;
	}

	//get new image info
	EFI_LOADED_IMAGE_PROTOCOL* new_img_proto;
	res = systab->BootServices->HandleProtocol(new_img_handle,
			&LoadedImageProtocolGuid, (VOID**) &new_img_proto);

	if (res)
	{
		ErrorPrint(L"Failed to get new image information. (Error %d)\r\n", res);
		return EFI_LOAD_ERROR ;
	}

	//set image data
	new_img_proto->DeviceHandle = img_proto->DeviceHandle;
	new_img_proto->ParentHandle = NULL;
	new_img_proto->FilePath = NULL; //maybe?

	//Start the new image
	Print(L"\r\nPress ESC to boot.\r\n\r\n");
	WaitForESC(systab);

	systab->BootServices->StartImage(new_img_handle, 0, NULL );

	return EFI_SUCCESS;
}
