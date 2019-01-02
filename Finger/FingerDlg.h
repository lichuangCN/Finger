
// FingerDlg.h : 头文件
#include "CZKFPEngX.h"	//引入指纹采集器
#include <fstream>
#include <iostream>
#include <iomanip>
#include <io.h>
#include <direct.h>	//for_mkdir
#pragma once

/********************************************
*											*
*			   宏定义声明					*
*											*										
********************************************/

#define DB_DIR "Database"	//指纹库文件夹
#define TEMP_DIR "temp" //临时文件夹，用于存放指纹入库前的所有中间处理结果
#define DB_INDEX_TXT "Database\\index.txt"	//指纹库索引文件
#define STEP_IMG_1 "temp\\Step1_Source.bmp"	//复制后的图像副本
#define STEP_TXT_1 "temp\\Step1_Source.txt"	//图像位图数据文件的指定文件名
#define STEP_TXT_2 "temp\\Step2_MidFilter.txt"	//中值滤波临时文件
#define STEP_IMG_2 "temp\\Step2_MidFilter.bmp"	//中值滤波临时图像
#define STEP_TXT_3 "temp\\Step3_Normalize.txt"	//直方图均衡化临时文件
#define STEP_IMG_3 "temp\\Step3_Normalize.bmp"	//直方图均衡化临时图像
#define STEP_TXT_4 "temp\\Step4_Direction.txt"	//脊线方向计算临时文件
#define STEP_IMG_4 "temp\\Step4_Direction.bmp"	//脊线方向计算临时图像
#define STEP_TXT_5 "temp\\Step5_Frequency.txt"	//指纹脊线频率计算临时文件
#define STEP_IMG_5 "temp\\Step5_Frequency.bmp"	//指纹脊线频率计算临时图像
#define STEP_TXT_6 "temp\\Step6_Mask.txt"	//指纹掩码计算临时文件
#define STEP_IMG_6 "temp\\Step6_Mask.bmp"	//指纹掩码计算临时图片
#define STEP_TXT_7 "temp\\Step7_GaborEnhance.txt"	//Gaber滤波增强临时文件
#define STEP_IMG_7 "temp\\Step7_GaborEnhance.bmp"	//Gaber滤波增强临时图像
#define STEP_TXT_8 "temp\\Step8_Binary.txt"		//二值化临时文件
#define STEP_IMG_8 "temp\\Step8_Binary.bmp"		//二值化临时图像
#define STEP_TXT_9 "temp\\Step9_Thinning.txt"	//细化临时文件
#define STEP_IMG_9 "temp\\Step9_Thinning.bmp"	//细化临时图像
#define STEP_TXT_10 "temp\\Step10_MinuExtract.txt"		//特征提取临时文件
#define STEP_IMG_10 "temp\\Step10_MinuExtract.bmp"		//特征提取临时图像
#define STEP_TXT_11 "temp\\Step11_MinuFilter.txt"		//特征过滤临时文件
#define STEP_IMG_11 "temp\\Step11_MinuFilter.bmp"		//特征过滤临时图像
#define STEP_IMG_11_MDL "temp\\Step11_MinuFilter_MDL.mdl"		//特征模板文件
#define STEP_IMG_12	"temp\\Step12_Result.bmp"	//指纹匹配结果图像

using namespace std;
using std::ofstream;

//相邻特征点结构
struct NEIGHBOR
{
	int x;		//横坐标
	int y;		//纵坐标
	int type;		//特征点类型
	float Theta;		//两点连线角度
	float Theta2Ridge;		//两点脊线方向夹角
	float ThetaThisNibor;	//相邻特征点的脊线方向
	int distance;		//两点距离
};

//特征点结构
struct MINUTIAE
{
	int x;		//横坐标
	int y;		//纵坐标
	int type;		//特征点类型
	float theta;		//两点连线角度
	NEIGHBOR *neibors;	//相邻特征点序列
};

// CFingerDlg 对话框
class CFingerDlg : public CDialogEx
{
// 构造
public:
	CFingerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CStatic m_staticInfo;
	CStatic m_picCtrl1;
	CStatic m_picCtrl2;
	CString m_name;
	CZKFPEngX m_zkfpEng;
	int m_minuCount;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
	DECLARE_EVENTSINK_MAP();
	afx_msg void OnImageReceivedZkfpengx(BOOL FAR* AImageValid);
public:
	afx_msg void OnBnClickedBtnDatabase();
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnBnClickedBtnRegister();
	afx_msg void OnBnClickedBtnIdentify();
	afx_msg void FingerConnect();	//启动时自检测指纹器是否连接
	afx_msg void InitControl();		//初始化各个控件位置及大小
};

