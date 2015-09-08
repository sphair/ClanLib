/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#pragma once

namespace clan
{
	class Hid
	{
	public:
		Hid();
		~Hid();

		typedef USHORT USAGE;

		struct USAGE_AND_PAGE
		{
			USAGE Usage;
			USAGE UsagePage;
		};

		struct CAPS
		{
			USAGE  Usage;
			USAGE  UsagePage;
			USHORT InputReportByteLength;
			USHORT OutputReportByteLength;
			USHORT FeatureReportByteLength;
			USHORT Reserved[17];
			USHORT NumberLinkCollectionNodes;
			USHORT NumberInputButtonCaps;
			USHORT NumberInputValueCaps;
			USHORT NumberInputDataIndices;
			USHORT NumberOutputButtonCaps;
			USHORT NumberOutputValueCaps;
			USHORT NumberOutputDataIndices;
			USHORT NumberFeatureButtonCaps;
			USHORT NumberFeatureValueCaps;
			USHORT NumberFeatureDataIndices;
		};

		struct BUTTON_CAPS
		{
			USAGE   UsagePage;
			UCHAR   ReportID;
			BOOLEAN IsAlias;
			USHORT  BitField;
			USHORT  LinkCollection;
			USAGE   LinkUsage;
			USAGE   LinkUsagePage;
			BOOLEAN IsRange;
			BOOLEAN IsStringRange;
			BOOLEAN IsDesignatorRange;
			BOOLEAN IsAbsolute;
			ULONG   Reserved[10];
			union
			{
				struct
				{
					USAGE  UsageMin;
					USAGE  UsageMax;
					USHORT StringMin;
					USHORT StringMax;
					USHORT DesignatorMin;
					USHORT DesignatorMax;
					USHORT DataIndexMin;
					USHORT DataIndexMax;
				} Range;
				struct
				{
					USAGE  Usage;
					USAGE  Reserved1;
					USHORT StringIndex;
					USHORT Reserved2;
					USHORT DesignatorIndex;
					USHORT Reserved3;
					USHORT DataIndex;
					USHORT Reserved4;
				} NotRange;
			};
		};

		struct VALUE_CAPS
		{
			USAGE   UsagePage;
			UCHAR   ReportID;
			BOOLEAN IsAlias;
			USHORT  BitField;
			USHORT  LinkCollection;
			USAGE   LinkUsage;
			USAGE   LinkUsagePage;
			BOOLEAN IsRange;
			BOOLEAN IsStringRange;
			BOOLEAN IsDesignatorRange;
			BOOLEAN IsAbsolute;
			BOOLEAN HasNull;
			UCHAR   Reserved;
			USHORT  BitSize;
			USHORT  ReportCount;
			USHORT  Reserved2[5];
			ULONG   UnitsExp;
			ULONG   Units;
			LONG    LogicalMin;
			LONG    LogicalMax;
			LONG    PhysicalMin;
			LONG    PhysicalMax;
			union
			{
				struct
				{
					USAGE  UsageMin;
					USAGE  UsageMax;
					USHORT StringMin;
					USHORT StringMax;
					USHORT DesignatorMin;
					USHORT DesignatorMax;
					USHORT DataIndexMin;
					USHORT DataIndexMax;
				} Range;
				struct
				{
					USAGE  Usage;
					USAGE  Reserved1;
					USHORT StringIndex;
					USHORT Reserved2;
					USHORT DesignatorIndex;
					USHORT Reserved3;
					USHORT DataIndex;
					USHORT Reserved4;
				} NotRange;
			};
		};

		enum REPORT_TYPE
		{
			HidP_Input,
			HidP_Output,
			HidP_Feature
		};

		struct DATA
		{
			USHORT DataIndex;
			USHORT Reserved;
			union
			{
				ULONG   RawValue;
				BOOLEAN On;
			};
		};

