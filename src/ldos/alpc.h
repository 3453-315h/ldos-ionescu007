#pragma once

#include <Windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

typedef USHORT CSHORT;
typedef ULONG CLONG;

typedef struct _QUAD
{
	double DoNotUseThisField;
} QUAD, *PQUAD, UQUAD, *PUQUAD;

typedef struct _PORT_MESSAGE
{
	union
	{
		struct
		{
			CSHORT DataLength;
			CSHORT TotalLength;
		} s1;
		ULONG Length;
	} u1;
	union
	{
		struct
		{
			CSHORT Type;
			CSHORT DataInfoOffset;
		} s2;
		ULONG ZeroInit;
	} u2;
	union
	{
		CLIENT_ID ClientId;
		QUAD DoNotUseThisField;
	};
	ULONG MessageId;
	union
	{
		SIZE_T ClientViewSize; // only valid for LPC_CONNECTION_REQUEST messages
		ULONG CallbackId; // only valid for LPC_REQUEST messages
	};
} PORT_MESSAGE, *PPORT_MESSAGE;

typedef struct _ALPC_MESSAGE_ATTRIBUTES
{
	ULONG AllocatedAttributes;
	ULONG ValidAttributes;
} ALPC_MESSAGE_ATTRIBUTES, *PALPC_MESSAGE_ATTRIBUTES;

typedef struct _ALPC_PORT_ATTRIBUTES
{
	ULONG Flags;
	SECURITY_QUALITY_OF_SERVICE SecurityQos;
	SIZE_T MaxMessageLength;
	SIZE_T MemoryBandwidth;
	SIZE_T MaxPoolUsage;
	SIZE_T MaxSectionSize;
	SIZE_T MaxViewSize;
	SIZE_T MaxTotalSectionSize;
	ULONG DupObjectTypes;
#ifdef _M_X64
	ULONG Reserved;
#endif
} ALPC_PORT_ATTRIBUTES, *PALPC_PORT_ATTRIBUTES;

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAlpcConnectPort(
	__out PHANDLE PortHandle,
	__in PUNICODE_STRING PortName,
	__in POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt PALPC_PORT_ATTRIBUTES PortAttributes,
	__in ULONG Flags,
	__in_opt PSID RequiredServerSid,
	__inout PPORT_MESSAGE ConnectionMessage,
	__inout_opt PULONG BufferLength,
	__inout_opt PALPC_MESSAGE_ATTRIBUTES OutMessageAttributes,
	__inout_opt PALPC_MESSAGE_ATTRIBUTES InMessageAttributes,
	__in_opt PLARGE_INTEGER Timeout
);

NTSYSCALLAPI
NTSTATUS
NTAPI
NtAlpcSendWaitReceivePort(
	__in HANDLE PortHandle,
	__in ULONG Flags,
	__in_opt PPORT_MESSAGE SendMessage_,
	__in_opt PALPC_MESSAGE_ATTRIBUTES SendMessageAttributes,
	__inout_opt PPORT_MESSAGE ReceiveMessage,
	__inout_opt PULONG BufferLength,
	__inout_opt PALPC_MESSAGE_ATTRIBUTES ReceiveMessageAttributes,
	__in_opt PLARGE_INTEGER Timeout
);