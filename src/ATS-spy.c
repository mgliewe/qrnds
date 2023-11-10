	#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>

#include "../include/AlazarApi.h"
#include "../include/AlazarDSP.h"

//#define API_SO "../lib/libATSApi.so"
#define API_SO "/lib/x86_64-linux-gnu/libATSApi.so.0"

#define LOGFILE "/tmp/ats-api.log"
#define MAX_REPEAT 4

struct api_functions {

	int show_AlazarGetOEMFPGAName;
	RETURN_CODE (*AlazarGetOEMFPGAName) (int opcodeID, char *FullPath, unsigned long *error);
	int show_AlazarOEMSetWorkingDirectory;
	RETURN_CODE (*AlazarOEMSetWorkingDirectory) (char *wDir, unsigned long *error);
	int show_AlazarOEMGetWorkingDirectory;
	RETURN_CODE (*AlazarOEMGetWorkingDirectory) (char *wDir, unsigned long *error);
	int show_AlazarParseFPGAName;
	RETURN_CODE (*AlazarParseFPGAName) (const char *FullName, char *Name, U32 *Type, U32 *MemSize, U32 *MajVer, U32 *MinVer, U32 *MajRev, U32 *MinRev, U32 *error);
	int show_AlazarDownLoadFPGA;
	RETURN_CODE (*AlazarDownLoadFPGA) (HANDLE handle, char *FileName, U32 *RetValue);
	int show_AlazarOEMDownLoadFPGA;
	RETURN_CODE (*AlazarOEMDownLoadFPGA) (HANDLE handle, char *FileName, U32 *RetValue);
	int show_AlazarReadWriteTest;
	RETURN_CODE (*AlazarReadWriteTest) (HANDLE handle, U32 *Buffer, U32 SizeToWrite, U32 SizeToRead);
	int show_AlazarMemoryTest;
	RETURN_CODE (*AlazarMemoryTest) (HANDLE handle, U32 *errors);
	int show_AlazarTriggeredFlag;
	RETURN_CODE (*AlazarTriggeredFlag) (HANDLE handle, int *TriggeredFlag);
	int show_AlazarGetSDKVersion;
	RETURN_CODE (*AlazarGetSDKVersion) (U8 *major, U8 *minor, U8 *revision);
	int show_AlazarGetDriverVersion;
	RETURN_CODE (*AlazarGetDriverVersion) (U8 *major, U8 *minor, U8 *revision);
	int show_AlazarGetBoardRevision;
	RETURN_CODE (*AlazarGetBoardRevision) (HANDLE handle, U8 *major, U8 *minor);
	int show_AlazarBoardsFound;
	U32 (*AlazarBoardsFound) (void);
	int show_AlazarOpen;
	HANDLE (*AlazarOpen) (char *boardName);
	int show_AlazarClose;
	void (*AlazarClose) (HANDLE handle);
	int show_AlazarGetBoardKind;
	U32 (*AlazarGetBoardKind) (HANDLE handle);
	int show_AlazarGetCPLDVersion;
	RETURN_CODE (*AlazarGetCPLDVersion) (HANDLE handle, U8 *major, U8 *minor);
	int show_AlazarAutoCalibrate;
	RETURN_CODE (*AlazarAutoCalibrate) (HANDLE handle);
	int show_AlazarGetChannelInfo;
	RETURN_CODE (*AlazarGetChannelInfo) (HANDLE handle, U32 *memorySize, U8 *bitsPerSample);
	int show_AlazarGetChannelInfoEx;
	RETURN_CODE (*AlazarGetChannelInfoEx) (HANDLE handle, S64 *memorySize, U8 *bitsPerSample);
	int show_AlazarInputControl;
	RETURN_CODE (*AlazarInputControl) (HANDLE handle, U8 channel, U32 coupling, U32 inputRange, U32 impedance);
	int show_AlazarInputControlEx;
	RETURN_CODE (*AlazarInputControlEx) (HANDLE handle, U32 channel, U32 couplingId, U32 rangeId, U32 impedenceId);
	int show_AlazarSetPosition;
	RETURN_CODE (*AlazarSetPosition) (HANDLE handle, U8 Channel, int PMPercent, U32 InputRange);
	int show_AlazarSetExternalTrigger;
	RETURN_CODE (*AlazarSetExternalTrigger) (HANDLE handle, U32 couplingId, U32 rangeId);
	int show_AlazarSetTriggerDelay;
	RETURN_CODE (*AlazarSetTriggerDelay) (HANDLE handle, U32 Delay);
	int show_AlazarSetTriggerTimeOut;
	RETURN_CODE (*AlazarSetTriggerTimeOut) (HANDLE handle, U32 timeout_ticks);
	int show_AlazarTriggerTimedOut;
	U32 (*AlazarTriggerTimedOut) (HANDLE h);
	int show_AlazarSetTriggerOperation;
	RETURN_CODE (*AlazarSetTriggerOperation) (HANDLE handle, U32 TriggerOperation, U32 TriggerEngine1, U32 Source1, U32 Slope1, U32 Level1, U32 TriggerEngine2, U32 Source2, U32 Slope2, U32 Level2);
	int show_AlazarGetTriggerTimestamp;
	RETURN_CODE (*AlazarGetTriggerTimestamp) (HANDLE handle, U32 Record, U64 *Timestamp_samples);
	int show_AlazarSetTriggerOperationForScanning;
	RETURN_CODE (*AlazarSetTriggerOperationForScanning) (HANDLE handle, U32 slopeId, U32 level, U32 options);
	int show_AlazarAbortCapture;
	RETURN_CODE (*AlazarAbortCapture) (HANDLE handle);
	int show_AlazarForceTrigger;
	RETURN_CODE (*AlazarForceTrigger) (HANDLE handle);
	int show_AlazarForceTriggerEnable;
	RETURN_CODE (*AlazarForceTriggerEnable) (HANDLE handle);
	int show_AlazarStartCapture;
	RETURN_CODE (*AlazarStartCapture) (HANDLE handle);
	int show_AlazarCaptureMode;
	RETURN_CODE (*AlazarCaptureMode) (HANDLE handle, U32 Mode);
	int show_AlazarStreamCapture;
	RETURN_CODE (*AlazarStreamCapture) (HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error);
	int show_AlazarHyperDisp;
	RETURN_CODE (*AlazarHyperDisp) (HANDLE handle, void *buffer, U32 bufferSize, U8 *viewBuffer, U32 viewBufferSize, U32 numOfPixels, U32 option, U32 channelSelect, U32 record, long transferOffset, U32 *error);
	int show_AlazarFastPRRCapture;
	RETURN_CODE (*AlazarFastPRRCapture) (HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error);
	int show_AlazarBusy;
	U32 (*AlazarBusy) (HANDLE handle);
	int show_AlazarTriggered;
	U32 (*AlazarTriggered) (HANDLE handle);
	int show_AlazarGetStatus;
	U32 (*AlazarGetStatus) (HANDLE handle);
	int show_AlazarDetectMultipleRecord;
	U32 (*AlazarDetectMultipleRecord) (HANDLE handle);
	int show_AlazarSetRecordCount;
	RETURN_CODE (*AlazarSetRecordCount) (HANDLE handle, U32 Count);
	int show_AlazarSetRecordSize;
	RETURN_CODE (*AlazarSetRecordSize) (HANDLE handle, U32 preTriggerSamples, U32 postTriggerSamples);
	int show_AlazarSetCaptureClock;
	RETURN_CODE (*AlazarSetCaptureClock) (HANDLE handle, U32 source, U32 sampleRateIdOrValue, U32 edgeId, U32 decimation);
	int show_AlazarSetExternalClockLevel;
	RETURN_CODE (*AlazarSetExternalClockLevel) (HANDLE handle, float level_percent);
	int show_AlazarSetClockSwitchOver;
	RETURN_CODE (*AlazarSetClockSwitchOver) (HANDLE handleBoard, U32 uMode, U32 uDummyClockOnTime_ns, U32 uReserved);
	int show_AlazarRead;
	U32 (*AlazarRead) (HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, long transferOffset, U32 transferLength);
	int show_AlazarReadEx;
	U32 (*AlazarReadEx) (HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, INT64 transferOffset, U32 transferLength);
	int show_AlazarSetParameter;
	RETURN_CODE (*AlazarSetParameter) (HANDLE handle, U8 channel, U32 parameter, long value);
	int show_AlazarSetParameterUL;
	RETURN_CODE (*AlazarSetParameterUL) (HANDLE handle, U8 channel, U32 parameter, U32 value);
	int show_AlazarSetParameterLL;
	RETURN_CODE (*AlazarSetParameterLL) (HANDLE handle, U8 channel, U32 parameter, S64 value);
	int show_AlazarGetParameter;
	RETURN_CODE (*AlazarGetParameter) (HANDLE handle, U8 channel, U32 parameter, long *retValue);
	int show_AlazarGetParameterUL;
	RETURN_CODE (*AlazarGetParameterUL) (HANDLE handle, U8 channel, U32 parameter, U32 *retValue);
	int show_AlazarGetParameterLL;
	RETURN_CODE (*AlazarGetParameterLL) (HANDLE handle, U8 channel, U32 parameter, S64 *retValue);
	int show_AlazarGetSystemHandle;
	HANDLE (*AlazarGetSystemHandle) (U32 systemId);
	int show_AlazarNumOfSystems;
	U32 (*AlazarNumOfSystems) (void);
	int show_AlazarBoardsInSystemBySystemID;
	U32 (*AlazarBoardsInSystemBySystemID) (U32 systemId);
	int show_AlazarBoardsInSystemByHandle;
	U32 (*AlazarBoardsInSystemByHandle) (HANDLE systemHandle);
	int show_AlazarGetBoardBySystemID;
	HANDLE (*AlazarGetBoardBySystemID) (U32 systemId, U32 boardId);
	int show_AlazarGetBoardBySystemHandle;
	HANDLE (*AlazarGetBoardBySystemHandle) (HANDLE systemHandle, U32 boardId);
	int show_AlazarSetLED;
	RETURN_CODE (*AlazarSetLED) (HANDLE handle, U32 state);
	int show_AlazarQueryCapability;
	RETURN_CODE (*AlazarQueryCapability) (HANDLE handle, U32 capability, U32 reserved, U32 *retValue);
	int show_AlazarQueryCapabilityLL;
	RETURN_CODE (*AlazarQueryCapabilityLL) (HANDLE handle, U32 capability, U32 reserved, S64 *retValue);
	int show_AlazarMaxSglTransfer;
	U32 (*AlazarMaxSglTransfer) (ALAZAR_BOARDTYPES bt);
	int show_AlazarGetMaxRecordsCapable;
	RETURN_CODE (*AlazarGetMaxRecordsCapable) (HANDLE handle, U32 samplesPerRecord, U32 *maxRecordsPerCapture);
	int show_AlazarGetWhoTriggeredBySystemHandle;
	U32 (*AlazarGetWhoTriggeredBySystemHandle) (HANDLE systemHandle, U32 boardId, U32 recordNumber);
	int show_AlazarGetWhoTriggeredBySystemID;
	U32 (*AlazarGetWhoTriggeredBySystemID) (U32 systemId, U32 boardId, U32 recordNumber);
	int show_AlazarSetBWLimit;
	RETURN_CODE (*AlazarSetBWLimit) (HANDLE handle, U32 channel, U32 enable);
	int show_AlazarSleepDevice;
	RETURN_CODE (*AlazarSleepDevice) (HANDLE handle, U32 sleepState);
	int show_AlazarBeforeAsyncRead;
	RETURN_CODE (*AlazarBeforeAsyncRead) (HANDLE handle, U32 channelSelect, long transferOffset, U32 transferLength, U32 recordsPerBuffer, U32 recordsPerAcquisition, U32 flags);
	int show_AlazarAbortAsyncRead;
	RETURN_CODE (*AlazarAbortAsyncRead) (HANDLE handle);
	int show_AlazarPostAsyncBuffer;
	RETURN_CODE (*AlazarPostAsyncBuffer) (HANDLE handle, void *buffer, U32 bufferLength_bytes);
	int show_AlazarWaitAsyncBufferComplete;
	RETURN_CODE (*AlazarWaitAsyncBufferComplete) (HANDLE handle, void *buffer, U32 timeout_ms);
	int show_AlazarWaitNextAsyncBufferComplete;
	RETURN_CODE (*AlazarWaitNextAsyncBufferComplete) (HANDLE handle, void *buffer, U32 bytesToCopy, U32 timeout_ms);
	int show_AlazarResetTimeStamp;
	RETURN_CODE (*AlazarResetTimeStamp) (HANDLE handle, U32 option);
	int show_AlazarReadRegister;
	RETURN_CODE (*AlazarReadRegister) (HANDLE handle, U32 offset, U32 *retVal, U32 pswrd);
	int show_AlazarWriteRegister;
	RETURN_CODE (*AlazarWriteRegister) (HANDLE handle, U32 offset, U32 Val,U32 pswrd);
	int show_ATS9462FlashSectorPageRead;
	RETURN_CODE (*ATS9462FlashSectorPageRead) (HANDLE handle, U32 address, U16 *PageBuff);
	int show_ATS9462PageWriteToFlash;
	RETURN_CODE (*ATS9462PageWriteToFlash) (HANDLE handle, U32 address, U16 *PageBuff);
	int show_ATS9462FlashSectorErase;
	RETURN_CODE (*ATS9462FlashSectorErase) (HANDLE handle, int sectorNum);
	int show_ATS9462FlashChipErase;
	RETURN_CODE (*ATS9462FlashChipErase) (HANDLE h);
	int show_SetControlCommand;
	RETURN_CODE (*SetControlCommand) (HANDLE handle, int cmd);
	int show_AlazarDACSetting;
	RETURN_CODE (*AlazarDACSetting) (HANDLE handle, U32 SetGet, U32 OriginalOrModified, U8 Channel, U32 DACNAME, U32 Coupling, U32 InputRange, U32 Impedance, U32 *getVal, U32 setVal, U32 *error);
	int show_AlazarWrite;
	RETURN_CODE (*AlazarWrite) (HANDLE handle, void *buffer, long bufLen, int DmaChannel, U32 firstPoint, U32 startAddress, U32 endAddress, BOOL waitTillEnd, U32 *error);
	int show_AlazarConfigureAuxIO;
	RETURN_CODE (*AlazarConfigureAuxIO) (HANDLE handle, U32 mode, U32 parameter);
	int show_AlazarConfigureSampleSkipping;
	RETURN_CODE (*AlazarConfigureSampleSkipping) (HANDLE handle, U32 mode, U32 sampleClocksPerRecord, U16 *sampleSkipBitmap);
	int show_AlazarCoprocessorRegisterRead;
	RETURN_CODE (*AlazarCoprocessorRegisterRead) (HANDLE handle, U32 offset, U32 *value);
	int show_AlazarCoprocessorRegisterWrite;
	RETURN_CODE (*AlazarCoprocessorRegisterWrite) (HANDLE handle, U32 offset, U32 value);
	int show_AlazarCoprocessorDownloadA;
	RETURN_CODE (*AlazarCoprocessorDownloadA) (HANDLE handle, char *fileName, U32 options);
	int show_AlazarCoprocessorDownloadBuffer;
	RETURN_CODE (*AlazarCoprocessorDownloadBuffer) (HANDLE handle, U8 *pbBuffer, U32 uBytesToWrite, U32 options);
	int show_AlazarConfigureRecordAverage;
	RETURN_CODE (*AlazarConfigureRecordAverage) (HANDLE handle, U32 mode, U32 samplesPerRecord, U32 recordsPerAverage, U32 options);
	int show_AlazarAllocBufferU8;
	U8 * (*AlazarAllocBufferU8) (HANDLE handle, U32 sampleCount);
	int show_AlazarFreeBufferU8;
	RETURN_CODE (*AlazarFreeBufferU8) (HANDLE handle, U8 *buffer);
	int show_AlazarAllocBufferU16;
	U16 * (*AlazarAllocBufferU16) (HANDLE handle, U32 sampleCount);
	int show_AlazarFreeBufferU16;
	RETURN_CODE (*AlazarFreeBufferU16) (HANDLE handle, U16 *buffer);
	int show_AlazarAllocBufferU8Ex;
	U8 * (*AlazarAllocBufferU8Ex) (HANDLE handle, U64 sampleCount);
	int show_AlazarFreeBufferU8Ex;
	RETURN_CODE (*AlazarFreeBufferU8Ex) (HANDLE handle, U8 *buffer);
	int show_AlazarAllocBufferU16Ex;
	U16 * (*AlazarAllocBufferU16Ex) (HANDLE handle, U64 sampleCount);
	int show_AlazarFreeBufferU16Ex;
	RETURN_CODE (*AlazarFreeBufferU16Ex) (HANDLE handle, U16 *buffer);
	int show_AlazarConfigureLSB;
	RETURN_CODE (*AlazarConfigureLSB) (HANDLE handle, U32 valueLsb0, U32 valueLsb1);
	int show_AlazarExtractFFTNPTFooters;
	RETURN_CODE (*AlazarExtractFFTNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
	int show_AlazarExtractTimeDomainNPTFooters;
	RETURN_CODE (*AlazarExtractTimeDomainNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
	int show_AlazarExtractNPTFooters;
	RETURN_CODE (*AlazarExtractNPTFooters) (void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract);
	int show_AlazarDisableDSP;
	RETURN_CODE (*AlazarDisableDSP) (HANDLE boardHandle);
	int show_AlazarOCTIgnoreBadClock;
	RETURN_CODE (*AlazarOCTIgnoreBadClock) (HANDLE handle, U32 enable, double goodClockDuration_seconds, double badClockDuration_seconds, double *triggerCycleTime_seconds, double *triggerPulseWidth_seconds);
	int show_AlazarSetADCBackgroundCompensation;
	RETURN_CODE (*AlazarSetADCBackgroundCompensation) (HANDLE handle, BOOL active);
	int show_AlazarDSPGetModules;
	RETURN_CODE (*AlazarDSPGetModules) (HANDLE boardHandle, U32 numEntries, dsp_module_handle *modules, U32 *numModules);
	int show_AlazarDSPGetInfo;
	RETURN_CODE (*AlazarDSPGetInfo) (dsp_module_handle dspHandle, U32 *dspModuleId, U16 *versionMajor, U16 *versionMinor, U32 *maxLength, U32 *reserved0, U32 *reserved1);
	int show_AlazarDSPGenerateWindowFunction;
	RETURN_CODE (*AlazarDSPGenerateWindowFunction) (U32 windowType, float *window, U32 windowLength_samples, U32 paddingLength_samples);
	int show_AlazarFFTGetMaxTriggerRepeatRate;
	RETURN_CODE (*AlazarFFTGetMaxTriggerRepeatRate) (dsp_module_handle dspHandle, U32 fftSize, double *maxTriggerRepeatRate);
	int show_AlazarFFTBackgroundSubtractionSetRecordS16;
	RETURN_CODE (*AlazarFFTBackgroundSubtractionSetRecordS16) (dsp_module_handle dspHandle, const S16 *record, U32 size_samples);
	int show_AlazarFFTBackgroundSubtractionGetRecordS16;
	RETURN_CODE (*AlazarFFTBackgroundSubtractionGetRecordS16) (dsp_module_handle dspHandle, S16 *backgroundRecord, U32 size_samples);
	int show_AlazarFFTBackgroundSubtractionSetEnabled;
	RETURN_CODE (*AlazarFFTBackgroundSubtractionSetEnabled) (dsp_module_handle dspHandle, BOOL enabled);
	int show_AlazarFFTSetWindowFunction;
	RETURN_CODE (*AlazarFFTSetWindowFunction) (dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray);
	int show_AlazarFFTGetWindowFunction;
	RETURN_CODE (*AlazarFFTGetWindowFunction) (dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray);
	int show_AlazarFFTVerificationMode;
	RETURN_CODE (*AlazarFFTVerificationMode) (dsp_module_handle dspHandle, BOOL enable, S16 *realArray, S16 *imagArray, size_t recordLength_samples);
	int show_AlazarFFTSetup;
	RETURN_CODE (*AlazarFFTSetup) (dsp_module_handle dspHandle, U16 inputChannelMask, U32 recordLength_samples, U32 fftLength_samples, U32 outputFormat, U32 footer, U32 reserved, U32 *bytesPerOutputRecord);
	int show_AlazarFFTSetScalingAndSlicing;
	RETURN_CODE (*AlazarFFTSetScalingAndSlicing) (dsp_module_handle dspHandle, U8 slice_pos, float loge_ampl_mult);
	int show_AlazarDSPGetBuffer;
	RETURN_CODE (*AlazarDSPGetBuffer) (HANDLE boardHandle, void *buffer, U32 timeout_ms);
	int show_AlazarDSPGetNextBuffer;
	RETURN_CODE (*AlazarDSPGetNextBuffer) (HANDLE boardHandle, void *buffer, U32 bytesToCopy, U32 timeout_ms);
	int show_AlazarDSPGetParameterU32;
	RETURN_CODE (*AlazarDSPGetParameterU32) (dsp_module_handle dspHandle, U32 parameter, U32 *_result);
	int show_AlazarDSPSetParameterU32;
	RETURN_CODE (*AlazarDSPSetParameterU32) (dsp_module_handle dspHandle, U32 parameter, U32 value);
	int show_AlazarDSPGetParameterS32;
	RETURN_CODE (*AlazarDSPGetParameterS32) (dsp_module_handle dspHandle, U32 parameter, S32 *_result);
	int show_AlazarDSPSetParameterS32;
	RETURN_CODE (*AlazarDSPSetParameterS32) (dsp_module_handle dspHandle, U32 parameter, S32 value);
	int show_AlazarDSPGetParameterFloat;
	RETURN_CODE (*AlazarDSPGetParameterFloat) (dsp_module_handle dspHandle, U32 parameter, float *_result);
	int show_AlazarDSPSetParameterFloat;
	RETURN_CODE (*AlazarDSPSetParameterFloat) (dsp_module_handle dspHandle, U32 parameter, float value);
	int show_AlazarDSPConfigureSelfTrigger;
	RETURN_CODE (*AlazarDSPConfigureSelfTrigger) (dsp_module_handle dspHandle, BOOL enable, U32 counter);
	int show_AlazarDSPConfigureSampleSkipping;
	RETURN_CODE (*AlazarDSPConfigureSampleSkipping) (dsp_module_handle dspHandle, BOOL independentMode, U32 count, U16 *vector);
	int show_AlazarDSPAbortCapture;
	RETURN_CODE (*AlazarDSPAbortCapture) (HANDLE boardHandle);
};


static struct api_functions api_functions;

static int last_func;
static int repeat_count;

static void out(const char *str) {
	write(2, str, strlen(str));
}


static void load_api_functions() {
	void *p;

	api_functions.show_AlazarGetOEMFPGAName = 0;
	p = dlsym(RTLD_NEXT, "AlazarGetOEMFPGAName");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetOEMFPGAName\n"); }
	api_functions.AlazarGetOEMFPGAName = p;

	api_functions.show_AlazarOEMSetWorkingDirectory = 1;
	p = dlsym(RTLD_NEXT, "AlazarOEMSetWorkingDirectory");
	if(!p) { out("FATAL: cannot dlsym of AlazarOEMSetWorkingDirectory\n"); }
	api_functions.AlazarOEMSetWorkingDirectory = p;

	api_functions.show_AlazarOEMGetWorkingDirectory = 1;
	p = dlsym(RTLD_NEXT, "AlazarOEMGetWorkingDirectory");
	if(!p) { out("FATAL: cannot dlsym of AlazarOEMGetWorkingDirectory\n"); }
	api_functions.AlazarOEMGetWorkingDirectory = p;

	api_functions.show_AlazarParseFPGAName = 1;
	p = dlsym(RTLD_NEXT, "AlazarParseFPGAName");
	if(!p) { out("FATAL: cannot dlsym of AlazarParseFPGAName\n"); }
	api_functions.AlazarParseFPGAName = p;

	api_functions.show_AlazarDownLoadFPGA = 1;
	p = dlsym(RTLD_NEXT, "AlazarDownLoadFPGA");
	if(!p) { out("FATAL: cannot dlsym of AlazarDownLoadFPGA\n"); }
	api_functions.AlazarDownLoadFPGA = p;

	api_functions.show_AlazarOEMDownLoadFPGA = 1;
	p = dlsym(RTLD_NEXT, "AlazarOEMDownLoadFPGA");
	if(!p) { out("FATAL: cannot dlsym of AlazarOEMDownLoadFPGA\n"); }
	api_functions.AlazarOEMDownLoadFPGA = p;

	api_functions.show_AlazarReadWriteTest = 1;
	p = dlsym(RTLD_NEXT, "AlazarReadWriteTest");
	if(!p) { out("FATAL: cannot dlsym of AlazarReadWriteTest\n"); }
	api_functions.AlazarReadWriteTest = p;

	api_functions.show_AlazarMemoryTest = 1;
	p = dlsym(RTLD_NEXT, "AlazarMemoryTest");
	if(!p) { out("FATAL: cannot dlsym of AlazarMemoryTest\n"); }
	api_functions.AlazarMemoryTest = p;

	api_functions.show_AlazarTriggeredFlag = 1;
	p = dlsym(RTLD_NEXT, "AlazarTriggeredFlag");
	if(!p) { out("FATAL: cannot dlsym of AlazarTriggeredFlag\n"); }
	api_functions.AlazarTriggeredFlag = p;

	api_functions.show_AlazarGetSDKVersion = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetSDKVersion");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetSDKVersion\n"); }
	api_functions.AlazarGetSDKVersion = p;

