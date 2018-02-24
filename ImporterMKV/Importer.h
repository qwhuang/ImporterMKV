#pragma once

#include "lavf.h"
#include "premiere_cs6/PrSDKImport.h"
#include "premiere_cs6/PrSDKPPixCreatorSuite.h"
#include "premiere_cs6/PrSDKPPixCacheSuite.h"
#include "premiere_cs6/PrSDKPPixCreator2Suite.h"
#include "premiere_cs6/PrSDKPPix2Suite.h"
#include "premiere_cs6/PrSDKMemoryManagerSuite.h"
#include "Decoder.h"

// Declare plug-in entry point with C linkage
extern "C" {
	PREMPLUGENTRY DllExport xImportEntry(csSDK_int32 selector, imStdParms *stdParms, void *param1, void *param2);
}

typedef struct
{
	csSDK_int32	importerID;
	csSDK_int32 width;
	csSDK_int32 height;
	Decoder *decoder;
	AVRational framerate;
	AVPixelFormat pix_fmt;
	prUTF16Char	fileName[256];
	PlugMemoryFuncsPtr		memFuncs;
	SPBasicSuite			*BasicSuite;
	PrSDKPPixCreatorSuite	*PPixCreatorSuite;
	PrSDKPPixCacheSuite		*PPixCacheSuite;
	PrSDKPPixSuite			*PPixSuite;
	PrSDKPPixCreator2Suite	*PPixCreator2Suite;
	PrSDKPPix2Suite			*PPix2Suite;
	PrSDKTimeSuite			*TimeSuite;
} impCtx, *impCtxPtr, **ImporterLocalRec8H;

static prMALError SDKInit(imStdParms *stdParms, imImportInfoRec *importInfo);
static prMALError SDKGetIndFormat(imStdParms *stdparms, csSDK_size_t index, imIndFormatRec *SDKIndFormatRec);
static prMALError SDKGetPrefs8(imStdParms *stdParms, imFileAccessRec8 *SDKfileAccessRec8, imGetPrefsRec *prefsRec);
prMALError SDKOpenFile8(imStdParms *stdParms, imFileRef *SDKfileRef, imFileOpenRec8 *SDKfileOpenRec8);
static prMALError SDKQuietFile(imStdParms *stdParms, imFileRef *SDKfileRef, void *privateData);
static prMALError SDKCloseFile(imStdParms *stdParms, imFileRef *SDKfileRef, void *privateData);
prMALError SDKGetInfo8(imStdParms *stdparms, imFileAccessRec8 *fileAccessInfo8, imFileInfoRec8 *SDKFileInfo8);
static prMALError SDKGetIndPixelFormat(imStdParms *stdParms, csSDK_size_t idx, imIndPixelFormatRec *SDKIndPixelFormatRec);
static prMALError SDKPreferredFrameSize(imStdParms *stdparms, imPreferredFrameSizeRec *preferredFrameSizeRec);
static prMALError SDKGetTimeInfo8(imStdParms *stdParms, imFileRef SDKfileRef, imTimeInfoRec8 *SDKtimeInfoRec8);
static prMALError SDKGetMetaData(imStdParms *stdParms, imFileRef SDKfileRef, imMetaDataRec *SDKMetaDataRec);
static prMALError SDKSetMetaData(imStdParms *stdParms, imFileRef SDKfileRef, imMetaDataRec *SDKMetaDataRec);
static prMALError SDKGetSourceVideo(imStdParms *stdparms, imFileRef fileRef, imSourceVideoRec *sourceVideoRec);