/********************************************
*											*
*				全局函数					*
*											*											
********************************************/
//图像控件中显示图像
int ShowImageInCtrl(CStatic &picCtrl, char *filename);

//多字节字符串转换为Unicode宽字符串
wchar_t *ToWideChar(char *str);

//3.5.2 获取图像参数信息
//获取图像的高度，宽度，深度信息
int ReadBMPImgFilePara(char *filename, int &width, int &height, int &depth);

//3.5.3 从图文件读取数据
//获取图像位图数据
int ReadBMPImgFileData(char *filename, unsigned char *data);

//3.5.4 用文本文件保存数据
//保存位图数据到文本文件
//dstFile(目标文件)，data(无符号字符数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, unsigned char *data, int width, int height);
//dstFile(目标文件)，data(浮点型数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, float *data, int width, int height);

//3.6.1	创建指纹库和索引文件
void InitDatabase();	//创建并初始化
void InitFile(char *filename);	//创建并初始化文件

//3.6.2	遍历指纹库
void GetDatabaseInfo(char *info);	//获取指纹库统计信息

//3.8.1	从文本文件读取数据
//从文本文件读取无符号自负类型的图像位图数据
//srcFile(源文件),data(无符号字符型数据数组),iWidth(图像宽度),iHeight(图像高度)
int ReadDataFromTextFile(char *srcFile, unsigned char *data, int iWidth, int iHeight);

//3.8.2	图像中值滤波
//参数：ucImg(原图数据),ucDstImg(结果图像数据)，iWidth(图像宽度),iHeight(图像高度)
int MidFilter(unsigned char* ucImg, unsigned char* ucDstImg, int iWidth, int iHeight);
//使用冒泡法对叔组进行升序排序
//参数:data(数组),dsize(数组长度)
void Sort(unsigned char* data, int dsize);

//3.8.3
//保存数据到BMP图像文件
int SaveDataToImageFile(char *srcFile, char *dstFile, unsigned char *data);
//保存数据到图像文件
//srcFile(源文件名),dstFile(目标文件名),data(待保存数据数组),scale(转换比例)
int SaveDataToImageFile(char *srcFile, char *dstFile, float *data, float scale);
//将位图数据写入BMP图像文件
//dstFileName(目标文件名),pusImgData(待保存数据数组)
int WriteBMPImgFile(char *dstFileName, unsigned char **pusImgData);

//3.9.1 图像直方图均衡化
//ucImg(源图数据），ucNormImg(结果图数据,iWidth(图像宽度），iHeight(图像高度）
int HistoNormalize(unsigned char *ucImg, unsigned char *unNormImg, int iWidth, int iHeight);

//3.11.1 指纹脊线方向计算
//ucImg(图像数据),fDire(脊线方向数据),iWidth(图像宽度),iHeight(图像高度)
int ImgDirection(unsigned char *ucImg, float *fDire, int iWidth, int iHeight);

//3.11.2 脊线方向低通滤波
//fDire(脊线方向原始数据),fFitDire(脊线方向滤波结果数据),iWidth(图像宽度),iHeight(图像高度)
int DircLowPass(float *fDire, float *fFitDire, int iWidth, int iHeight);

//3.12.1 指纹脊线频率计算
//ucImg(源图数据),fDirection(脊线方向数据),fFrequency(脊线频率结果数据),iWidth(图像宽度),iHeight(图像高度)
int Frequency(unsigned char * ucImg, float * fDirection, float * fFrequency, int iWidth, int iHeight);

//3.13.1 指纹掩码计算
//实现指纹掩码计算
int GetMask(unsigned char * ucImg, float * fDirection, float *fFrequency, unsigned char * ucMask, int iWidth, int iHeight);

//3.14 指纹图像Gabor滤波增强
//ucImg(源图数据),fDirection(脊线方向数据),ucMask(掩码数据),ucImgEnhanced（滤波增强结果数据),iWidth(图像宽度),iHeight(图像高度)
int GaborEnhance(unsigned char * ucImg, float *fDirection, float *fFrequency, unsigned char * ucMask, unsigned char *ucImgEnhanced, int iWidth, int iHeight);

