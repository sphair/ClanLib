/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "Display/precomp.h"
#include "hid.h"

namespace clan
{
	Hid::Hid()
		: module(0)
	{
		module = LoadLibrary(L"hid.dll");
		if (module == 0)
			throw Exception("Unable to load hid.dll");

		HidD_GetProductString = reinterpret_cast<FuncHidD_GetProductString *>(GetProcAddress(module, "HidD_GetProductString"));
		HidD_GetManufacturerString = reinterpret_cast<FuncHidD_GetManufacturerString *>(GetProcAddress(module, "HidD_GetManufacturerString"));
		HidD_GetSerialNumberString = reinterpret_cast<FuncHidD_GetSerialNumberString *>(GetProcAddress(module, "HidD_GetSerialNumberString"));
		HidD_GetPhysicalDescriptor = reinterpret_cast<FuncHidD_GetPhysicalDescriptor *>(GetProcAddress(module, "HidD_GetPhysicalDescriptor"));
		HidD_GetIndexedString = reinterpret_cast<FuncHidD_GetIndexedString *>(GetProcAddress(module, "HidD_GetIndexedString"));
		HidD_GetAttributes = reinterpret_cast<FuncHidD_GetAttributes *>(GetProcAddress(module, "HidD_GetAttributes"));
		HidP_MaxDataListLength = reinterpret_cast<FuncHidP_MaxDataListLength *>(GetProcAddress(module, "HidP_MaxDataListLength"));
		HidP_MaxUsageListLength = reinterpret_cast<FuncHidP_MaxUsageListLength *>(GetProcAddress(module, "HidP_MaxUsageListLength"));
		HidP_GetCaps = reinterpret_cast<FuncHidP_GetCaps *>(GetProcAddress(module, "HidP_GetCaps"));
		HidP_GetButtonCaps = reinterpret_cast<FuncHidP_GetButtonCaps *>(GetProcAddress(module, "HidP_GetButtonCaps"));
		HidP_GetValueCaps = reinterpret_cast<FuncHidP_GetValueCaps *>(GetProcAddress(module, "HidP_GetValueCaps"));
		HidP_GetData = reinterpret_cast<FuncHidP_GetData *>(GetProcAddress(module, "HidP_GetData"));
		HidP_SetData = reinterpret_cast<FuncHidP_SetData *>(GetProcAddress(module, "HidP_SetData"));
		HidP_GetSpecificButtonCaps = reinterpret_cast<FuncHidP_GetSpecificButtonCaps *>(GetProcAddress(module, "HidP_GetSpecificButtonCaps"));
		HidP_GetSpecificValueCaps = reinterpret_cast<FuncHidP_GetSpecificValueCaps *>(GetProcAddress(module, "HidP_GetSpecificValueCaps"));
		HidP_GetUsages = reinterpret_cast<FuncHidP_GetUsages *>(GetProcAddress(module, "HidP_GetUsages"));
		HidP_GetUsagesEx = reinterpret_cast<FuncHidP_GetUsagesEx *>(GetProcAddress(module, "HidP_GetUsagesEx"));
		HidP_SetUsages = reinterpret_cast<FuncHidP_SetUsages *>(GetProcAddress(module, "HidP_SetUsages"));
		HidP_UnsetUsages = reinterpret_cast<FuncHidP_UnsetUsages *>(GetProcAddress(module, "HidP_UnsetUsages"));
		HidP_GetUsageValue = reinterpret_cast<FuncHidP_GetUsageValue *>(GetProcAddress(module, "HidP_GetUsageValue"));
		HidP_SetUsageValue = reinterpret_cast<FuncHidP_SetUsageValue *>(GetProcAddress(module, "HidP_SetUsageValue"));
		HidP_GetScaledUsageValue = reinterpret_cast<FuncHidP_GetScaledUsageValue *>(GetProcAddress(module, "HidP_GetScaledUsageValue"));
		HidP_SetScaledUsageValue = reinterpret_cast<FuncHidP_SetScaledUsageValue *>(GetProcAddress(module, "HidP_SetScaledUsageValue"));
		HidP_GetUsageValueArray = reinterpret_cast<FuncHidP_GetUsageValueArray *>(GetProcAddress(module, "HidP_GetUsageValueArray"));
		HidP_SetUsageValueArray = reinterpret_cast<FuncHidP_SetUsageValueArray *>(GetProcAddress(module, "HidP_SetUsageValueArray"));
		HidP_InitializeReportForID = reinterpret_cast<FuncHidP_InitializeReportForID *>(GetProcAddress(module, "HidP_InitializeReportForID"));
		HidD_GetFeature = reinterpret_cast<FuncHidD_GetFeature *>(GetProcAddress(module, "HidD_GetFeature"));
		HidD_SetFeature = reinterpret_cast<FuncHidD_SetFeature *>(GetProcAddress(module, "HidD_SetFeature"));
		HidD_SetOutputReport = reinterpret_cast<FuncHidD_SetOutputReport *>(GetProcAddress(module, "HidD_SetOutputReport"));
	}