	api_functions.show_AlazarGetDriverVersion = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetDriverVersion");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetDriverVersion\n"); }
	api_functions.AlazarGetDriverVersion = p;

	api_functions.show_AlazarGetBoardRevision = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetBoardRevision");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetBoardRevision\n"); }
	api_functions.AlazarGetBoardRevision = p;

	api_functions.show_AlazarBoardsFound = 1;
	p = dlsym(RTLD_NEXT, "AlazarBoardsFound");
	if(!p) { out("FATAL: cannot dlsym of AlazarBoardsFound\n"); }
	api_functions.AlazarBoardsFound = p;

	api_functions.show_AlazarOpen = 1;
	p = dlsym(RTLD_NEXT, "AlazarOpen");
	if(!p) { out("FATAL: cannot dlsym of AlazarOpen\n"); }
	api_functions.AlazarOpen = p;

	api_functions.show_AlazarClose = 1;
	p = dlsym(RTLD_NEXT, "AlazarClose");
	if(!p) { out("FATAL: cannot dlsym of AlazarClose\n"); }
	api_functions.AlazarClose = p;

	api_functions.show_AlazarGetBoardKind = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetBoardKind");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetBoardKind\n"); }
	api_functions.AlazarGetBoardKind = p;

	api_functions.show_AlazarGetCPLDVersion = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetCPLDVersion");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetCPLDVersion\n"); }
	api_functions.AlazarGetCPLDVersion = p;

	api_functions.show_AlazarAutoCalibrate = 1;
	p = dlsym(RTLD_NEXT, "AlazarAutoCalibrate");
	if(!p) { out("FATAL: cannot dlsym of AlazarAutoCalibrate\n"); }
	api_functions.AlazarAutoCalibrate = p;

	api_functions.show_AlazarGetChannelInfo = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetChannelInfo");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetChannelInfo\n"); }
	api_functions.AlazarGetChannelInfo = p;

	api_functions.show_AlazarGetChannelInfoEx = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetChannelInfoEx");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetChannelInfoEx\n"); }
	api_functions.AlazarGetChannelInfoEx = p;

	api_functions.show_AlazarInputControl = 1;
	p = dlsym(RTLD_NEXT, "AlazarInputControl");
	if(!p) { out("FATAL: cannot dlsym of AlazarInputControl\n"); }
	api_functions.AlazarInputControl = p;

	api_functions.show_AlazarInputControlEx = 1;
	p = dlsym(RTLD_NEXT, "AlazarInputControlEx");
	if(!p) { out("FATAL: cannot dlsym of AlazarInputControlEx\n"); }
	api_functions.AlazarInputControlEx = p;

	api_functions.show_AlazarSetPosition = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetPosition");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetPosition\n"); }
	api_functions.AlazarSetPosition = p;

	api_functions.show_AlazarSetExternalTrigger = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetExternalTrigger");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetExternalTrigger\n"); }
	api_functions.AlazarSetExternalTrigger = p;

	api_functions.show_AlazarSetTriggerDelay = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetTriggerDelay");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetTriggerDelay\n"); }
	api_functions.AlazarSetTriggerDelay = p;

	api_functions.show_AlazarSetTriggerTimeOut = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetTriggerTimeOut");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetTriggerTimeOut\n"); }
	api_functions.AlazarSetTriggerTimeOut = p;

	api_functions.show_AlazarTriggerTimedOut = 1;
	p = dlsym(RTLD_NEXT, "AlazarTriggerTimedOut");
	if(!p) { out("FATAL: cannot dlsym of AlazarTriggerTimedOut\n"); }
	api_functions.AlazarTriggerTimedOut = p;

	api_functions.show_AlazarSetTriggerOperation = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetTriggerOperation");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetTriggerOperation\n"); }
	api_functions.AlazarSetTriggerOperation = p;

	api_functions.show_AlazarGetTriggerTimestamp = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetTriggerTimestamp");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetTriggerTimestamp\n"); }
	api_functions.AlazarGetTriggerTimestamp = p;

	api_functions.show_AlazarSetTriggerOperationForScanning = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetTriggerOperationForScanning");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetTriggerOperationForScanning\n"); }
	api_functions.AlazarSetTriggerOperationForScanning = p;

	api_functions.show_AlazarAbortCapture = 1;
	p = dlsym(RTLD_NEXT, "AlazarAbortCapture");
	if(!p) { out("FATAL: cannot dlsym of AlazarAbortCapture\n"); }
	api_functions.AlazarAbortCapture = p;

	api_functions.show_AlazarForceTrigger = 1;
	p = dlsym(RTLD_NEXT, "AlazarForceTrigger");
	if(!p) { out("FATAL: cannot dlsym of AlazarForceTrigger\n"); }
	api_functions.AlazarForceTrigger = p;

	api_functions.show_AlazarForceTriggerEnable = 1;
	p = dlsym(RTLD_NEXT, "AlazarForceTriggerEnable");
	if(!p) { out("FATAL: cannot dlsym of AlazarForceTriggerEnable\n"); }
	api_functions.AlazarForceTriggerEnable = p;

	api_functions.show_AlazarStartCapture = 1;
	p = dlsym(RTLD_NEXT, "AlazarStartCapture");
	if(!p) { out("FATAL: cannot dlsym of AlazarStartCapture\n"); }
	api_functions.AlazarStartCapture = p;

	api_functions.show_AlazarCaptureMode = 1;
	p = dlsym(RTLD_NEXT, "AlazarCaptureMode");
	if(!p) { out("FATAL: cannot dlsym of AlazarCaptureMode\n"); }
	api_functions.AlazarCaptureMode = p;

	api_functions.show_AlazarStreamCapture = 1;
	p = dlsym(RTLD_NEXT, "AlazarStreamCapture");
	if(!p) { out("FATAL: cannot dlsym of AlazarStreamCapture\n"); }
	api_functions.AlazarStreamCapture = p;

	api_functions.show_AlazarHyperDisp = 1;
	p = dlsym(RTLD_NEXT, "AlazarHyperDisp");
	if(!p) { out("FATAL: cannot dlsym of AlazarHyperDisp\n"); }
	api_functions.AlazarHyperDisp = p;

	api_functions.show_AlazarFastPRRCapture = 1;
	p = dlsym(RTLD_NEXT, "AlazarFastPRRCapture");
	if(!p) { out("FATAL: cannot dlsym of AlazarFastPRRCapture\n"); }
	api_functions.AlazarFastPRRCapture = p;

	api_functions.show_AlazarBusy = 1;
	p = dlsym(RTLD_NEXT, "AlazarBusy");
	if(!p) { out("FATAL: cannot dlsym of AlazarBusy\n"); }
	api_functions.AlazarBusy = p;

	api_functions.show_AlazarTriggered = 1;
	p = dlsym(RTLD_NEXT, "AlazarTriggered");
	if(!p) { out("FATAL: cannot dlsym of AlazarTriggered\n"); }
	api_functions.AlazarTriggered = p;

	api_functions.show_AlazarGetStatus = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetStatus");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetStatus\n"); }
	api_functions.AlazarGetStatus = p;

	api_functions.show_AlazarDetectMultipleRecord = 1;
	p = dlsym(RTLD_NEXT, "AlazarDetectMultipleRecord");
	if(!p) { out("FATAL: cannot dlsym of AlazarDetectMultipleRecord\n"); }
	api_functions.AlazarDetectMultipleRecord = p;

	api_functions.show_AlazarSetRecordCount = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetRecordCount");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetRecordCount\n"); }
	api_functions.AlazarSetRecordCount = p;

	api_functions.show_AlazarSetRecordSize = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetRecordSize");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetRecordSize\n"); }
	api_functions.AlazarSetRecordSize = p;

	api_functions.show_AlazarSetCaptureClock = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetCaptureClock");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetCaptureClock\n"); }
	api_functions.AlazarSetCaptureClock = p;

	api_functions.show_AlazarSetExternalClockLevel = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetExternalClockLevel");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetExternalClockLevel\n"); }
	api_functions.AlazarSetExternalClockLevel = p;

	api_functions.show_AlazarSetClockSwitchOver = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetClockSwitchOver");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetClockSwitchOver\n"); }
	api_functions.AlazarSetClockSwitchOver = p;

	api_functions.show_AlazarRead = 1;
	p = dlsym(RTLD_NEXT, "AlazarRead");
	if(!p) { out("FATAL: cannot dlsym of AlazarRead\n"); }
	api_functions.AlazarRead = p;

	api_functions.show_AlazarReadEx = 1;
	p = dlsym(RTLD_NEXT, "AlazarReadEx");
	if(!p) { out("FATAL: cannot dlsym of AlazarReadEx\n"); }
	api_functions.AlazarReadEx = p;

	api_functions.show_AlazarSetParameter = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetParameter");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetParameter\n"); }
	api_functions.AlazarSetParameter = p;

	api_functions.show_AlazarSetParameterUL = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetParameterUL");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetParameterUL\n"); }
	api_functions.AlazarSetParameterUL = p;

	api_functions.show_AlazarSetParameterLL = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetParameterLL");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetParameterLL\n"); }
	api_functions.AlazarSetParameterLL = p;

	api_functions.show_AlazarGetParameter = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetParameter");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetParameter\n"); }
	api_functions.AlazarGetParameter = p;

	api_functions.show_AlazarGetParameterUL = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetParameterUL");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetParameterUL\n"); }
	api_functions.AlazarGetParameterUL = p;

	api_functions.show_AlazarGetParameterLL = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetParameterLL");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetParameterLL\n"); }
	api_functions.AlazarGetParameterLL = p;

	api_functions.show_AlazarGetSystemHandle = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetSystemHandle");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetSystemHandle\n"); }
	api_functions.AlazarGetSystemHandle = p;

	api_functions.show_AlazarNumOfSystems = 1;
	p = dlsym(RTLD_NEXT, "AlazarNumOfSystems");
	if(!p) { out("FATAL: cannot dlsym of AlazarNumOfSystems\n"); }
	api_functions.AlazarNumOfSystems = p;

	api_functions.show_AlazarBoardsInSystemBySystemID = 1;
	p = dlsym(RTLD_NEXT, "AlazarBoardsInSystemBySystemID");
	if(!p) { out("FATAL: cannot dlsym of AlazarBoardsInSystemBySystemID\n"); }
	api_functions.AlazarBoardsInSystemBySystemID = p;

	api_functions.show_AlazarBoardsInSystemByHandle = 1;
	p = dlsym(RTLD_NEXT, "AlazarBoardsInSystemByHandle");
	if(!p) { out("FATAL: cannot dlsym of AlazarBoardsInSystemByHandle\n"); }
	api_functions.AlazarBoardsInSystemByHandle = p;

	api_functions.show_AlazarGetBoardBySystemID = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetBoardBySystemID");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetBoardBySystemID\n"); }
	api_functions.AlazarGetBoardBySystemID = p;

	api_functions.show_AlazarGetBoardBySystemHandle = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetBoardBySystemHandle");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetBoardBySystemHandle\n"); }
	api_functions.AlazarGetBoardBySystemHandle = p;

	api_functions.show_AlazarSetLED = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetLED");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetLED\n"); }
	api_functions.AlazarSetLED = p;

	api_functions.show_AlazarQueryCapability = 1;
	p = dlsym(RTLD_NEXT, "AlazarQueryCapability");
	if(!p) { out("FATAL: cannot dlsym of AlazarQueryCapability\n"); }
	api_functions.AlazarQueryCapability = p;

	api_functions.show_AlazarQueryCapabilityLL = 1;
	p = dlsym(RTLD_NEXT, "AlazarQueryCapabilityLL");
	if(!p) { out("FATAL: cannot dlsym of AlazarQueryCapabilityLL\n"); }
	api_functions.AlazarQueryCapabilityLL = p;

	api_functions.show_AlazarMaxSglTransfer = 1;
	p = dlsym(RTLD_NEXT, "AlazarMaxSglTransfer");
	if(!p) { out("FATAL: cannot dlsym of AlazarMaxSglTransfer\n"); }
	api_functions.AlazarMaxSglTransfer = p;

	api_functions.show_AlazarGetMaxRecordsCapable = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetMaxRecordsCapable");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetMaxRecordsCapable\n"); }
	api_functions.AlazarGetMaxRecordsCapable = p;

	api_functions.show_AlazarGetWhoTriggeredBySystemHandle = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetWhoTriggeredBySystemHandle");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetWhoTriggeredBySystemHandle\n"); }
	api_functions.AlazarGetWhoTriggeredBySystemHandle = p;

	api_functions.show_AlazarGetWhoTriggeredBySystemID = 1;
	p = dlsym(RTLD_NEXT, "AlazarGetWhoTriggeredBySystemID");
	if(!p) { out("FATAL: cannot dlsym of AlazarGetWhoTriggeredBySystemID\n"); }
	api_functions.AlazarGetWhoTriggeredBySystemID = p;

	api_functions.show_AlazarSetBWLimit = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetBWLimit");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetBWLimit\n"); }
	api_functions.AlazarSetBWLimit = p;

	api_functions.show_AlazarSleepDevice = 1;
	p = dlsym(RTLD_NEXT, "AlazarSleepDevice");
	if(!p) { out("FATAL: cannot dlsym of AlazarSleepDevice\n"); }
	api_functions.AlazarSleepDevice = p;

	api_functions.show_AlazarBeforeAsyncRead = 1;
	p = dlsym(RTLD_NEXT, "AlazarBeforeAsyncRead");
	if(!p) { out("FATAL: cannot dlsym of AlazarBeforeAsyncRead\n"); }
	api_functions.AlazarBeforeAsyncRead = p;

	api_functions.show_AlazarAbortAsyncRead = 1;
	p = dlsym(RTLD_NEXT, "AlazarAbortAsyncRead");
	if(!p) { out("FATAL: cannot dlsym of AlazarAbortAsyncRead\n"); }
	api_functions.AlazarAbortAsyncRead = p;

	api_functions.show_AlazarPostAsyncBuffer = 1;
	p = dlsym(RTLD_NEXT, "AlazarPostAsyncBuffer");
	if(!p) { out("FATAL: cannot dlsym of AlazarPostAsyncBuffer\n"); }
	api_functions.AlazarPostAsyncBuffer = p;

	api_functions.show_AlazarWaitAsyncBufferComplete = 1;
	p = dlsym(RTLD_NEXT, "AlazarWaitAsyncBufferComplete");
	if(!p) { out("FATAL: cannot dlsym of AlazarWaitAsyncBufferComplete\n"); }
	api_functions.AlazarWaitAsyncBufferComplete = p;

	api_functions.show_AlazarWaitNextAsyncBufferComplete = 1;
	p = dlsym(RTLD_NEXT, "AlazarWaitNextAsyncBufferComplete");
	if(!p) { out("FATAL: cannot dlsym of AlazarWaitNextAsyncBufferComplete\n"); }
	api_functions.AlazarWaitNextAsyncBufferComplete = p;

	api_functions.show_AlazarResetTimeStamp = 1;
	p = dlsym(RTLD_NEXT, "AlazarResetTimeStamp");
	if(!p) { out("FATAL: cannot dlsym of AlazarResetTimeStamp\n"); }
	api_functions.AlazarResetTimeStamp = p;

	api_functions.show_AlazarReadRegister = 1;
	p = dlsym(RTLD_NEXT, "AlazarReadRegister");
	if(!p) { out("FATAL: cannot dlsym of AlazarReadRegister\n"); }
	api_functions.AlazarReadRegister = p;

	api_functions.show_AlazarWriteRegister = 1;
	p = dlsym(RTLD_NEXT, "AlazarWriteRegister");
	if(!p) { out("FATAL: cannot dlsym of AlazarWriteRegister\n"); }
	api_functions.AlazarWriteRegister = p;

	api_functions.show_ATS9462FlashSectorPageRead = 1;
	p = dlsym(RTLD_NEXT, "ATS9462FlashSectorPageRead");
	if(!p) { out("FATAL: cannot dlsym of ATS9462FlashSectorPageRead\n"); }
	api_functions.ATS9462FlashSectorPageRead = p;

	api_functions.show_ATS9462PageWriteToFlash = 1;
	p = dlsym(RTLD_NEXT, "ATS9462PageWriteToFlash");
	if(!p) { out("FATAL: cannot dlsym of ATS9462PageWriteToFlash\n"); }
	api_functions.ATS9462PageWriteToFlash = p;

	api_functions.show_ATS9462FlashSectorErase = 1;
	p = dlsym(RTLD_NEXT, "ATS9462FlashSectorErase");
	if(!p) { out("FATAL: cannot dlsym of ATS9462FlashSectorErase\n"); }
	api_functions.ATS9462FlashSectorErase = p;

	api_functions.show_ATS9462FlashChipErase = 1;
	p = dlsym(RTLD_NEXT, "ATS9462FlashChipErase");
	if(!p) { out("FATAL: cannot dlsym of ATS9462FlashChipErase\n"); }
	api_functions.ATS9462FlashChipErase = p;

	api_functions.show_SetControlCommand = 1;
	p = dlsym(RTLD_NEXT, "SetControlCommand");
	if(!p) { out("FATAL: cannot dlsym of SetControlCommand\n"); }
	api_functions.SetControlCommand = p;

	api_functions.show_AlazarDACSetting = 1;
	p = dlsym(RTLD_NEXT, "AlazarDACSetting");
	if(!p) { out("FATAL: cannot dlsym of AlazarDACSetting\n"); }
	api_functions.AlazarDACSetting = p;

	api_functions.show_AlazarWrite = 1;
	p = dlsym(RTLD_NEXT, "AlazarWrite");
	if(!p) { out("FATAL: cannot dlsym of AlazarWrite\n"); }
	api_functions.AlazarWrite = p;

	api_functions.show_AlazarConfigureAuxIO = 1;
	p = dlsym(RTLD_NEXT, "AlazarConfigureAuxIO");
	if(!p) { out("FATAL: cannot dlsym of AlazarConfigureAuxIO\n"); }
	api_functions.AlazarConfigureAuxIO = p;

	api_functions.show_AlazarConfigureSampleSkipping = 1;
	p = dlsym(RTLD_NEXT, "AlazarConfigureSampleSkipping");
	if(!p) { out("FATAL: cannot dlsym of AlazarConfigureSampleSkipping\n"); }
	api_functions.AlazarConfigureSampleSkipping = p;

	api_functions.show_AlazarCoprocessorRegisterRead = 1;
	p = dlsym(RTLD_NEXT, "AlazarCoprocessorRegisterRead");
	if(!p) { out("FATAL: cannot dlsym of AlazarCoprocessorRegisterRead\n"); }
	api_functions.AlazarCoprocessorRegisterRead = p;

	api_functions.show_AlazarCoprocessorRegisterWrite = 1;
	p = dlsym(RTLD_NEXT, "AlazarCoprocessorRegisterWrite");
	if(!p) { out("FATAL: cannot dlsym of AlazarCoprocessorRegisterWrite\n"); }
	api_functions.AlazarCoprocessorRegisterWrite = p;

	api_functions.show_AlazarCoprocessorDownloadA = 1;
	p = dlsym(RTLD_NEXT, "AlazarCoprocessorDownloadA");
	if(!p) { out("FATAL: cannot dlsym of AlazarCoprocessorDownloadA\n"); }
	api_functions.AlazarCoprocessorDownloadA = p;

	api_functions.show_AlazarCoprocessorDownloadBuffer = 1;
	p = dlsym(RTLD_NEXT, "AlazarCoprocessorDownloadBuffer");
	if(!p) { out("FATAL: cannot dlsym of AlazarCoprocessorDownloadBuffer\n"); }
	api_functions.AlazarCoprocessorDownloadBuffer = p;

	api_functions.show_AlazarConfigureRecordAverage = 1;
	p = dlsym(RTLD_NEXT, "AlazarConfigureRecordAverage");
	if(!p) { out("FATAL: cannot dlsym of AlazarConfigureRecordAverage\n"); }
	api_functions.AlazarConfigureRecordAverage = p;

	api_functions.show_AlazarAllocBufferU8 = 1;
	p = dlsym(RTLD_NEXT, "AlazarAllocBufferU8");
	if(!p) { out("FATAL: cannot dlsym of AlazarAllocBufferU8\n"); }
	api_functions.AlazarAllocBufferU8 = p;

	api_functions.show_AlazarFreeBufferU8 = 1;
	p = dlsym(RTLD_NEXT, "AlazarFreeBufferU8");
	if(!p) { out("FATAL: cannot dlsym of AlazarFreeBufferU8\n"); }
	api_functions.AlazarFreeBufferU8 = p;

	api_functions.show_AlazarAllocBufferU16 = 1;
	p = dlsym(RTLD_NEXT, "AlazarAllocBufferU16");
	if(!p) { out("FATAL: cannot dlsym of AlazarAllocBufferU16\n"); }
	api_functions.AlazarAllocBufferU16 = p;

	api_functions.show_AlazarFreeBufferU16 = 1;
	p = dlsym(RTLD_NEXT, "AlazarFreeBufferU16");
	if(!p) { out("FATAL: cannot dlsym of AlazarFreeBufferU16\n"); }
	api_functions.AlazarFreeBufferU16 = p;

	api_functions.show_AlazarAllocBufferU8Ex = 1;
	p = dlsym(RTLD_NEXT, "AlazarAllocBufferU8Ex");
	if(!p) { out("FATAL: cannot dlsym of AlazarAllocBufferU8Ex\n"); }
	api_functions.AlazarAllocBufferU8Ex = p;

	api_functions.show_AlazarFreeBufferU8Ex = 1;
	p = dlsym(RTLD_NEXT, "AlazarFreeBufferU8Ex");
	if(!p) { out("FATAL: cannot dlsym of AlazarFreeBufferU8Ex\n"); }
	api_functions.AlazarFreeBufferU8Ex = p;

	api_functions.show_AlazarAllocBufferU16Ex = 1;
	p = dlsym(RTLD_NEXT, "AlazarAllocBufferU16Ex");
	if(!p) { out("FATAL: cannot dlsym of AlazarAllocBufferU16Ex\n"); }
	api_functions.AlazarAllocBufferU16Ex = p;

	api_functions.show_AlazarFreeBufferU16Ex = 1;
	p = dlsym(RTLD_NEXT, "AlazarFreeBufferU16Ex");
	if(!p) { out("FATAL: cannot dlsym of AlazarFreeBufferU16Ex\n"); }
	api_functions.AlazarFreeBufferU16Ex = p;

	api_functions.show_AlazarConfigureLSB = 1;
	p = dlsym(RTLD_NEXT, "AlazarConfigureLSB");
	if(!p) { out("FATAL: cannot dlsym of AlazarConfigureLSB\n"); }
	api_functions.AlazarConfigureLSB = p;

	api_functions.show_AlazarExtractFFTNPTFooters = 1;
	p = dlsym(RTLD_NEXT, "AlazarExtractFFTNPTFooters");
	if(!p) { out("FATAL: cannot dlsym of AlazarExtractFFTNPTFooters\n"); }
	api_functions.AlazarExtractFFTNPTFooters = p;

	api_functions.show_AlazarExtractTimeDomainNPTFooters = 1;
	p = dlsym(RTLD_NEXT, "AlazarExtractTimeDomainNPTFooters");
	if(!p) { out("FATAL: cannot dlsym of AlazarExtractTimeDomainNPTFooters\n"); }
	api_functions.AlazarExtractTimeDomainNPTFooters = p;

	api_functions.show_AlazarExtractNPTFooters = 1;
	p = dlsym(RTLD_NEXT, "AlazarExtractNPTFooters");
	if(!p) { out("FATAL: cannot dlsym of AlazarExtractNPTFooters\n"); }
	api_functions.AlazarExtractNPTFooters = p;

	api_functions.show_AlazarDisableDSP = 1;
	p = dlsym(RTLD_NEXT, "AlazarDisableDSP");
	if(!p) { out("FATAL: cannot dlsym of AlazarDisableDSP\n"); }
	api_functions.AlazarDisableDSP = p;

	api_functions.show_AlazarOCTIgnoreBadClock = 1;
	p = dlsym(RTLD_NEXT, "AlazarOCTIgnoreBadClock");
	if(!p) { out("FATAL: cannot dlsym of AlazarOCTIgnoreBadClock\n"); }
	api_functions.AlazarOCTIgnoreBadClock = p;

	api_functions.show_AlazarSetADCBackgroundCompensation = 1;
	p = dlsym(RTLD_NEXT, "AlazarSetADCBackgroundCompensation");
	if(!p) { out("FATAL: cannot dlsym of AlazarSetADCBackgroundCompensation\n"); }
	api_functions.AlazarSetADCBackgroundCompensation = p;

	api_functions.show_AlazarDSPGetModules = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetModules");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetModules\n"); }
	api_functions.AlazarDSPGetModules = p;

	api_functions.show_AlazarDSPGetInfo = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetInfo");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetInfo\n"); }
	api_functions.AlazarDSPGetInfo = p;

	api_functions.show_AlazarDSPGenerateWindowFunction = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGenerateWindowFunction");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGenerateWindowFunction\n"); }
	api_functions.AlazarDSPGenerateWindowFunction = p;

	api_functions.show_AlazarFFTGetMaxTriggerRepeatRate = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTGetMaxTriggerRepeatRate");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTGetMaxTriggerRepeatRate\n"); }
	api_functions.AlazarFFTGetMaxTriggerRepeatRate = p;

	api_functions.show_AlazarFFTBackgroundSubtractionSetRecordS16 = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionSetRecordS16");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTBackgroundSubtractionSetRecordS16\n"); }
	api_functions.AlazarFFTBackgroundSubtractionSetRecordS16 = p;

	api_functions.show_AlazarFFTBackgroundSubtractionGetRecordS16 = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionGetRecordS16");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTBackgroundSubtractionGetRecordS16\n"); }
	api_functions.AlazarFFTBackgroundSubtractionGetRecordS16 = p;

	api_functions.show_AlazarFFTBackgroundSubtractionSetEnabled = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTBackgroundSubtractionSetEnabled");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTBackgroundSubtractionSetEnabled\n"); }
	api_functions.AlazarFFTBackgroundSubtractionSetEnabled = p;

	api_functions.show_AlazarFFTSetWindowFunction = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTSetWindowFunction");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTSetWindowFunction\n"); }
	api_functions.AlazarFFTSetWindowFunction = p;

	api_functions.show_AlazarFFTGetWindowFunction = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTGetWindowFunction");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTGetWindowFunction\n"); }
	api_functions.AlazarFFTGetWindowFunction = p;

	api_functions.show_AlazarFFTVerificationMode = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTVerificationMode");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTVerificationMode\n"); }
	api_functions.AlazarFFTVerificationMode = p;

	api_functions.show_AlazarFFTSetup = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTSetup");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTSetup\n"); }
	api_functions.AlazarFFTSetup = p;

	api_functions.show_AlazarFFTSetScalingAndSlicing = 1;
	p = dlsym(RTLD_NEXT, "AlazarFFTSetScalingAndSlicing");
	if(!p) { out("FATAL: cannot dlsym of AlazarFFTSetScalingAndSlicing\n"); }
	api_functions.AlazarFFTSetScalingAndSlicing = p;

	api_functions.show_AlazarDSPGetBuffer = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetBuffer");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetBuffer\n"); }
	api_functions.AlazarDSPGetBuffer = p;

	api_functions.show_AlazarDSPGetNextBuffer = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetNextBuffer");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetNextBuffer\n"); }
	api_functions.AlazarDSPGetNextBuffer = p;

	api_functions.show_AlazarDSPGetParameterU32 = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetParameterU32");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetParameterU32\n"); }
	api_functions.AlazarDSPGetParameterU32 = p;

	api_functions.show_AlazarDSPSetParameterU32 = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPSetParameterU32");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPSetParameterU32\n"); }
	api_functions.AlazarDSPSetParameterU32 = p;

	api_functions.show_AlazarDSPGetParameterS32 = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetParameterS32");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetParameterS32\n"); }
	api_functions.AlazarDSPGetParameterS32 = p;

	api_functions.show_AlazarDSPSetParameterS32 = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPSetParameterS32");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPSetParameterS32\n"); }
	api_functions.AlazarDSPSetParameterS32 = p;

	api_functions.show_AlazarDSPGetParameterFloat = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPGetParameterFloat");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPGetParameterFloat\n"); }
	api_functions.AlazarDSPGetParameterFloat = p;

	api_functions.show_AlazarDSPSetParameterFloat = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPSetParameterFloat");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPSetParameterFloat\n"); }
	api_functions.AlazarDSPSetParameterFloat = p;

	api_functions.show_AlazarDSPConfigureSelfTrigger = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPConfigureSelfTrigger");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPConfigureSelfTrigger\n"); }
	api_functions.AlazarDSPConfigureSelfTrigger = p;

	api_functions.show_AlazarDSPConfigureSampleSkipping = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPConfigureSampleSkipping");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPConfigureSampleSkipping\n"); }
	api_functions.AlazarDSPConfigureSampleSkipping = p;

	api_functions.show_AlazarDSPAbortCapture = 1;
	p = dlsym(RTLD_NEXT, "AlazarDSPAbortCapture");
	if(!p) { out("FATAL: cannot dlsym of AlazarDSPAbortCapture\n"); }
	api_functions.AlazarDSPAbortCapture = p;

}

RETURN_CODE AlazarGetOEMFPGAName(int opcodeID, char *FullPath, unsigned long *error)
{
	if (last_func!=1) repeat_count=0;

	last_func = 1; repeat_count++;
	if (api_functions.show_AlazarGetOEMFPGAName && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetOEMFPGAName()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetOEMFPGAName()\n");
			fprintf(out, "\tint opcodeID: %d\n", opcodeID);
			fprintf(out, "\tchar * FullPath: %p\n", FullPath);
			fprintf(out, "\tunsigned long * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetOEMFPGAName(opcodeID, FullPath, error);
	if (api_functions.show_AlazarGetOEMFPGAName && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarOEMSetWorkingDirectory(char *wDir, unsigned long *error)
{
	if (last_func!=2) repeat_count=0;

	last_func = 2; repeat_count++;
	if (api_functions.show_AlazarOEMSetWorkingDirectory && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarOEMSetWorkingDirectory()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarOEMSetWorkingDirectory()\n");
			fprintf(out, "\tchar * wDir: %p\n", wDir);
			fprintf(out, "\tunsigned long * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarOEMSetWorkingDirectory(wDir, error);
	if (api_functions.show_AlazarOEMSetWorkingDirectory && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarOEMGetWorkingDirectory(char *wDir, unsigned long *error)
{
	if (last_func!=3) repeat_count=0;

	last_func = 3; repeat_count++;
	if (api_functions.show_AlazarOEMGetWorkingDirectory && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarOEMGetWorkingDirectory()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarOEMGetWorkingDirectory()\n");
			fprintf(out, "\tchar * wDir: %p\n", wDir);
			fprintf(out, "\tunsigned long * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarOEMGetWorkingDirectory(wDir, error);
	if (api_functions.show_AlazarOEMGetWorkingDirectory && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarParseFPGAName(const char *FullName, char *Name, U32 *Type, U32 *MemSize, U32 *MajVer, U32 *MinVer, U32 *MajRev, U32 *MinRev, U32 *error)
{
	if (last_func!=4) repeat_count=0;

	last_func = 4; repeat_count++;
	if (api_functions.show_AlazarParseFPGAName && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarParseFPGAName()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarParseFPGAName()\n");
			fprintf(out, "\tconst char * FullName: %p\n", FullName);
			fprintf(out, "\tchar * Name: %p\n", Name);
			fprintf(out, "\tU32 * Type: %p\n", Type);
			fprintf(out, "\tU32 * MemSize: %p\n", MemSize);
			fprintf(out, "\tU32 * MajVer: %p\n", MajVer);
			fprintf(out, "\tU32 * MinVer: %p\n", MinVer);
			fprintf(out, "\tU32 * MajRev: %p\n", MajRev);
			fprintf(out, "\tU32 * MinRev: %p\n", MinRev);
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarParseFPGAName(FullName, Name, Type, MemSize, MajVer, MinVer, MajRev, MinRev, error);
	if (api_functions.show_AlazarParseFPGAName && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDownLoadFPGA(HANDLE handle, char *FileName, U32 *RetValue)
{
	if (last_func!=5) repeat_count=0;

	last_func = 5; repeat_count++;
	if (api_functions.show_AlazarDownLoadFPGA && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDownLoadFPGA()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDownLoadFPGA()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tchar * FileName: %p\n", FileName);
			fprintf(out, "\tU32 * RetValue: %p\n", RetValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDownLoadFPGA(handle, FileName, RetValue);
	if (api_functions.show_AlazarDownLoadFPGA && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarOEMDownLoadFPGA(HANDLE handle, char *FileName, U32 *RetValue)
{
	if (last_func!=6) repeat_count=0;

	last_func = 6; repeat_count++;
	if (api_functions.show_AlazarOEMDownLoadFPGA && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarOEMDownLoadFPGA()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarOEMDownLoadFPGA()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tchar * FileName: %p\n", FileName);
			fprintf(out, "\tU32 * RetValue: %p\n", RetValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarOEMDownLoadFPGA(handle, FileName, RetValue);
	if (api_functions.show_AlazarOEMDownLoadFPGA && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarReadWriteTest(HANDLE handle, U32 *Buffer, U32 SizeToWrite, U32 SizeToRead)
{
	if (last_func!=7) repeat_count=0;

	last_func = 7; repeat_count++;
	if (api_functions.show_AlazarReadWriteTest && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarReadWriteTest()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarReadWriteTest()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 * Buffer: %p\n", Buffer);
			fprintf(out, "\tU32 SizeToWrite: %d (0x%08x)\n", SizeToWrite, SizeToWrite);
			fprintf(out, "\tU32 SizeToRead: %d (0x%08x)\n", SizeToRead, SizeToRead);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarReadWriteTest(handle, Buffer, SizeToWrite, SizeToRead);
	if (api_functions.show_AlazarReadWriteTest && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarMemoryTest(HANDLE handle, U32 *errors)
{
	if (last_func!=8) repeat_count=0;

	last_func = 8; repeat_count++;
	if (api_functions.show_AlazarMemoryTest && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarMemoryTest()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarMemoryTest()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 * errors: %p\n", errors);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarMemoryTest(handle, errors);
	if (api_functions.show_AlazarMemoryTest && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarTriggeredFlag(HANDLE handle, int *TriggeredFlag)
{
	if (last_func!=10) repeat_count=0;

	last_func = 10; repeat_count++;
	if (api_functions.show_AlazarTriggeredFlag && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarTriggeredFlag()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarTriggeredFlag()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tint * TriggeredFlag: %p\n", TriggeredFlag);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarTriggeredFlag(handle, TriggeredFlag);
	if (api_functions.show_AlazarTriggeredFlag && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetSDKVersion(U8 *major, U8 *minor, U8 *revision)
{
	if (last_func!=11) repeat_count=0;

	last_func = 11; repeat_count++;
	if (api_functions.show_AlazarGetSDKVersion && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetSDKVersion()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetSDKVersion()\n");
			fprintf(out, "\tU8 * major: %p\n", major);
			fprintf(out, "\tU8 * minor: %p\n", minor);
			fprintf(out, "\tU8 * revision: %p\n", revision);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetSDKVersion(major, minor, revision);
	if (api_functions.show_AlazarGetSDKVersion && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetDriverVersion(U8 *major, U8 *minor, U8 *revision)
{
	if (last_func!=12) repeat_count=0;

	last_func = 12; repeat_count++;
	if (api_functions.show_AlazarGetDriverVersion && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetDriverVersion()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetDriverVersion()\n");
			fprintf(out, "\tU8 * major: %p\n", major);
			fprintf(out, "\tU8 * minor: %p\n", minor);
			fprintf(out, "\tU8 * revision: %p\n", revision);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetDriverVersion(major, minor, revision);
	if (api_functions.show_AlazarGetDriverVersion && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetBoardRevision(HANDLE handle, U8 *major, U8 *minor)
{
	if (last_func!=13) repeat_count=0;

	last_func = 13; repeat_count++;
	if (api_functions.show_AlazarGetBoardRevision && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetBoardRevision()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetBoardRevision()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 * major: %p\n", major);
			fprintf(out, "\tU8 * minor: %p\n", minor);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetBoardRevision(handle, major, minor);
	if (api_functions.show_AlazarGetBoardRevision && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarBoardsFound(void)
{
	if (last_func!=14) repeat_count=0;

	last_func = 14; repeat_count++;
	if (api_functions.show_AlazarBoardsFound && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarBoardsFound()\n");
		} else {
			fprintf(out, "U32 AlazarBoardsFound()\n");
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarBoardsFound();
	if (api_functions.show_AlazarBoardsFound && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
HANDLE AlazarOpen(char *boardName)
{
	if (last_func!=15) repeat_count=0;

	last_func = 15; repeat_count++;
	if (api_functions.show_AlazarOpen && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarOpen()\n");
		} else {
			fprintf(out, "HANDLE AlazarOpen()\n");
			fprintf(out, "\tchar * boardName: %p\n", boardName);
		}
		fclose(out);
	}
	HANDLE  result = api_functions.AlazarOpen(boardName);
	if (api_functions.show_AlazarOpen && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
void AlazarClose(HANDLE handle)
{
	if (last_func!=16) repeat_count=0;

	last_func = 16; repeat_count++;
	if (api_functions.show_AlazarClose && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarClose()\n");
		} else {
			fprintf(out, "void AlazarClose()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	api_functions.AlazarClose(handle);
}
U32 AlazarGetBoardKind(HANDLE handle)
{
	if (last_func!=17) repeat_count=0;

	last_func = 17; repeat_count++;
	if (api_functions.show_AlazarGetBoardKind && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetBoardKind()\n");
		} else {
			fprintf(out, "U32 AlazarGetBoardKind()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarGetBoardKind(handle);
	if (api_functions.show_AlazarGetBoardKind && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetCPLDVersion(HANDLE handle, U8 *major, U8 *minor)
{
	if (last_func!=18) repeat_count=0;

	last_func = 18; repeat_count++;
	if (api_functions.show_AlazarGetCPLDVersion && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetCPLDVersion()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetCPLDVersion()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 * major: %p\n", major);
			fprintf(out, "\tU8 * minor: %p\n", minor);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetCPLDVersion(handle, major, minor);
	if (api_functions.show_AlazarGetCPLDVersion && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarAutoCalibrate(HANDLE handle)
{
	if (last_func!=19) repeat_count=0;

	last_func = 19; repeat_count++;
	if (api_functions.show_AlazarAutoCalibrate && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAutoCalibrate()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarAutoCalibrate()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarAutoCalibrate(handle);
	if (api_functions.show_AlazarAutoCalibrate && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetChannelInfo(HANDLE handle, U32 *memorySize, U8 *bitsPerSample)
{
	if (last_func!=20) repeat_count=0;

	last_func = 20; repeat_count++;
	if (api_functions.show_AlazarGetChannelInfo && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetChannelInfo()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetChannelInfo()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 * memorySize: %p\n", memorySize);
			fprintf(out, "\tU8 * bitsPerSample: %p\n", bitsPerSample);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetChannelInfo(handle, memorySize, bitsPerSample);
	if (api_functions.show_AlazarGetChannelInfo && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetChannelInfoEx(HANDLE handle, S64 *memorySize, U8 *bitsPerSample)
{
	if (last_func!=21) repeat_count=0;

	last_func = 21; repeat_count++;
	if (api_functions.show_AlazarGetChannelInfoEx && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetChannelInfoEx()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetChannelInfoEx()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tS64 * memorySize: %p\n", memorySize);
			fprintf(out, "\tU8 * bitsPerSample: %p\n", bitsPerSample);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetChannelInfoEx(handle, memorySize, bitsPerSample);
	if (api_functions.show_AlazarGetChannelInfoEx && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarInputControl(HANDLE handle, U8 channel, U32 coupling, U32 inputRange, U32 impedance)
{
	if (last_func!=22) repeat_count=0;

	last_func = 22; repeat_count++;
	if (api_functions.show_AlazarInputControl && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarInputControl()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarInputControl()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 coupling: %d (0x%08x)\n", coupling, coupling);
			fprintf(out, "\tU32 inputRange: %d (0x%08x)\n", inputRange, inputRange);
			fprintf(out, "\tU32 impedance: %d (0x%08x)\n", impedance, impedance);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarInputControl(handle, channel, coupling, inputRange, impedance);
	if (api_functions.show_AlazarInputControl && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarInputControlEx(HANDLE handle, U32 channel, U32 couplingId, U32 rangeId, U32 impedenceId)
{
	if (last_func!=23) repeat_count=0;

	last_func = 23; repeat_count++;
	if (api_functions.show_AlazarInputControlEx && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarInputControlEx()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarInputControlEx()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 channel: %d (0x%08x)\n", channel, channel);
			fprintf(out, "\tU32 couplingId: %d (0x%08x)\n", couplingId, couplingId);
			fprintf(out, "\tU32 rangeId: %d (0x%08x)\n", rangeId, rangeId);
			fprintf(out, "\tU32 impedenceId: %d (0x%08x)\n", impedenceId, impedenceId);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarInputControlEx(handle, channel, couplingId, rangeId, impedenceId);
	if (api_functions.show_AlazarInputControlEx && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetPosition(HANDLE handle, U8 Channel, int PMPercent, U32 InputRange)
{
	if (last_func!=24) repeat_count=0;

	last_func = 24; repeat_count++;
	if (api_functions.show_AlazarSetPosition && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetPosition()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetPosition()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 Channel: %d (0x%02x)\n", Channel, Channel);
			fprintf(out, "\tint PMPercent: %d\n", PMPercent);
			fprintf(out, "\tU32 InputRange: %d (0x%08x)\n", InputRange, InputRange);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetPosition(handle, Channel, PMPercent, InputRange);
	if (api_functions.show_AlazarSetPosition && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetExternalTrigger(HANDLE handle, U32 couplingId, U32 rangeId)
{
	if (last_func!=25) repeat_count=0;

	last_func = 25; repeat_count++;
	if (api_functions.show_AlazarSetExternalTrigger && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetExternalTrigger()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetExternalTrigger()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 couplingId: %d (0x%08x)\n", couplingId, couplingId);
			fprintf(out, "\tU32 rangeId: %d (0x%08x)\n", rangeId, rangeId);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetExternalTrigger(handle, couplingId, rangeId);
	if (api_functions.show_AlazarSetExternalTrigger && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetTriggerDelay(HANDLE handle, U32 Delay)
{
	if (last_func!=26) repeat_count=0;

	last_func = 26; repeat_count++;
	if (api_functions.show_AlazarSetTriggerDelay && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetTriggerDelay()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetTriggerDelay()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 Delay: %d (0x%08x)\n", Delay, Delay);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetTriggerDelay(handle, Delay);
	if (api_functions.show_AlazarSetTriggerDelay && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetTriggerTimeOut(HANDLE handle, U32 timeout_ticks)
{
	if (last_func!=27) repeat_count=0;

	last_func = 27; repeat_count++;
	if (api_functions.show_AlazarSetTriggerTimeOut && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetTriggerTimeOut()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetTriggerTimeOut()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 timeout_ticks: %d (0x%08x)\n", timeout_ticks, timeout_ticks);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetTriggerTimeOut(handle, timeout_ticks);
	if (api_functions.show_AlazarSetTriggerTimeOut && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarTriggerTimedOut(HANDLE h)
{
	if (last_func!=28) repeat_count=0;

	last_func = 28; repeat_count++;
	if (api_functions.show_AlazarTriggerTimedOut && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarTriggerTimedOut()\n");
		} else {
			fprintf(out, "U32 AlazarTriggerTimedOut()\n");
			fprintf(out, "\tHANDLE h: %p\n", h);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarTriggerTimedOut(h);
	if (api_functions.show_AlazarTriggerTimedOut && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetTriggerOperation(HANDLE handle, U32 TriggerOperation, U32 TriggerEngine1, U32 Source1, U32 Slope1, U32 Level1, U32 TriggerEngine2, U32 Source2, U32 Slope2, U32 Level2)
{
	if (last_func!=30) repeat_count=0;

	last_func = 30; repeat_count++;
	if (api_functions.show_AlazarSetTriggerOperation && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetTriggerOperation()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetTriggerOperation()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 TriggerOperation: %d (0x%08x)\n", TriggerOperation, TriggerOperation);
			fprintf(out, "\tU32 TriggerEngine1: %d (0x%08x)\n", TriggerEngine1, TriggerEngine1);
			fprintf(out, "\tU32 Source1: %d (0x%08x)\n", Source1, Source1);
			fprintf(out, "\tU32 Slope1: %d (0x%08x)\n", Slope1, Slope1);
			fprintf(out, "\tU32 Level1: %d (0x%08x)\n", Level1, Level1);
			fprintf(out, "\tU32 TriggerEngine2: %d (0x%08x)\n", TriggerEngine2, TriggerEngine2);
			fprintf(out, "\tU32 Source2: %d (0x%08x)\n", Source2, Source2);
			fprintf(out, "\tU32 Slope2: %d (0x%08x)\n", Slope2, Slope2);
			fprintf(out, "\tU32 Level2: %d (0x%08x)\n", Level2, Level2);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetTriggerOperation(handle, TriggerOperation, TriggerEngine1, Source1, Slope1, Level1, TriggerEngine2, Source2, Slope2, Level2);
	if (api_functions.show_AlazarSetTriggerOperation && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetTriggerTimestamp(HANDLE handle, U32 Record, U64 *Timestamp_samples)
{
	if (last_func!=31) repeat_count=0;

	last_func = 31; repeat_count++;
	if (api_functions.show_AlazarGetTriggerTimestamp && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetTriggerTimestamp()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetTriggerTimestamp()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 Record: %d (0x%08x)\n", Record, Record);
			fprintf(out, "\tU64 * Timestamp_samples: %p\n", Timestamp_samples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetTriggerTimestamp(handle, Record, Timestamp_samples);
	if (api_functions.show_AlazarGetTriggerTimestamp && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetTriggerOperationForScanning(HANDLE handle, U32 slopeId, U32 level, U32 options)
{
	if (last_func!=32) repeat_count=0;

	last_func = 32; repeat_count++;
	if (api_functions.show_AlazarSetTriggerOperationForScanning && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetTriggerOperationForScanning()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetTriggerOperationForScanning()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 slopeId: %d (0x%08x)\n", slopeId, slopeId);
			fprintf(out, "\tU32 level: %d (0x%08x)\n", level, level);
			fprintf(out, "\tU32 options: %d (0x%08x)\n", options, options);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetTriggerOperationForScanning(handle, slopeId, level, options);
	if (api_functions.show_AlazarSetTriggerOperationForScanning && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarAbortCapture(HANDLE handle)
{
	if (last_func!=33) repeat_count=0;

	last_func = 33; repeat_count++;
	if (api_functions.show_AlazarAbortCapture && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAbortCapture()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarAbortCapture()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarAbortCapture(handle);
	if (api_functions.show_AlazarAbortCapture && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarForceTrigger(HANDLE handle)
{
	if (last_func!=34) repeat_count=0;

	last_func = 34; repeat_count++;
	if (api_functions.show_AlazarForceTrigger && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarForceTrigger()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarForceTrigger()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarForceTrigger(handle);
	if (api_functions.show_AlazarForceTrigger && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarForceTriggerEnable(HANDLE handle)
{
	if (last_func!=35) repeat_count=0;

	last_func = 35; repeat_count++;
	if (api_functions.show_AlazarForceTriggerEnable && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarForceTriggerEnable()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarForceTriggerEnable()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarForceTriggerEnable(handle);
	if (api_functions.show_AlazarForceTriggerEnable && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarStartCapture(HANDLE handle)
{
	if (last_func!=36) repeat_count=0;

	last_func = 36; repeat_count++;
	if (api_functions.show_AlazarStartCapture && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarStartCapture()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarStartCapture()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarStartCapture(handle);
	if (api_functions.show_AlazarStartCapture && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarCaptureMode(HANDLE handle, U32 Mode)
{
	if (last_func!=37) repeat_count=0;

	last_func = 37; repeat_count++;
	if (api_functions.show_AlazarCaptureMode && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarCaptureMode()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarCaptureMode()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 Mode: %d (0x%08x)\n", Mode, Mode);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarCaptureMode(handle, Mode);
	if (api_functions.show_AlazarCaptureMode && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarStreamCapture(HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error)
{
	if (last_func!=38) repeat_count=0;

	last_func = 38; repeat_count++;
	if (api_functions.show_AlazarStreamCapture && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarStreamCapture()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarStreamCapture()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * Buffer: %p\n", Buffer);
			fprintf(out, "\tU32 BufferSize: %d (0x%08x)\n", BufferSize, BufferSize);
			fprintf(out, "\tU32 DeviceOption: %d (0x%08x)\n", DeviceOption, DeviceOption);
			fprintf(out, "\tU32 ChannelSelect: %d (0x%08x)\n", ChannelSelect, ChannelSelect);
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarStreamCapture(handle, Buffer, BufferSize, DeviceOption, ChannelSelect, error);
	if (api_functions.show_AlazarStreamCapture && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarHyperDisp(HANDLE handle, void *buffer, U32 bufferSize, U8 *viewBuffer, U32 viewBufferSize, U32 numOfPixels, U32 option, U32 channelSelect, U32 record, long transferOffset, U32 *error)
{
	if (last_func!=39) repeat_count=0;

	last_func = 39; repeat_count++;
	if (api_functions.show_AlazarHyperDisp && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarHyperDisp()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarHyperDisp()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 bufferSize: %d (0x%08x)\n", bufferSize, bufferSize);
			fprintf(out, "\tU8 * viewBuffer: %p\n", viewBuffer);
			fprintf(out, "\tU32 viewBufferSize: %d (0x%08x)\n", viewBufferSize, viewBufferSize);
			fprintf(out, "\tU32 numOfPixels: %d (0x%08x)\n", numOfPixels, numOfPixels);
			fprintf(out, "\tU32 option: %d (0x%08x)\n", option, option);
			fprintf(out, "\tU32 channelSelect: %d (0x%08x)\n", channelSelect, channelSelect);
			fprintf(out, "\tU32 record: %d (0x%08x)\n", record, record);
			fprintf(out, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarHyperDisp(handle, buffer, bufferSize, viewBuffer, viewBufferSize, numOfPixels, option, channelSelect, record, transferOffset, error);
	if (api_functions.show_AlazarHyperDisp && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFastPRRCapture(HANDLE handle, void *Buffer, U32 BufferSize, U32 DeviceOption, U32 ChannelSelect, U32 *error)
{
	if (last_func!=40) repeat_count=0;

	last_func = 40; repeat_count++;
	if (api_functions.show_AlazarFastPRRCapture && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFastPRRCapture()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFastPRRCapture()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * Buffer: %p\n", Buffer);
			fprintf(out, "\tU32 BufferSize: %d (0x%08x)\n", BufferSize, BufferSize);
			fprintf(out, "\tU32 DeviceOption: %d (0x%08x)\n", DeviceOption, DeviceOption);
			fprintf(out, "\tU32 ChannelSelect: %d (0x%08x)\n", ChannelSelect, ChannelSelect);
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFastPRRCapture(handle, Buffer, BufferSize, DeviceOption, ChannelSelect, error);
	if (api_functions.show_AlazarFastPRRCapture && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarBusy(HANDLE handle)
{
	if (last_func!=41) repeat_count=0;

	last_func = 41; repeat_count++;
	if (api_functions.show_AlazarBusy && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarBusy()\n");
		} else {
			fprintf(out, "U32 AlazarBusy()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarBusy(handle);
	if (api_functions.show_AlazarBusy && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarTriggered(HANDLE handle)
{
	if (last_func!=42) repeat_count=0;

	last_func = 42; repeat_count++;
	if (api_functions.show_AlazarTriggered && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarTriggered()\n");
		} else {
			fprintf(out, "U32 AlazarTriggered()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarTriggered(handle);
	if (api_functions.show_AlazarTriggered && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarGetStatus(HANDLE handle)
{
	if (last_func!=43) repeat_count=0;

	last_func = 43; repeat_count++;
	if (api_functions.show_AlazarGetStatus && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetStatus()\n");
		} else {
			fprintf(out, "U32 AlazarGetStatus()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarGetStatus(handle);
	if (api_functions.show_AlazarGetStatus && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarDetectMultipleRecord(HANDLE handle)
{
	if (last_func!=44) repeat_count=0;

	last_func = 44; repeat_count++;
	if (api_functions.show_AlazarDetectMultipleRecord && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDetectMultipleRecord()\n");
		} else {
			fprintf(out, "U32 AlazarDetectMultipleRecord()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarDetectMultipleRecord(handle);
	if (api_functions.show_AlazarDetectMultipleRecord && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetRecordCount(HANDLE handle, U32 Count)
{
	if (last_func!=45) repeat_count=0;

	last_func = 45; repeat_count++;
	if (api_functions.show_AlazarSetRecordCount && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetRecordCount()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetRecordCount()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 Count: %d (0x%08x)\n", Count, Count);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetRecordCount(handle, Count);
	if (api_functions.show_AlazarSetRecordCount && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetRecordSize(HANDLE handle, U32 preTriggerSamples, U32 postTriggerSamples)
{
	if (last_func!=46) repeat_count=0;

	last_func = 46; repeat_count++;
	if (api_functions.show_AlazarSetRecordSize && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetRecordSize()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetRecordSize()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 preTriggerSamples: %d (0x%08x)\n", preTriggerSamples, preTriggerSamples);
			fprintf(out, "\tU32 postTriggerSamples: %d (0x%08x)\n", postTriggerSamples, postTriggerSamples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetRecordSize(handle, preTriggerSamples, postTriggerSamples);
	if (api_functions.show_AlazarSetRecordSize && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetCaptureClock(HANDLE handle, U32 source, U32 sampleRateIdOrValue, U32 edgeId, U32 decimation)
{
	if (last_func!=47) repeat_count=0;

	last_func = 47; repeat_count++;
	if (api_functions.show_AlazarSetCaptureClock && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetCaptureClock()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetCaptureClock()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 source: %d (0x%08x)\n", source, source);
			fprintf(out, "\tU32 sampleRateIdOrValue: %d (0x%08x)\n", sampleRateIdOrValue, sampleRateIdOrValue);
			fprintf(out, "\tU32 edgeId: %d (0x%08x)\n", edgeId, edgeId);
			fprintf(out, "\tU32 decimation: %d (0x%08x)\n", decimation, decimation);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetCaptureClock(handle, source, sampleRateIdOrValue, edgeId, decimation);
	if (api_functions.show_AlazarSetCaptureClock && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetExternalClockLevel(HANDLE handle, float level_percent)
{
	if (last_func!=48) repeat_count=0;

	last_func = 48; repeat_count++;
	if (api_functions.show_AlazarSetExternalClockLevel && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetExternalClockLevel()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetExternalClockLevel()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tfloat level_percent: %g\n", (double)level_percent);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetExternalClockLevel(handle, level_percent);
	if (api_functions.show_AlazarSetExternalClockLevel && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetClockSwitchOver(HANDLE handleBoard, U32 uMode, U32 uDummyClockOnTime_ns, U32 uReserved)
{
	if (last_func!=49) repeat_count=0;

	last_func = 49; repeat_count++;
	if (api_functions.show_AlazarSetClockSwitchOver && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetClockSwitchOver()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetClockSwitchOver()\n");
			fprintf(out, "\tHANDLE handleBoard: %p\n", handleBoard);
			fprintf(out, "\tU32 uMode: %d (0x%08x)\n", uMode, uMode);
			fprintf(out, "\tU32 uDummyClockOnTime_ns: %d (0x%08x)\n", uDummyClockOnTime_ns, uDummyClockOnTime_ns);
			fprintf(out, "\tU32 uReserved: %d (0x%08x)\n", uReserved, uReserved);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetClockSwitchOver(handleBoard, uMode, uDummyClockOnTime_ns, uReserved);
	if (api_functions.show_AlazarSetClockSwitchOver && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarRead(HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, long transferOffset, U32 transferLength)
{
	if (last_func!=50) repeat_count=0;

	last_func = 50; repeat_count++;
	if (api_functions.show_AlazarRead && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarRead()\n");
		} else {
			fprintf(out, "U32 AlazarRead()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 channelId: %d (0x%08x)\n", channelId, channelId);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tint elementSize: %d\n", elementSize);
			fprintf(out, "\tlong record: %ld\n", record);
			fprintf(out, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(out, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarRead(handle, channelId, buffer, elementSize, record, transferOffset, transferLength);
	if (api_functions.show_AlazarRead && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarReadEx(HANDLE handle, U32 channelId, void *buffer, int elementSize, long record, INT64 transferOffset, U32 transferLength)
{
	if (last_func!=51) repeat_count=0;

	last_func = 51; repeat_count++;
	if (api_functions.show_AlazarReadEx && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarReadEx()\n");
		} else {
			fprintf(out, "U32 AlazarReadEx()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 channelId: %d (0x%08x)\n", channelId, channelId);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tint elementSize: %d\n", elementSize);
			fprintf(out, "\tlong record: %ld\n", record);
			fprintf(out, "\tINT64 transferOffset: %lld (0x%08llx)\n", transferOffset, transferOffset);
			fprintf(out, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarReadEx(handle, channelId, buffer, elementSize, record, transferOffset, transferLength);
	if (api_functions.show_AlazarReadEx && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetParameter(HANDLE handle, U8 channel, U32 parameter, long value)
{
	if (last_func!=52) repeat_count=0;

	last_func = 52; repeat_count++;
	if (api_functions.show_AlazarSetParameter && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetParameter()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetParameter()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tlong value: %ld\n", value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetParameter(handle, channel, parameter, value);
	if (api_functions.show_AlazarSetParameter && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetParameterUL(HANDLE handle, U8 channel, U32 parameter, U32 value)
{
	if (last_func!=53) repeat_count=0;

	last_func = 53; repeat_count++;
	if (api_functions.show_AlazarSetParameterUL && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetParameterUL()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetParameterUL()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tU32 value: %d (0x%08x)\n", value, value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetParameterUL(handle, channel, parameter, value);
	if (api_functions.show_AlazarSetParameterUL && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetParameterLL(HANDLE handle, U8 channel, U32 parameter, S64 value)
{
	if (last_func!=54) repeat_count=0;

	last_func = 54; repeat_count++;
	if (api_functions.show_AlazarSetParameterLL && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetParameterLL()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetParameterLL()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tS64 value: %lld (0x%08llx)\n", value, value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetParameterLL(handle, channel, parameter, value);
	if (api_functions.show_AlazarSetParameterLL && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetParameter(HANDLE handle, U8 channel, U32 parameter, long *retValue)
{
	if (last_func!=55) repeat_count=0;

	last_func = 55; repeat_count++;
	if (api_functions.show_AlazarGetParameter && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetParameter()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetParameter()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tlong * retValue: %p\n", retValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetParameter(handle, channel, parameter, retValue);
	if (api_functions.show_AlazarGetParameter && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetParameterUL(HANDLE handle, U8 channel, U32 parameter, U32 *retValue)
{
	if (last_func!=56) repeat_count=0;

	last_func = 56; repeat_count++;
	if (api_functions.show_AlazarGetParameterUL && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetParameterUL()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetParameterUL()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tU32 * retValue: %p\n", retValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetParameterUL(handle, channel, parameter, retValue);
	if (api_functions.show_AlazarGetParameterUL && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetParameterLL(HANDLE handle, U8 channel, U32 parameter, S64 *retValue)
{
	if (last_func!=57) repeat_count=0;

	last_func = 57; repeat_count++;
	if (api_functions.show_AlazarGetParameterLL && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetParameterLL()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetParameterLL()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 channel: %d (0x%02x)\n", channel, channel);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tS64 * retValue: %p\n", retValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetParameterLL(handle, channel, parameter, retValue);
	if (api_functions.show_AlazarGetParameterLL && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
HANDLE AlazarGetSystemHandle(U32 systemId)
{
	if (last_func!=58) repeat_count=0;

	last_func = 58; repeat_count++;
	if (api_functions.show_AlazarGetSystemHandle && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetSystemHandle()\n");
		} else {
			fprintf(out, "HANDLE AlazarGetSystemHandle()\n");
			fprintf(out, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
		}
		fclose(out);
	}
	HANDLE  result = api_functions.AlazarGetSystemHandle(systemId);
	if (api_functions.show_AlazarGetSystemHandle && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
U32 AlazarNumOfSystems(void)
{
	if (last_func!=59) repeat_count=0;

	last_func = 59; repeat_count++;
	if (api_functions.show_AlazarNumOfSystems && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarNumOfSystems()\n");
		} else {
			fprintf(out, "U32 AlazarNumOfSystems()\n");
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarNumOfSystems();
	if (api_functions.show_AlazarNumOfSystems && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarBoardsInSystemBySystemID(U32 systemId)
{
	if (last_func!=60) repeat_count=0;

	last_func = 60; repeat_count++;
	if (api_functions.show_AlazarBoardsInSystemBySystemID && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarBoardsInSystemBySystemID()\n");
		} else {
			fprintf(out, "U32 AlazarBoardsInSystemBySystemID()\n");
			fprintf(out, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarBoardsInSystemBySystemID(systemId);
	if (api_functions.show_AlazarBoardsInSystemBySystemID && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarBoardsInSystemByHandle(HANDLE systemHandle)
{
	if (last_func!=61) repeat_count=0;

	last_func = 61; repeat_count++;
	if (api_functions.show_AlazarBoardsInSystemByHandle && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarBoardsInSystemByHandle()\n");
		} else {
			fprintf(out, "U32 AlazarBoardsInSystemByHandle()\n");
			fprintf(out, "\tHANDLE systemHandle: %p\n", systemHandle);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarBoardsInSystemByHandle(systemHandle);
	if (api_functions.show_AlazarBoardsInSystemByHandle && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
HANDLE AlazarGetBoardBySystemID(U32 systemId, U32 boardId)
{
	if (last_func!=62) repeat_count=0;

	last_func = 62; repeat_count++;
	if (api_functions.show_AlazarGetBoardBySystemID && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetBoardBySystemID()\n");
		} else {
			fprintf(out, "HANDLE AlazarGetBoardBySystemID()\n");
			fprintf(out, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
			fprintf(out, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
		}
		fclose(out);
	}
	HANDLE  result = api_functions.AlazarGetBoardBySystemID(systemId, boardId);
	if (api_functions.show_AlazarGetBoardBySystemID && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
HANDLE AlazarGetBoardBySystemHandle(HANDLE systemHandle, U32 boardId)
{
	if (last_func!=63) repeat_count=0;

	last_func = 63; repeat_count++;
	if (api_functions.show_AlazarGetBoardBySystemHandle && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetBoardBySystemHandle()\n");
		} else {
			fprintf(out, "HANDLE AlazarGetBoardBySystemHandle()\n");
			fprintf(out, "\tHANDLE systemHandle: %p\n", systemHandle);
			fprintf(out, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
		}
		fclose(out);
	}
	HANDLE  result = api_functions.AlazarGetBoardBySystemHandle(systemHandle, boardId);
	if (api_functions.show_AlazarGetBoardBySystemHandle && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetLED(HANDLE handle, U32 state)
{
	if (last_func!=64) repeat_count=0;

	last_func = 64; repeat_count++;
	if (api_functions.show_AlazarSetLED && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetLED()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetLED()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 state: %d (0x%08x)\n", state, state);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetLED(handle, state);
	if (api_functions.show_AlazarSetLED && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarQueryCapability(HANDLE handle, U32 capability, U32 reserved, U32 *retValue)
{
	if (last_func!=65) repeat_count=0;

	last_func = 65; repeat_count++;
	if (api_functions.show_AlazarQueryCapability && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarQueryCapability()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarQueryCapability()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 capability: %d (0x%08x)\n", capability, capability);
			fprintf(out, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(out, "\tU32 * retValue: %p\n", retValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarQueryCapability(handle, capability, reserved, retValue);
	if (api_functions.show_AlazarQueryCapability && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarQueryCapabilityLL(HANDLE handle, U32 capability, U32 reserved, S64 *retValue)
{
	if (last_func!=66) repeat_count=0;

	last_func = 66; repeat_count++;
	if (api_functions.show_AlazarQueryCapabilityLL && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarQueryCapabilityLL()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarQueryCapabilityLL()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 capability: %d (0x%08x)\n", capability, capability);
			fprintf(out, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(out, "\tS64 * retValue: %p\n", retValue);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarQueryCapabilityLL(handle, capability, reserved, retValue);
	if (api_functions.show_AlazarQueryCapabilityLL && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarMaxSglTransfer(ALAZAR_BOARDTYPES bt)
{
	if (last_func!=67) repeat_count=0;

	last_func = 67; repeat_count++;
	if (api_functions.show_AlazarMaxSglTransfer && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarMaxSglTransfer()\n");
		} else {
			fprintf(out, "U32 AlazarMaxSglTransfer()\n");
			fprintf(out, "\tALAZAR_BOARDTYPES bt: %d (0x%02x)\n", bt, bt);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarMaxSglTransfer(bt);
	if (api_functions.show_AlazarMaxSglTransfer && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarGetMaxRecordsCapable(HANDLE handle, U32 samplesPerRecord, U32 *maxRecordsPerCapture)
{
	if (last_func!=68) repeat_count=0;

	last_func = 68; repeat_count++;
	if (api_functions.show_AlazarGetMaxRecordsCapable && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetMaxRecordsCapable()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarGetMaxRecordsCapable()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(out, "\tU32 * maxRecordsPerCapture: %p\n", maxRecordsPerCapture);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarGetMaxRecordsCapable(handle, samplesPerRecord, maxRecordsPerCapture);
	if (api_functions.show_AlazarGetMaxRecordsCapable && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U32 AlazarGetWhoTriggeredBySystemHandle(HANDLE systemHandle, U32 boardId, U32 recordNumber)
{
	if (last_func!=69) repeat_count=0;

	last_func = 69; repeat_count++;
	if (api_functions.show_AlazarGetWhoTriggeredBySystemHandle && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetWhoTriggeredBySystemHandle()\n");
		} else {
			fprintf(out, "U32 AlazarGetWhoTriggeredBySystemHandle()\n");
			fprintf(out, "\tHANDLE systemHandle: %p\n", systemHandle);
			fprintf(out, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
			fprintf(out, "\tU32 recordNumber: %d (0x%08x)\n", recordNumber, recordNumber);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarGetWhoTriggeredBySystemHandle(systemHandle, boardId, recordNumber);
	if (api_functions.show_AlazarGetWhoTriggeredBySystemHandle && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
U32 AlazarGetWhoTriggeredBySystemID(U32 systemId, U32 boardId, U32 recordNumber)
{
	if (last_func!=70) repeat_count=0;

	last_func = 70; repeat_count++;
	if (api_functions.show_AlazarGetWhoTriggeredBySystemID && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarGetWhoTriggeredBySystemID()\n");
		} else {
			fprintf(out, "U32 AlazarGetWhoTriggeredBySystemID()\n");
			fprintf(out, "\tU32 systemId: %d (0x%08x)\n", systemId, systemId);
			fprintf(out, "\tU32 boardId: %d (0x%08x)\n", boardId, boardId);
			fprintf(out, "\tU32 recordNumber: %d (0x%08x)\n", recordNumber, recordNumber);
		}
		fclose(out);
	}
	U32  result = api_functions.AlazarGetWhoTriggeredBySystemID(systemId, boardId, recordNumber);
	if (api_functions.show_AlazarGetWhoTriggeredBySystemID && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %d (0x%08x)\n", result, result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetBWLimit(HANDLE handle, U32 channel, U32 enable)
{
	if (last_func!=71) repeat_count=0;

	last_func = 71; repeat_count++;
	if (api_functions.show_AlazarSetBWLimit && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetBWLimit()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetBWLimit()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 channel: %d (0x%08x)\n", channel, channel);
			fprintf(out, "\tU32 enable: %d (0x%08x)\n", enable, enable);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetBWLimit(handle, channel, enable);
	if (api_functions.show_AlazarSetBWLimit && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSleepDevice(HANDLE handle, U32 sleepState)
{
	if (last_func!=72) repeat_count=0;

	last_func = 72; repeat_count++;
	if (api_functions.show_AlazarSleepDevice && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSleepDevice()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSleepDevice()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 sleepState: %d (0x%08x)\n", sleepState, sleepState);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSleepDevice(handle, sleepState);
	if (api_functions.show_AlazarSleepDevice && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarBeforeAsyncRead(HANDLE handle, U32 channelSelect, long transferOffset, U32 transferLength, U32 recordsPerBuffer, U32 recordsPerAcquisition, U32 flags)
{
	if (last_func!=73) repeat_count=0;

	last_func = 73; repeat_count++;
	if (api_functions.show_AlazarBeforeAsyncRead && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarBeforeAsyncRead()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarBeforeAsyncRead()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 channelSelect: %d (0x%08x)\n", channelSelect, channelSelect);
			fprintf(out, "\tlong transferOffset: %ld\n", transferOffset);
			fprintf(out, "\tU32 transferLength: %d (0x%08x)\n", transferLength, transferLength);
			fprintf(out, "\tU32 recordsPerBuffer: %d (0x%08x)\n", recordsPerBuffer, recordsPerBuffer);
			fprintf(out, "\tU32 recordsPerAcquisition: %d (0x%08x)\n", recordsPerAcquisition, recordsPerAcquisition);
			fprintf(out, "\tU32 flags: %d (0x%08x)\n", flags, flags);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarBeforeAsyncRead(handle, channelSelect, transferOffset, transferLength, recordsPerBuffer, recordsPerAcquisition, flags);
	if (api_functions.show_AlazarBeforeAsyncRead && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarAbortAsyncRead(HANDLE handle)
{
	if (last_func!=74) repeat_count=0;

	last_func = 74; repeat_count++;
	if (api_functions.show_AlazarAbortAsyncRead && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAbortAsyncRead()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarAbortAsyncRead()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarAbortAsyncRead(handle);
	if (api_functions.show_AlazarAbortAsyncRead && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarPostAsyncBuffer(HANDLE handle, void *buffer, U32 bufferLength_bytes)
{
	if (last_func!=75) repeat_count=0;

	last_func = 75; repeat_count++;
	if (api_functions.show_AlazarPostAsyncBuffer && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarPostAsyncBuffer()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarPostAsyncBuffer()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 bufferLength_bytes: %d (0x%08x)\n", bufferLength_bytes, bufferLength_bytes);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarPostAsyncBuffer(handle, buffer, bufferLength_bytes);
	if (api_functions.show_AlazarPostAsyncBuffer && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarWaitAsyncBufferComplete(HANDLE handle, void *buffer, U32 timeout_ms)
{
	if (last_func!=76) repeat_count=0;

	last_func = 76; repeat_count++;
	if (api_functions.show_AlazarWaitAsyncBufferComplete && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarWaitAsyncBufferComplete()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarWaitAsyncBufferComplete()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarWaitAsyncBufferComplete(handle, buffer, timeout_ms);
	if (api_functions.show_AlazarWaitAsyncBufferComplete && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarWaitNextAsyncBufferComplete(HANDLE handle, void *buffer, U32 bytesToCopy, U32 timeout_ms)
{
	if (last_func!=77) repeat_count=0;

	last_func = 77; repeat_count++;
	if (api_functions.show_AlazarWaitNextAsyncBufferComplete && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarWaitNextAsyncBufferComplete()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarWaitNextAsyncBufferComplete()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 bytesToCopy: %d (0x%08x)\n", bytesToCopy, bytesToCopy);
			fprintf(out, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarWaitNextAsyncBufferComplete(handle, buffer, bytesToCopy, timeout_ms);
	if (api_functions.show_AlazarWaitNextAsyncBufferComplete && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarResetTimeStamp(HANDLE handle, U32 option)
{
	if (last_func!=79) repeat_count=0;

	last_func = 79; repeat_count++;
	if (api_functions.show_AlazarResetTimeStamp && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarResetTimeStamp()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarResetTimeStamp()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 option: %d (0x%08x)\n", option, option);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarResetTimeStamp(handle, option);
	if (api_functions.show_AlazarResetTimeStamp && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarReadRegister(HANDLE handle, U32 offset, U32 *retVal, U32 pswrd)
{
	if (last_func!=80) repeat_count=0;

	last_func = 80; repeat_count++;
	if (api_functions.show_AlazarReadRegister && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarReadRegister()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarReadRegister()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(out, "\tU32 * retVal: %p\n", retVal);
			fprintf(out, "\tU32 pswrd: %d (0x%08x)\n", pswrd, pswrd);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarReadRegister(handle, offset, retVal, pswrd);
	if (api_functions.show_AlazarReadRegister && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarWriteRegister(HANDLE handle, U32 offset, U32 Val,U32 pswrd)
{
	if (last_func!=81) repeat_count=0;

	last_func = 81; repeat_count++;
	if (api_functions.show_AlazarWriteRegister && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarWriteRegister()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarWriteRegister()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(out, "\tU32 Val: %d (0x%08x)\n", Val, Val);
			fprintf(out, "\tU32 pswrd: %d (0x%08x)\n", pswrd, pswrd);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarWriteRegister(handle, offset, Val, pswrd);
	if (api_functions.show_AlazarWriteRegister && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE ATS9462FlashSectorPageRead(HANDLE handle, U32 address, U16 *PageBuff)
{
	if (last_func!=86) repeat_count=0;

	last_func = 86; repeat_count++;
	if (api_functions.show_ATS9462FlashSectorPageRead && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to ATS9462FlashSectorPageRead()\n");
		} else {
			fprintf(out, "RETURN_CODE ATS9462FlashSectorPageRead()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 address: %d (0x%08x)\n", address, address);
			fprintf(out, "\tU16 * PageBuff: %p\n", PageBuff);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.ATS9462FlashSectorPageRead(handle, address, PageBuff);
	if (api_functions.show_ATS9462FlashSectorPageRead && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE ATS9462PageWriteToFlash(HANDLE handle, U32 address, U16 *PageBuff)
{
	if (last_func!=87) repeat_count=0;

	last_func = 87; repeat_count++;
	if (api_functions.show_ATS9462PageWriteToFlash && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to ATS9462PageWriteToFlash()\n");
		} else {
			fprintf(out, "RETURN_CODE ATS9462PageWriteToFlash()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 address: %d (0x%08x)\n", address, address);
			fprintf(out, "\tU16 * PageBuff: %p\n", PageBuff);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.ATS9462PageWriteToFlash(handle, address, PageBuff);
	if (api_functions.show_ATS9462PageWriteToFlash && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE ATS9462FlashSectorErase(HANDLE handle, int sectorNum)
{
	if (last_func!=88) repeat_count=0;

	last_func = 88; repeat_count++;
	if (api_functions.show_ATS9462FlashSectorErase && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to ATS9462FlashSectorErase()\n");
		} else {
			fprintf(out, "RETURN_CODE ATS9462FlashSectorErase()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tint sectorNum: %d\n", sectorNum);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.ATS9462FlashSectorErase(handle, sectorNum);
	if (api_functions.show_ATS9462FlashSectorErase && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE ATS9462FlashChipErase(HANDLE h)
{
	if (last_func!=89) repeat_count=0;

	last_func = 89; repeat_count++;
	if (api_functions.show_ATS9462FlashChipErase && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to ATS9462FlashChipErase()\n");
		} else {
			fprintf(out, "RETURN_CODE ATS9462FlashChipErase()\n");
			fprintf(out, "\tHANDLE h: %p\n", h);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.ATS9462FlashChipErase(h);
	if (api_functions.show_ATS9462FlashChipErase && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE SetControlCommand(HANDLE handle, int cmd)
{
	if (last_func!=90) repeat_count=0;

	last_func = 90; repeat_count++;
	if (api_functions.show_SetControlCommand && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to SetControlCommand()\n");
		} else {
			fprintf(out, "RETURN_CODE SetControlCommand()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tint cmd: %d\n", cmd);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.SetControlCommand(handle, cmd);
	if (api_functions.show_SetControlCommand && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDACSetting(HANDLE handle, U32 SetGet, U32 OriginalOrModified, U8 Channel, U32 DACNAME, U32 Coupling, U32 InputRange, U32 Impedance, U32 *getVal, U32 setVal, U32 *error)
{
	if (last_func!=91) repeat_count=0;

	last_func = 91; repeat_count++;
	if (api_functions.show_AlazarDACSetting && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDACSetting()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDACSetting()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 SetGet: %d (0x%08x)\n", SetGet, SetGet);
			fprintf(out, "\tU32 OriginalOrModified: %d (0x%08x)\n", OriginalOrModified, OriginalOrModified);
			fprintf(out, "\tU8 Channel: %d (0x%02x)\n", Channel, Channel);
			fprintf(out, "\tU32 DACNAME: %d (0x%08x)\n", DACNAME, DACNAME);
			fprintf(out, "\tU32 Coupling: %d (0x%08x)\n", Coupling, Coupling);
			fprintf(out, "\tU32 InputRange: %d (0x%08x)\n", InputRange, InputRange);
			fprintf(out, "\tU32 Impedance: %d (0x%08x)\n", Impedance, Impedance);
			fprintf(out, "\tU32 * getVal: %p\n", getVal);
			fprintf(out, "\tU32 setVal: %d (0x%08x)\n", setVal, setVal);
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDACSetting(handle, SetGet, OriginalOrModified, Channel, DACNAME, Coupling, InputRange, Impedance, getVal, setVal, error);
	if (api_functions.show_AlazarDACSetting && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarWrite(HANDLE handle, void *buffer, long bufLen, int DmaChannel, U32 firstPoint, U32 startAddress, U32 endAddress, BOOL waitTillEnd, U32 *error)
{
	if (last_func!=92) repeat_count=0;

	last_func = 92; repeat_count++;
	if (api_functions.show_AlazarWrite && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarWrite()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarWrite()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tlong bufLen: %ld\n", bufLen);
			fprintf(out, "\tint DmaChannel: %d\n", DmaChannel);
			fprintf(out, "\tU32 firstPoint: %d (0x%08x)\n", firstPoint, firstPoint);
			fprintf(out, "\tU32 startAddress: %d (0x%08x)\n", startAddress, startAddress);
			fprintf(out, "\tU32 endAddress: %d (0x%08x)\n", endAddress, endAddress);
			fprintf(out, "\tBOOL waitTillEnd: %s\n", waitTillEnd?"true":"false");
			fprintf(out, "\tU32 * error: %p\n", error);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarWrite(handle, buffer, bufLen, DmaChannel, firstPoint, startAddress, endAddress, waitTillEnd, error);
	if (api_functions.show_AlazarWrite && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarConfigureAuxIO(HANDLE handle, U32 mode, U32 parameter)
{
	if (last_func!=93) repeat_count=0;

	last_func = 93; repeat_count++;
	if (api_functions.show_AlazarConfigureAuxIO && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarConfigureAuxIO()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarConfigureAuxIO()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarConfigureAuxIO(handle, mode, parameter);
	if (api_functions.show_AlazarConfigureAuxIO && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarConfigureSampleSkipping(HANDLE handle, U32 mode, U32 sampleClocksPerRecord, U16 *sampleSkipBitmap)
{
	if (last_func!=95) repeat_count=0;

	last_func = 95; repeat_count++;
	if (api_functions.show_AlazarConfigureSampleSkipping && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarConfigureSampleSkipping()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarConfigureSampleSkipping()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(out, "\tU32 sampleClocksPerRecord: %d (0x%08x)\n", sampleClocksPerRecord, sampleClocksPerRecord);
			fprintf(out, "\tU16 * sampleSkipBitmap: %p\n", sampleSkipBitmap);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarConfigureSampleSkipping(handle, mode, sampleClocksPerRecord, sampleSkipBitmap);
	if (api_functions.show_AlazarConfigureSampleSkipping && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarCoprocessorRegisterRead(HANDLE handle, U32 offset, U32 *value)
{
	if (last_func!=96) repeat_count=0;

	last_func = 96; repeat_count++;
	if (api_functions.show_AlazarCoprocessorRegisterRead && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarCoprocessorRegisterRead()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarCoprocessorRegisterRead()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(out, "\tU32 * value: %p\n", value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarCoprocessorRegisterRead(handle, offset, value);
	if (api_functions.show_AlazarCoprocessorRegisterRead && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarCoprocessorRegisterWrite(HANDLE handle, U32 offset, U32 value)
{
	if (last_func!=97) repeat_count=0;

	last_func = 97; repeat_count++;
	if (api_functions.show_AlazarCoprocessorRegisterWrite && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarCoprocessorRegisterWrite()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarCoprocessorRegisterWrite()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 offset: %d (0x%08x)\n", offset, offset);
			fprintf(out, "\tU32 value: %d (0x%08x)\n", value, value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarCoprocessorRegisterWrite(handle, offset, value);
	if (api_functions.show_AlazarCoprocessorRegisterWrite && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarCoprocessorDownloadA(HANDLE handle, char *fileName, U32 options)
{
	if (last_func!=98) repeat_count=0;

	last_func = 98; repeat_count++;
	if (api_functions.show_AlazarCoprocessorDownloadA && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarCoprocessorDownloadA()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarCoprocessorDownloadA()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tchar * fileName: %p\n", fileName);
			fprintf(out, "\tU32 options: %d (0x%08x)\n", options, options);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarCoprocessorDownloadA(handle, fileName, options);
	if (api_functions.show_AlazarCoprocessorDownloadA && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarCoprocessorDownloadBuffer(HANDLE handle, U8 *pbBuffer, U32 uBytesToWrite, U32 options)
{
	if (last_func!=99) repeat_count=0;

	last_func = 99; repeat_count++;
	if (api_functions.show_AlazarCoprocessorDownloadBuffer && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarCoprocessorDownloadBuffer()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarCoprocessorDownloadBuffer()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 * pbBuffer: %p\n", pbBuffer);
			fprintf(out, "\tU32 uBytesToWrite: %d (0x%08x)\n", uBytesToWrite, uBytesToWrite);
			fprintf(out, "\tU32 options: %d (0x%08x)\n", options, options);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarCoprocessorDownloadBuffer(handle, pbBuffer, uBytesToWrite, options);
	if (api_functions.show_AlazarCoprocessorDownloadBuffer && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarConfigureRecordAverage(HANDLE handle, U32 mode, U32 samplesPerRecord, U32 recordsPerAverage, U32 options)
{
	if (last_func!=100) repeat_count=0;

	last_func = 100; repeat_count++;
	if (api_functions.show_AlazarConfigureRecordAverage && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarConfigureRecordAverage()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarConfigureRecordAverage()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 mode: %d (0x%08x)\n", mode, mode);
			fprintf(out, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(out, "\tU32 recordsPerAverage: %d (0x%08x)\n", recordsPerAverage, recordsPerAverage);
			fprintf(out, "\tU32 options: %d (0x%08x)\n", options, options);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarConfigureRecordAverage(handle, mode, samplesPerRecord, recordsPerAverage, options);
	if (api_functions.show_AlazarConfigureRecordAverage && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U8  *AlazarAllocBufferU8(HANDLE handle, U32 sampleCount)
{
	if (last_func!=101) repeat_count=0;

	last_func = 101; repeat_count++;
	if (api_functions.show_AlazarAllocBufferU8 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAllocBufferU8()\n");
		} else {
			fprintf(out, "U8 AlazarAllocBufferU8()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 sampleCount: %d (0x%08x)\n", sampleCount, sampleCount);
		}
		fclose(out);
	}
	U8  * result = api_functions.AlazarAllocBufferU8(handle, sampleCount);
	if (api_functions.show_AlazarAllocBufferU8 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU8(HANDLE handle, U8 *buffer)
{
	if (last_func!=102) repeat_count=0;

	last_func = 102; repeat_count++;
	if (api_functions.show_AlazarFreeBufferU8 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFreeBufferU8()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFreeBufferU8()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 * buffer: %p\n", buffer);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFreeBufferU8(handle, buffer);
	if (api_functions.show_AlazarFreeBufferU8 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U16  *AlazarAllocBufferU16(HANDLE handle, U32 sampleCount)
{
	if (last_func!=103) repeat_count=0;

	last_func = 103; repeat_count++;
	if (api_functions.show_AlazarAllocBufferU16 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAllocBufferU16()\n");
		} else {
			fprintf(out, "U16 AlazarAllocBufferU16()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 sampleCount: %d (0x%08x)\n", sampleCount, sampleCount);
		}
		fclose(out);
	}
	U16  * result = api_functions.AlazarAllocBufferU16(handle, sampleCount);
	if (api_functions.show_AlazarAllocBufferU16 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU16(HANDLE handle, U16 *buffer)
{
	if (last_func!=104) repeat_count=0;

	last_func = 104; repeat_count++;
	if (api_functions.show_AlazarFreeBufferU16 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFreeBufferU16()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFreeBufferU16()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU16 * buffer: %p\n", buffer);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFreeBufferU16(handle, buffer);
	if (api_functions.show_AlazarFreeBufferU16 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U8  *AlazarAllocBufferU8Ex(HANDLE handle, U64 sampleCount)
{
	if (last_func!=105) repeat_count=0;

	last_func = 105; repeat_count++;
	if (api_functions.show_AlazarAllocBufferU8Ex && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAllocBufferU8Ex()\n");
		} else {
			fprintf(out, "U8 AlazarAllocBufferU8Ex()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU64 sampleCount: %lld (0x%08llx)\n", sampleCount, sampleCount);
		}
		fclose(out);
	}
	U8  * result = api_functions.AlazarAllocBufferU8Ex(handle, sampleCount);
	if (api_functions.show_AlazarAllocBufferU8Ex && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU8Ex(HANDLE handle, U8 *buffer)
{
	if (last_func!=106) repeat_count=0;

	last_func = 106; repeat_count++;
	if (api_functions.show_AlazarFreeBufferU8Ex && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFreeBufferU8Ex()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFreeBufferU8Ex()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU8 * buffer: %p\n", buffer);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFreeBufferU8Ex(handle, buffer);
	if (api_functions.show_AlazarFreeBufferU8Ex && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
U16  *AlazarAllocBufferU16Ex(HANDLE handle, U64 sampleCount)
{
	if (last_func!=107) repeat_count=0;

	last_func = 107; repeat_count++;
	if (api_functions.show_AlazarAllocBufferU16Ex && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarAllocBufferU16Ex()\n");
		} else {
			fprintf(out, "U16 AlazarAllocBufferU16Ex()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU64 sampleCount: %lld (0x%08llx)\n", sampleCount, sampleCount);
		}
		fclose(out);
	}
	U16  * result = api_functions.AlazarAllocBufferU16Ex(handle, sampleCount);
	if (api_functions.show_AlazarAllocBufferU16Ex && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %p\n", result);
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFreeBufferU16Ex(HANDLE handle, U16 *buffer)
{
	if (last_func!=108) repeat_count=0;

	last_func = 108; repeat_count++;
	if (api_functions.show_AlazarFreeBufferU16Ex && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFreeBufferU16Ex()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFreeBufferU16Ex()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU16 * buffer: %p\n", buffer);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFreeBufferU16Ex(handle, buffer);
	if (api_functions.show_AlazarFreeBufferU16Ex && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarConfigureLSB(HANDLE handle, U32 valueLsb0, U32 valueLsb1)
{
	if (last_func!=109) repeat_count=0;

	last_func = 109; repeat_count++;
	if (api_functions.show_AlazarConfigureLSB && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarConfigureLSB()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarConfigureLSB()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 valueLsb0: %d (0x%08x)\n", valueLsb0, valueLsb0);
			fprintf(out, "\tU32 valueLsb1: %d (0x%08x)\n", valueLsb1, valueLsb1);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarConfigureLSB(handle, valueLsb0, valueLsb1);
	if (api_functions.show_AlazarConfigureLSB && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarExtractFFTNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	if (last_func!=110) repeat_count=0;

	last_func = 110; repeat_count++;
	if (api_functions.show_AlazarExtractFFTNPTFooters && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarExtractFFTNPTFooters()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarExtractFFTNPTFooters()\n");
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(out, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(out, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(out, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarExtractFFTNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (api_functions.show_AlazarExtractFFTNPTFooters && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarExtractTimeDomainNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	if (last_func!=111) repeat_count=0;

	last_func = 111; repeat_count++;
	if (api_functions.show_AlazarExtractTimeDomainNPTFooters && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarExtractTimeDomainNPTFooters()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarExtractTimeDomainNPTFooters()\n");
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(out, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(out, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(out, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarExtractTimeDomainNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (api_functions.show_AlazarExtractTimeDomainNPTFooters && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarExtractNPTFooters(void *buffer, U32 recordSize_bytes, U32 bufferSize_bytes, NPTFooter *footersArray, U32 numFootersToExtract)
{
	if (last_func!=112) repeat_count=0;

	last_func = 112; repeat_count++;
	if (api_functions.show_AlazarExtractNPTFooters && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarExtractNPTFooters()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarExtractNPTFooters()\n");
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 recordSize_bytes: %d (0x%08x)\n", recordSize_bytes, recordSize_bytes);
			fprintf(out, "\tU32 bufferSize_bytes: %d (0x%08x)\n", bufferSize_bytes, bufferSize_bytes);
			fprintf(out, "\tNPTFooter * footersArray: %p\n", footersArray);
			fprintf(out, "\tU32 numFootersToExtract: %d (0x%08x)\n", numFootersToExtract, numFootersToExtract);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarExtractNPTFooters(buffer, recordSize_bytes, bufferSize_bytes, footersArray, numFootersToExtract);
	if (api_functions.show_AlazarExtractNPTFooters && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDisableDSP(HANDLE boardHandle)
{
	if (last_func!=113) repeat_count=0;

	last_func = 113; repeat_count++;
	if (api_functions.show_AlazarDisableDSP && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDisableDSP()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDisableDSP()\n");
			fprintf(out, "\tHANDLE boardHandle: %p\n", boardHandle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDisableDSP(boardHandle);
	if (api_functions.show_AlazarDisableDSP && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarOCTIgnoreBadClock(HANDLE handle, U32 enable, double goodClockDuration_seconds, double badClockDuration_seconds, double *triggerCycleTime_seconds, double *triggerPulseWidth_seconds)
{
	if (last_func!=114) repeat_count=0;

	last_func = 114; repeat_count++;
	if (api_functions.show_AlazarOCTIgnoreBadClock && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarOCTIgnoreBadClock()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarOCTIgnoreBadClock()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tU32 enable: %d (0x%08x)\n", enable, enable);
			fprintf(out, "\tdouble goodClockDuration_seconds: %g\n", goodClockDuration_seconds);
			fprintf(out, "\tdouble badClockDuration_seconds: %g\n", badClockDuration_seconds);
			fprintf(out, "\tdouble * triggerCycleTime_seconds: %p\n", triggerCycleTime_seconds);
			fprintf(out, "\tdouble * triggerPulseWidth_seconds: %p\n", triggerPulseWidth_seconds);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarOCTIgnoreBadClock(handle, enable, goodClockDuration_seconds, badClockDuration_seconds, triggerCycleTime_seconds, triggerPulseWidth_seconds);
	if (api_functions.show_AlazarOCTIgnoreBadClock && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarSetADCBackgroundCompensation(HANDLE handle, BOOL active)
{
	if (last_func!=115) repeat_count=0;

	last_func = 115; repeat_count++;
	if (api_functions.show_AlazarSetADCBackgroundCompensation && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarSetADCBackgroundCompensation()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarSetADCBackgroundCompensation()\n");
			fprintf(out, "\tHANDLE handle: %p\n", handle);
			fprintf(out, "\tBOOL active: %s\n", active?"true":"false");
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarSetADCBackgroundCompensation(handle, active);
	if (api_functions.show_AlazarSetADCBackgroundCompensation && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetModules(HANDLE boardHandle, U32 numEntries, dsp_module_handle *modules, U32 *numModules)
{
	if (last_func!=116) repeat_count=0;

	last_func = 116; repeat_count++;
	if (api_functions.show_AlazarDSPGetModules && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetModules()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetModules()\n");
			fprintf(out, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(out, "\tU32 numEntries: %d (0x%08x)\n", numEntries, numEntries);
			fprintf(out, "\tdsp_module_handle * modules: %p\n", modules);
			fprintf(out, "\tU32 * numModules: %p\n", numModules);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetModules(boardHandle, numEntries, modules, numModules);
	if (api_functions.show_AlazarDSPGetModules && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetInfo(dsp_module_handle dspHandle, U32 *dspModuleId, U16 *versionMajor, U16 *versionMinor, U32 *maxLength, U32 *reserved0, U32 *reserved1)
{
	if (last_func!=117) repeat_count=0;

	last_func = 117; repeat_count++;
	if (api_functions.show_AlazarDSPGetInfo && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetInfo()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetInfo()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 * dspModuleId: %p\n", dspModuleId);
			fprintf(out, "\tU16 * versionMajor: %p\n", versionMajor);
			fprintf(out, "\tU16 * versionMinor: %p\n", versionMinor);
			fprintf(out, "\tU32 * maxLength: %p\n", maxLength);
			fprintf(out, "\tU32 * reserved0: %p\n", reserved0);
			fprintf(out, "\tU32 * reserved1: %p\n", reserved1);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetInfo(dspHandle, dspModuleId, versionMajor, versionMinor, maxLength, reserved0, reserved1);
	if (api_functions.show_AlazarDSPGetInfo && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGenerateWindowFunction(U32 windowType, float *window, U32 windowLength_samples, U32 paddingLength_samples)
{
	if (last_func!=118) repeat_count=0;

	last_func = 118; repeat_count++;
	if (api_functions.show_AlazarDSPGenerateWindowFunction && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGenerateWindowFunction()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGenerateWindowFunction()\n");
			fprintf(out, "\tU32 windowType: %d (0x%08x)\n", windowType, windowType);
			fprintf(out, "\tfloat * window: %p\n", window);
			fprintf(out, "\tU32 windowLength_samples: %d (0x%08x)\n", windowLength_samples, windowLength_samples);
			fprintf(out, "\tU32 paddingLength_samples: %d (0x%08x)\n", paddingLength_samples, paddingLength_samples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGenerateWindowFunction(windowType, window, windowLength_samples, paddingLength_samples);
	if (api_functions.show_AlazarDSPGenerateWindowFunction && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTGetMaxTriggerRepeatRate(dsp_module_handle dspHandle, U32 fftSize, double *maxTriggerRepeatRate)
{
	if (last_func!=119) repeat_count=0;

	last_func = 119; repeat_count++;
	if (api_functions.show_AlazarFFTGetMaxTriggerRepeatRate && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTGetMaxTriggerRepeatRate()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTGetMaxTriggerRepeatRate()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 fftSize: %d (0x%08x)\n", fftSize, fftSize);
			fprintf(out, "\tdouble * maxTriggerRepeatRate: %p\n", maxTriggerRepeatRate);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTGetMaxTriggerRepeatRate(dspHandle, fftSize, maxTriggerRepeatRate);
	if (api_functions.show_AlazarFFTGetMaxTriggerRepeatRate && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionSetRecordS16(dsp_module_handle dspHandle, const S16 *record, U32 size_samples)
{
	if (last_func!=120) repeat_count=0;

	last_func = 120; repeat_count++;
	if (api_functions.show_AlazarFFTBackgroundSubtractionSetRecordS16 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTBackgroundSubtractionSetRecordS16()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTBackgroundSubtractionSetRecordS16()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tconst S16 * record: %p\n", record);
			fprintf(out, "\tU32 size_samples: %d (0x%08x)\n", size_samples, size_samples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTBackgroundSubtractionSetRecordS16(dspHandle, record, size_samples);
	if (api_functions.show_AlazarFFTBackgroundSubtractionSetRecordS16 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionGetRecordS16(dsp_module_handle dspHandle, S16 *backgroundRecord, U32 size_samples)
{
	if (last_func!=121) repeat_count=0;

	last_func = 121; repeat_count++;
	if (api_functions.show_AlazarFFTBackgroundSubtractionGetRecordS16 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTBackgroundSubtractionGetRecordS16()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTBackgroundSubtractionGetRecordS16()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tS16 * backgroundRecord: %p\n", backgroundRecord);
			fprintf(out, "\tU32 size_samples: %d (0x%08x)\n", size_samples, size_samples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTBackgroundSubtractionGetRecordS16(dspHandle, backgroundRecord, size_samples);
	if (api_functions.show_AlazarFFTBackgroundSubtractionGetRecordS16 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTBackgroundSubtractionSetEnabled(dsp_module_handle dspHandle, BOOL enabled)
{
	if (last_func!=122) repeat_count=0;

	last_func = 122; repeat_count++;
	if (api_functions.show_AlazarFFTBackgroundSubtractionSetEnabled && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTBackgroundSubtractionSetEnabled()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTBackgroundSubtractionSetEnabled()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tBOOL enabled: %s\n", enabled?"true":"false");
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTBackgroundSubtractionSetEnabled(dspHandle, enabled);
	if (api_functions.show_AlazarFFTBackgroundSubtractionSetEnabled && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTSetWindowFunction(dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray)
{
	if (last_func!=123) repeat_count=0;

	last_func = 123; repeat_count++;
	if (api_functions.show_AlazarFFTSetWindowFunction && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTSetWindowFunction()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTSetWindowFunction()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(out, "\tfloat * realWindowArray: %p\n", realWindowArray);
			fprintf(out, "\tfloat * imagWindowArray: %p\n", imagWindowArray);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTSetWindowFunction(dspHandle, samplesPerRecord, realWindowArray, imagWindowArray);
	if (api_functions.show_AlazarFFTSetWindowFunction && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTGetWindowFunction(dsp_module_handle dspHandle, U32 samplesPerRecord, float *realWindowArray, float *imagWindowArray)
{
	if (last_func!=124) repeat_count=0;

	last_func = 124; repeat_count++;
	if (api_functions.show_AlazarFFTGetWindowFunction && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTGetWindowFunction()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTGetWindowFunction()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 samplesPerRecord: %d (0x%08x)\n", samplesPerRecord, samplesPerRecord);
			fprintf(out, "\tfloat * realWindowArray: %p\n", realWindowArray);
			fprintf(out, "\tfloat * imagWindowArray: %p\n", imagWindowArray);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTGetWindowFunction(dspHandle, samplesPerRecord, realWindowArray, imagWindowArray);
	if (api_functions.show_AlazarFFTGetWindowFunction && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTVerificationMode(dsp_module_handle dspHandle, BOOL enable, S16 *realArray, S16 *imagArray, size_t recordLength_samples)
{
	if (last_func!=125) repeat_count=0;

	last_func = 125; repeat_count++;
	if (api_functions.show_AlazarFFTVerificationMode && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTVerificationMode()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTVerificationMode()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tBOOL enable: %s\n", enable?"true":"false");
			fprintf(out, "\tS16 * realArray: %p\n", realArray);
			fprintf(out, "\tS16 * imagArray: %p\n", imagArray);
			fprintf(out, "\tsize_t recordLength_samples: %ld\n", recordLength_samples);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTVerificationMode(dspHandle, enable, realArray, imagArray, recordLength_samples);
	if (api_functions.show_AlazarFFTVerificationMode && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTSetup(dsp_module_handle dspHandle, U16 inputChannelMask, U32 recordLength_samples, U32 fftLength_samples, U32 outputFormat, U32 footer, U32 reserved, U32 *bytesPerOutputRecord)
{
	if (last_func!=126) repeat_count=0;

	last_func = 126; repeat_count++;
	if (api_functions.show_AlazarFFTSetup && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTSetup()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTSetup()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU16 inputChannelMask: %d (0x%04x)\n", inputChannelMask, inputChannelMask);
			fprintf(out, "\tU32 recordLength_samples: %d (0x%08x)\n", recordLength_samples, recordLength_samples);
			fprintf(out, "\tU32 fftLength_samples: %d (0x%08x)\n", fftLength_samples, fftLength_samples);
			fprintf(out, "\tU32 outputFormat: %d (0x%08x)\n", outputFormat, outputFormat);
			fprintf(out, "\tU32 footer: %d (0x%08x)\n", footer, footer);
			fprintf(out, "\tU32 reserved: %d (0x%08x)\n", reserved, reserved);
			fprintf(out, "\tU32 * bytesPerOutputRecord: %p\n", bytesPerOutputRecord);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTSetup(dspHandle, inputChannelMask, recordLength_samples, fftLength_samples, outputFormat, footer, reserved, bytesPerOutputRecord);
	if (api_functions.show_AlazarFFTSetup && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarFFTSetScalingAndSlicing(dsp_module_handle dspHandle, U8 slice_pos, float loge_ampl_mult)
{
	if (last_func!=127) repeat_count=0;

	last_func = 127; repeat_count++;
	if (api_functions.show_AlazarFFTSetScalingAndSlicing && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarFFTSetScalingAndSlicing()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarFFTSetScalingAndSlicing()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU8 slice_pos: %d (0x%02x)\n", slice_pos, slice_pos);
			fprintf(out, "\tfloat loge_ampl_mult: %g\n", (double)loge_ampl_mult);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarFFTSetScalingAndSlicing(dspHandle, slice_pos, loge_ampl_mult);
	if (api_functions.show_AlazarFFTSetScalingAndSlicing && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetBuffer(HANDLE boardHandle, void *buffer, U32 timeout_ms)
{
	if (last_func!=128) repeat_count=0;

	last_func = 128; repeat_count++;
	if (api_functions.show_AlazarDSPGetBuffer && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetBuffer()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetBuffer()\n");
			fprintf(out, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetBuffer(boardHandle, buffer, timeout_ms);
	if (api_functions.show_AlazarDSPGetBuffer && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetNextBuffer(HANDLE boardHandle, void *buffer, U32 bytesToCopy, U32 timeout_ms)
{
	if (last_func!=129) repeat_count=0;

	last_func = 129; repeat_count++;
	if (api_functions.show_AlazarDSPGetNextBuffer && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetNextBuffer()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetNextBuffer()\n");
			fprintf(out, "\tHANDLE boardHandle: %p\n", boardHandle);
			fprintf(out, "\tvoid * buffer: %p\n", buffer);
			fprintf(out, "\tU32 bytesToCopy: %d (0x%08x)\n", bytesToCopy, bytesToCopy);
			fprintf(out, "\tU32 timeout_ms: %d (0x%08x)\n", timeout_ms, timeout_ms);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetNextBuffer(boardHandle, buffer, bytesToCopy, timeout_ms);
	if (api_functions.show_AlazarDSPGetNextBuffer && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterU32(dsp_module_handle dspHandle, U32 parameter, U32 *_result)
{
	if (last_func!=130) repeat_count=0;

	last_func = 130; repeat_count++;
	if (api_functions.show_AlazarDSPGetParameterU32 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetParameterU32()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetParameterU32()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tU32 * _result: %p\n", _result);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetParameterU32(dspHandle, parameter, _result);
	if (api_functions.show_AlazarDSPGetParameterU32 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterU32(dsp_module_handle dspHandle, U32 parameter, U32 value)
{
	if (last_func!=131) repeat_count=0;

	last_func = 131; repeat_count++;
	if (api_functions.show_AlazarDSPSetParameterU32 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPSetParameterU32()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPSetParameterU32()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tU32 value: %d (0x%08x)\n", value, value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPSetParameterU32(dspHandle, parameter, value);
	if (api_functions.show_AlazarDSPSetParameterU32 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterS32(dsp_module_handle dspHandle, U32 parameter, S32 *_result)
{
	if (last_func!=132) repeat_count=0;

	last_func = 132; repeat_count++;
	if (api_functions.show_AlazarDSPGetParameterS32 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetParameterS32()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetParameterS32()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tS32 * _result: %p\n", _result);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetParameterS32(dspHandle, parameter, _result);
	if (api_functions.show_AlazarDSPGetParameterS32 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterS32(dsp_module_handle dspHandle, U32 parameter, S32 value)
{
	if (last_func!=133) repeat_count=0;

	last_func = 133; repeat_count++;
	if (api_functions.show_AlazarDSPSetParameterS32 && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPSetParameterS32()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPSetParameterS32()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tS32 value: %d\n", value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPSetParameterS32(dspHandle, parameter, value);
	if (api_functions.show_AlazarDSPSetParameterS32 && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPGetParameterFloat(dsp_module_handle dspHandle, U32 parameter, float *_result)
{
	if (last_func!=134) repeat_count=0;

	last_func = 134; repeat_count++;
	if (api_functions.show_AlazarDSPGetParameterFloat && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPGetParameterFloat()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPGetParameterFloat()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tfloat * _result: %p\n", _result);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPGetParameterFloat(dspHandle, parameter, _result);
	if (api_functions.show_AlazarDSPGetParameterFloat && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPSetParameterFloat(dsp_module_handle dspHandle, U32 parameter, float value)
{
	if (last_func!=135) repeat_count=0;

	last_func = 135; repeat_count++;
	if (api_functions.show_AlazarDSPSetParameterFloat && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPSetParameterFloat()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPSetParameterFloat()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tU32 parameter: %d (0x%08x)\n", parameter, parameter);
			fprintf(out, "\tfloat value: %g\n", (double)value);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPSetParameterFloat(dspHandle, parameter, value);
	if (api_functions.show_AlazarDSPSetParameterFloat && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPConfigureSelfTrigger(dsp_module_handle dspHandle, BOOL enable, U32 counter)
{
	if (last_func!=136) repeat_count=0;

	last_func = 136; repeat_count++;
	if (api_functions.show_AlazarDSPConfigureSelfTrigger && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPConfigureSelfTrigger()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPConfigureSelfTrigger()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tBOOL enable: %s\n", enable?"true":"false");
			fprintf(out, "\tU32 counter: %d (0x%08x)\n", counter, counter);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPConfigureSelfTrigger(dspHandle, enable, counter);
	if (api_functions.show_AlazarDSPConfigureSelfTrigger && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPConfigureSampleSkipping(dsp_module_handle dspHandle, BOOL independentMode, U32 count, U16 *vector)
{
	if (last_func!=137) repeat_count=0;

	last_func = 137; repeat_count++;
	if (api_functions.show_AlazarDSPConfigureSampleSkipping && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPConfigureSampleSkipping()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPConfigureSampleSkipping()\n");
			fprintf(out, "\tdsp_module_handle dspHandle: %p\n", dspHandle);
			fprintf(out, "\tBOOL independentMode: %s\n", independentMode?"true":"false");
			fprintf(out, "\tU32 count: %d (0x%08x)\n", count, count);
			fprintf(out, "\tU16 * vector: %p\n", vector);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPConfigureSampleSkipping(dspHandle, independentMode, count, vector);
	if (api_functions.show_AlazarDSPConfigureSampleSkipping && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}
RETURN_CODE AlazarDSPAbortCapture(HANDLE boardHandle)
{
	if (last_func!=138) repeat_count=0;

	last_func = 138; repeat_count++;
	if (api_functions.show_AlazarDSPAbortCapture && repeat_count<=MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		if (repeat_count == MAX_REPEAT) {
			fprintf(out, "... repeated call to AlazarDSPAbortCapture()\n");
		} else {
			fprintf(out, "RETURN_CODE AlazarDSPAbortCapture()\n");
			fprintf(out, "\tHANDLE boardHandle: %p\n", boardHandle);
		}
		fclose(out);
	}
	RETURN_CODE  result = api_functions.AlazarDSPAbortCapture(boardHandle);
	if (api_functions.show_AlazarDSPAbortCapture && repeat_count<MAX_REPEAT) {
		FILE *out = fopen(LOGFILE, "a");
		fprintf(out, "\t=> %s\n", AlazarErrorToText(result));
		fclose(out);
	}
	return result;
}

static void init_ats_log() __attribute__((constructor));

void init_ats_log() {
	load_api_functions();
}


