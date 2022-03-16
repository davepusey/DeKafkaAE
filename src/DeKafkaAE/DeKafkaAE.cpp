#include "DeKafkaAE.h"
#include "DeKafkaMask.h"

static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"%s v%d.%d\r%s",
											STR(StrID_Name), 
											MAJOR_VERSION, 
											MINOR_VERSION, 
											STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	// for Premiere Pro
	if (in_data->appl_id == 'PrMr')
	{
		AEFX_SuiteScoper<PF_PixelFormatSuite1> pixelFormatSuite = AEFX_SuiteScoper<PF_PixelFormatSuite1>(in_data, kPFPixelFormatSuite, kPFPixelFormatSuiteVersion1, out_data);

		// clear supported pixel formats
		(*pixelFormatSuite->ClearSupportedPixelFormats)(in_data->effect_ref);

		// add supported pixel formats
		(*pixelFormatSuite->AddSupportedPixelFormat) (in_data->effect_ref, PrPixelFormat_ARGB_4444_8u);
	}

	return PF_Err_NONE;
}

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Left_Param_Name),
		DEKAFKA_LEFT_MIN,
		DEKAFKA_LEFT_MAX,
		DEKAFKA_LEFT_MIN,
		DEKAFKA_LEFT_MAX,
		DEKAFKA_LEFT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		LEFT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Top_Param_Name),
		DEKAFKA_TOP_MIN,
		DEKAFKA_TOP_MAX,
		DEKAFKA_TOP_MIN,
		DEKAFKA_TOP_MAX,
		DEKAFKA_TOP_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		TOP_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Width_Param_Name),
		DEKAFKA_WIDTH_MIN,
		DEKAFKA_WIDTH_MAX,
		DEKAFKA_WIDTH_MIN,
		DEKAFKA_WIDTH_MAX,
		DEKAFKA_WIDTH_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		WIDTH_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Height_Param_Name),
		DEKAFKA_HEIGHT_MIN,
		DEKAFKA_HEIGHT_MAX,
		DEKAFKA_HEIGHT_MIN,
		DEKAFKA_HEIGHT_MAX,
		DEKAFKA_HEIGHT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		HEIGHT_DISK_ID);

	AEFX_CLR_STRUCT(def);

	PF_ADD_FLOAT_SLIDERX(STR(StrID_Amount_Param_Name),
		DEKAFKA_AMOUNT_MIN,
		DEKAFKA_AMOUNT_MAX,
		DEKAFKA_AMOUNT_MIN,
		DEKAFKA_AMOUNT_MAX,
		DEKAFKA_AMOUNT_DFLT,
		PF_Precision_INTEGER,
		0,
		0,
		AMOUNT_DISK_ID);

	out_data->num_params = DEKAFKA_NUM_PARAMS;

	return err;
}

static PF_Err 
Render (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);

	/*	Put interesting code here. */
	
	PF_FpLong x = params[DEKAFKA_LEFT]->u.fs_d.value;
	PF_FpLong y = params[DEKAFKA_TOP]->u.fs_d.value;
	PF_FpLong w = params[DEKAFKA_WIDTH]->u.fs_d.value;
	PF_FpLong h = params[DEKAFKA_HEIGHT]->u.fs_d.value;
	PF_FpLong amount = params[DEKAFKA_AMOUNT]->u.fs_d.value;

	// for Premiere Pro
	if (in_data->appl_id == 'PrMr')
	{
		//y /= 2;
		//h /= 2;
	}

	PF_Rect maskRect;
	maskRect.left = A_long(x) - 2;
	maskRect.top = A_long(y) - 2;
	maskRect.right = A_long(x) + A_long(w) + 2;
	maskRect.bottom = A_long(y) + A_long(h) + 2;

	PF_EffectWorld maskWorld;
	AEFX_CLR_STRUCT(maskWorld);
	ERR(PF_NEW_WORLD(A_long(w) + 4, A_long(h) + 4, NULL, &maskWorld));

	ERR(PF_COPY(&params[DEKAFKA_INPUT]->u.ld, &maskWorld, &maskRect, NULL));

	ApplyMask(&maskWorld, x, y, w, h, amount);

	ERR(PF_COPY(&params[DEKAFKA_INPUT]->u.ld, output, NULL, NULL));
	ERR(PF_COPY(&maskWorld, output, NULL, &maskRect));

	return err;
}


extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		STR(StrID_Name), // Name
		STR(StrID_Name), // Match Name
		STR(StrID_Category), // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}


PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