	Hid::~Hid()
	{
		FreeLibrary(module);
		module = 0;
	}

	BOOLEAN Hid::GetProductString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength)
	{
		return HidD_GetProductString(HidDeviceObject, Buffer, BufferLength);
	}

	BOOLEAN Hid::GetManufacturerString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength)
	{
		return HidD_GetManufacturerString(HidDeviceObject, Buffer, BufferLength);
	}

	BOOLEAN Hid::GetSerialNumberString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength)
	{
		return HidD_GetSerialNumberString(HidDeviceObject, Buffer, BufferLength);
	}

	BOOLEAN Hid::GetPhysicalDescriptor(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength)
	{
		return HidD_GetPhysicalDescriptor(HidDeviceObject, Buffer, BufferLength);
	}

	BOOLEAN Hid::GetIndexedString(HANDLE HidDeviceObject, ULONG StringIndex, void *Buffer, ULONG BufferLength)
	{
		return HidD_GetIndexedString(HidDeviceObject, StringIndex, Buffer, BufferLength);
	}

	BOOLEAN Hid::GetAttributes(HANDLE HidDeviceObject, ATTRIBUTES *Attributes)
	{
		return HidD_GetAttributes(HidDeviceObject, Attributes);
	}

	ULONG Hid::MaxDataListLength(REPORT_TYPE ReportType, void *PreparsedData)
	{
		return HidP_MaxDataListLength(ReportType, PreparsedData);
	}

	ULONG Hid::MaxUsageListLength(REPORT_TYPE ReportType, USAGE UsagePage, void *PreparsedData)
	{
		return HidP_MaxUsageListLength(ReportType, UsagePage, PreparsedData);
	}

	NTSTATUS Hid::GetCaps(void *PreparsedData, CAPS *caps)
	{
		return HidP_GetCaps(PreparsedData, caps);
	}

	NTSTATUS Hid::GetButtonCaps(REPORT_TYPE ReportType, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData)
	{
		return HidP_GetButtonCaps(ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);
	}

	NTSTATUS Hid::GetValueCaps(REPORT_TYPE ReportType, VALUE_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData)
	{
		return HidP_GetValueCaps(ReportType, ValueCaps, ValueCapsLength, PreparsedData);
	}

	NTSTATUS Hid::GetData(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_GetData(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::SetData(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_SetData(ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::GetSpecificButtonCaps(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData)
	{
		return HidP_GetSpecificButtonCaps(ReportType, UsagePage, LinkCollection, Usage, ButtonCaps, ButtonCapsLength, PreparsedData);
	}

	NTSTATUS Hid::GetSpecificValueCaps(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData)
	{
		return HidP_GetSpecificValueCaps(ReportType, UsagePage, LinkCollection, Usage, ValueCaps, ValueCapsLength, PreparsedData);
	}

	NTSTATUS Hid::GetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_GetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::GetUsagesEx(REPORT_TYPE ReportType, USHORT LinkCollection, USAGE_AND_PAGE *ButtonList, ULONG *UsageLength, void *PreparsedData, PCHAR Report, ULONG ReportLength)
	{
		return HidP_GetUsagesEx(ReportType, LinkCollection, ButtonList, UsageLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::SetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_SetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::UnsetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_UnsetUsages(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::GetUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_GetUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::SetUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_SetUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::GetScaledUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_GetScaledUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::SetScaledUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_SetScaledUsageValue(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::GetUsageValueArray(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_GetUsageValueArray(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::SetUsageValueArray(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_SetUsageValueArray(ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
	}

	NTSTATUS Hid::InitializeReportForID(REPORT_TYPE ReportType, UCHAR ReportID, void *PreparsedData, void *Report, ULONG ReportLength)
	{
		return HidP_InitializeReportForID(ReportType, ReportID, PreparsedData, Report, ReportLength);
	}

	BOOLEAN Hid::GetFeature(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength)
	{
		return HidD_GetFeature(HidDeviceObject, ReportBuffer, ReportBufferLength);
	}

	BOOLEAN Hid::SetFeature(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength)
	{
		return HidD_SetFeature(HidDeviceObject, ReportBuffer, ReportBufferLength);
	}

	BOOLEAN Hid::SetOutputReport(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength)
	{
		return HidD_SetOutputReport(HidDeviceObject, ReportBuffer, ReportBufferLength);
	}
}
