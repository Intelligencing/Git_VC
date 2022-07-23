
#include "pch.h"
#include "ImageProc.h"
//图像处理
int ImageProc(HObject Image, HTuple hWndID,
	PINPUTDATA pInputData, POUTPUTDATA pOutputData)
{
	try
	{
		HObject ho_ImageReduced,ho_ImageMean, ho_RegionDynThresh, ho_ConnectedRegions, 
			ho_SelectedRegions,ho_Circle;//补充图像定义
		HTuple hv_Row,  hv_Column,  hv_Radius,  hv_Number;//补充变量定义

		Rgb1ToGray(Image, &Image);

		ReduceDomain(Image, pInputData->RegionRoi, &ho_ImageReduced);//输入输出
		//找产品
		MeanImage(ho_ImageReduced, &ho_ImageMean, 21, 21);
		DynThreshold(ho_ImageReduced, ho_ImageMean, &ho_RegionDynThresh, 5, "light");
		Connection(ho_RegionDynThresh, &ho_ConnectedRegions);
		//选出小圆
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 140, 240);
		SmallestCircle(ho_SelectedRegions, &hv_Row, &hv_Column, &hv_Radius);
		GenCircle(&ho_Circle, hv_Row, hv_Column, hv_Radius);
		//确认检索到目标
		CountObj(ho_Circle, &hv_Number);
		if (0 != (int(hv_Number != 1)))
		{
			return PROC_NO_PRODUCT;//没找到产品
			// stop(...); only in hdevelop
		}
		//找边缘
		HObject ho_RegionBorder, ho_SelectedRegions1, ho_RegionFillUp, ho_RegionBorder1;
		HTuple hv_Number1;

		Boundary(ho_Circle, &ho_RegionBorder, "inner");
		//选取大框
		SelectShape(ho_ConnectedRegions, &ho_SelectedRegions1, "area", "and", 3800, 4800);
		FillUp(ho_SelectedRegions1, &ho_RegionFillUp);
		Boundary(ho_RegionFillUp, &ho_RegionBorder1, "inner");
		CountObj(ho_RegionBorder1, &hv_Number1);
		if (0 != (int(hv_Number1 != 1)))
		{
			return PROC_NO_BIANYUAN;
			// stop(...); only in hdevelop
		}
	}
	catch (...)
	{
		return PROC_NG;
	}

	return PROC_OK;
}