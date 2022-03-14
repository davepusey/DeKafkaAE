#include "DeKafkaAE.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;



TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						"",
	StrID_Name,						"DeKafka",
	StrID_Description,				"A port of the DeKafka avisynth function, but as an Adobe After Effects plug-in.",
	StrID_Category,					"DeKafka",
	StrID_Gain_Param_Name,			"Gain",
	StrID_Color_Param_Name,			"Color",
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	