//3.16.1 图像二值化
//图像二值化
//ucImage(源图数据),ucBinImage(结果图数据),iWidth(图像宽度),iHeight(图像高度),uThreshold(二值化灰度阈值)
int BinaryImg(unsigned char* ucImage, unsigned char* ucBinImage, int iWidth, int iHeight, unsigned char uThreshold);

//3.16.2 二值图显示转换
//二值图显示转换([0,1]->[0,255])
//ucBinImg(源图数据),ucGrayImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int BinaryToGray(unsigned char * ucBinImg, unsigned char* ucGrayImg, int iWidth, int iHeight);

//3.17.1 图像细化算法
//ucBinedImg(原图数据), ucThinnedImage(结果图数据), iWidth(图像宽度), iHeight(图像高度), iIterativeLimit(最大迭代次数)
int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit);

//3.18.1 指纹特征提取
//ucThinImg(源图数据), ucMinuImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int Extract(unsigned char *ucThinImg, unsigned char *ucMinuImg, int iWidth, int iHeight);

//3.19.1 指纹特征过滤
//参数:minuData(特征图数据),thinData(细化图数据),minutiaes(特征点数组),minuCount(特征点数量),iWidth(图像宽度),iHeight(图像高度)
int  MinuFilter(unsigned char *minuData, unsigned char *thinData, MINUTIAE *minutiaes, int &minuCount, int iWidth, int iHeight);

//3.19.2 去除边缘特征点
//参数:minutiaes(特征点数组), count(特征点数目), ucImg(源图数据), iWidth(图像宽度), iHeight(图像高度)
int CutEdge(MINUTIAE *minutiaes, int count, unsigned char *ucImg, int iWidth, int iHeight);

//3.20.1 保存特征模板文件
//参数:minutiaes(特征点数组)，count(特征点数量),filename(模板文件名)
int SaveMinutiae(MINUTIAE *minutiaes, int count, char * filename);

//3.20.2 获得新指纹编号
int GetNewIndexInDB();

//3.20.4 显示指纹入库结果
//宽字符串转换为多字节字符串
char *ToChar(wchar_t *str);

//3.23 指纹特征匹配
//3.23.1 构建特征点相邻关系
//参数：minutiae(特征点数组),minuCount(特征点数目)
int BuildNabors(MINUTIAE *minutiae, int minuCount);

//计算连接两个坐标点的线段倾斜度
//参数: x1(端点1 横坐标), y1(端点1 纵坐标), 参数: x2(端点2 横坐标), y2(端点2 纵坐标)
float Angle2Points(int x1, int y1, int x2, int y2);

//3.23.2 指纹特征匹配相似度计算
//参数:iWidth(图像宽度), iHeight(图像高度)，minutiae1(指纹1的特征点数组)，account(指纹1的特征点数量)
//minutiae2(指纹2的特征点数组)，account(指纹2的特征点数量)
float MinuSimilarity(int iWidth, int iHeight, MINUTIAE *minutiae1, int count1, MINUTIAE *minutiae2, int count2);

//3.23.3 读取特征模板文件
//参数:fileName(特征模板文件名),minutiae(特征点数组)
int ReadMinutiae(char *fileName, MINUTIAE **minutiae);

//3.24.1 指纹库预检
bool EmptyDB();

//3.25指纹登记全程集成
//3.25.1指纹登记过程集成
//regName(登记人姓名),srcImgFile(源图文件名),outImgFile(特征结果图文件名),info(返回操作成功或失败提示信息),bSaveImg(是否保存所有中间结果图像)
int Enroll(char *regName, char *srcImgFile, char *outImgFile, char *info, bool bSaveImg);

//3.26
//指纹识别过程集成
//srcImgFile(源图文件名),outImgFile(识别结果图文件名,info(返回操作成功或失败提示信息），bSaveImg(是否保存所有中间结果图像）
int Identify(char *srcImgFile, char *outImgFile, char *info, bool bSaveImg);

//将CString类型转换为char[]类型
//选取指定图片文件时使用
void CStirngToChar(CString cs, char ch[]);

//获取Debug或Release所在的路径
CString GetModuleDir();

//删除指定的文件夹
void DeleteDirectory(CString strDir);

//删除文件夹
void DeleteAll();

