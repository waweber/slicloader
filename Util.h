/*
 *  SLICLoader, an EFI program to modify ACPI tables before boot.
 *  Copyright (C) 2013 wweber
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SL_UTIL_H_
#define SL_UTIL_H_

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

/**
 * @file Util.h
 * Utilities and shortcuts.
 */

/**
 * Waits for escape key to be pressed
 * @param systab System table
 * @return
 */
void WaitForESC(EFI_SYSTEM_TABLE* systab);

/**
 * Loads a file from the ESP. The caller is responsible for freeing the memory.
 * @param ImageHandle The current boot image handle
 * @param systab The system table
 * @param filename The file path to load.
 * @param[out] dataPtr Pointer to pointer to the buffer.
 * @param[out] size Pointer to the variable that will store the size.
 * @param[out] dev_path Pointer to pointer to set to device path. May be NULL
 * @return an EFI_STATUS
 */
EFI_STATUS LoadFile(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* systab,
		CHAR16* filename, void** dataPtr, UINTN* size,
		EFI_DEVICE_PATH_PROTOCOL** dev_path);

/**
 * Allocate a block of memory.
 * @param size Size in bytes
 * @param systab Pointer to the system table.
 * @return Pointer to memory, or NULL on failure.
 */
void* Allocate(UINTN size, EFI_SYSTEM_TABLE* systab);

/**
 * Allocates memory in the ACPI region
 * @param size Size in bytes
 * @param systab Pointer to the system table.
 * @return Pointer to memory, or NULL on failure.
 */
void* AllocateACPI(UINTN size, EFI_SYSTEM_TABLE* systab);

/**
 * Frees memory.
 * @param mem
 * @param systab System table
 * @return
 */
void Free(VOID* mem, EFI_SYSTEM_TABLE* systab);

#endif
