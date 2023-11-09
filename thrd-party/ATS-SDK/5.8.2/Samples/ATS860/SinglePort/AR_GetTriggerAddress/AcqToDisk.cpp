//---------------------------------------------------------------------------
//
// Copyright (c) 2008-2010 AlazarTech, Inc.
// 
// AlazarTech, Inc. licenses this software under specific terms and
// conditions. Use of any of the software or derviatives thereof in any
// product without an AlazarTech digitizer board is strictly prohibited. 
// 
// AlazarTech, Inc. provides this software AS IS, WITHOUT ANY WARRANTY,
// EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION, ANY WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. AlazarTech makes no 
// guarantee or representations regarding the use of, or the results of the 
// use of, the software and documentation in terms of correctness, accuracy,
// reliability, currentness, or otherwise; and you rely on the software,
// documentation and results solely at your own risk.
// 
// IN NO EVENT SHALL ALAZARTECH BE LIABLE FOR ANY LOSS OF USE, LOSS OF 
// BUSINESS, LOSS OF PROFITS, INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL 
// DAMAGES OF ANY KIND. IN NO EVENT SHALL ALAZARTECH'S TOTAL LIABILITY EXCEED
// THE SUM PAID TO ALAZARTECH FOR THE PRODUCT LICENSED HEREUNDER.
// 
//---------------------------------------------------------------------------

// AcqToDisk.cpp :
//
// This program demonstrates how to configure an ATS860 to acquire to 
// on-board memory, and use AlazarRead to transfer records from on-board memory
// to an application buffer.
//

#include <stdio.h>
#include <conio.h>
#include "AlazarError.h"
#include "AlazarApi.h"
#include "AlazarCmd.h"

// Global variables

double SamplesPerSec = 0.;

// Function prototypes

BOOL ConfigureBoard(HANDLE boardHandle);
BOOL AcquireData(HANDLE boardHandle);

//----------------------------------------------------------------------------
//
// Function    :  main
//
// Description :  Program entry point
//
//----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	// TODO: Select a board

	U32 systemId = 1;
	U32 boardId = 1;
	 
	// Get a handle to the board

	HANDLE boardHandle = AlazarGetBoardBySystemID(systemId, boardId);
	if (boardHandle == NULL)
	{
		printf("Error: Unable to open board system ID %u board ID %u\n", systemId, boardId);
		return 1;
	}

	// Configure the board's sample rate, input, and trigger settings

	if (!ConfigureBoard(boardHandle))
	{
		printf("Error: Board configuration failed\n");
		return 1;
	}

	// Make an acquisition, optionally saving sample data to a file

	if (!AcquireData(boardHandle))
	{
		printf("Error: Acquisition failed\n");
		return 1;
	}

	return 0;
}

//----------------------------------------------------------------------------
//
// Function    :  ConfigureBoard
//
// Description :  Configure sample rate, input, and trigger settings
//
//----------------------------------------------------------------------------

