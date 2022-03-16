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
	StrID_Left_Param_Name,			"Left",
	StrID_Top_Param_Name,			"Top",
	StrID_Width_Param_Name,			"Width",
	StrID_Height_Param_Name,		"Height",
	StrID_Amount_Param_Name,		"Amount"
};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	