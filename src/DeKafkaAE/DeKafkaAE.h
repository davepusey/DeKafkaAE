#pragma once

#ifndef DEKAFKAAE_H
#define DEKAFKAAE_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"
#include "PrSDKAESupport.h"
#include "AEFX_SuiteHelper.h"

#include "DeKafkaAE_Strings.h"

/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

#define	DEKAFKA_LEFT_MIN		0
#define	DEKAFKA_LEFT_MAX		1920
#define	DEKAFKA_LEFT_DFLT		1578

#define	DEKAFKA_TOP_MIN			0
#define	DEKAFKA_TOP_MAX			1080
#define	DEKAFKA_TOP_DFLT		28

#define	DEKAFKA_WIDTH_MIN		0
#define	DEKAFKA_WIDTH_MAX		1920
#define	DEKAFKA_WIDTH_DFLT		194

#define	DEKAFKA_HEIGHT_MIN		0
#define	DEKAFKA_HEIGHT_MAX		1080
#define	DEKAFKA_HEIGHT_DFLT		32

#define	DEKAFKA_AMOUNT_MIN		0
#define	DEKAFKA_AMOUNT_MAX		255
#define	DEKAFKA_AMOUNT_DFLT		255

enum {
	DEKAFKA_INPUT = 0,
	DEKAFKA_LEFT,
	DEKAFKA_TOP,
	DEKAFKA_WIDTH,
	DEKAFKA_HEIGHT,
	DEKAFKA_AMOUNT,
	DEKAFKA_NUM_PARAMS
};

enum {
	LEFT_DISK_ID = 1,
	TOP_DISK_ID,
	WIDTH_DISK_ID,
	HEIGHT_DISK_ID,
	AMOUNT_DISK_ID,
};

extern "C" {

	DllExport
	PF_Err
	EffectMain(
		PF_Cmd			cmd,
		PF_InData		*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output,
		void			*extra);

}

#endif // DEKAFKAAE_H