BOOL ConfigureBoard(HANDLE boardHandle)
{
	RETURN_CODE retCode;

	// TODO: Specify the sample rate (see sample rate id below)

	SamplesPerSec = 10.e6;

	// TODO: Select clock parameters as required to generate this sample rate.
	//
	// For example: if samplesPerSec is 100.e6 (100 MS/s), then:
	// - select clock source INTERNAL_CLOCK and sample rate SAMPLE_RATE_100MSPS
	// - select clock source FAST_EXTERNAL_CLOCK, sample rate SAMPLE_RATE_USER_DEF,
	//   and connect a 100 MHz signalto the EXT CLK BNC connector.

	retCode = 
		AlazarSetCaptureClock(
			boardHandle,			// HANDLE -- board handle
			INTERNAL_CLOCK,			// U32 -- clock source id
			SAMPLE_RATE_10MSPS,		// U32 -- sample rate id
			CLOCK_EDGE_RISING,		// U32 -- clock edge id
			0						// U32 -- clock decimation 
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetCaptureClock failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select CHA input parameters as required
	
	retCode = 
		AlazarInputControl(
			boardHandle,			// HANDLE -- board handle
			CHANNEL_A,				// U8 -- input channel 
			DC_COUPLING,			// U32 -- input coupling id
			INPUT_RANGE_PM_800_MV,	// U32 -- input range id
			IMPEDANCE_50_OHM		// U32 -- input impedance id
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarInputControl failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select CHA bandwidth limit as required
	
	retCode = 
		AlazarSetBWLimit(
			boardHandle,			// HANDLE -- board handle
			CHANNEL_A,				// U8 -- channel identifier
			0						// U32 -- 0 = disable, 1 = enable
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetBWLimit failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select CHB input parameters as required
	
	retCode = 
		AlazarInputControl(
			boardHandle,			// HANDLE -- board handle
			CHANNEL_B,				// U8 -- channel identifier
			DC_COUPLING,			// U32 -- input coupling id
			INPUT_RANGE_PM_800_MV,	// U32 -- input range id
			IMPEDANCE_50_OHM		// U32 -- input impedance id
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarInputControl failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select CHB bandwidth limit as required
	
	retCode = 
		AlazarSetBWLimit(
			boardHandle,			// HANDLE -- board handle
			CHANNEL_B,				// U8 -- channel identifier
			0						// U32 -- 0 = disable, 1 = enable
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetBWLimit failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select trigger inputs and levels as required

	retCode = 
		AlazarSetTriggerOperation(
			boardHandle,			// HANDLE -- board handle
			TRIG_ENGINE_OP_J,		// U32 -- trigger operation 
			TRIG_ENGINE_J,			// U32 -- trigger engine id
			TRIG_CHAN_A,			// U32 -- trigger source id
			TRIGGER_SLOPE_POSITIVE,	// U32 -- trigger slope id
			128,					// U32 -- trigger level from 0 (-range) to 255 (+range)
			TRIG_ENGINE_K,			// U32 -- trigger engine id
			TRIG_DISABLE,			// U32 -- trigger source id for engine K
			TRIGGER_SLOPE_POSITIVE,	// U32 -- trigger slope id
			128						// U32 -- trigger level from 0 (-range) to 255 (+range)
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetTriggerOperation failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Select external trigger parameters as required

	retCode =
		AlazarSetExternalTrigger( 
			boardHandle,			// HANDLE -- board handle
			DC_COUPLING,			// U32 -- external trigger coupling id
			ETR_5V					// U32 -- external trigger range id
			);

	// TODO: Set trigger delay as required. 
	
	double triggerDelay_sec = 0.;
	U32 triggerDelay_samples = (U32) (triggerDelay_sec * SamplesPerSec + 0.5);
	retCode = AlazarSetTriggerDelay(boardHandle, triggerDelay_samples);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetTriggerDelay failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Set trigger timeout as required. 

	// NOTE:
	// The board will wait for a for this amount of time for a trigger event. 
	// If a trigger event does not arrive, then the board will automatically 
	// trigger. Set the trigger timeout value to 0 to force the board to wait 
	// forever for a trigger event.
	//
	// IMPORTANT: 
	// The trigger timeout value should be set to zero after appropriate 
	// trigger parameters have been determined, otherwise the 
	// board may trigger if the timeout interval expires before a 
	// hardware trigger event arrives.

	double triggerTimeout_sec = 0.;
	U32 triggerTimeout_clocks = (U32) (triggerTimeout_sec / 10.e-6 + 0.5);

	retCode = 
		AlazarSetTriggerTimeOut(
			boardHandle,			// HANDLE -- board handle
			triggerTimeout_clocks	// U32 -- timeout_sec / 10.e-6 (0 == wait forever)
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetTriggerTimeOut failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// TODO: Configure AUX I/O connector as required

	retCode = 
		AlazarConfigureAuxIO(
			boardHandle,			// HANDLE -- board handle
			AUX_OUT_TRIGGER,		// U32 -- mode
			0						// U32 -- parameter
			);	
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarConfigureAuxIO failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
//
// Function    :  AcquireData
//
// Description :  Make acquisition and optionally save samples to a file
//
//----------------------------------------------------------------------------

BOOL AcquireData(HANDLE boardHandle)
{
	// TODO: Select the number of pre-trigger samples per record 

	U32 preTriggerSamples = 4096;

	// TODO: Select the number of post-trigger samples per record 

	U32 postTriggerSamples = 4096;

	// TODO: Select the number of records in the acquisition

	U32 recordsPerCapture = 1000;

	// TODO: Select the amount of time, in seconds, to wait for the 
	//       acquisiton to complete to on-board memory.

	DWORD timeout_ms = 10000;

	// TODO: Select if you wish to save the sample data to a file

	BOOL saveData = TRUE;

	// TODO: Select if you wish to save the record timestamps to a file

	BOOL saveTimestamps = TRUE;

	// TODO: Select which channels read from on-board memory (A, B, or both)

	U32 channelMask = CHANNEL_A | CHANNEL_B; 

	// Calculate the number of enabled channels from the channel mask 

	int channelCount = 0;

	switch (channelMask)
	{
	case CHANNEL_A:
	case CHANNEL_B:
		channelCount = 1;
		break;
	case CHANNEL_A | CHANNEL_B:
		channelCount = 2;
		break;
	default:
		printf("Error: Invalid channel mask %08X\n", channelMask);
		return FALSE;
	}

	// Get the sample and memory size

	U8 bitsPerSample;
	U32 maxSamplesPerChannel;
	RETURN_CODE retCode = AlazarGetChannelInfo(boardHandle, &maxSamplesPerChannel, &bitsPerSample);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarGetChannelInfo failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// Get the number of samples per timestamp count if required

	long samplesPerTimestampCount = 0;

	if (saveTimestamps)
	{
		retCode = 
			AlazarGetParameter(
				boardHandle,						// HANDLE -- board handle
				0,									// U8 -- channel
				GET_SAMPLES_PER_TIMESTAMP_CLOCK,	// U32 -- parameter identifier
				&samplesPerTimestampCount			// long* -- parameter value
				);
		if (retCode != ApiSuccess)
		{
			printf("Error: AlazarGetParameter failed -- %s\n", AlazarErrorToText(retCode));
			return FALSE;
		}
	}

	// Calculate the size of each record in bytes

	U32 bytesPerSample = (bitsPerSample + 7) / 8;
	U32 samplesPerRecord = preTriggerSamples + postTriggerSamples;
	U32	bytesPerRecord = bytesPerSample * samplesPerRecord;

	// Calculate the size of a record buffer in bytes
	// Note that the buffer must be at least 16 samples larger than the transfer size

	U32 bytesPerBuffer = bytesPerSample * (samplesPerRecord + 16); 

	// Configure the number of samples per record

	retCode = 
		AlazarSetRecordSize (
			boardHandle,			// HANDLE -- board handle
			preTriggerSamples,		// U32 -- pre-trigger samples
			postTriggerSamples		// U32 -- post-trigger samples
			);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetRecordSize failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// Configure the number of records in the acquisition

	retCode = AlazarSetRecordCount(boardHandle, recordsPerCapture);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarSetRecordCount failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// Arm the board to wait for a trigger event to begin the acquisition 

	retCode = AlazarStartCapture(boardHandle);
	if (retCode != ApiSuccess)
	{
		printf("Error: AlazarStartCapture failed -- %s\n", AlazarErrorToText(retCode));
		return FALSE;
	}

	// Wait for the board to capture all records to on-board memory

	printf("Capturing %d records ... press any key to abort\n", recordsPerCapture);

	DWORD startTickCount = GetTickCount();
	DWORD timeoutTickCount = startTickCount + timeout_ms;
	BOOL success = TRUE;

	while (AlazarBusy(boardHandle) && (success == TRUE))
	{
		if (GetTickCount() > timeoutTickCount)
		{
			printf("Error: Capture timeout after %lu ms\n", timeout_ms);
			success = FALSE;
		}
		else if (_kbhit())
		{
			printf("Error: Acquisition aborted\n");
			success = FALSE;
		}
		else
		{
			Sleep(10);
		}
	}

	if (!success)
	{
		// Abort the acquisition
		retCode = AlazarAbortCapture(boardHandle);
		if (retCode != ApiSuccess)
		{
			printf("Error: AlazarAbortCapture failed -- %s\n", AlazarErrorToText(retCode));
		}

		return FALSE;
	}

	// The board captured all records to on-board memory

	double captureTime_sec = (GetTickCount() - startTickCount) / 1000.;
	double recordsPerSec;
	if (captureTime_sec > 0.)
		recordsPerSec = recordsPerCapture / captureTime_sec;
	else
		recordsPerSec = 0.;
	printf("Captured %u records in %g sec (%.4g records / sec)\n", recordsPerCapture, captureTime_sec, recordsPerSec);

	// Allocate a buffer to store one record

	U8* buffer = (U8 *) malloc(bytesPerBuffer); 
	if (buffer == NULL)
	{
		printf("Error: alloc %d bytes failed\n", bytesPerBuffer);
		return FALSE;
	}

	// Create a data file if required

	FILE *fpData = NULL;

	if (saveData)
	{
		fpData = fopen("data.bin", "wb");
		if (fpData == NULL)
		{
			printf("Error: Unable to create data file -- %u\n", GetLastError());
			success = FALSE;
		}
	}

	// Create a timestamp file if required

	FILE *fpTimestamp = NULL;

	if (saveTimestamps)
	{
		fpTimestamp = fopen("timestamp.txt", "w");
		if (fpTimestamp == NULL)
		{
			printf("Error: Unable to create timestamp file -- %u\n", GetLastError());
			success = FALSE;
		}
		else
		{
			// Write a header to the timestamp file 

			if (fprintf(fpTimestamp, "Record \tTimestamp (sec)\n") < 0)
			{
				printf("Error: Write timestamp header failed -- %u\n", GetLastError());
				success = FALSE;
			}
		}
	}
	
	// Transfer the records from on-board memory to our buffer

	printf("Transferring %u records ... press any key to cancel\n", recordsPerCapture);
	startTickCount = GetTickCount();
	INT64 bytesTransferred = 0;

	U32 record;
	for (record = 0; (record < recordsPerCapture) && (success == TRUE); record++)
	{
		if (saveTimestamps)
		{
			// Get record timestamp

			U32 triggerAddress;
			U32 timestampHigh;
			U32 timestampLow;

			retCode = 
				AlazarGetTriggerAddress (
					boardHandle,			// HANDLE -- board handle
					record + 1,				// U32 -- record number (1-indexed)
					&triggerAddress,		// U32* -- trigger address
					&timestampHigh,			// U32* -- timestamp high part
					&timestampLow			// U32* -- timestamp low part
					);
			if (retCode != ApiSuccess)
			{
				printf("Error: AlazarGetTriggerAddress %d failed -- %s\n", record, AlazarErrorToText(retCode));
				success = FALSE;
			}
			else
			{
				// Convert the record timestamp from counts to seconds

				INT64 timeStamp_cnt;
				timeStamp_cnt = ((INT64) timestampHigh) << 8;
				timeStamp_cnt |= timestampLow & 0x0ff;
			
				double timeStamp_sec = (double) samplesPerTimestampCount * timeStamp_cnt / SamplesPerSec;

				// Write the record timestamp to the timestamp file

				if (fprintf(fpTimestamp, "%d \t%.9lf\n", record + 1, timeStamp_sec) < 0)
				{
					printf("Error: Write record timestamp %d failed -- %u\n", record, GetLastError());
					success = FALSE;
				}
			}
		}

		for (int channel = 0; (channel < channelCount) && (success == TRUE); channel++)
		{
			// Find the current channel Id

			char channelId;
			if (channelCount == 1)
			{
				if (channelMask & CHANNEL_A)
					channelId = CHANNEL_A;
				else
					channelId = CHANNEL_B;
			}
			else
			{
				if (channel == 0)
					channelId = CHANNEL_A;
				else
					channelId = CHANNEL_B;
			}

			// Transfer one full record from on-board memory to our buffer

			retCode = (RETURN_CODE)
				AlazarRead (
					boardHandle,				// HANDLE -- board handle
					channelId,					// U32 -- channel Id
					buffer,						// void* -- buffer
					(int) bytesPerSample,		// int -- bytes per sample
					(long) record + 1,			// long -- record (1 indexed)
					-((long)preTriggerSamples),	// long -- offset from trigger in samples
					samplesPerRecord			// U32 -- samples to transfer
					);
			if (retCode != ApiSuccess)
			{
				printf("Error: AlazarRead record %u failed -- %s\n", record, AlazarErrorToText(retCode));
				success = FALSE;
			}
			else
			{
				bytesTransferred += bytesPerRecord;

				// TODO: Process record here.
				// 
				// Samples values are arranged contiguously in the buffer, with
				// 8-bit sample codes in each 8-bit sample value. 
				//
				// Sample codes are unsigned by default so that:
				// - a sample code of 0x00 represents a negative full scale input signal;
				// - a sample code of 0x80 represents a ~0V signal;
				// - a sample code of 0xFF represents a positive full scale input signal.

				if (saveData)
				{
					size_t samplesWritten = fwrite(buffer, bytesPerSample, samplesPerRecord, fpData);
					if (samplesWritten != samplesPerRecord)
					{
						printf("Error: Write record %u failed -- %u\n", record, GetLastError());
						success = FALSE;
					}
				}
			}
		}

		// If a key was pressed, then stop processing records.

		if (_kbhit())
		{
			printf("Error: Transfer aborted ...\n");
			success = FALSE;
		}
	}

	// Display results

	double transferTime_sec = (GetTickCount() - startTickCount) / 1000.;

	double bytesPerSec;
	if (transferTime_sec > 0.)
		bytesPerSec = bytesTransferred / transferTime_sec;
	else
		bytesPerSec = 0.;

	printf("Transferred %I64d bytes in %g sec (%.4g bytes per sec)\n", bytesTransferred, transferTime_sec, bytesPerSec);

	// Free the DMA buffer 
	
	free(buffer);

	// Close the data file

	if (fpData != NULL)
		fclose(fpData);

	// Close the timestamp file

	if (fpTimestamp != NULL)
		fclose(fpTimestamp);

	return success;
}
