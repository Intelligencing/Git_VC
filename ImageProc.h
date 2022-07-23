
#include "halconcpp.h"
#pragma comment(lib,"halcon.lib")
using namespace HalconCpp;//����HalconCpp���б�ʶ��
//��������ṹ�嶨��
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

enum result//���ö��
{
	PROC_OK,
	PROC_NO_PRODUCT,
	PROC_NO_BIANYUAN,
	PROC_NO_LIGHT,
	PROC_NG,
};

int ImageProc(HObject Image, HTuple hWndID,
	PINPUTDATA pInputData, POUTPUTDATA pOutputData); //ͼ������
