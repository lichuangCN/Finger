
// FingerDlg.h : ͷ�ļ�
#include "CZKFPEngX.h"	//����ָ�Ʋɼ���
#include <fstream>
#include <iostream>
#include <iomanip>
#include <io.h>
#include <direct.h>	//for_mkdir
#pragma once

/********************************************
*											*
*			   �궨������					*
*											*										
********************************************/

#define DB_DIR "Database"	//ָ�ƿ��ļ���
#define TEMP_DIR "temp" //��ʱ�ļ��У����ڴ��ָ�����ǰ�������м䴦����
#define DB_INDEX_TXT "Database\\index.txt"	//ָ�ƿ������ļ�
#define STEP_IMG_1 "temp\\Step1_Source.bmp"	//���ƺ��ͼ�񸱱�
#define STEP_TXT_1 "temp\\Step1_Source.txt"	//ͼ��λͼ�����ļ���ָ���ļ���
#define STEP_TXT_2 "temp\\Step2_MidFilter.txt"	//��ֵ�˲���ʱ�ļ�
#define STEP_IMG_2 "temp\\Step2_MidFilter.bmp"	//��ֵ�˲���ʱͼ��
#define STEP_TXT_3 "temp\\Step3_Normalize.txt"	//ֱ��ͼ���⻯��ʱ�ļ�
#define STEP_IMG_3 "temp\\Step3_Normalize.bmp"	//ֱ��ͼ���⻯��ʱͼ��
#define STEP_TXT_4 "temp\\Step4_Direction.txt"	//���߷��������ʱ�ļ�
#define STEP_IMG_4 "temp\\Step4_Direction.bmp"	//���߷��������ʱͼ��
#define STEP_TXT_5 "temp\\Step5_Frequency.txt"	//ָ�Ƽ���Ƶ�ʼ�����ʱ�ļ�
#define STEP_IMG_5 "temp\\Step5_Frequency.bmp"	//ָ�Ƽ���Ƶ�ʼ�����ʱͼ��
#define STEP_TXT_6 "temp\\Step6_Mask.txt"	//ָ�����������ʱ�ļ�
#define STEP_IMG_6 "temp\\Step6_Mask.bmp"	//ָ�����������ʱͼƬ
#define STEP_TXT_7 "temp\\Step7_GaborEnhance.txt"	//Gaber�˲���ǿ��ʱ�ļ�
#define STEP_IMG_7 "temp\\Step7_GaborEnhance.bmp"	//Gaber�˲���ǿ��ʱͼ��
#define STEP_TXT_8 "temp\\Step8_Binary.txt"		//��ֵ����ʱ�ļ�
#define STEP_IMG_8 "temp\\Step8_Binary.bmp"		//��ֵ����ʱͼ��
#define STEP_TXT_9 "temp\\Step9_Thinning.txt"	//ϸ����ʱ�ļ�
#define STEP_IMG_9 "temp\\Step9_Thinning.bmp"	//ϸ����ʱͼ��
#define STEP_TXT_10 "temp\\Step10_MinuExtract.txt"		//������ȡ��ʱ�ļ�
#define STEP_IMG_10 "temp\\Step10_MinuExtract.bmp"		//������ȡ��ʱͼ��
#define STEP_TXT_11 "temp\\Step11_MinuFilter.txt"		//����������ʱ�ļ�
#define STEP_IMG_11 "temp\\Step11_MinuFilter.bmp"		//����������ʱͼ��
#define STEP_IMG_11_MDL "temp\\Step11_MinuFilter_MDL.mdl"		//����ģ���ļ�
#define STEP_IMG_12	"temp\\Step12_Result.bmp"	//ָ��ƥ����ͼ��

using namespace std;
using std::ofstream;

//����������ṹ
struct NEIGHBOR
{
	int x;		//������
	int y;		//������
	int type;		//����������
	float Theta;		//�������߽Ƕ�
	float Theta2Ridge;		//���㼹�߷���н�
	float ThetaThisNibor;	//����������ļ��߷���
	int distance;		//�������
};

//������ṹ
struct MINUTIAE
{
	int x;		//������
	int y;		//������
	int type;		//����������
	float theta;		//�������߽Ƕ�
	NEIGHBOR *neibors;	//��������������
};

// CFingerDlg �Ի���
class CFingerDlg : public CDialogEx
{
// ����
public:
	CFingerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CStatic m_staticInfo;
	CStatic m_picCtrl1;
	CStatic m_picCtrl2;
	CString m_name;
	CZKFPEngX m_zkfpEng;
	int m_minuCount;

	// ���ɵ���Ϣӳ�亯��
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
	afx_msg void FingerConnect();	//����ʱ�Լ��ָ�����Ƿ�����
	afx_msg void InitControl();		//��ʼ�������ؼ�λ�ü���С
};

/********************************************
*											*
*				ȫ�ֺ���					*
*											*											
********************************************/
//ͼ��ؼ�����ʾͼ��
int ShowImageInCtrl(CStatic &picCtrl, char *filename);

//���ֽ��ַ���ת��ΪUnicode���ַ���
wchar_t *ToWideChar(char *str);

//3.5.2 ��ȡͼ�������Ϣ
//��ȡͼ��ĸ߶ȣ���ȣ������Ϣ
int ReadBMPImgFilePara(char *filename, int &width, int &height, int &depth);

//3.5.3 ��ͼ�ļ���ȡ����
//��ȡͼ��λͼ����
int ReadBMPImgFileData(char *filename, unsigned char *data);

//3.5.4 ���ı��ļ���������
//����λͼ���ݵ��ı��ļ�
//dstFile(Ŀ���ļ�)��data(�޷����ַ���������)��width(ͼ����)��height(ͼ��߶�)
int SaveDataToTextFile(char *dstFile, unsigned char *data, int width, int height);
//dstFile(Ŀ���ļ�)��data(��������������)��width(ͼ����)��height(ͼ��߶�)
int SaveDataToTextFile(char *dstFile, float *data, int width, int height);

//3.6.1	����ָ�ƿ�������ļ�
void InitDatabase();	//��������ʼ��
void InitFile(char *filename);	//��������ʼ���ļ�

//3.6.2	����ָ�ƿ�
void GetDatabaseInfo(char *info);	//��ȡָ�ƿ�ͳ����Ϣ

//3.8.1	���ı��ļ���ȡ����
//���ı��ļ���ȡ�޷����Ը����͵�ͼ��λͼ����
//srcFile(Դ�ļ�),data(�޷����ַ�����������),iWidth(ͼ����),iHeight(ͼ��߶�)
int ReadDataFromTextFile(char *srcFile, unsigned char *data, int iWidth, int iHeight);

//3.8.2	ͼ����ֵ�˲�
//������ucImg(ԭͼ����),ucDstImg(���ͼ������)��iWidth(ͼ����),iHeight(ͼ��߶�)
int MidFilter(unsigned char* ucImg, unsigned char* ucDstImg, int iWidth, int iHeight);
//ʹ��ð�ݷ������������������
//����:data(����),dsize(���鳤��)
void Sort(unsigned char* data, int dsize);

//3.8.3
//�������ݵ�BMPͼ���ļ�
int SaveDataToImageFile(char *srcFile, char *dstFile, unsigned char *data);
//�������ݵ�ͼ���ļ�
//srcFile(Դ�ļ���),dstFile(Ŀ���ļ���),data(��������������),scale(ת������)
int SaveDataToImageFile(char *srcFile, char *dstFile, float *data, float scale);
//��λͼ����д��BMPͼ���ļ�
//dstFileName(Ŀ���ļ���),pusImgData(��������������)
int WriteBMPImgFile(char *dstFileName, unsigned char **pusImgData);

//3.9.1 ͼ��ֱ��ͼ���⻯
//ucImg(Դͼ���ݣ���ucNormImg(���ͼ����,iWidth(ͼ���ȣ���iHeight(ͼ��߶ȣ�
int HistoNormalize(unsigned char *ucImg, unsigned char *unNormImg, int iWidth, int iHeight);

//3.11.1 ָ�Ƽ��߷������
//ucImg(ͼ������),fDire(���߷�������),iWidth(ͼ����),iHeight(ͼ��߶�)
int ImgDirection(unsigned char *ucImg, float *fDire, int iWidth, int iHeight);

//3.11.2 ���߷����ͨ�˲�
//fDire(���߷���ԭʼ����),fFitDire(���߷����˲��������),iWidth(ͼ����),iHeight(ͼ��߶�)
int DircLowPass(float *fDire, float *fFitDire, int iWidth, int iHeight);

//3.12.1 ָ�Ƽ���Ƶ�ʼ���
//ucImg(Դͼ����),fDirection(���߷�������),fFrequency(����Ƶ�ʽ������),iWidth(ͼ����),iHeight(ͼ��߶�)
int Frequency(unsigned char * ucImg, float * fDirection, float * fFrequency, int iWidth, int iHeight);

//3.13.1 ָ���������
//ʵ��ָ���������
int GetMask(unsigned char * ucImg, float * fDirection, float *fFrequency, unsigned char * ucMask, int iWidth, int iHeight);

//3.14 ָ��ͼ��Gabor�˲���ǿ
//ucImg(Դͼ����),fDirection(���߷�������),ucMask(��������),ucImgEnhanced���˲���ǿ�������),iWidth(ͼ����),iHeight(ͼ��߶�)
int GaborEnhance(unsigned char * ucImg, float *fDirection, float *fFrequency, unsigned char * ucMask, unsigned char *ucImgEnhanced, int iWidth, int iHeight);

//3.16.1 ͼ���ֵ��
//ͼ���ֵ��
//ucImage(Դͼ����),ucBinImage(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�),uThreshold(��ֵ���Ҷ���ֵ)
int BinaryImg(unsigned char* ucImage, unsigned char* ucBinImage, int iWidth, int iHeight, unsigned char uThreshold);

//3.16.2 ��ֵͼ��ʾת��
//��ֵͼ��ʾת��([0,1]->[0,255])
//ucBinImg(Դͼ����),ucGrayImg(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�)
int BinaryToGray(unsigned char * ucBinImg, unsigned char* ucGrayImg, int iWidth, int iHeight);

//3.17.1 ͼ��ϸ���㷨
//ucBinedImg(ԭͼ����), ucThinnedImage(���ͼ����), iWidth(ͼ����), iHeight(ͼ��߶�), iIterativeLimit(����������)
int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit);

//3.18.1 ָ��������ȡ
//ucThinImg(Դͼ����), ucMinuImg(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�)
int Extract(unsigned char *ucThinImg, unsigned char *ucMinuImg, int iWidth, int iHeight);

//3.19.1 ָ����������
//����:minuData(����ͼ����),thinData(ϸ��ͼ����),minutiaes(����������),minuCount(����������),iWidth(ͼ����),iHeight(ͼ��߶�)
int  MinuFilter(unsigned char *minuData, unsigned char *thinData, MINUTIAE *minutiaes, int &minuCount, int iWidth, int iHeight);

//3.19.2 ȥ����Ե������
//����:minutiaes(����������), count(��������Ŀ), ucImg(Դͼ����), iWidth(ͼ����), iHeight(ͼ��߶�)
int CutEdge(MINUTIAE *minutiaes, int count, unsigned char *ucImg, int iWidth, int iHeight);

//3.20.1 ��������ģ���ļ�
//����:minutiaes(����������)��count(����������),filename(ģ���ļ���)
int SaveMinutiae(MINUTIAE *minutiaes, int count, char * filename);

//3.20.2 �����ָ�Ʊ��
int GetNewIndexInDB();

//3.20.4 ��ʾָ�������
//���ַ���ת��Ϊ���ֽ��ַ���
char *ToChar(wchar_t *str);

//3.23 ָ������ƥ��
//3.23.1 �������������ڹ�ϵ
//������minutiae(����������),minuCount(��������Ŀ)
int BuildNabors(MINUTIAE *minutiae, int minuCount);

//�������������������߶���б��
//����: x1(�˵�1 ������), y1(�˵�1 ������), ����: x2(�˵�2 ������), y2(�˵�2 ������)
float Angle2Points(int x1, int y1, int x2, int y2);

//3.23.2 ָ������ƥ�����ƶȼ���
//����:iWidth(ͼ����), iHeight(ͼ��߶�)��minutiae1(ָ��1������������)��account(ָ��1������������)
//minutiae2(ָ��2������������)��account(ָ��2������������)
float MinuSimilarity(int iWidth, int iHeight, MINUTIAE *minutiae1, int count1, MINUTIAE *minutiae2, int count2);

//3.23.3 ��ȡ����ģ���ļ�
//����:fileName(����ģ���ļ���),minutiae(����������)
int ReadMinutiae(char *fileName, MINUTIAE **minutiae);

//3.24.1 ָ�ƿ�Ԥ��
bool EmptyDB();

//3.25ָ�ƵǼ�ȫ�̼���
//3.25.1ָ�ƵǼǹ��̼���
//regName(�Ǽ�������),srcImgFile(Դͼ�ļ���),outImgFile(�������ͼ�ļ���),info(���ز����ɹ���ʧ����ʾ��Ϣ),bSaveImg(�Ƿ񱣴������м���ͼ��)
int Enroll(char *regName, char *srcImgFile, char *outImgFile, char *info, bool bSaveImg);

//3.26
//ָ��ʶ����̼���
//srcImgFile(Դͼ�ļ���),outImgFile(ʶ����ͼ�ļ���,info(���ز����ɹ���ʧ����ʾ��Ϣ����bSaveImg(�Ƿ񱣴������м���ͼ��
int Identify(char *srcImgFile, char *outImgFile, char *info, bool bSaveImg);

//��CString����ת��Ϊchar[]����
//ѡȡָ��ͼƬ�ļ�ʱʹ��
void CStirngToChar(CString cs, char ch[]);

//��ȡDebug��Release���ڵ�·��
CString GetModuleDir();

//ɾ��ָ�����ļ���
void DeleteDirectory(CString strDir);

//ɾ���ļ���
void DeleteAll();

