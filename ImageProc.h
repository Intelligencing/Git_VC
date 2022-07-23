
#include "halconcpp.h"
#pragma comment(lib,"halcon.lib")
using namespace HalconCpp;//调用HalconCpp所有标识符
//输入输出结构体定义
typedef struct InputData
{
	int ProductGray;
	int LightArea;

	HObject RegionRoi;
}INPUTDATA, * PINPUTDATA;

typedef struct OutputData
{
	HTuple minDis;

	HObject hCircle;
	HObject hLine;
}OUTPUTDATA, * POUTPUTDATA;

enum result//结果枚举
{
	PROC_OK,
	PROC_NO_PRODUCT,
	PROC_NO_BIANYUAN,
	PROC_NO_LIGHT,
	PROC_NG,
};

int ImageProc(HObject Image, HTuple hWndID,
	PINPUTDATA pInputData, POUTPUTDATA pOutputData); //图像处理函数
