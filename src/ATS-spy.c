#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

#include "../include/AlazarApi.h"
#include "../include/AlazarDSP.h"


#define MAX_REPEAT 4

static RETURN_CODE (* old_AlazarGetOEMFPGAName) (int opcodeID, char *FullPath, unsigned long *error);
static RETURN_CODE (* old_AlazarOEMSetWorkingDirectory) (char *wDir, unsigned long *error);
static RETURN_CODE (* old_AlazarOEMGetWorkingDirectory) (char *wDir, unsigned long *error);
static RETURN_CODE (* old_AlazarParseFPGAName) (const char *FullName, char *Name, U32 *Type, U32 *MemSize, U32 *MajVer, U32 *MinVer, U32 *MajRev, U32 *MinRev, U32 *error);
static RETURN_CODE (* old_AlazarDownLoadFPGA) (HANDLE handle, char *FileName, U32 *RetValue);
static RETURN_CODE (* old_AlazarOEMDownLoadFPGA) (HANDLE handle, char *FileName, U32 *RetValue);
static RETURN_CODE (* old_AlazarReadWriteTest) (HANDLE handle, U32 *Buffer, U32 SizeToWrite, U32 SizeToRead);
static RETURN_CODE (* old_AlazarMemoryTest) (HANDLE handle, U32 *errors);
static RETURN_CODE (* old_AlazarTriggeredFlag) (HANDLE handle, int *TriggeredFlag);
static RETURN_CODE (* old_AlazarGetSDKVersion) (U8 *major, U8 *minor, U8 *revision);
static RETURN_CODE (* old_AlazarGetDriverVersion) (U8 *major, U8 *minor, U8 *revision);
static RETURN_CODE (* old_AlazarGetBoardRevision) (HANDLE handle, U8 *major, U8 *minor);
static U32 (* old_AlazarBoardsFound) (void);
static HANDLE (* old_AlazarOpen) (char *boardName);
static void (* old_AlazarClose) (HANDLE handle);
static U32 (* old_AlazarGetBoardKind) (HANDLE handle);
static RETURN_CODE (* old_AlazarGetCPLDVersion) (HANDLE handle, U8 *major, U8 *minor);
static RETURN_CODE (* old_AlazarAutoCalibrate) (HANDLE handle);
static RETURN_CODE (* old_AlazarGetChannelInfo) (HANDLE handle, U32 *memorySize, U8 *bitsPerSample);
static RETURN_CODE (* old_AlazarGetChannelInfoEx) (HANDLE handle, S64 *memorySize, U8 *bitsPerSample);
static RETURN_CODE (* old_AlazarInputControl) (HANDLE handle, U8 channel, U32 coupling, U32 inputRange, U32 impedance);
static RETURN_CODE (* old_AlazarInputControlEx) (HANDLE handle, U32 channel, U32 couplingId, U32 rangeId, U32 impedenceId);
static RETURN_CODE (* old_AlazarSetPosition) (HANDLE handle, U8 Channel, int PMPercent, U32 InputRange);
static RETURN_CODE (* old_AlazarSetExternalTrigger) (HANDLE handle, U32 couplingId, U32 rangeId);
static RETURN_CODE (* old_AlazarSetTriggerDelay) (HANDLE handle, U32 Delay);
static RETURN_CODE (* old_AlazarSetTriggerTimeOut) (HANDLE handle, U32 timeout_ticks);
static U32 (* old_AlazarTriggerTimedOut) (HANDLE h);
static RETURN_CODE (* old_AlazarSetTriggerOperation) (HANDLE handle, U32 TriggerOperation, U32 TriggerEngine1, U32 Source1, U32 Slope1, U32 Level1, U32 TriggerEngine2, U32 Source2, U32 Slope2, U32 Level2);
static RETURN_CODE (* old_AlazarGetTriggerTimestamp) (HANDLE handle, U32 Record, U64 *Timestamp_samples);
static RETURN_CODE (* old_AlazarSetTriggerOperationForScanning) (HANDLE handle, U32 slopeId, U32 level, U32 options);
static RETURN_CODE (* old_AlazarAbortCapture) (HANDLE handle);
static RETURN_CODE (* old_AlazarForceTrigger) (HANDLE handle);
static RETURN_CODE (* old_AlazarForceTriggerEnable) (HANDLE handle);
static RETURN_CODE (* old_AlazarStartCapture) (HANDLE handle);
static RETURN_CODE (* old_AlazarCaptureMode) (HANDLE handle, U32 Mode);
static RETURN_CODE (* old_AlazarStreamCapture) (HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error);
static RETURN_CODE (* old_AlazarHyperDisp) (HANDLE handle, void *buffer, U32 bufferSize, U8 *viewBuffer, U32 viewBufferSize, U32 numOfPixels, U32 option, U32 channelSelect, U32 record, long transferOffset, U32 *error);
static RETURN_CODE (* old_AlazarFastPRRCapture) (HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error);
static U32 (* old_AlazarBusy) (HANDLE handle);
static U32 (* old_AlazarTriggered) (HANDLE handle);
static U32 (* old_AlazarGetStatus) (HANDLE handle);
static U32 (* old_AlazarDetectMultipleRecord) (HANDLE handle);
static RETURN_CODE (* old_AlazarSetRecordCount) (HANDLE handle, U32 Count);
static RETURN_CODE (* old_AlazarSetRecordSize) (HANDLE handle, U32 preTriggerSamples, U32 postTriggerSamples);
static RETURN_CODE (* old_AlazarSetCaptureClock) (HANDLE handle, U32 source, U32 sampleRateIdOrValue, U32 edgeId, U32 decimation);
static RETURN_CODE (* old_AlazarSetExternalClockLevel) (HANDLE handle, float level_percent);
static RETURN_CODE (* old_AlazarSetClockSwitchOver) (HANDLE handleBoard, U32 uMode, U32 uDummyClockOnTime_ns, U32 uReserved);
static U32 (* old_AlazarRead) (HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, long transferOffset, U32 transferLength);
static U32 (* old_AlazarReadEx) (HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, INT64 transferOffset, U32 transferLength);
static RETURN_CODE (* old_AlazarSetParameter) (HANDLE handle, U8 channel, U32 parameter, long value);
static RETURN_CODE (* old_AlazarSetParameterUL) (HANDLE handle, U8 channel, U32 parameter, U32 value);
static RETURN_CODE (* old_AlazarSetParameterLL) (HANDLE handle, U8 channel, U32 parameter, S64 value);
static RETURN_CODE (* old_AlazarGetParameter) (HANDLE handle, U8 channel, U32 parameter, long *retValue);
static RETURN_CODE (* old_AlazarGetParameterUL) (HANDLE handle, U8 channel, U32 parameter, U32 *retValue);
static RETURN_CODE (* old_AlazarGetParameterLL) (HANDLE handle, U8 channel, U32 parameter, S64 *retValue);
static HANDLE (* old_AlazarGetSystemHandle) (U32 systemId);
static U32 (* old_AlazarNumOfSystems) (void);
static U32 (* old_AlazarBoardsInSystemBySystemID) (U32 systemId);
static U32 (* old_AlazarBoardsInSystemByHandle) (HANDLE systemHandle);
static HANDLE (* old_AlazarGetBoardBySystemID) (U32 systemId, U32 boardId);
static HANDLE (* old_AlazarGetBoardBySystemHandle) (HANDLE systemHandle, U32 boardId);
static RETURN_CODE (* old_AlazarSetLED) (HANDLE handle, U32 state);
static RETURN_CODE (* old_AlazarQueryCapability) (HANDLE handle, U32 capability, U32 reserved, U32 *retValue);
static RETURN_CODE (* old_AlazarQueryCapabilityLL) (HANDLE handle, U32 capability, U32 reserved, S64 *retValue);
static U32 (* old_AlazarMaxSglTransfer) (ALAZAR_BOARDTYPES bt);
static RETURN_CODE (* old_AlazarGetMaxRecordsCapable) (HANDLE handle, U32 samplesPerRecord, U32 *maxRecordsPerCapture);
static U32 (* old_AlazarGetWhoTriggeredBySystemHandle) (HANDLE systemHandle, U32 boardId, U32 recordNumber);
static U32 (* old_AlazarGetWhoTriggeredBySystemID) (U32 systemId, U32 boardId, U32 recordNumber);
static RETURN_CODE (* old_AlazarSetBWLimit) (HANDLE handle, U32 channel, U32 enable);
static RETURN_CODE (* old_AlazarSleepDevice) (HANDLE handle, U32 sleepState);
static RETURN_CODE (* old_AlazarBeforeAsyncRead) (HANDLE handle, U32 channelSelect, long transferOffset, U32 transferLength, U32 recordsPerBuffer, U32 recordsPerAcquisition, U32 flags);
static RETURN_CODE (* old_AlazarAbortAsyncRead) (HANDLE handle);
static RETURN_CODE (* old_AlazarPostAsyncBuffer) (HANDLE handle, void *buffer, U32 bufferLength_bytes);
static RETURN_CODE (* old_AlazarWaitAsyncBufferComplete) (HANDLE handle, void *buffer, U32 timeout_ms);
static RETURN_CODE (* old_AlazarWaitNextAsyncBufferComplete) (HANDLE handle, void *buffer, U32 bytesToCopy, U32 timeout_ms);
static RETURN_CODE (* old_AlazarResetTimeStamp) (HANDLE handle, U32 option);
static RETURN_CODE (* old_AlazarReadRegister) (HANDLE handle, U32 offset, U32 *retVal, U32 pswrd);
static RETURN_CODE (* old_AlazarWriteRegister) (HANDLE handle, U32 offset, U32 Val,U32 pswrd);
static RETURN_CODE (* old_ATS9462FlashSectorPageRead) (HANDLE handle, U32 address, U16 *PageBuff);
static RETURN_CODE (* old_ATS9462PageWriteToFlash) (HANDLE handle, U32 address, U16 *PageBuff);
static RETURN_CODE (* old_ATS9462FlashSectorErase) (HANDLE handle, int sectorNum);
static RETURN_CODE (* old_ATS9462FlashChipErase) (HANDLE h);
static RETURN_CODE (* old_SetControlCommand) (HANDLE handle, int cmd);
static RETURN_CODE (* old_AlazarDACSetting) (HANDLE handle, U32 SetGet, U32 OriginalOrModified, U8 Channel, U32 DACNAME, U32 Coupling, U32 InputRange, U32 Impedance, U32 *getVal, U32 setVal, U32 *error);
static RETURN_CODE (* old_AlazarWrite) (HANDLE handle, void *buffer, long bufLen, int DmaChannel, U32 firstPoint, U32 startAddress, U32 endAddress, BOOL waitTillEnd, U32 *error);
static RETURN_CODE (* old_AlazarConfigureAuxIO) (HANDLE handle, U32 mode, U32 parameter);
static RETURN_CODE (* old_AlazarConfigureSampleSkipping) (HANDLE handle, U32 mode, U32 sampleClocksPerRecord, U16 *sampleSkipBitmap);
static RETURN_CODE (* old_AlazarCoprocessorRegisterRead) (HANDLE handle, U32 offset, U32 *value);
static RETURN_CODE (* old_AlazarCoprocessorRegisterWrite) (HANDLE handle, U32 offset, U32 value);
static RETURN_CODE (* old_AlazarCoprocessorDownloadA) (HANDLE handle, char *fileName, U32 options);
static RETURN_CODE (* old_AlazarCoprocessorDownloadBuffer) (HANDLE handle, U8 *pbBuffer, U32 uBytesToWrite, U32 options);
static RETURN_CODE (* old_AlazarConfigureRecordAverage) (HANDLE handle, U32 mode, U32 samplesPerRecord, U32 recordsPerAverage, U32 options);
static U8 * (* old_AlazarAllocBufferU8) (HANDLE handle, U32 sampleCount);
static RETURN_CODE (* old_AlazarFreeBufferU8) (HANDLE handle, U8 *buffer);
static U16 * (* old_AlazarAllocBufferU16) (HANDLE handle, U32 sampleCount);
static RETURN_CODE (* old_AlazarFreeBufferU16) (HANDLE handle, U16 *buffer);
static U8 * (* old_AlazarAllocBufferU8Ex) (HANDLE handle, U64 sampleCount);
static RETURN_CODE (* old_AlazarFreeBufferU8Ex) (HANDLE handle, U8 *buffer);
static U16 * (* old_AlazarAllocBufferU16Ex) (HANDLE handle, U64 sampleCount);
static RETURN_CODE (* old_AlazarFreeBufferU16Ex) (HANDLE handle, U16 *buffer);
static RETURN_CODE (* old_AlazarConfigureLSB) (HANDLE handle, U32 valueLsb0, U32 valueLsb1);
static RETURN_CODE (* old_AlazarExtractFFTNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
static RETURN_CODE (* old_AlazarExtractTimeDomainNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
static RETURN_CODE (* old_AlazarExtractNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
static RETURN_CODE (* old_AlazarDisableDSP) (HANDLE boardHandle);
static RETURN_CODE (* old_AlazarOCTIgnoreBadClock) (HANDLE handle, U32 enable, double goodClockDuration_seconds, double badClockDuration_seconds, double *triggerCycleTime_seconds, double *triggerPulseWidth_seconds);
static RETURN_CODE (* old_AlazarSetADCBackgroundCompensation) (HANDLE handle, BOOL active);
static RETURN_CODE (* old_AlazarDSPGetModules) (HANDLE boardHandle, U32 numEntries, dsp_module_handle *modules, U32 *numModules);
static RETURN_CODE (* old_AlazarDSPGetInfo) (dsp_module_handle dspHandle, U32 *dspModuleId, U16 *versionMajor, U16 *versionMinor, U32 *maxLength, U32 *reserved0, U32 *reserved1);
static RETURN_CODE (* old_AlazarDSPGenerateWindowFunction) (U32 windowType, float *window, U32 windowLength_samples, U32 paddingLength_samples);
static RETURN_CODE (* old_AlazarFFTGetMaxTriggerRepeatRate) (dsp_module_handle dspHandle, U32 fftSize, double *maxTriggerRepeatRate);
static RETURN_CODE (* old_AlazarFFTBackgroundSubtractionSetRecordS16) (dsp_module_handle dspHandle, const S16 *record, U32 size_samples);
static RETURN_CODE (* old_AlazarFFTBackgroundSubtractionGetRecordS16) (dsp_module_handle dspHandle, S16 *backgroundRecord, U32 size_samples);
static RETURN_CODE (* old_AlazarFFTBackgroundSubtractionSetEnabled) (dsp_module_handle dspHandle, BOOL enabled);
static RETURN_CODE (* old_AlazarFFTSetWindowFunction) (dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray);
static RETURN_CODE (* old_AlazarFFTGetWindowFunction) (dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray);
static RETURN_CODE (* old_AlazarFFTVerificationMode) (dsp_module_handle dspHandle, BOOL enable, S16 *realArray, S16 *imagArray, size_t recordLength_samples);
static RETURN_CODE (* old_AlazarFFTSetup) (dsp_module_handle dspHandle, U16 inputChannelMask, U32 recordLength_samples, U32 fftLength_samples, U32 outputFormat, U32 footer, U32 reserved, U32 *bytesPerOutputRecord);
static RETURN_CODE (* old_AlazarFFTSetScalingAndSlicing) (dsp_module_handle dspHandle, U8 slice_pos, float loge_ampl_mult);
static RETURN_CODE (* old_AlazarDSPGetBuffer) (HANDLE boardHandle, void *buffer, U32 timeout_ms);
static RETURN_CODE (* old_AlazarDSPGetNextBuffer) (HANDLE boardHandle, void *buffer, U32 bytesToCopy, U32 timeout_ms);
static RETURN_CODE (* old_AlazarDSPGetParameterU32) (dsp_module_handle dspHandle, U32 parameter, U32 *_result);
static RETURN_CODE (* old_AlazarDSPSetParameterU32) (dsp_module_handle dspHandle, U32 parameter, U32 value);
static RETURN_CODE (* old_AlazarDSPGetParameterS32) (dsp_module_handle dspHandle, U32 parameter, S32 *_result);
static RETURN_CODE (* old_AlazarDSPSetParameterS32) (dsp_module_handle dspHandle, U32 parameter, S32 value);
static RETURN_CODE (* old_AlazarDSPGetParameterFloat) (dsp_module_handle dspHandle, U32 parameter, float *_result);
static RETURN_CODE (* old_AlazarDSPSetParameterFloat) (dsp_module_handle dspHandle, U32 parameter, float value);
static RETURN_CODE (* old_AlazarDSPConfigureSelfTrigger) (dsp_module_handle dspHandle, BOOL enable, U32 counter);
static RETURN_CODE (* old_AlazarDSPConfigureSampleSkipping) (dsp_module_handle dspHandle, BOOL independentMode, U32 count, U16 *vector);
static RETURN_CODE (* old_AlazarDSPAbortCapture) (HANDLE boardHandle);

static int last_func;
static int repeat_count;

static void out(const char *str) {
	write(2, str, strlen(str));
}

RETURN_CODE AlazarGetOEMFPGAName(int opcodeID, char *FullPath, unsigned long *error)
{
	old_AlazarGetOEMFPGAName = dlsym(RTLD_NEXT, "AlazarGetOEMFPGAName");
	if (last_func!=1) repeat_count=0;

	last_func = 1; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetOEMFPGAName()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetOEMFPGAName()\n");
			fprintf(stderr, "\tint opcodeID: %d\n", opcodeID);
			fprintf(stderr, "\tchar * FullPath: %s\n", FullPath);
			fprintf(stderr, "\tunsigned long * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarGetOEMFPGAName(opcodeID, FullPath, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarOEMSetWorkingDirectory(char *wDir, unsigned long *error)
{
	old_AlazarOEMSetWorkingDirectory = dlsym(RTLD_NEXT, "AlazarOEMSetWorkingDirectory");
	if (last_func!=2) repeat_count=0;

	last_func = 2; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarOEMSetWorkingDirectory()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarOEMSetWorkingDirectory()\n");
			fprintf(stderr, "\tchar * wDir: %s\n", wDir);
			fprintf(stderr, "\tunsigned long * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarOEMSetWorkingDirectory(wDir, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarOEMGetWorkingDirectory(char *wDir, unsigned long *error)
{
	old_AlazarOEMGetWorkingDirectory = dlsym(RTLD_NEXT, "AlazarOEMGetWorkingDirectory");
	if (last_func!=3) repeat_count=0;

	last_func = 3; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarOEMGetWorkingDirectory()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarOEMGetWorkingDirectory()\n");
			fprintf(stderr, "\tchar * wDir: %s\n", wDir);
			fprintf(stderr, "\tunsigned long * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarOEMGetWorkingDirectory(wDir, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarParseFPGAName(const char *FullName, char *Name, U32 *Type, U32 *MemSize, U32 *MajVer, U32 *MinVer, U32 *MajRev, U32 *MinRev, U32 *error)
{
	old_AlazarParseFPGAName = dlsym(RTLD_NEXT, "AlazarParseFPGAName");
	if (last_func!=4) repeat_count=0;

	last_func = 4; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarParseFPGAName()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarParseFPGAName()\n");
			fprintf(stderr, "\tconst char * FullName: %s\n", FullName);
			fprintf(stderr, "\tchar * Name: %s\n", Name);
			fprintf(stderr, "\tU32 * Type: %p\n", Type);
			fprintf(stderr, "\tU32 * MemSize: %p\n", MemSize);
			fprintf(stderr, "\tU32 * MajVer: %p\n", MajVer);
			fprintf(stderr, "\tU32 * MinVer: %p\n", MinVer);
			fprintf(stderr, "\tU32 * MajRev: %p\n", MajRev);
			fprintf(stderr, "\tU32 * MinRev: %p\n", MinRev);
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarParseFPGAName(FullName, Name, Type, MemSize, MajVer, MinVer, MajRev, MinRev, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDownLoadFPGA(HANDLE handle, char *FileName, U32 *RetValue)
{
	old_AlazarDownLoadFPGA = dlsym(RTLD_NEXT, "AlazarDownLoadFPGA");
	if (last_func!=5) repeat_count=0;

	last_func = 5; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDownLoadFPGA()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDownLoadFPGA()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tchar * FileName: %s\n", FileName);
			fprintf(stderr, "\tU32 * RetValue: %p\n", RetValue);
		}
	}
	RETURN_CODE  result = old_AlazarDownLoadFPGA(handle, FileName, RetValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarOEMDownLoadFPGA(HANDLE handle, char *FileName, U32 *RetValue)
{
	old_AlazarOEMDownLoadFPGA = dlsym(RTLD_NEXT, "AlazarOEMDownLoadFPGA");
	if (last_func!=6) repeat_count=0;

	last_func = 6; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarOEMDownLoadFPGA()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarOEMDownLoadFPGA()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tchar * FileName: %s\n", FileName);
			fprintf(stderr, "\tU32 * RetValue: %p\n", RetValue);
		}
	}
	RETURN_CODE  result = old_AlazarOEMDownLoadFPGA(handle, FileName, RetValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarReadWriteTest(HANDLE handle, U32 *Buffer, U32 SizeToWrite, U32 SizeToRead)
{
	old_AlazarReadWriteTest = dlsym(RTLD_NEXT, "AlazarReadWriteTest");
	if (last_func!=7) repeat_count=0;

	last_func = 7; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarReadWriteTest()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarReadWriteTest()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 * Buffer: %p\n", Buffer);
			fprintf(stderr, "\tU32 SizeToWrite: %d (0x%08x)\n", SizeToWrite, SizeToWrite);
			fprintf(stderr, "\tU32 SizeToRead: %d (0x%08x)\n", SizeToRead, SizeToRead);
		}
	}
	RETURN_CODE  result = old_AlazarReadWriteTest(handle, Buffer, SizeToWrite, SizeToRead);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarMemoryTest(HANDLE handle, U32 *errors)
{
	old_AlazarMemoryTest = dlsym(RTLD_NEXT, "AlazarMemoryTest");
	if (last_func!=8) repeat_count=0;

	last_func = 8; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarMemoryTest()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarMemoryTest()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 * errors: %p\n", errors);
		}
	}
	RETURN_CODE  result = old_AlazarMemoryTest(handle, errors);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarTriggeredFlag(HANDLE handle, int *TriggeredFlag)
{
	old_AlazarTriggeredFlag = dlsym(RTLD_NEXT, "AlazarTriggeredFlag");
	if (last_func!=10) repeat_count=0;

	last_func = 10; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarTriggeredFlag()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarTriggeredFlag()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tint * TriggeredFlag: %p\n", TriggeredFlag);
		}
	}
	RETURN_CODE  result = old_AlazarTriggeredFlag(handle, TriggeredFlag);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetSDKVersion(U8 *major, U8 *minor, U8 *revision)
{
	old_AlazarGetSDKVersion = dlsym(RTLD_NEXT, "AlazarGetSDKVersion");
	if (last_func!=11) repeat_count=0;

	last_func = 11; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetSDKVersion()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetSDKVersion()\n");
			fprintf(stderr, "\tU8 * major: %p\n", major);
			fprintf(stderr, "\tU8 * minor: %p\n", minor);
			fprintf(stderr, "\tU8 * revision: %p\n", revision);
		}
	}
	RETURN_CODE  result = old_AlazarGetSDKVersion(major, minor, revision);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetDriverVersion(U8 *major, U8 *minor, U8 *revision)
{
	old_AlazarGetDriverVersion = dlsym(RTLD_NEXT, "AlazarGetDriverVersion");
	if (last_func!=12) repeat_count=0;

	last_func = 12; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetDriverVersion()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetDriverVersion()\n");
			fprintf(stderr, "\tU8 * major: %p\n", major);
			fprintf(stderr, "\tU8 * minor: %p\n", minor);
			fprintf(stderr, "\tU8 * revision: %p\n", revision);
		}
	}
	RETURN_CODE  result = old_AlazarGetDriverVersion(major, minor, revision);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetBoardRevision(HANDLE handle, U8 *major, U8 *minor)
{
	old_AlazarGetBoardRevision = dlsym(RTLD_NEXT, "AlazarGetBoardRevision");
	if (last_func!=13) repeat_count=0;

	last_func = 13; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetBoardRevision()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetBoardRevision()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 * major: %p\n", major);
			fprintf(stderr, "\tU8 * minor: %p\n", minor);
		}
	}
	RETURN_CODE  result = old_AlazarGetBoardRevision(handle, major, minor);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarBoardsFound(void)
{
	old_AlazarBoardsFound = dlsym(RTLD_NEXT, "AlazarBoardsFound");
	if (last_func!=14) repeat_count=0;

	last_func = 14; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarBoardsFound()\n");
		} else {
			fprintf(stderr, "U32 AlazarBoardsFound()\n");
		}
	}
	U32  result = old_AlazarBoardsFound();
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
HANDLE AlazarOpen(char *boardName)
{
	old_AlazarOpen = dlsym(RTLD_NEXT, "AlazarOpen");
	if (last_func!=15) repeat_count=0;

	last_func = 15; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarOpen()\n");
		} else {
			fprintf(stderr, "HANDLE AlazarOpen()\n");
			fprintf(stderr, "\tchar * boardName: %s\n", boardName);
		}
	}
	HANDLE  result = old_AlazarOpen(boardName);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
void AlazarClose(HANDLE handle)
{
	old_AlazarClose = dlsym(RTLD_NEXT, "AlazarClose");
	if (last_func!=16) repeat_count=0;

	last_func = 16; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarClose()\n");
		} else {
			fprintf(stderr, "void AlazarClose()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	old_AlazarClose(handle);
}
U32 AlazarGetBoardKind(HANDLE handle)
{
	old_AlazarGetBoardKind = dlsym(RTLD_NEXT, "AlazarGetBoardKind");
	if (last_func!=17) repeat_count=0;

	last_func = 17; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetBoardKind()\n");
		} else {
			fprintf(stderr, "U32 AlazarGetBoardKind()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	U32  result = old_AlazarGetBoardKind(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarGetCPLDVersion(HANDLE handle, U8 *major, U8 *minor)
{
	old_AlazarGetCPLDVersion = dlsym(RTLD_NEXT, "AlazarGetCPLDVersion");
	if (last_func!=18) repeat_count=0;

	last_func = 18; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetCPLDVersion()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetCPLDVersion()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 * major: %p\n", major);
			fprintf(stderr, "\tU8 * minor: %p\n", minor);
		}
	}
	RETURN_CODE  result = old_AlazarGetCPLDVersion(handle, major, minor);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarAutoCalibrate(HANDLE handle)
{
	old_AlazarAutoCalibrate = dlsym(RTLD_NEXT, "AlazarAutoCalibrate");
	if (last_func!=19) repeat_count=0;

	last_func = 19; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAutoCalibrate()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarAutoCalibrate()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarAutoCalibrate(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetChannelInfo(HANDLE handle, U32 *memorySize, U8 *bitsPerSample)
{
	old_AlazarGetChannelInfo = dlsym(RTLD_NEXT, "AlazarGetChannelInfo");
	if (last_func!=20) repeat_count=0;

	last_func = 20; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetChannelInfo()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetChannelInfo()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 * memorySize: %p\n", memorySize);
			fprintf(stderr, "\tU8 * bitsPerSample: %p\n", bitsPerSample);
		}
	}
	RETURN_CODE  result = old_AlazarGetChannelInfo(handle, memorySize, bitsPerSample);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
							fprintf(stderr, "\t    memorySize: %d\n", *memorySize);
							fprintf(stderr, "\t    bitsPerSample: %d\n", *bitsPerSample);
						
	}
	return result;
}
RETURN_CODE AlazarGetChannelInfoEx(HANDLE handle, S64 *memorySize, U8 *bitsPerSample)
{
	old_AlazarGetChannelInfoEx = dlsym(RTLD_NEXT, "AlazarGetChannelInfoEx");
	if (last_func!=21) repeat_count=0;

	last_func = 21; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetChannelInfoEx()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetChannelInfoEx()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tS64 * memorySize: %p\n", memorySize);
			fprintf(stderr, "\tU8 * bitsPerSample: %p\n", bitsPerSample);
		}
	}
	RETURN_CODE  result = old_AlazarGetChannelInfoEx(handle, memorySize, bitsPerSample);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarInputControl(HANDLE handle, U8 channel, U32 coupling, U32 inputRange, U32 impedance)
{
	old_AlazarInputControl = dlsym(RTLD_NEXT, "AlazarInputControl");
	if (last_func!=22) repeat_count=0;

	last_func = 22; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarInputControl()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarInputControl()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 coupling: %d (0x%08x)\n", coupling, coupling);
			fprintf(stderr, "\tU32 inputRange: %d (0x%08x)\n", inputRange, inputRange);
			fprintf(stderr, "\tU32 impedance: %d (0x%08x)\n", impedance, impedance);
		}
	}
	RETURN_CODE  result = old_AlazarInputControl(handle, channel, coupling, inputRange, impedance);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarInputControlEx(HANDLE handle, U32 channel, U32 couplingId, U32 rangeId, U32 impedenceId)
{
	old_AlazarInputControlEx = dlsym(RTLD_NEXT, "AlazarInputControlEx");
	if (last_func!=23) repeat_count=0;

	last_func = 23; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarInputControlEx()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarInputControlEx()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 channel: %d (0x%08x)\n", channel, channel);
			fprintf(stderr, "\tU32 couplingId: %d (0x%08x)\n", couplingId, couplingId);
			fprintf(stderr, "\tU32 rangeId: %d (0x%08x)\n", rangeId, rangeId);
			fprintf(stderr, "\tU32 impedenceId: %d (0x%08x)\n", impedenceId, impedenceId);
		}
	}
	RETURN_CODE  result = old_AlazarInputControlEx(handle, channel, couplingId, rangeId, impedenceId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetPosition(HANDLE handle, U8 Channel, int PMPercent, U32 InputRange)
{
	old_AlazarSetPosition = dlsym(RTLD_NEXT, "AlazarSetPosition");
	if (last_func!=24) repeat_count=0;

	last_func = 24; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetPosition()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetPosition()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 Channel: %d (0x%02x)\n", Channel, Channel);
			fprintf(stderr, "\tint PMPercent: %d\n", PMPercent);
			fprintf(stderr, "\tU32 InputRange: %d (0x%08x)\n", InputRange, InputRange);
		}
	}
	RETURN_CODE  result = old_AlazarSetPosition(handle, Channel, PMPercent, InputRange);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetExternalTrigger(HANDLE handle, U32 couplingId, U32 rangeId)
{
	old_AlazarSetExternalTrigger = dlsym(RTLD_NEXT, "AlazarSetExternalTrigger");
	if (last_func!=25) repeat_count=0;

	last_func = 25; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetExternalTrigger()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetExternalTrigger()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 couplingId: %d (0x%08x)\n", couplingId, couplingId);
			fprintf(stderr, "\tU32 rangeId: %d (0x%08x)\n", rangeId, rangeId);
		}
	}
	RETURN_CODE  result = old_AlazarSetExternalTrigger(handle, couplingId, rangeId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetTriggerDelay(HANDLE handle, U32 Delay)
{
	old_AlazarSetTriggerDelay = dlsym(RTLD_NEXT, "AlazarSetTriggerDelay");
	if (last_func!=26) repeat_count=0;

	last_func = 26; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetTriggerDelay()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetTriggerDelay()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 Delay: %d (0x%08x)\n", Delay, Delay);
		}
	}
	RETURN_CODE  result = old_AlazarSetTriggerDelay(handle, Delay);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetTriggerTimeOut(HANDLE handle, U32 timeout_ticks)
{
	old_AlazarSetTriggerTimeOut = dlsym(RTLD_NEXT, "AlazarSetTriggerTimeOut");
	if (last_func!=27) repeat_count=0;

	last_func = 27; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetTriggerTimeOut()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetTriggerTimeOut()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 timeout_ticks: %d (0x%08x)\n", timeout_ticks, timeout_ticks);
		}
	}
	RETURN_CODE  result = old_AlazarSetTriggerTimeOut(handle, timeout_ticks);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarTriggerTimedOut(HANDLE h)
{
	old_AlazarTriggerTimedOut = dlsym(RTLD_NEXT, "AlazarTriggerTimedOut");
	if (last_func!=28) repeat_count=0;

	last_func = 28; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarTriggerTimedOut()\n");
		} else {
			fprintf(stderr, "U32 AlazarTriggerTimedOut()\n");
			fprintf(stderr, "\tHANDLE h: %p\n", h);
		}
	}
	U32  result = old_AlazarTriggerTimedOut(h);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarSetTriggerOperation(HANDLE handle, U32 TriggerOperation, U32 TriggerEngine1, U32 Source1, U32 Slope1, U32 Level1, U32 TriggerEngine2, U32 Source2, U32 Slope2, U32 Level2)
{
	old_AlazarSetTriggerOperation = dlsym(RTLD_NEXT, "AlazarSetTriggerOperation");
	if (last_func!=30) repeat_count=0;

	last_func = 30; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetTriggerOperation()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetTriggerOperation()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 TriggerOperation: %d (0x%08x)\n", TriggerOperation, TriggerOperation);
			fprintf(stderr, "\tU32 TriggerEngine1: %d (0x%08x)\n", TriggerEngine1, TriggerEngine1);
			fprintf(stderr, "\tU32 Source1: %d (0x%08x)\n", Source1, Source1);
			fprintf(stderr, "\tU32 Slope1: %d (0x%08x)\n", Slope1, Slope1);
			fprintf(stderr, "\tU32 Level1: %d (0x%08x)\n", Level1, Level1);
			fprintf(stderr, "\tU32 TriggerEngine2: %d (0x%08x)\n", TriggerEngine2, TriggerEngine2);
			fprintf(stderr, "\tU32 Source2: %d (0x%08x)\n", Source2, Source2);
			fprintf(stderr, "\tU32 Slope2: %d (0x%08x)\n", Slope2, Slope2);
			fprintf(stderr, "\tU32 Level2: %d (0x%08x)\n", Level2, Level2);
		}
	}
	RETURN_CODE  result = old_AlazarSetTriggerOperation(handle, TriggerOperation, TriggerEngine1, Source1, Slope1, Level1, TriggerEngine2, Source2, Slope2, Level2);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetTriggerTimestamp(HANDLE handle, U32 Record, U64 *Timestamp_samples)
{
	old_AlazarGetTriggerTimestamp = dlsym(RTLD_NEXT, "AlazarGetTriggerTimestamp");
	if (last_func!=31) repeat_count=0;

	last_func = 31; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetTriggerTimestamp()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetTriggerTimestamp()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 Record: %d (0x%08x)\n", Record, Record);
			fprintf(stderr, "\tU64 * Timestamp_samples: %p\n", Timestamp_samples);
		}
	}
	RETURN_CODE  result = old_AlazarGetTriggerTimestamp(handle, Record, Timestamp_samples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetTriggerOperationForScanning(HANDLE handle, U32 slopeId, U32 level, U32 options)
{
	old_AlazarSetTriggerOperationForScanning = dlsym(RTLD_NEXT, "AlazarSetTriggerOperationForScanning");
	if (last_func!=32) repeat_count=0;

	last_func = 32; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetTriggerOperationForScanning()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetTriggerOperationForScanning()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 slopeId: %d (0x%08x)\n", slopeId, slopeId);
			fprintf(stderr, "\tU32 level: %d (0x%08x)\n", level, level);
			fprintf(stderr, "\tU32 options: %d (0x%08x)\n", options, options);
		}
	}
	RETURN_CODE  result = old_AlazarSetTriggerOperationForScanning(handle, slopeId, level, options);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarAbortCapture(HANDLE handle)
{
	old_AlazarAbortCapture = dlsym(RTLD_NEXT, "AlazarAbortCapture");
	if (last_func!=33) repeat_count=0;

	last_func = 33; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAbortCapture()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarAbortCapture()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarAbortCapture(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarForceTrigger(HANDLE handle)
{
	old_AlazarForceTrigger = dlsym(RTLD_NEXT, "AlazarForceTrigger");
	if (last_func!=34) repeat_count=0;

	last_func = 34; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarForceTrigger()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarForceTrigger()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarForceTrigger(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarForceTriggerEnable(HANDLE handle)
{
	old_AlazarForceTriggerEnable = dlsym(RTLD_NEXT, "AlazarForceTriggerEnable");
	if (last_func!=35) repeat_count=0;

	last_func = 35; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarForceTriggerEnable()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarForceTriggerEnable()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarForceTriggerEnable(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarStartCapture(HANDLE handle)
{
	old_AlazarStartCapture = dlsym(RTLD_NEXT, "AlazarStartCapture");
	if (last_func!=36) repeat_count=0;

	last_func = 36; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarStartCapture()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarStartCapture()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarStartCapture(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarCaptureMode(HANDLE handle, U32 Mode)
{
	old_AlazarCaptureMode = dlsym(RTLD_NEXT, "AlazarCaptureMode");
	if (last_func!=37) repeat_count=0;

	last_func = 37; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarCaptureMode()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarCaptureMode()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 Mode: %d (0x%08x)\n", Mode, Mode);
		}
	}
	RETURN_CODE  result = old_AlazarCaptureMode(handle, Mode);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarStreamCapture(HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error)
{
	old_AlazarStreamCapture = dlsym(RTLD_NEXT, "AlazarStreamCapture");
	if (last_func!=38) repeat_count=0;

	last_func = 38; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarStreamCapture()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarStreamCapture()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * Buffer: %p\n", Buffer);
			fprintf(stderr, "\tU32 BufferSize: %d (0x%08x)\n", BufferSize, BufferSize);
			fprintf(stderr, "\tU32 DeviceOption: %d (0x%08x)\n", DeviceOption, DeviceOption);
			fprintf(stderr, "\tU32 ChannelSelect: %d (0x%08x)\n", ChannelSelect, ChannelSelect);
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarStreamCapture(handle, Buffer, BufferSize, DeviceOption, ChannelSelect, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarHyperDisp(HANDLE handle, void *buffer, U32 bufferSize, U8 *viewBuffer, U32 viewBufferSize, U32 numOfPixels, U32 option, U32 channelSelect, U32 record, long transferOffset, U32 *error)
{
	old_AlazarHyperDisp = dlsym(RTLD_NEXT, "AlazarHyperDisp");
	if (last_func!=39) repeat_count=0;

	last_func = 39; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarHyperDisp()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarHyperDisp()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 bufferSize: %d (0x%08x)\n", bufferSize, bufferSize);
			fprintf(stderr, "\tU8 * viewBuffer: %p\n", viewBuffer);
			fprintf(stderr, "\tU32 viewBufferSize: %d (0x%08x)\n", viewBufferSize, viewBufferSize);
			fprintf(stderr, "\tU32 numOfPixels: %d (0x%08x)\n", numOfPixels, numOfPixels);
			fprintf(stderr, "\tU32 option: %d (0x%08x)\n", option, option);
			fprintf(stderr, "\tU32 channelSelect: %d (0x%08x)\n", channelSelect, channelSelect);
			fprintf(stderr, "\tU32 record: %d (0x%08x)\n", record, record);
			fprintf(stderr, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarHyperDisp(handle, buffer, bufferSize, viewBuffer, viewBufferSize, numOfPixels, option, channelSelect, record, transferOffset, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFastPRRCapture(HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error)
{
	old_AlazarFastPRRCapture = dlsym(RTLD_NEXT, "AlazarFastPRRCapture");
	if (last_func!=40) repeat_count=0;

	last_func = 40; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFastPRRCapture()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFastPRRCapture()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * Buffer: %p\n", Buffer);
			fprintf(stderr, "\tU32 BufferSize: %d (0x%08x)\n", BufferSize, BufferSize);
			fprintf(stderr, "\tU32 DeviceOption: %d (0x%08x)\n", DeviceOption, DeviceOption);
			fprintf(stderr, "\tU32 ChannelSelect: %d (0x%08x)\n", ChannelSelect, ChannelSelect);
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarFastPRRCapture(handle, Buffer, BufferSize, DeviceOption, ChannelSelect, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarBusy(HANDLE handle)
{
	old_AlazarBusy = dlsym(RTLD_NEXT, "AlazarBusy");
	if (last_func!=41) repeat_count=0;

	last_func = 41; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarBusy()\n");
		} else {
			fprintf(stderr, "U32 AlazarBusy()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	U32  result = old_AlazarBusy(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarTriggered(HANDLE handle)
{
	old_AlazarTriggered = dlsym(RTLD_NEXT, "AlazarTriggered");
	if (last_func!=42) repeat_count=0;

	last_func = 42; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarTriggered()\n");
		} else {
			fprintf(stderr, "U32 AlazarTriggered()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	U32  result = old_AlazarTriggered(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarGetStatus(HANDLE handle)
{
	old_AlazarGetStatus = dlsym(RTLD_NEXT, "AlazarGetStatus");
	if (last_func!=43) repeat_count=0;

	last_func = 43; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetStatus()\n");
		} else {
			fprintf(stderr, "U32 AlazarGetStatus()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	U32  result = old_AlazarGetStatus(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarDetectMultipleRecord(HANDLE handle)
{
	old_AlazarDetectMultipleRecord = dlsym(RTLD_NEXT, "AlazarDetectMultipleRecord");
	if (last_func!=44) repeat_count=0;

	last_func = 44; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDetectMultipleRecord()\n");
		} else {
			fprintf(stderr, "U32 AlazarDetectMultipleRecord()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	U32  result = old_AlazarDetectMultipleRecord(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarSetRecordCount(HANDLE handle, U32 Count)
{
	old_AlazarSetRecordCount = dlsym(RTLD_NEXT, "AlazarSetRecordCount");
	if (last_func!=45) repeat_count=0;

	last_func = 45; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetRecordCount()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetRecordCount()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 Count: %d (0x%08x)\n", Count, Count);
		}
	}
	RETURN_CODE  result = old_AlazarSetRecordCount(handle, Count);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetRecordSize(HANDLE handle, U32 preTriggerSamples, U32 postTriggerSamples)
{
	old_AlazarSetRecordSize = dlsym(RTLD_NEXT, "AlazarSetRecordSize");
	if (last_func!=46) repeat_count=0;

	last_func = 46; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetRecordSize()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetRecordSize()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 preTriggerSamples: %d (0x%08x)\n", preTriggerSamples, preTriggerSamples);
			fprintf(stderr, "\tU32 postTriggerSamples: %d (0x%08x)\n", postTriggerSamples, postTriggerSamples);
		}
	}
	RETURN_CODE  result = old_AlazarSetRecordSize(handle, preTriggerSamples, postTriggerSamples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetCaptureClock(HANDLE handle, U32 source, U32 sampleRateIdOrValue, U32 edgeId, U32 decimation)
{
	old_AlazarSetCaptureClock = dlsym(RTLD_NEXT, "AlazarSetCaptureClock");
	if (last_func!=47) repeat_count=0;

	last_func = 47; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetCaptureClock()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetCaptureClock()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 source: %d (0x%08x)\n", source, source);
			fprintf(stderr, "\tU32 sampleRateIdOrValue: %d (0x%08x)\n", sampleRateIdOrValue, sampleRateIdOrValue);
			fprintf(stderr, "\tU32 edgeId: %d (0x%08x)\n", edgeId, edgeId);
			fprintf(stderr, "\tU32 decimation: %d (0x%08x)\n", decimation, decimation);
		}
	}
	RETURN_CODE  result = old_AlazarSetCaptureClock(handle, source, sampleRateIdOrValue, edgeId, decimation);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetExternalClockLevel(HANDLE handle, float level_percent)
{
	old_AlazarSetExternalClockLevel = dlsym(RTLD_NEXT, "AlazarSetExternalClockLevel");
	if (last_func!=48) repeat_count=0;

	last_func = 48; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetExternalClockLevel()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetExternalClockLevel()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tfloat level_percent: %g\n", (double)level_percent);
		}
	}
	RETURN_CODE  result = old_AlazarSetExternalClockLevel(handle, level_percent);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetClockSwitchOver(HANDLE handleBoard, U32 uMode, U32 uDummyClockOnTime_ns, U32 uReserved)
{
	old_AlazarSetClockSwitchOver = dlsym(RTLD_NEXT, "AlazarSetClockSwitchOver");
	if (last_func!=49) repeat_count=0;

	last_func = 49; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetClockSwitchOver()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetClockSwitchOver()\n");
			fprintf(stderr, "\tHANDLE handleBoard: %p\n", handleBoard);
			fprintf(stderr, "\tU32 uMode: %d (0x%08x)\n", uMode, uMode);
			fprintf(stderr, "\tU32 uDummyClockOnTime_ns: %d (0x%08x)\n", uDummyClockOnTime_ns, uDummyClockOnTime_ns);
			fprintf(stderr, "\tU32 uReserved: %d (0x%08x)\n", uReserved, uReserved);
		}
	}
	RETURN_CODE  result = old_AlazarSetClockSwitchOver(handleBoard, uMode, uDummyClockOnTime_ns, uReserved);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarRead(HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, long transferOffset, U32 transferLength)
{
	old_AlazarRead = dlsym(RTLD_NEXT, "AlazarRead");
	if (last_func!=50) repeat_count=0;

	last_func = 50; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarRead()\n");
		} else {
			fprintf(stderr, "U32 AlazarRead()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 channelId: %d (0x%08x)\n", channelId, channelId);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tint elementSize: %d\n", elementSize);
			fprintf(stderr, "\tlong record: %ld\n", record);
			fprintf(stderr, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(stderr, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
		}
	}
	U32  result = old_AlazarRead(handle, channelId, buffer, elementSize, record, transferOffset, transferLength);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarReadEx(HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, INT64 transferOffset, U32 transferLength)
{
	old_AlazarReadEx = dlsym(RTLD_NEXT, "AlazarReadEx");
	if (last_func!=51) repeat_count=0;

	last_func = 51; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarReadEx()\n");
		} else {
			fprintf(stderr, "U32 AlazarReadEx()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 channelId: %d (0x%08x)\n", channelId, channelId);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tint elementSize: %d\n", elementSize);
			fprintf(stderr, "\tlong record: %ld\n", record);
			fprintf(stderr, "\tINT64 transferOffset: %lld (0x%08llx)\n", transferOffset, transferOffset);
			fprintf(stderr, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
		}
	}
	U32  result = old_AlazarReadEx(handle, channelId, buffer, elementSize, record, transferOffset, transferLength);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarSetParameter(HANDLE handle, U8 channel, U32 parameter, long value)
{
	old_AlazarSetParameter = dlsym(RTLD_NEXT, "AlazarSetParameter");
	if (last_func!=52) repeat_count=0;

	last_func = 52; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetParameter()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetParameter()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tlong value: %ld\n", value);
		}
	}
	RETURN_CODE  result = old_AlazarSetParameter(handle, channel, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetParameterUL(HANDLE handle, U8 channel, U32 parameter, U32 value)
{
	old_AlazarSetParameterUL = dlsym(RTLD_NEXT, "AlazarSetParameterUL");
	if (last_func!=53) repeat_count=0;

	last_func = 53; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetParameterUL()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetParameterUL()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tU32 value: %d (0x%08x)\n", value, value);
		}
	}
	RETURN_CODE  result = old_AlazarSetParameterUL(handle, channel, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetParameterLL(HANDLE handle, U8 channel, U32 parameter, S64 value)
{
	old_AlazarSetParameterLL = dlsym(RTLD_NEXT, "AlazarSetParameterLL");
	if (last_func!=54) repeat_count=0;

	last_func = 54; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetParameterLL()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetParameterLL()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tS64 value: %lld (0x%08llx)\n", value, value);
		}
	}
	RETURN_CODE  result = old_AlazarSetParameterLL(handle, channel, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetParameter(HANDLE handle, U8 channel, U32 parameter, long *retValue)
{
	old_AlazarGetParameter = dlsym(RTLD_NEXT, "AlazarGetParameter");
	if (last_func!=55) repeat_count=0;

	last_func = 55; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetParameter()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetParameter()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tlong * retValue: %p\n", retValue);
		}
	}
	RETURN_CODE  result = old_AlazarGetParameter(handle, channel, parameter, retValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetParameterUL(HANDLE handle, U8 channel, U32 parameter, U32 *retValue)
{
	old_AlazarGetParameterUL = dlsym(RTLD_NEXT, "AlazarGetParameterUL");
	if (last_func!=56) repeat_count=0;

	last_func = 56; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetParameterUL()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetParameterUL()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tU32 * retValue: %p\n", retValue);
		}
	}
	RETURN_CODE  result = old_AlazarGetParameterUL(handle, channel, parameter, retValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarGetParameterLL(HANDLE handle, U8 channel, U32 parameter, S64 *retValue)
{
	old_AlazarGetParameterLL = dlsym(RTLD_NEXT, "AlazarGetParameterLL");
	if (last_func!=57) repeat_count=0;

	last_func = 57; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetParameterLL()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetParameterLL()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tS64 * retValue: %p\n", retValue);
		}
	}
	RETURN_CODE  result = old_AlazarGetParameterLL(handle, channel, parameter, retValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
HANDLE AlazarGetSystemHandle(U32 systemId)
{
	old_AlazarGetSystemHandle = dlsym(RTLD_NEXT, "AlazarGetSystemHandle");
	if (last_func!=58) repeat_count=0;

	last_func = 58; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetSystemHandle()\n");
		} else {
			fprintf(stderr, "HANDLE AlazarGetSystemHandle()\n");
			fprintf(stderr, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
		}
	}
	HANDLE  result = old_AlazarGetSystemHandle(systemId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
U32 AlazarNumOfSystems(void)
{
	old_AlazarNumOfSystems = dlsym(RTLD_NEXT, "AlazarNumOfSystems");
	if (last_func!=59) repeat_count=0;

	last_func = 59; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarNumOfSystems()\n");
		} else {
			fprintf(stderr, "U32 AlazarNumOfSystems()\n");
		}
	}
	U32  result = old_AlazarNumOfSystems();
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarBoardsInSystemBySystemID(U32 systemId)
{
	old_AlazarBoardsInSystemBySystemID = dlsym(RTLD_NEXT, "AlazarBoardsInSystemBySystemID");
	if (last_func!=60) repeat_count=0;

	last_func = 60; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarBoardsInSystemBySystemID()\n");
		} else {
			fprintf(stderr, "U32 AlazarBoardsInSystemBySystemID()\n");
			fprintf(stderr, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
		}
	}
	U32  result = old_AlazarBoardsInSystemBySystemID(systemId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarBoardsInSystemByHandle(HANDLE systemHandle)
{
	old_AlazarBoardsInSystemByHandle = dlsym(RTLD_NEXT, "AlazarBoardsInSystemByHandle");
	if (last_func!=61) repeat_count=0;

	last_func = 61; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarBoardsInSystemByHandle()\n");
		} else {
			fprintf(stderr, "U32 AlazarBoardsInSystemByHandle()\n");
			fprintf(stderr, "\tHANDLE systemHandle: %p\n", systemHandle);
		}
	}
	U32  result = old_AlazarBoardsInSystemByHandle(systemHandle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
HANDLE AlazarGetBoardBySystemID(U32 systemId, U32 boardId)
{
	old_AlazarGetBoardBySystemID = dlsym(RTLD_NEXT, "AlazarGetBoardBySystemID");
	if (last_func!=62) repeat_count=0;

	last_func = 62; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetBoardBySystemID()\n");
		} else {
			fprintf(stderr, "HANDLE AlazarGetBoardBySystemID()\n");
			fprintf(stderr, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
			fprintf(stderr, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
		}
	}
	HANDLE  result = old_AlazarGetBoardBySystemID(systemId, boardId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
HANDLE AlazarGetBoardBySystemHandle(HANDLE systemHandle, U32 boardId)
{
	old_AlazarGetBoardBySystemHandle = dlsym(RTLD_NEXT, "AlazarGetBoardBySystemHandle");
	if (last_func!=63) repeat_count=0;

	last_func = 63; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetBoardBySystemHandle()\n");
		} else {
			fprintf(stderr, "HANDLE AlazarGetBoardBySystemHandle()\n");
			fprintf(stderr, "\tHANDLE systemHandle: %p\n", systemHandle);
			fprintf(stderr, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
		}
	}
	HANDLE  result = old_AlazarGetBoardBySystemHandle(systemHandle, boardId);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
RETURN_CODE AlazarSetLED(HANDLE handle, U32 state)
{
	old_AlazarSetLED = dlsym(RTLD_NEXT, "AlazarSetLED");
	if (last_func!=64) repeat_count=0;

	last_func = 64; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetLED()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetLED()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 state: %d (0x%08x)\n", state, state);
		}
	}
	RETURN_CODE  result = old_AlazarSetLED(handle, state);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarQueryCapability(HANDLE handle, U32 capability, U32 reserved, U32 *retValue)
{
	old_AlazarQueryCapability = dlsym(RTLD_NEXT, "AlazarQueryCapability");
	if (last_func!=65) repeat_count=0;

	last_func = 65; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarQueryCapability()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarQueryCapability()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 capability: %d (0x%08x)\n", capability, capability);
			fprintf(stderr, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(stderr, "\tU32 * retValue: %p\n", retValue);
		}
	}
	RETURN_CODE  result = old_AlazarQueryCapability(handle, capability, reserved, retValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarQueryCapabilityLL(HANDLE handle, U32 capability, U32 reserved, S64 *retValue)
{
	old_AlazarQueryCapabilityLL = dlsym(RTLD_NEXT, "AlazarQueryCapabilityLL");
	if (last_func!=66) repeat_count=0;

	last_func = 66; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarQueryCapabilityLL()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarQueryCapabilityLL()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 capability: %d (0x%08x)\n", capability, capability);
			fprintf(stderr, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(stderr, "\tS64 * retValue: %p\n", retValue);
		}
	}
	RETURN_CODE  result = old_AlazarQueryCapabilityLL(handle, capability, reserved, retValue);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarMaxSglTransfer(ALAZAR_BOARDTYPES bt)
{
	old_AlazarMaxSglTransfer = dlsym(RTLD_NEXT, "AlazarMaxSglTransfer");
	if (last_func!=67) repeat_count=0;

	last_func = 67; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarMaxSglTransfer()\n");
		} else {
			fprintf(stderr, "U32 AlazarMaxSglTransfer()\n");
			fprintf(stderr, "\tALAZAR_BOARDTYPES bt: %d (0x%02x)\n", bt, bt);
		}
	}
	U32  result = old_AlazarMaxSglTransfer(bt);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarGetMaxRecordsCapable(HANDLE handle, U32 samplesPerRecord, U32 *maxRecordsPerCapture)
{
	old_AlazarGetMaxRecordsCapable = dlsym(RTLD_NEXT, "AlazarGetMaxRecordsCapable");
	if (last_func!=68) repeat_count=0;

	last_func = 68; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetMaxRecordsCapable()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarGetMaxRecordsCapable()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(stderr, "\tU32 * maxRecordsPerCapture: %p\n", maxRecordsPerCapture);
		}
	}
	RETURN_CODE  result = old_AlazarGetMaxRecordsCapable(handle, samplesPerRecord, maxRecordsPerCapture);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U32 AlazarGetWhoTriggeredBySystemHandle(HANDLE systemHandle, U32 boardId, U32 recordNumber)
{
	old_AlazarGetWhoTriggeredBySystemHandle = dlsym(RTLD_NEXT, "AlazarGetWhoTriggeredBySystemHandle");
	if (last_func!=69) repeat_count=0;

	last_func = 69; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetWhoTriggeredBySystemHandle()\n");
		} else {
			fprintf(stderr, "U32 AlazarGetWhoTriggeredBySystemHandle()\n");
			fprintf(stderr, "\tHANDLE systemHandle: %p\n", systemHandle);
			fprintf(stderr, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
			fprintf(stderr, "\tU32 recordNumber: %d (0x%08x)\n", recordNumber, recordNumber);
		}
	}
	U32  result = old_AlazarGetWhoTriggeredBySystemHandle(systemHandle, boardId, recordNumber);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
U32 AlazarGetWhoTriggeredBySystemID(U32 systemId, U32 boardId, U32 recordNumber)
{
	old_AlazarGetWhoTriggeredBySystemID = dlsym(RTLD_NEXT, "AlazarGetWhoTriggeredBySystemID");
	if (last_func!=70) repeat_count=0;

	last_func = 70; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarGetWhoTriggeredBySystemID()\n");
		} else {
			fprintf(stderr, "U32 AlazarGetWhoTriggeredBySystemID()\n");
			fprintf(stderr, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
			fprintf(stderr, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
			fprintf(stderr, "\tU32 recordNumber: %d (0x%08x)\n", recordNumber, recordNumber);
		}
	}
	U32  result = old_AlazarGetWhoTriggeredBySystemID(systemId, boardId, recordNumber);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %d (0x%08x)\n", result, result);
	}
	return result;
}
RETURN_CODE AlazarSetBWLimit(HANDLE handle, U32 channel, U32 enable)
{
	old_AlazarSetBWLimit = dlsym(RTLD_NEXT, "AlazarSetBWLimit");
	if (last_func!=71) repeat_count=0;

	last_func = 71; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetBWLimit()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetBWLimit()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 channel: %d (0x%08x)\n", channel, channel);
			fprintf(stderr, "\tU32 enable: %d (0x%08x)\n", enable, enable);
		}
	}
	RETURN_CODE  result = old_AlazarSetBWLimit(handle, channel, enable);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSleepDevice(HANDLE handle, U32 sleepState)
{
	old_AlazarSleepDevice = dlsym(RTLD_NEXT, "AlazarSleepDevice");
	if (last_func!=72) repeat_count=0;

	last_func = 72; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSleepDevice()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSleepDevice()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 sleepState: %d (0x%08x)\n", sleepState, sleepState);
		}
	}
	RETURN_CODE  result = old_AlazarSleepDevice(handle, sleepState);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarBeforeAsyncRead(HANDLE handle, U32 channelSelect, long transferOffset, U32 transferLength, U32 recordsPerBuffer, U32 recordsPerAcquisition, U32 flags)
{
	old_AlazarBeforeAsyncRead = dlsym(RTLD_NEXT, "AlazarBeforeAsyncRead");
	if (last_func!=73) repeat_count=0;

	last_func = 73; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarBeforeAsyncRead()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarBeforeAsyncRead()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 channelSelect: %d (0x%08x)\n", channelSelect, channelSelect);
			fprintf(stderr, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(stderr, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
			fprintf(stderr, "\tU32 recordsPerBuffer: %d (0x%08x)\n", recordsPerBuffer, recordsPerBuffer);
			fprintf(stderr, "\tU32 recordsPerAcquisition: %d (0x%08x)\n", recordsPerAcquisition, recordsPerAcquisition);
			fprintf(stderr, "\tU32 flags: %d (0x%08x)\n", flags, flags);
		}
	}
	RETURN_CODE  result = old_AlazarBeforeAsyncRead(handle, channelSelect, transferOffset, transferLength, recordsPerBuffer, recordsPerAcquisition, flags);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarAbortAsyncRead(HANDLE handle)
{
	old_AlazarAbortAsyncRead = dlsym(RTLD_NEXT, "AlazarAbortAsyncRead");
	if (last_func!=74) repeat_count=0;

	last_func = 74; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAbortAsyncRead()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarAbortAsyncRead()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
		}
	}
	RETURN_CODE  result = old_AlazarAbortAsyncRead(handle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarPostAsyncBuffer(HANDLE handle, void *buffer, U32 bufferLength_bytes)
{
	old_AlazarPostAsyncBuffer = dlsym(RTLD_NEXT, "AlazarPostAsyncBuffer");
	if (last_func!=75) repeat_count=0;

	last_func = 75; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarPostAsyncBuffer()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarPostAsyncBuffer()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 bufferLength_bytes: %d (0x%08x)\n", bufferLength_bytes, bufferLength_bytes);
		}
	}
	RETURN_CODE  result = old_AlazarPostAsyncBuffer(handle, buffer, bufferLength_bytes);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarWaitAsyncBufferComplete(HANDLE handle, void *buffer, U32 timeout_ms)
{
	old_AlazarWaitAsyncBufferComplete = dlsym(RTLD_NEXT, "AlazarWaitAsyncBufferComplete");
	if (last_func!=76) repeat_count=0;

	last_func = 76; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarWaitAsyncBufferComplete()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarWaitAsyncBufferComplete()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
	}
	RETURN_CODE  result = old_AlazarWaitAsyncBufferComplete(handle, buffer, timeout_ms);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarWaitNextAsyncBufferComplete(HANDLE handle, void *buffer, U32 bytesToCopy, U32 timeout_ms)
{
	old_AlazarWaitNextAsyncBufferComplete = dlsym(RTLD_NEXT, "AlazarWaitNextAsyncBufferComplete");
	if (last_func!=77) repeat_count=0;

	last_func = 77; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarWaitNextAsyncBufferComplete()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarWaitNextAsyncBufferComplete()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 bytesToCopy: %d (0x%08x)\n", bytesToCopy, bytesToCopy);
			fprintf(stderr, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
	}
	RETURN_CODE  result = old_AlazarWaitNextAsyncBufferComplete(handle, buffer, bytesToCopy, timeout_ms);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarResetTimeStamp(HANDLE handle, U32 option)
{
	old_AlazarResetTimeStamp = dlsym(RTLD_NEXT, "AlazarResetTimeStamp");
	if (last_func!=79) repeat_count=0;

	last_func = 79; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarResetTimeStamp()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarResetTimeStamp()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 option: %d (0x%08x)\n", option, option);
		}
	}
	RETURN_CODE  result = old_AlazarResetTimeStamp(handle, option);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarReadRegister(HANDLE handle, U32 offset, U32 *retVal, U32 pswrd)
{
	old_AlazarReadRegister = dlsym(RTLD_NEXT, "AlazarReadRegister");
	if (last_func!=80) repeat_count=0;

	last_func = 80; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarReadRegister()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarReadRegister()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(stderr, "\tU32 * retVal: %p\n", retVal);
			fprintf(stderr, "\tU32 pswrd: %d (0x%08x)\n", pswrd, pswrd);
		}
	}
	RETURN_CODE  result = old_AlazarReadRegister(handle, offset, retVal, pswrd);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarWriteRegister(HANDLE handle, U32 offset, U32 Val,U32 pswrd)
{
	old_AlazarWriteRegister = dlsym(RTLD_NEXT, "AlazarWriteRegister");
	if (last_func!=81) repeat_count=0;

	last_func = 81; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarWriteRegister()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarWriteRegister()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(stderr, "\tU32 Val: %d (0x%08x)\n", Val, Val);
			fprintf(stderr, "\tU32 pswrd: %d (0x%08x)\n", pswrd, pswrd);
		}
	}
	RETURN_CODE  result = old_AlazarWriteRegister(handle, offset, Val, pswrd);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE ATS9462FlashSectorPageRead(HANDLE handle, U32 address, U16 *PageBuff)
{
	old_ATS9462FlashSectorPageRead = dlsym(RTLD_NEXT, "ATS9462FlashSectorPageRead");
	if (last_func!=86) repeat_count=0;

	last_func = 86; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to ATS9462FlashSectorPageRead()\n");
		} else {
			fprintf(stderr, "RETURN_CODE ATS9462FlashSectorPageRead()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 address: %d (0x%08x)\n", address, address);
			fprintf(stderr, "\tU16 * PageBuff: %p\n", PageBuff);
		}
	}
	RETURN_CODE  result = old_ATS9462FlashSectorPageRead(handle, address, PageBuff);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE ATS9462PageWriteToFlash(HANDLE handle, U32 address, U16 *PageBuff)
{
	old_ATS9462PageWriteToFlash = dlsym(RTLD_NEXT, "ATS9462PageWriteToFlash");
	if (last_func!=87) repeat_count=0;

	last_func = 87; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to ATS9462PageWriteToFlash()\n");
		} else {
			fprintf(stderr, "RETURN_CODE ATS9462PageWriteToFlash()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 address: %d (0x%08x)\n", address, address);
			fprintf(stderr, "\tU16 * PageBuff: %p\n", PageBuff);
		}
	}
	RETURN_CODE  result = old_ATS9462PageWriteToFlash(handle, address, PageBuff);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE ATS9462FlashSectorErase(HANDLE handle, int sectorNum)
{
	old_ATS9462FlashSectorErase = dlsym(RTLD_NEXT, "ATS9462FlashSectorErase");
	if (last_func!=88) repeat_count=0;

	last_func = 88; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to ATS9462FlashSectorErase()\n");
		} else {
			fprintf(stderr, "RETURN_CODE ATS9462FlashSectorErase()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tint sectorNum: %d\n", sectorNum);
		}
	}
	RETURN_CODE  result = old_ATS9462FlashSectorErase(handle, sectorNum);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE ATS9462FlashChipErase(HANDLE h)
{
	old_ATS9462FlashChipErase = dlsym(RTLD_NEXT, "ATS9462FlashChipErase");
	if (last_func!=89) repeat_count=0;

	last_func = 89; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to ATS9462FlashChipErase()\n");
		} else {
			fprintf(stderr, "RETURN_CODE ATS9462FlashChipErase()\n");
			fprintf(stderr, "\tHANDLE h: %p\n", h);
		}
	}
	RETURN_CODE  result = old_ATS9462FlashChipErase(h);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE SetControlCommand(HANDLE handle, int cmd)
{
	old_SetControlCommand = dlsym(RTLD_NEXT, "SetControlCommand");
	if (last_func!=90) repeat_count=0;

	last_func = 90; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to SetControlCommand()\n");
		} else {
			fprintf(stderr, "RETURN_CODE SetControlCommand()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tint cmd: %d\n", cmd);
		}
	}
	RETURN_CODE  result = old_SetControlCommand(handle, cmd);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDACSetting(HANDLE handle, U32 SetGet, U32 OriginalOrModified, U8 Channel, U32 DACNAME, U32 Coupling, U32 InputRange, U32 Impedance, U32 *getVal, U32 setVal, U32 *error)
{
	old_AlazarDACSetting = dlsym(RTLD_NEXT, "AlazarDACSetting");
	if (last_func!=91) repeat_count=0;

	last_func = 91; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDACSetting()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDACSetting()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 SetGet: %d (0x%08x)\n", SetGet, SetGet);
			fprintf(stderr, "\tU32 OriginalOrModified: %d (0x%08x)\n", OriginalOrModified, OriginalOrModified);
			fprintf(stderr, "\tU8 Channel: %d (0x%02x)\n", Channel, Channel);
			fprintf(stderr, "\tU32 DACNAME: %d (0x%08x)\n", DACNAME, DACNAME);
			fprintf(stderr, "\tU32 Coupling: %d (0x%08x)\n", Coupling, Coupling);
			fprintf(stderr, "\tU32 InputRange: %d (0x%08x)\n", InputRange, InputRange);
			fprintf(stderr, "\tU32 Impedance: %d (0x%08x)\n", Impedance, Impedance);
			fprintf(stderr, "\tU32 * getVal: %p\n", getVal);
			fprintf(stderr, "\tU32 setVal: %d (0x%08x)\n", setVal, setVal);
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarDACSetting(handle, SetGet, OriginalOrModified, Channel, DACNAME, Coupling, InputRange, Impedance, getVal, setVal, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarWrite(HANDLE handle, void *buffer, long bufLen, int DmaChannel, U32 firstPoint, U32 startAddress, U32 endAddress, BOOL waitTillEnd, U32 *error)
{
	old_AlazarWrite = dlsym(RTLD_NEXT, "AlazarWrite");
	if (last_func!=92) repeat_count=0;

	last_func = 92; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarWrite()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarWrite()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tlong bufLen: %ld\n", bufLen);
			fprintf(stderr, "\tint DmaChannel: %d\n", DmaChannel);
			fprintf(stderr, "\tU32 firstPoint: %d (0x%08x)\n", firstPoint, firstPoint);
			fprintf(stderr, "\tU32 startAddress: %d (0x%08x)\n", startAddress, startAddress);
			fprintf(stderr, "\tU32 endAddress: %d (0x%08x)\n", endAddress, endAddress);
			fprintf(stderr, "\tBOOL waitTillEnd: %s\n", waitTillEnd?"true":"false");
			fprintf(stderr, "\tU32 * error: %p\n", error);
		}
	}
	RETURN_CODE  result = old_AlazarWrite(handle, buffer, bufLen, DmaChannel, firstPoint, startAddress, endAddress, waitTillEnd, error);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarConfigureAuxIO(HANDLE handle, U32 mode, U32 parameter)
{
	old_AlazarConfigureAuxIO = dlsym(RTLD_NEXT, "AlazarConfigureAuxIO");
	if (last_func!=93) repeat_count=0;

	last_func = 93; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarConfigureAuxIO()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarConfigureAuxIO()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
		}
	}
	RETURN_CODE  result = old_AlazarConfigureAuxIO(handle, mode, parameter);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarConfigureSampleSkipping(HANDLE handle, U32 mode, U32 sampleClocksPerRecord, U16 *sampleSkipBitmap)
{
	old_AlazarConfigureSampleSkipping = dlsym(RTLD_NEXT, "AlazarConfigureSampleSkipping");
	if (last_func!=95) repeat_count=0;

	last_func = 95; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarConfigureSampleSkipping()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarConfigureSampleSkipping()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(stderr, "\tU32 sampleClocksPerRecord: %d (0x%08x)\n", sampleClocksPerRecord, sampleClocksPerRecord);
			fprintf(stderr, "\tU16 * sampleSkipBitmap: %p\n", sampleSkipBitmap);
		}
	}
	RETURN_CODE  result = old_AlazarConfigureSampleSkipping(handle, mode, sampleClocksPerRecord, sampleSkipBitmap);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarCoprocessorRegisterRead(HANDLE handle, U32 offset, U32 *value)
{
	old_AlazarCoprocessorRegisterRead = dlsym(RTLD_NEXT, "AlazarCoprocessorRegisterRead");
	if (last_func!=96) repeat_count=0;

	last_func = 96; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarCoprocessorRegisterRead()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarCoprocessorRegisterRead()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(stderr, "\tU32 * value: %p\n", value);
		}
	}
	RETURN_CODE  result = old_AlazarCoprocessorRegisterRead(handle, offset, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarCoprocessorRegisterWrite(HANDLE handle, U32 offset, U32 value)
{
	old_AlazarCoprocessorRegisterWrite = dlsym(RTLD_NEXT, "AlazarCoprocessorRegisterWrite");
	if (last_func!=97) repeat_count=0;

	last_func = 97; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarCoprocessorRegisterWrite()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarCoprocessorRegisterWrite()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(stderr, "\tU32 value: %d (0x%08x)\n", value, value);
		}
	}
	RETURN_CODE  result = old_AlazarCoprocessorRegisterWrite(handle, offset, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarCoprocessorDownloadA(HANDLE handle, char *fileName, U32 options)
{
	old_AlazarCoprocessorDownloadA = dlsym(RTLD_NEXT, "AlazarCoprocessorDownloadA");
	if (last_func!=98) repeat_count=0;

	last_func = 98; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarCoprocessorDownloadA()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarCoprocessorDownloadA()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tchar * fileName: %s\n", fileName);
			fprintf(stderr, "\tU32 options: %d (0x%08x)\n", options, options);
		}
	}
	RETURN_CODE  result = old_AlazarCoprocessorDownloadA(handle, fileName, options);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarCoprocessorDownloadBuffer(HANDLE handle, U8 *pbBuffer, U32 uBytesToWrite, U32 options)
{
	old_AlazarCoprocessorDownloadBuffer = dlsym(RTLD_NEXT, "AlazarCoprocessorDownloadBuffer");
	if (last_func!=99) repeat_count=0;

	last_func = 99; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarCoprocessorDownloadBuffer()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarCoprocessorDownloadBuffer()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 * pbBuffer: %p\n", pbBuffer);
			fprintf(stderr, "\tU32 uBytesToWrite: %d (0x%08x)\n", uBytesToWrite, uBytesToWrite);
			fprintf(stderr, "\tU32 options: %d (0x%08x)\n", options, options);
		}
	}
	RETURN_CODE  result = old_AlazarCoprocessorDownloadBuffer(handle, pbBuffer, uBytesToWrite, options);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarConfigureRecordAverage(HANDLE handle, U32 mode, U32 samplesPerRecord, U32 recordsPerAverage, U32 options)
{
	old_AlazarConfigureRecordAverage = dlsym(RTLD_NEXT, "AlazarConfigureRecordAverage");
	if (last_func!=100) repeat_count=0;

	last_func = 100; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarConfigureRecordAverage()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarConfigureRecordAverage()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(stderr, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(stderr, "\tU32 recordsPerAverage: %d (0x%08x)\n", recordsPerAverage, recordsPerAverage);
			fprintf(stderr, "\tU32 options: %d (0x%08x)\n", options, options);
		}
	}
	RETURN_CODE  result = old_AlazarConfigureRecordAverage(handle, mode, samplesPerRecord, recordsPerAverage, options);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U8  *AlazarAllocBufferU8(HANDLE handle, U32 sampleCount)
{
	old_AlazarAllocBufferU8 = dlsym(RTLD_NEXT, "AlazarAllocBufferU8");
	if (last_func!=101) repeat_count=0;

	last_func = 101; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAllocBufferU8()\n");
		} else {
			fprintf(stderr, "U8 AlazarAllocBufferU8()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 sampleCount: %d (0x%08x)\n", sampleCount, sampleCount);
		}
	}
	U8  * result = old_AlazarAllocBufferU8(handle, sampleCount);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU8(HANDLE handle, U8 *buffer)
{
	old_AlazarFreeBufferU8 = dlsym(RTLD_NEXT, "AlazarFreeBufferU8");
	if (last_func!=102) repeat_count=0;

	last_func = 102; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFreeBufferU8()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFreeBufferU8()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 * buffer: %p\n", buffer);
		}
	}
	RETURN_CODE  result = old_AlazarFreeBufferU8(handle, buffer);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U16  *AlazarAllocBufferU16(HANDLE handle, U32 sampleCount)
{
	old_AlazarAllocBufferU16 = dlsym(RTLD_NEXT, "AlazarAllocBufferU16");
	if (last_func!=103) repeat_count=0;

	last_func = 103; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAllocBufferU16()\n");
		} else {
			fprintf(stderr, "U16 AlazarAllocBufferU16()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 sampleCount: %d (0x%08x)\n", sampleCount, sampleCount);
		}
	}
	U16  * result = old_AlazarAllocBufferU16(handle, sampleCount);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU16(HANDLE handle, U16 *buffer)
{
	old_AlazarFreeBufferU16 = dlsym(RTLD_NEXT, "AlazarFreeBufferU16");
	if (last_func!=104) repeat_count=0;

	last_func = 104; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFreeBufferU16()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFreeBufferU16()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU16 * buffer: %p\n", buffer);
		}
	}
	RETURN_CODE  result = old_AlazarFreeBufferU16(handle, buffer);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U8  *AlazarAllocBufferU8Ex(HANDLE handle, U64 sampleCount)
{
	old_AlazarAllocBufferU8Ex = dlsym(RTLD_NEXT, "AlazarAllocBufferU8Ex");
	if (last_func!=105) repeat_count=0;

	last_func = 105; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAllocBufferU8Ex()\n");
		} else {
			fprintf(stderr, "U8 AlazarAllocBufferU8Ex()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU64 sampleCount: %lld (0x%08llx)\n", sampleCount, sampleCount);
		}
	}
	U8  * result = old_AlazarAllocBufferU8Ex(handle, sampleCount);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU8Ex(HANDLE handle, U8 *buffer)
{
	old_AlazarFreeBufferU8Ex = dlsym(RTLD_NEXT, "AlazarFreeBufferU8Ex");
	if (last_func!=106) repeat_count=0;

	last_func = 106; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFreeBufferU8Ex()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFreeBufferU8Ex()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU8 * buffer: %p\n", buffer);
		}
	}
	RETURN_CODE  result = old_AlazarFreeBufferU8Ex(handle, buffer);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
U16  *AlazarAllocBufferU16Ex(HANDLE handle, U64 sampleCount)
{
	old_AlazarAllocBufferU16Ex = dlsym(RTLD_NEXT, "AlazarAllocBufferU16Ex");
	if (last_func!=107) repeat_count=0;

	last_func = 107; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarAllocBufferU16Ex()\n");
		} else {
			fprintf(stderr, "U16 AlazarAllocBufferU16Ex()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU64 sampleCount: %lld (0x%08llx)\n", sampleCount, sampleCount);
		}
	}
	U16  * result = old_AlazarAllocBufferU16Ex(handle, sampleCount);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %p\n", result);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU16Ex(HANDLE handle, U16 *buffer)
{
	old_AlazarFreeBufferU16Ex = dlsym(RTLD_NEXT, "AlazarFreeBufferU16Ex");
	if (last_func!=108) repeat_count=0;

	last_func = 108; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFreeBufferU16Ex()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFreeBufferU16Ex()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU16 * buffer: %p\n", buffer);
		}
	}
	RETURN_CODE  result = old_AlazarFreeBufferU16Ex(handle, buffer);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarConfigureLSB(HANDLE handle, U32 valueLsb0, U32 valueLsb1)
{
	old_AlazarConfigureLSB = dlsym(RTLD_NEXT, "AlazarConfigureLSB");
	if (last_func!=109) repeat_count=0;

	last_func = 109; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarConfigureLSB()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarConfigureLSB()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 valueLsb0: %d (0x%08x)\n", valueLsb0, valueLsb0);
			fprintf(stderr, "\tU32 valueLsb1: %d (0x%08x)\n", valueLsb1, valueLsb1);
		}
	}
	RETURN_CODE  result = old_AlazarConfigureLSB(handle, valueLsb0, valueLsb1);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarExtractFFTNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	old_AlazarExtractFFTNPTFooters = dlsym(RTLD_NEXT, "AlazarExtractFFTNPTFooters");
	if (last_func!=110) repeat_count=0;

	last_func = 110; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarExtractFFTNPTFooters()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarExtractFFTNPTFooters()\n");
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(stderr, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(stderr, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(stderr, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
	}
	RETURN_CODE  result = old_AlazarExtractFFTNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarExtractTimeDomainNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	old_AlazarExtractTimeDomainNPTFooters = dlsym(RTLD_NEXT, "AlazarExtractTimeDomainNPTFooters");
	if (last_func!=111) repeat_count=0;

	last_func = 111; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarExtractTimeDomainNPTFooters()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarExtractTimeDomainNPTFooters()\n");
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(stderr, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(stderr, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(stderr, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
	}
	RETURN_CODE  result = old_AlazarExtractTimeDomainNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarExtractNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	old_AlazarExtractNPTFooters = dlsym(RTLD_NEXT, "AlazarExtractNPTFooters");
	if (last_func!=112) repeat_count=0;

	last_func = 112; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarExtractNPTFooters()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarExtractNPTFooters()\n");
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(stderr, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(stderr, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(stderr, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
	}
	RETURN_CODE  result = old_AlazarExtractNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDisableDSP(HANDLE boardHandle)
{
	old_AlazarDisableDSP = dlsym(RTLD_NEXT, "AlazarDisableDSP");
	if (last_func!=113) repeat_count=0;

	last_func = 113; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDisableDSP()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDisableDSP()\n");
			fprintf(stderr, "\tHANDLE boardHandle: %p\n", boardHandle);
		}
	}
	RETURN_CODE  result = old_AlazarDisableDSP(boardHandle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarOCTIgnoreBadClock(HANDLE handle, U32 enable, double goodClockDuration_seconds, double badClockDuration_seconds, double *triggerCycleTime_seconds, double *triggerPulseWidth_seconds)
{
	old_AlazarOCTIgnoreBadClock = dlsym(RTLD_NEXT, "AlazarOCTIgnoreBadClock");
	if (last_func!=114) repeat_count=0;

	last_func = 114; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarOCTIgnoreBadClock()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarOCTIgnoreBadClock()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tU32 enable: %d (0x%08x)\n", enable, enable);
			fprintf(stderr, "\tdouble goodClockDuration_seconds: %g\n", goodClockDuration_seconds);
			fprintf(stderr, "\tdouble badClockDuration_seconds: %g\n", badClockDuration_seconds);
			fprintf(stderr, "\tdouble * triggerCycleTime_seconds: %p\n", triggerCycleTime_seconds);
			fprintf(stderr, "\tdouble * triggerPulseWidth_seconds: %p\n", triggerPulseWidth_seconds);
		}
	}
	RETURN_CODE  result = old_AlazarOCTIgnoreBadClock(handle, enable, goodClockDuration_seconds, badClockDuration_seconds, triggerCycleTime_seconds, triggerPulseWidth_seconds);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarSetADCBackgroundCompensation(HANDLE handle, BOOL active)
{
	old_AlazarSetADCBackgroundCompensation = dlsym(RTLD_NEXT, "AlazarSetADCBackgroundCompensation");
	if (last_func!=115) repeat_count=0;

	last_func = 115; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarSetADCBackgroundCompensation()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarSetADCBackgroundCompensation()\n");
			fprintf(stderr, "\tHANDLE handle: %p\n", handle);
			fprintf(stderr, "\tBOOL active: %s\n", active?"true":"false");
		}
	}
	RETURN_CODE  result = old_AlazarSetADCBackgroundCompensation(handle, active);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetModules(HANDLE boardHandle, U32 numEntries, dsp_module_handle *modules, U32 *numModules)
{
	old_AlazarDSPGetModules = dlsym(RTLD_NEXT, "AlazarDSPGetModules");
	if (last_func!=116) repeat_count=0;

	last_func = 116; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetModules()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetModules()\n");
			fprintf(stderr, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(stderr, "\tU32 numEntries: %d (0x%08x)\n", numEntries, numEntries);
			fprintf(stderr, "\tdsp_module_handle * modules: %p\n", modules);
			fprintf(stderr, "\tU32 * numModules: %p\n", numModules);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetModules(boardHandle, numEntries, modules, numModules);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetInfo(dsp_module_handle dspHandle, U32 *dspModuleId, U16 *versionMajor, U16 *versionMinor, U32 *maxLength, U32 *reserved0, U32 *reserved1)
{
	old_AlazarDSPGetInfo = dlsym(RTLD_NEXT, "AlazarDSPGetInfo");
	if (last_func!=117) repeat_count=0;

	last_func = 117; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetInfo()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetInfo()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 * dspModuleId: %p\n", dspModuleId);
			fprintf(stderr, "\tU16 * versionMajor: %p\n", versionMajor);
			fprintf(stderr, "\tU16 * versionMinor: %p\n", versionMinor);
			fprintf(stderr, "\tU32 * maxLength: %p\n", maxLength);
			fprintf(stderr, "\tU32 * reserved0: %p\n", reserved0);
			fprintf(stderr, "\tU32 * reserved1: %p\n", reserved1);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetInfo(dspHandle, dspModuleId, versionMajor, versionMinor, maxLength, reserved0, reserved1);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGenerateWindowFunction(U32 windowType, float *window, U32 windowLength_samples, U32 paddingLength_samples)
{
	old_AlazarDSPGenerateWindowFunction = dlsym(RTLD_NEXT, "AlazarDSPGenerateWindowFunction");
	if (last_func!=118) repeat_count=0;

	last_func = 118; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGenerateWindowFunction()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGenerateWindowFunction()\n");
			fprintf(stderr, "\tU32 windowType: %d (0x%08x)\n", windowType, windowType);
			fprintf(stderr, "\tfloat * window: %p\n", window);
			fprintf(stderr, "\tU32 windowLength_samples: %d (0x%08x)\n", windowLength_samples, windowLength_samples);
			fprintf(stderr, "\tU32 paddingLength_samples: %d (0x%08x)\n", paddingLength_samples, paddingLength_samples);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGenerateWindowFunction(windowType, window, windowLength_samples, paddingLength_samples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTGetMaxTriggerRepeatRate(dsp_module_handle dspHandle, U32 fftSize, double *maxTriggerRepeatRate)
{
	old_AlazarFFTGetMaxTriggerRepeatRate = dlsym(RTLD_NEXT, "AlazarFFTGetMaxTriggerRepeatRate");
	if (last_func!=119) repeat_count=0;

	last_func = 119; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTGetMaxTriggerRepeatRate()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTGetMaxTriggerRepeatRate()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 fftSize: %d (0x%08x)\n", fftSize, fftSize);
			fprintf(stderr, "\tdouble * maxTriggerRepeatRate: %p\n", maxTriggerRepeatRate);
		}
	}
	RETURN_CODE  result = old_AlazarFFTGetMaxTriggerRepeatRate(dspHandle, fftSize, maxTriggerRepeatRate);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionSetRecordS16(dsp_module_handle dspHandle, const S16 *record, U32 size_samples)
{
	old_AlazarFFTBackgroundSubtractionSetRecordS16 = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionSetRecordS16");
	if (last_func!=120) repeat_count=0;

	last_func = 120; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTBackgroundSubtractionSetRecordS16()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTBackgroundSubtractionSetRecordS16()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tconst S16 * record: %p\n", record);
			fprintf(stderr, "\tU32 size_samples: %d (0x%08x)\n", size_samples, size_samples);
		}
	}
	RETURN_CODE  result = old_AlazarFFTBackgroundSubtractionSetRecordS16(dspHandle, record, size_samples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionGetRecordS16(dsp_module_handle dspHandle, S16 *backgroundRecord, U32 size_samples)
{
	old_AlazarFFTBackgroundSubtractionGetRecordS16 = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionGetRecordS16");
	if (last_func!=121) repeat_count=0;

	last_func = 121; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTBackgroundSubtractionGetRecordS16()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTBackgroundSubtractionGetRecordS16()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tS16 * backgroundRecord: %p\n", backgroundRecord);
			fprintf(stderr, "\tU32 size_samples: %d (0x%08x)\n", size_samples, size_samples);
		}
	}
	RETURN_CODE  result = old_AlazarFFTBackgroundSubtractionGetRecordS16(dspHandle, backgroundRecord, size_samples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionSetEnabled(dsp_module_handle dspHandle, BOOL enabled)
{
	old_AlazarFFTBackgroundSubtractionSetEnabled = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionSetEnabled");
	if (last_func!=122) repeat_count=0;

	last_func = 122; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTBackgroundSubtractionSetEnabled()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTBackgroundSubtractionSetEnabled()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tBOOL enabled: %s\n", enabled?"true":"false");
		}
	}
	RETURN_CODE  result = old_AlazarFFTBackgroundSubtractionSetEnabled(dspHandle, enabled);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTSetWindowFunction(dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray)
{
	old_AlazarFFTSetWindowFunction = dlsym(RTLD_NEXT, "AlazarFFTSetWindowFunction");
	if (last_func!=123) repeat_count=0;

	last_func = 123; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTSetWindowFunction()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTSetWindowFunction()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(stderr, "\tfloat * realWindowArray: %p\n", realWindowArray);
			fprintf(stderr, "\tfloat * imagWindowArray: %p\n", imagWindowArray);
		}
	}
	RETURN_CODE  result = old_AlazarFFTSetWindowFunction(dspHandle, samplesPerRecord, realWindowArray, imagWindowArray);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTGetWindowFunction(dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray)
{
	old_AlazarFFTGetWindowFunction = dlsym(RTLD_NEXT, "AlazarFFTGetWindowFunction");
	if (last_func!=124) repeat_count=0;

	last_func = 124; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTGetWindowFunction()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTGetWindowFunction()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(stderr, "\tfloat * realWindowArray: %p\n", realWindowArray);
			fprintf(stderr, "\tfloat * imagWindowArray: %p\n", imagWindowArray);
		}
	}
	RETURN_CODE  result = old_AlazarFFTGetWindowFunction(dspHandle, samplesPerRecord, realWindowArray, imagWindowArray);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTVerificationMode(dsp_module_handle dspHandle, BOOL enable, S16 *realArray, S16 *imagArray, size_t recordLength_samples)
{
	old_AlazarFFTVerificationMode = dlsym(RTLD_NEXT, "AlazarFFTVerificationMode");
	if (last_func!=125) repeat_count=0;

	last_func = 125; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTVerificationMode()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTVerificationMode()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tBOOL enable: %s\n", enable?"true":"false");
			fprintf(stderr, "\tS16 * realArray: %p\n", realArray);
			fprintf(stderr, "\tS16 * imagArray: %p\n", imagArray);
			fprintf(stderr, "\tsize_t recordLength_samples: %ld\n", recordLength_samples);
		}
	}
	RETURN_CODE  result = old_AlazarFFTVerificationMode(dspHandle, enable, realArray, imagArray, recordLength_samples);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTSetup(dsp_module_handle dspHandle, U16 inputChannelMask, U32 recordLength_samples, U32 fftLength_samples, U32 outputFormat, U32 footer, U32 reserved, U32 *bytesPerOutputRecord)
{
	old_AlazarFFTSetup = dlsym(RTLD_NEXT, "AlazarFFTSetup");
	if (last_func!=126) repeat_count=0;

	last_func = 126; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTSetup()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTSetup()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU16 inputChannelMask: %d (0x%04x)\n", inputChannelMask, inputChannelMask);
			fprintf(stderr, "\tU32 recordLength_samples: %d (0x%08x)\n", recordLength_samples, recordLength_samples);
			fprintf(stderr, "\tU32 fftLength_samples: %d (0x%08x)\n", fftLength_samples, fftLength_samples);
			fprintf(stderr, "\tU32 outputFormat: %d (0x%08x)\n", outputFormat, outputFormat);
			fprintf(stderr, "\tU32 footer: %d (0x%08x)\n", footer, footer);
			fprintf(stderr, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(stderr, "\tU32 * bytesPerOutputRecord: %p\n", bytesPerOutputRecord);
		}
	}
	RETURN_CODE  result = old_AlazarFFTSetup(dspHandle, inputChannelMask, recordLength_samples, fftLength_samples, outputFormat, footer, reserved, bytesPerOutputRecord);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarFFTSetScalingAndSlicing(dsp_module_handle dspHandle, U8 slice_pos, float loge_ampl_mult)
{
	old_AlazarFFTSetScalingAndSlicing = dlsym(RTLD_NEXT, "AlazarFFTSetScalingAndSlicing");
	if (last_func!=127) repeat_count=0;

	last_func = 127; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarFFTSetScalingAndSlicing()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarFFTSetScalingAndSlicing()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU8 slice_pos: %d (0x%02x)\n", slice_pos, slice_pos);
			fprintf(stderr, "\tfloat loge_ampl_mult: %g\n", (double)loge_ampl_mult);
		}
	}
	RETURN_CODE  result = old_AlazarFFTSetScalingAndSlicing(dspHandle, slice_pos, loge_ampl_mult);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetBuffer(HANDLE boardHandle, void *buffer, U32 timeout_ms)
{
	old_AlazarDSPGetBuffer = dlsym(RTLD_NEXT, "AlazarDSPGetBuffer");
	if (last_func!=128) repeat_count=0;

	last_func = 128; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetBuffer()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetBuffer()\n");
			fprintf(stderr, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetBuffer(boardHandle, buffer, timeout_ms);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetNextBuffer(HANDLE boardHandle, void *buffer, U32 bytesToCopy, U32 timeout_ms)
{
	old_AlazarDSPGetNextBuffer = dlsym(RTLD_NEXT, "AlazarDSPGetNextBuffer");
	if (last_func!=129) repeat_count=0;

	last_func = 129; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetNextBuffer()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetNextBuffer()\n");
			fprintf(stderr, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(stderr, "\tvoid * buffer: %p\n", buffer);
			fprintf(stderr, "\tU32 bytesToCopy: %d (0x%08x)\n", bytesToCopy, bytesToCopy);
			fprintf(stderr, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetNextBuffer(boardHandle, buffer, bytesToCopy, timeout_ms);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterU32(dsp_module_handle dspHandle, U32 parameter, U32 *_result)
{
	old_AlazarDSPGetParameterU32 = dlsym(RTLD_NEXT, "AlazarDSPGetParameterU32");
	if (last_func!=130) repeat_count=0;

	last_func = 130; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetParameterU32()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetParameterU32()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tU32 * _result: %p\n", _result);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetParameterU32(dspHandle, parameter, _result);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterU32(dsp_module_handle dspHandle, U32 parameter, U32 value)
{
	old_AlazarDSPSetParameterU32 = dlsym(RTLD_NEXT, "AlazarDSPSetParameterU32");
	if (last_func!=131) repeat_count=0;

	last_func = 131; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPSetParameterU32()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPSetParameterU32()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tU32 value: %d (0x%08x)\n", value, value);
		}
	}
	RETURN_CODE  result = old_AlazarDSPSetParameterU32(dspHandle, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterS32(dsp_module_handle dspHandle, U32 parameter, S32 *_result)
{
	old_AlazarDSPGetParameterS32 = dlsym(RTLD_NEXT, "AlazarDSPGetParameterS32");
	if (last_func!=132) repeat_count=0;

	last_func = 132; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetParameterS32()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetParameterS32()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tS32 * _result: %p\n", _result);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetParameterS32(dspHandle, parameter, _result);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterS32(dsp_module_handle dspHandle, U32 parameter, S32 value)
{
	old_AlazarDSPSetParameterS32 = dlsym(RTLD_NEXT, "AlazarDSPSetParameterS32");
	if (last_func!=133) repeat_count=0;

	last_func = 133; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPSetParameterS32()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPSetParameterS32()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tS32 value: %d\n", value);
		}
	}
	RETURN_CODE  result = old_AlazarDSPSetParameterS32(dspHandle, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterFloat(dsp_module_handle dspHandle, U32 parameter, float *_result)
{
	old_AlazarDSPGetParameterFloat = dlsym(RTLD_NEXT, "AlazarDSPGetParameterFloat");
	if (last_func!=134) repeat_count=0;

	last_func = 134; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPGetParameterFloat()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPGetParameterFloat()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tfloat * _result: %p\n", _result);
		}
	}
	RETURN_CODE  result = old_AlazarDSPGetParameterFloat(dspHandle, parameter, _result);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterFloat(dsp_module_handle dspHandle, U32 parameter, float value)
{
	old_AlazarDSPSetParameterFloat = dlsym(RTLD_NEXT, "AlazarDSPSetParameterFloat");
	if (last_func!=135) repeat_count=0;

	last_func = 135; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPSetParameterFloat()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPSetParameterFloat()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(stderr, "\tfloat value: %g\n", (double)value);
		}
	}
	RETURN_CODE  result = old_AlazarDSPSetParameterFloat(dspHandle, parameter, value);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPConfigureSelfTrigger(dsp_module_handle dspHandle, BOOL enable, U32 counter)
{
	old_AlazarDSPConfigureSelfTrigger = dlsym(RTLD_NEXT, "AlazarDSPConfigureSelfTrigger");
	if (last_func!=136) repeat_count=0;

	last_func = 136; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPConfigureSelfTrigger()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPConfigureSelfTrigger()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tBOOL enable: %s\n", enable?"true":"false");
			fprintf(stderr, "\tU32 counter: %d (0x%08x)\n", counter, counter);
		}
	}
	RETURN_CODE  result = old_AlazarDSPConfigureSelfTrigger(dspHandle, enable, counter);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPConfigureSampleSkipping(dsp_module_handle dspHandle, BOOL independentMode, U32 count, U16 *vector)
{
	old_AlazarDSPConfigureSampleSkipping = dlsym(RTLD_NEXT, "AlazarDSPConfigureSampleSkipping");
	if (last_func!=137) repeat_count=0;

	last_func = 137; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPConfigureSampleSkipping()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPConfigureSampleSkipping()\n");
			fprintf(stderr, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(stderr, "\tBOOL independentMode: %s\n", independentMode?"true":"false");
			fprintf(stderr, "\tU32 count: %d (0x%08x)\n", count, count);
			fprintf(stderr, "\tU16 * vector: %p\n", vector);
		}
	}
	RETURN_CODE  result = old_AlazarDSPConfigureSampleSkipping(dspHandle, independentMode, count, vector);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
RETURN_CODE AlazarDSPAbortCapture(HANDLE boardHandle)
{
	old_AlazarDSPAbortCapture = dlsym(RTLD_NEXT, "AlazarDSPAbortCapture");
	if (last_func!=138) repeat_count=0;

	last_func = 138; repeat_count++;
	if (repeat_count<=MAX_REPEAT) {
		if (repeat_count == MAX_REPEAT) {
			fprintf(stderr, "... repeated call to AlazarDSPAbortCapture()\n");
		} else {
			fprintf(stderr, "RETURN_CODE AlazarDSPAbortCapture()\n");
			fprintf(stderr, "\tHANDLE boardHandle: %p\n", boardHandle);
		}
	}
	RETURN_CODE  result = old_AlazarDSPAbortCapture(boardHandle);
	if (repeat_count<MAX_REPEAT) {
		fprintf(stderr, "\t=> %s\n", AlazarErrorToText(result));
	}
	return result;
}