		struct ATTRIBUTES
		{
			ULONG  Size;
			USHORT VendorID;
			USHORT ProductID;
			USHORT VersionNumber;
		};

		BOOLEAN GetProductString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		BOOLEAN GetManufacturerString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		BOOLEAN GetSerialNumberString(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		BOOLEAN GetPhysicalDescriptor(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		BOOLEAN GetIndexedString(HANDLE HidDeviceObject, ULONG StringIndex, void *Buffer, ULONG BufferLength);

		BOOLEAN GetAttributes(HANDLE HidDeviceObject, ATTRIBUTES *Attributes);

		ULONG MaxDataListLength(REPORT_TYPE ReportType, void *PreparsedData);
		ULONG MaxUsageListLength(REPORT_TYPE ReportType, USAGE UsagePage, void *PreparsedData);

		NTSTATUS GetCaps(void *PreparsedData, CAPS *caps);
		NTSTATUS GetButtonCaps(REPORT_TYPE ReportType, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData);
		NTSTATUS GetValueCaps(REPORT_TYPE ReportType, VALUE_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData);

		NTSTATUS GetData(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS SetData(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength);

		NTSTATUS GetSpecificButtonCaps(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData);
		NTSTATUS GetSpecificValueCaps(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData);

		NTSTATUS GetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS GetUsagesEx(REPORT_TYPE ReportType, USHORT LinkCollection, USAGE_AND_PAGE *ButtonList, ULONG *UsageLength, void *PreparsedData, PCHAR Report, ULONG ReportLength);
		NTSTATUS SetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS UnsetUsages(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);

		NTSTATUS GetUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS SetUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);

		NTSTATUS GetScaledUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS SetScaledUsageValue(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);

		NTSTATUS GetUsageValueArray(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength);
		NTSTATUS SetUsageValueArray(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength);

		NTSTATUS InitializeReportForID(REPORT_TYPE ReportType, UCHAR ReportID, void *PreparsedData, void *Report, ULONG ReportLength);

		BOOLEAN GetFeature(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);
		BOOLEAN SetFeature(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);

		BOOLEAN SetOutputReport(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);

	private:
		Hid(const Hid &other) = delete;
		Hid &operator=(const Hid &other) = delete;

		typedef BOOLEAN(WINAPI FuncHidD_GetProductString)(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		typedef BOOLEAN(WINAPI FuncHidD_GetManufacturerString)(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		typedef BOOLEAN(WINAPI FuncHidD_GetSerialNumberString)(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		typedef BOOLEAN(WINAPI FuncHidD_GetPhysicalDescriptor)(HANDLE HidDeviceObject, void *Buffer, ULONG BufferLength);
		typedef BOOLEAN(WINAPI FuncHidD_GetIndexedString)(HANDLE HidDeviceObject, ULONG StringIndex, void *Buffer, ULONG BufferLength);

		typedef BOOLEAN(WINAPI FuncHidD_GetAttributes)(HANDLE HidDeviceObject, ATTRIBUTES *Attributes);

		typedef ULONG(WINAPI FuncHidP_MaxDataListLength)(REPORT_TYPE ReportType, void *PreparsedData);
		typedef ULONG(WINAPI FuncHidP_MaxUsageListLength)(REPORT_TYPE ReportType, USAGE UsagePage, void *PreparsedData);

		typedef NTSTATUS(WINAPI FuncHidP_GetCaps)(void *PreparsedData, CAPS *caps);
		typedef NTSTATUS(WINAPI FuncHidP_GetButtonCaps)(REPORT_TYPE ReportType, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData);
		typedef NTSTATUS(WINAPI FuncHidP_GetValueCaps)(REPORT_TYPE ReportType, VALUE_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData);

		typedef NTSTATUS(WINAPI FuncHidP_GetData)(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_SetData)(REPORT_TYPE ReportType, DATA *DataList, ULONG *DataLength, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef NTSTATUS(WINAPI FuncHidP_GetSpecificButtonCaps)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ButtonCaps, USHORT *ButtonCapsLength, void *PreparsedData);
		typedef NTSTATUS(WINAPI FuncHidP_GetSpecificValueCaps)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, BUTTON_CAPS *ValueCaps, USHORT *ValueCapsLength, void *PreparsedData);

		typedef NTSTATUS(WINAPI FuncHidP_GetUsages)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_GetUsagesEx)(REPORT_TYPE ReportType, USHORT LinkCollection, USAGE_AND_PAGE *ButtonList, ULONG *UsageLength, void *PreparsedData, PCHAR Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_SetUsages)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_UnsetUsages)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE *UsageList, ULONG *UsageLength, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef NTSTATUS(WINAPI FuncHidP_GetUsageValue)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_SetUsageValue)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef NTSTATUS(WINAPI FuncHidP_GetScaledUsageValue)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG *UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_SetScaledUsageValue)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef NTSTATUS(WINAPI FuncHidP_GetUsageValueArray)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength);
		typedef NTSTATUS(WINAPI FuncHidP_SetUsageValueArray)(REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, void *UsageValue, USHORT UsageValueByteLength, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef NTSTATUS(WINAPI FuncHidP_InitializeReportForID)(REPORT_TYPE ReportType, UCHAR ReportID, void *PreparsedData, void *Report, ULONG ReportLength);

		typedef BOOLEAN(WINAPI FuncHidD_GetFeature)(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);
		typedef BOOLEAN(WINAPI FuncHidD_SetFeature)(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);

		typedef BOOLEAN(WINAPI FuncHidD_SetOutputReport)(HANDLE HidDeviceObject, void *ReportBuffer, ULONG ReportBufferLength);

		HMODULE module;

		FuncHidD_GetProductString *HidD_GetProductString;
		FuncHidD_GetManufacturerString *HidD_GetManufacturerString;
		FuncHidD_GetSerialNumberString *HidD_GetSerialNumberString;
		FuncHidD_GetPhysicalDescriptor *HidD_GetPhysicalDescriptor;
		FuncHidD_GetIndexedString *HidD_GetIndexedString;
		FuncHidD_GetAttributes *HidD_GetAttributes;
		FuncHidP_MaxDataListLength *HidP_MaxDataListLength;
		FuncHidP_MaxUsageListLength *HidP_MaxUsageListLength;
		FuncHidP_GetCaps *HidP_GetCaps;
		FuncHidP_GetButtonCaps *HidP_GetButtonCaps;
		FuncHidP_GetValueCaps *HidP_GetValueCaps;
		FuncHidP_GetData *HidP_GetData;
		FuncHidP_SetData *HidP_SetData;
		FuncHidP_GetSpecificButtonCaps *HidP_GetSpecificButtonCaps;
		FuncHidP_GetSpecificValueCaps *HidP_GetSpecificValueCaps;
		FuncHidP_GetUsages *HidP_GetUsages;
		FuncHidP_GetUsagesEx *HidP_GetUsagesEx;
		FuncHidP_SetUsages *HidP_SetUsages;
		FuncHidP_UnsetUsages *HidP_UnsetUsages;
		FuncHidP_GetUsageValue *HidP_GetUsageValue;
		FuncHidP_SetUsageValue *HidP_SetUsageValue;
		FuncHidP_GetScaledUsageValue *HidP_GetScaledUsageValue;
		FuncHidP_SetScaledUsageValue *HidP_SetScaledUsageValue;
		FuncHidP_GetUsageValueArray *HidP_GetUsageValueArray;
		FuncHidP_SetUsageValueArray *HidP_SetUsageValueArray;
		FuncHidP_InitializeReportForID *HidP_InitializeReportForID;
		FuncHidD_GetFeature *HidD_GetFeature;
		FuncHidD_SetFeature *HidD_SetFeature;
		FuncHidD_SetOutputReport *HidD_SetOutputReport;
	};
}
