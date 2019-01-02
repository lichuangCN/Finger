
// FingerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Finger.h"
#include "FingerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFingerDlg �Ի���



CFingerDlg::CFingerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FINGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//�����ؼ��İ󶨹�ϵ
void CFingerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_ZKFPENGX, m_zkfpEng);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_STATIC_IMG_1, m_picCtrl1);
	DDX_Control(pDX, IDC_STATIC_IMG_2, m_picCtrl2);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
}

BEGIN_MESSAGE_MAP(CFingerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DATABASE, &CFingerDlg::OnBnClickedBtnDatabase)
	ON_BN_CLICKED(IDC_BTN_EXIT, &CFingerDlg::OnBnClickedBtnExit)
	ON_BN_CLICKED(IDC_BTN_REGISTER, &CFingerDlg::OnBnClickedBtnRegister)
	ON_BN_CLICKED(IDC_BTN_IDENTIFY, &CFingerDlg::OnBnClickedBtnIdentify)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CFingerDlg, CDialog)
	ON_EVENT(CFingerDlg, IDC_ZKFPENGX, 8, OnImageReceivedZkfpengx, VTS_PBOOL)
END_EVENTSINK_MAP()

// CFingerDlg ��Ϣ�������

BOOL CFingerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	InitControl();	//��ʼ���ؼ�

	InitDatabase();	//��������ʼ��ָ�ƿ�

	FingerConnect();	//�Զ�����ָ��ʶ����


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFingerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFingerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//����ָ��ʶ�������ܵ���ͼ��
void CFingerDlg::OnImageReceivedZkfpengx(BOOL * AImageValid)
{
	HDC hdc = this->GetDC()->m_hDC;	//�����ʾ�豸�����Ļ��������
	
	//int x = 80;	//ͼ����������ϽǺ�����
	//int y = 180;	

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_IMG_1);//IDC_pictureΪpicture�ؼ�ID
	pWnd->GetClientRect(&rect);//rcΪ�ؼ��Ĵ�С��
	int width = rect.Width()-2;	//ͼ����������
	int height = rect.Height()-2;	//ͼ��������߶�
	int x = 41;	//ͼ����������ϽǺ�����
	int y = 71;		//ͼ����������Ͻ�������

	m_zkfpEng.PrintImageAt(int(hdc), x, y, width, height);	//����ͼ��
}

/********************************************
*											*
*			CFingerprintDlg���Ա����		*
*											*											
********************************************/

//ָ�����Ǽ�
void CFingerDlg::OnBnClickedBtnRegister()
{
	//�����Ҳ�PictureΪ�հ�
	char emptyImgFile[MAX_PATH] = "empty.bmp";
	ShowImageInCtrl(m_picCtrl2, emptyImgFile);
	//�����Ϣ
	m_staticInfo.SetWindowText(ToWideChar(""));

	//��ʼ�����������Ϣ
	char info[MAX_PATH] = { 0 };

	//��ȡ������������(�Ǽ�������)
	UpdateData(true);

	//ʵʱ�ɼ�ͼ��
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));
	
	//ָ�ƵǼ�
	bool bSaveImg = false;	//�Ƿ񱣴����м�ͼ��
	char outImgFile[MAX_PATH] = { 0 };
	if (m_name.IsEmpty())
	{
		MessageBox(_T("�Ǽ�������Ϊ��."), _T("��ʾ"));//��ʾ��ʾ��Ϣ
		return;
	}

	Enroll(ToChar(m_name.GetBuffer()), srcImgFile, outImgFile, info, bSaveImg);

	//��ʾ���������Ϣ
	m_staticInfo.SetWindowText(ToWideChar(info));

	//��ʾͼ��
	ShowImageInCtrl(m_picCtrl1, srcImgFile);
	ShowImageInCtrl(m_picCtrl2, outImgFile);
}

//ָ����ʶ��
void CFingerDlg::OnBnClickedBtnIdentify()
{
	//�����Ҳ�PictureΪ�հ�
	char emptyImgFile[MAX_PATH] = "empty.bmp";
	ShowImageInCtrl(m_picCtrl2, emptyImgFile);
	//�����Ϣ
	m_staticInfo.SetWindowText(ToWideChar(""));

	//��ʼ�����������Ϣ
	char info[MAX_PATH] = { 0 };

	//��ȡ������������(�Ǽ�������)
	UpdateData(true);
	
	//ʵʱ�ɼ�ͼ��
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));

	//ָ��ʶ��
	bool bSaveImg = false;	//�Ƿ񱣴����м�ͼ��
	char outImgFile[MAX_PATH] = { 0 };

	if (Identify(srcImgFile, outImgFile, info, bSaveImg) < 0)
	{
		//��ʾ���������Ϣ
		m_staticInfo.SetWindowText(ToWideChar(info));
	}
	else
	{
		//��ʾ���������Ϣ
		m_staticInfo.SetWindowText(ToWideChar(info));
		//��ʾͼ��
		ShowImageInCtrl(m_picCtrl1, srcImgFile);
		ShowImageInCtrl(m_picCtrl2, outImgFile);
	}
}

//ָ��ʶ�����Զ�����
void CFingerDlg::FingerConnect()
{
	if (m_zkfpEng.InitEngine() == 0)//����ָ�Ʋɼ���
	{
		//MessageBox(_T("�ɼ������ӳɹ�."), _T("��ʾ"));//��ʾ��ʾ��Ϣ
		m_staticInfo.SetWindowText(ToWideChar("�ɼ������ӳɹ�."));	//��ʾ��ʾ��Ϣ
	}
	else
	{
		MessageBox(_T("�ɼ�������ʧ��,\n����ɼ����Ƿ�����."), _T("��ʾ"));//��ʾ��ʾ��Ϣ
		DeleteAll();		 //ɾ���ļ���
		OnOK();	//�ر�ϵͳ����
	}
}

//��ʼ�������ؼ�λ�ü���С
void CFingerDlg::InitControl()
{
	CWnd *pWnd1, *pWnd2;
	pWnd1 = GetDlgItem(IDC_STATIC_IMG_1); //��ȡ�ؼ�ָ��
	pWnd1->SetWindowPos(NULL, 40, 70, 160, 200, SWP_NOZORDER); //�Ѱ�ť�Ƶ����ڵ�(50,120)��,��ߣ�280,350��

	pWnd1 = GetDlgItem(IDC_STATIC_IMG_2); //��ȡ�ؼ�ָ��
	pWnd1->SetWindowPos(NULL, 280, 70, 160, 200, SWP_NOZORDER); //�Ѱ�ť�Ƶ����ڵ�(50,120)��,��ߣ�280,350��
}

//��ʾָ�ƿ���Ϣ
void CFingerDlg::OnBnClickedBtnDatabase()
{
	char info[MAX_PATH] = { 0 };	//	��ʼ����ʾ��Ϣ

	//�ж�ָ�ƿ��Ƿ�Ϊ��
	if (EmptyDB())
	{
		//MessageBox(_T("ָ�ƿ�Ϊ�գ�"), _T("��ʾ"));
		m_staticInfo.SetWindowText(ToWideChar("ָ�ƿ���ϢΪ��."));	//��ʾָ�ƿ���Ϣ
	}
	else
	{
		GetDatabaseInfo(info);	//��ȡָ�ƿ���Ϣ
		m_staticInfo.SetWindowText(ToWideChar(info));	//��ʾָ�ƿ���Ϣ
	}
}

//�˳�ָ��ʶ��ϵͳ
void CFingerDlg::OnBnClickedBtnExit()
{
	//ɾ���ļ���
	DeleteAll();
	OnOK();	//�ر�ϵͳ����
}

/********************************************
*											*
*				ȫ�ֺ���					*
*											*
********************************************/

//��ʾͼƬ
//������picCtrl(ͼ��ؼ�)��filename(ͼ���ļ�·��)
int ShowImageInCtrl(CStatic &picCtrl, char *filename)
{
	//����ͼ��
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(filename));

	//���ͼ�����
	int width = image.GetWidth();//ͼ��߶�
	int height = image.GetHeight();//ͼ��߶�

								   //������ʾ����
	CRect rect;//������ζ���
	picCtrl.GetClientRect(&rect);//���picture�ؼ����ڵľ�������
	CDC *pDc = picCtrl.GetWindowDC();//���picture�ؼ����豸�������
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//����Ϊͼ����ģʽ

													//��ʾͼ��
	image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//��ͼ�񻭵�Picture�ؼ���ʾ�ľ�������

												//���¿ؼ���ʾ
	picCtrl.Invalidate(false);

	//�ͷű����ռ�
	image.Destroy();
	picCtrl.ReleaseDC(pDc);//�ͷ�picture�ؼ����豸�������

	return 0;
}

//���ֽ��ַ���ת��ΪUnicode���ַ���
wchar_t *ToWideChar(char *str)
{
	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t *wideStr = new wchar_t[num];
	MultiByteToWideChar(0, 0, str, -1, wideStr, num);
	return wideStr;
}

//3.5.2 ��ȡͼ�������Ϣ
//��ȡͼ��ĸ߶ȣ���ȣ������Ϣ
int ReadBMPImgFilePara(char *filename, int &width, int &height, int &depth)
{
	CImage image;//����ͼ��
	HRESULT hResult = image.Load(ToWideChar(filename));
	if (FAILED(hResult) || image.IsNull())	//ͼƬ�𻵻����ļ������ڵ����ؾ��ɵ��¼���ʧ��
	{
		return -1;
	}

	//���ͼ�����
	width = image.GetWidth();//���������Ϊ��λ��ͼ����
	height = image.GetHeight();//���������Ϊ��λ��ͼ��߶�
	depth = image.GetBPP();//���������Ϊ��λ��ͼ�����
	if (depth != 8) //��֧��8λλͼ
	{
		return -2;
	}

	image.Destroy();//�ͷű����ռ�

	return 0;
}

//3.5.3 ��ͼ�ļ���ȡ����
//��ȡͼ��λͼ����
int ReadBMPImgFileData(char *filename, unsigned char *data)
{
	CImage image;//����λͼ
	HRESULT hResult = image.Load(ToWideChar(filename));
	if (FAILED(hResult) || image.IsNull())	//ͼƬ�𻵻����ļ������ڵ����ؾ��ɵ��¼���ʧ��
	{
		return -1;
	}

	//���ͼ�����
	int width = image.GetWidth();//���������Ϊ��λ��ͼ����
	int height = image.GetHeight();//���������Ϊ��λ��ͼ��߶�
	int depth = image.GetBPP();//���������Ϊ��λ��ͼ����ȣ�ÿ�����ص�λ����
	if (depth != 8) //��֧��8λλͼ
	{
		return -2;
	}

	memset(data, 0, width*height);	//��ʼ���������

									//��ȡͼ������
	int pitch = image.GetPitch();	//ÿ���������صĵ�ַƫ����
	unsigned char* pData1 = (unsigned char*)image.GetBits();	//ͼ��λͼ��������ַ
	unsigned char* pData2 = data;
	unsigned char gray = 0;

	unsigned char *pRow1, *pRow2, *pPix1, *pPix2;
	for (int y = 0; y < height; y++)	//����ɨ��
	{
		pRow1 = pData1 + pitch*y;
		pRow2 = pData2 + width*y;
		for (int x = 0; x < width; x++)	//����ɨ��
		{
			//��ȡԴͼ��Ҷ�ֵ
			pPix1 = pRow1 + x;
			gray = *pPix1;

			//���浽�������
			pPix2 = pRow2 + x;
			*pPix2 = gray;
		}//end for(x)
	}//end for (y)

	image.Destroy();
	return 0;
}

//3.5.4 ���ı��ļ���������
//����λͼ���ݵ��ı��ļ�
//dstFile(Ŀ���ļ�)��data(�޷����ַ���������)��width(ͼ����)��height(ͼ��߶�)
int SaveDataToTextFile(char *dstFile, unsigned char *data, int width, int height)
{
	//��Ŀ���ļ�
	ofstream fout(dstFile, ios::out);//ʹ�ø���д�뷽��
	if (!fout)
	{
		return -1;
	}

	//��ָ����ʽ���ļ�д������2
	int space = 5;	//ÿ���޷����ַ��������ݾ���5���ַ�λ�ȿ�洢
	for (int i = 0; i < height*width; i++)	//��������
	{
		fout << setw(space) << int(data[i]);//�ȿ�д������
		if (i % width == (width - 1))//��β
		{
			fout << endl;//����
		}
	}
	//�ر��ļ�
	fout.close();
	return 0;
}

//����λͼ���ݵ��ı���
//dstFile(Ŀ���ļ�)��data(��������������)��width(ͼ����)��height(ͼ��߶�)
int SaveDataToTextFile(char *dstFile, float *data, int width, int height)
{
	//��Ŀ���ļ�
	ofstream fout(dstFile, ios::out);//ʹ�ø���д�뷽��
	if (!fout)
	{
		return -1;
	}
	//��ָ����ʽ���ļ�д������
	int preci = 6;	//ÿ�����������ݾ�����6λС��λ
	int space = 16;	//ÿ�����������ݾ���16���ַ�λ�ȿ�洢

	fout.precision(preci);
	for (int i = 0; i < height*width; i++)
	{
		fout << " " << setw(space) << data[i];	//�ȿ�д������
		if (i%width == (width - 1))	//��β
		{
			fout << endl;	//����
		}
	}
	fout.close();	//�ر��ļ�

	return 0;
}

//3.5.5	����Ҫ�ķ���
//��CString����ת��Ϊchar[]����
void CStirngToChar(CString cs, char ch[]) {
	int i;
	char *tmpch;
	int wLwn = WideCharToMultiByte(CP_ACP, 0, cs, -1, NULL, 0, NULL, NULL);
	tmpch = new char[wLwn + 1];
	WideCharToMultiByte(CP_ACP, 0, cs, -1, tmpch, wLwn, NULL, NULL);
	for (i = 0; tmpch[i] != '\0'; i++)
	{
		ch[i] = tmpch[i];
	}
	ch[i] = '\0';
}

//3.6.1	����ָ�ƿ�������ļ�
//��������ʼ��ָ�ƿ�
void InitDatabase()
{
	_mkdir(TEMP_DIR);	//������ʱ�ļ���
	_mkdir(DB_DIR);		//����ָ�ƿ��ļ���
	InitFile(DB_INDEX_TXT);		//���������ļ�
}
//��������ʼ���ļ�
void InitFile(char * filename)
{
	FILE *index = fopen(filename, "w");	//���ļ��������򴴽������Ѵ��������������
	fclose(index);	//�ر��ļ�
}

//3.6.2	����ָ�ƿ�
//��ȡָ�ƿ�ͳ����Ϣ
void GetDatabaseInfo(char * info)
{
	//����ָ�ƿ�
	int	pNo = 0;	//ָ�����
	char name[MAX_PATH] = { 0 };	//�Ǽ�������
	char srcFile[MAX_PATH] = { 0 };	//ָ��ͼ���ļ�
	char mdlFile[MAX_PATH] = { 0 };	//ָ�������ļ�

	FILE *index = fopen(DB_INDEX_TXT, "r");		//�������ļ�
	while (!feof(index))
	{
		fscanf(index, "%d %s %s %s\n", &pNo, srcFile, mdlFile, name);
	}
	fclose(index);	//�ر��ļ�

	//ͳ��ָ�ƿ���Ϣ
	sprintf(info, "��ǰָ�ƿ��й��� %d ����¼", pNo);

}

//3.8.1	���ı��ļ���ȡ����
//���ı��ļ���ȡ�޷����Ը����͵�ͼ��λͼ����
//srcFile(Դ�ļ�),data(�޷����ַ�����������),iWidth(ͼ����),iHeight(ͼ��߶�)
int ReadDataFromTextFile(char * srcFile, unsigned char * data, int iWidth, int iHeight)
{
	//��Դ�ļ�
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail());
	{
		return -1;
	}

	//��ȡ����
	int d = 0;
	for (int i = 0; i < iHeight *iWidth; i++)
	{
		fin >> d;
		data[i] = (unsigned char)d;
	}

	//�ر��ļ�
	fin.close();

	return 0;
}

int ReadDataFromTextFile(char * srcFile, float * data, int iWidth, int iHeight)
{
	//��Դ�ļ�
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail());
	{
		return -1;
	}

	//��ȡ����
	for (int i = 0; i < iHeight*iWidth; i++)
	{
		fin >> data[i];
	}

	//�ر��ļ�
	fin.close();

	return 0;
}

//3.8.2	ͼ����ֵ�˲�
//������ucImg(ԭͼ����),ucDstImg(���ͼ������)��iWidth(ͼ����),iHeight(ͼ��߶�)
int MidFilter(unsigned char * ucImg, unsigned char * ucDstImg, int iWidth, int iHeight)
{
	//Step1:���ͼ�����ݳ�ʼ��
	memset(ucDstImg, 0, iWidth* iHeight);

	//Step2: ���������˲�(ʹ��3*3����)
	unsigned char* pUp, *pDown, *pImg;//����ȷ��3*3�����3��ͼ������ָ��(���ļ��"��ԡָ��")
	unsigned char x[9];//3*3����ͼ����������
	for (int i = 1; i < iHeight - 1; i++)//������2�е�������2��
	{
		//��ʼ����ԡָ��
		pUp = ucImg + (i - 1)*iWidth;
		pImg = ucImg + i*iWidth;
		pDown = ucImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++)//������2�е�������2��
		{
			//�ƶ���ԡָ��
			pUp++;
			pImg++;
			pDown++;

			//��ȡ3*3��������
			x[0] = *(pUp - 1);
			x[1] = *(pImg - 1);
			x[2] = *(pDown - 1);

			x[3] = *pUp;
			x[4] = *pImg;
			x[5] = *pDown;

			x[6] = *(pUp + 1);
			x[7] = *(pImg + 1);
			x[8] = *(pDown + 1);

			//��������
			Sort(x, 9);

			//���ͼ������ȡ������ֵ
			*(ucDstImg + i*iWidth + j) = x[4];
		}
	}

	//Step:3:��1�к����1���˲�(ʹ��2*3����)

	//��1��
	pDown = ucImg + iWidth;//����ָ���ʼ��
	for (int j = 1; j < iWidth - 1; j++)//��1�б�����2�е�������2��
	{
		//��ȡ2*3��������
		x[0] = *(ucImg + j - 1);
		x[1] = *(ucImg + j);
		x[2] = *(ucImg + j + 1);

		x[3] = *(pDown + j - 1);
		x[4] = *(pDown + j);
		x[5] = *(pDown + j + 1);

		//��������
		Sort(x, 6);

		//�ӹ�ȡ��ֵ
		*(ucDstImg + j) = x[3];
	}

	//���1��(������1��)
	pUp = ucImg + iWidth*(iHeight - 2);//����ָ���ʼ��
	pDown = ucImg + iWidth*(iHeight - 1);//����ָ���ʼ��
	for (int j = 1; j<iWidth - 1; j++)//���һ�б�����2�е�������2��
	{
		//��ȡ2*3��������
		x[0] = *(pDown + j - 1);
		x[1] = *(pDown + j);
		x[2] = *(pDown + j + 1);

		x[3] = *(pUp + j - 1);
		x[4] = *(pUp + j);
		x[5] = *(pUp + j + 1);

		//��������
		Sort(x, 6);

		//���ȡ��ֵ
		*(ucDstImg + iWidth*(iHeight - 1) + j) = x[3];
	}

	//Step4:4����������(ʹ��2*2����)

	//���Ͻǵ�
	x[0] = *(ucImg);//��ȡ2*2��������
	x[1] = *(ucImg + 1);
	x[2] = *(ucImg + iWidth);
	x[3] = *(ucImg + iWidth + 1);
	Sort(x, 4);//��������
	*(ucDstImg) = x[2];

	//���Ͻǵ�
	x[0] = *(ucImg + iWidth - 1);//��ȡ2*2��������
	x[1] = *(ucImg + iWidth - 2);
	x[2] = *(ucImg + 2 * iWidth - 1);
	x[3] = *(ucImg + 2 * iWidth - 2);
	Sort(x, 4);//��������
	*(ucDstImg + iWidth - 1) = x[2];//���ȡ��ֵ

									//���½ǵ�
	x[0] = *(ucImg + (iHeight - 1)*iWidth);//��ȡ2*2��������
	x[1] = *(ucImg + (iHeight - 2)*iWidth);
	x[2] = *(ucImg + (iHeight - 1)*iWidth + 1);
	x[3] = *(ucImg + (iHeight - 2)*iWidth + 1);
	Sort(x, 4);//��������
	*(ucDstImg + (iHeight - 1)*iWidth) = x[2];//���ȡ��ֵ

											  //���½ǵ�
	x[0] = *(ucImg + (iHeight - 0)*iWidth - 1);//��ȡ2*2��������
	x[1] = *(ucImg + (iHeight - 1)*iWidth - 1);
	x[2] = *(ucImg + (iHeight - 0)*iWidth - 2);
	x[3] = *(ucImg + (iHeight - 1)*iWidth - 2);
	Sort(x, 4);//��������
	*(ucDstImg + (iHeight - 0)*iWidth - 1) = x[2];//���ȡ��ֵ
	return 0;
}

//ʹ��ð�ݷ������������������
//����:data(����),dsize(���鳤��)
void Sort(unsigned char* data, int dsize)
{
	unsigned char temp = 0;
	for (int i = 0; i < dsize; i++)
	{
		for (int j = dsize - 1; j > i; j--)
		{
			if (data[j] < data[j - 1])//��������
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
		}
	}
}

//3.8.3
//�������ݵ�BMPͼ���ļ�
int SaveDataToImageFile(char *srcFile, char *dstFile, unsigned char *data)
{
	//�ļ�����
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//д������
	WriteBMPImgFile(dstFile, &data);

	return 0;
}

//�������ݵ�ͼ���ļ�
//srcFile(Դ�ļ���),dstFile(Ŀ���ļ���),data(��������������),scale(ת������)
int SaveDataToImageFile(char *srcFile, char *dstFile, float *data, float scale)
{
	//��ȡͼ�����
	int iWidth, iHeight, iDepth;//ͼ���ȡ��߶ȡ����
	ReadBMPImgFilePara(srcFile, iWidth, iHeight, iDepth);

	//�ļ�����
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//����ת��
	unsigned char *tmpData = new unsigned char[iWidth * iHeight];
	for (int i = 0; i<int(iWidth*iHeight); i++)
	{
		tmpData[i] = unsigned char((scale * data[i]));
	}

	//д������
	WriteBMPImgFile(dstFile, &tmpData);

	//�ͷ��ڴ�ռ�
	delete[] tmpData;

	return 0;
}

//��λͼ����д��BMPͼ���ļ�
//dstFileName(Ŀ���ļ���),pusImgData(��������������)
int WriteBMPImgFile(char *dstFileName, unsigned char **pusImgData)
{
	//���ļ�
	FILE *fp = fopen(dstFileName, "r+b");
	if (!fp)
	{
		return -1;
	}

	//��ȡͼ�������Ϣ(���ڶ�λ������д������)
	int imgType, iWidth, iHeight;//ͼ���ȡ��߶ȡ����
	int iStartPos = 0; 	//λͼ��������ʼ��ַ
	fseek(fp, 10L, SEEK_SET);
	fread((char *)(&iStartPos), 4, 1, fp);//��ȡλͼ��������ʼ��ַ(���ֽ�Ϊ��λ)
	fseek(fp, 18L, SEEK_SET);
	fread((char *)(&iWidth), 4, 1, fp);//��ȡͼ����(������Ϊ��λ)
	fread((char *)(&iHeight), 4, 1, fp);//��ȡͼ��߶�(������Ϊ��λ)

	unsigned short temp;
	fseek(fp, 28L, SEEK_SET);
	fread((char *)(&temp), 2, 1, fp);//��ȡͼ�����(ÿ�����ص�λ������λΪ��λ)

	imgType = temp;
	if (imgType != 8)	//��֧��8λ��BMPλͼͼ��
	{
		return -2;
	}

	//��������д������
	unsigned char *usImgData = *pusImgData;	//���������������ַ
	int iWidthInFile = 0;	//�ļ�ÿ�����ؿ�ȡ�����
	if (iWidth % 4 > 0)
	{
		iWidthInFile = iWidth - iWidth % 4 + 4;
	}
	else
	{
		iWidthInFile = iWidth;
	}
	for (int i = iHeight - 1; i >= 0; i--)
	{
		fseek(fp, iStartPos, SEEK_SET);	//��λ���׵�ַ
		fwrite((usImgData + i*iWidth), 1, iWidth, fp);	//д��һ������
		iStartPos += iWidthInFile;	//�������׵�ַ
	}

	//�ر��ļ�
	fclose(fp);

	return 0;
}

//3.9.1ͼ��ֱ��ͼ���⻯
//ucImg(Դͼ���ݣ���ucNormImg(���ͼ����,iWidth(ͼ���ȣ���iHeight(ͼ��߶ȣ�
int HistoNormalize(unsigned char* ucImg, unsigned char* unNormImg, int iWidth, int iHeight)
{
	//����Դͼ�Ҷ�ֱ��ͼ
	unsigned int Histogram[256];
	memset(Histogram, 0, 256 * sizeof(int));
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j <iWidth; j++)
		{
			Histogram[ucImg[i*iWidth + j]]++;
		}
	}

	double dMean = 0;
	for (int i = 1; i < 255; i++)
	{
		dMean += i*Histogram[i];
	}

	dMean = int(dMean / (iWidth*iHeight));

	double dSigma = 0;
	for (int i = 0; i < 255; i++)
	{
		dSigma += Histogram[i] * (i - dMean)*(i - dMean);
	}

	dSigma /= (iWidth*iHeight);
	dSigma = sqrt(dSigma);

	double dMean0 = 128, DSigma0 = 128;
	double dCoeff = DSigma0 / dSigma;
	for (int i = 0; i < iHeight; i++)
	{
		for (int j = 0; j < iWidth; j++)
		{
			double dVal = ucImg[i*iWidth + j];
			dVal = dMean0 + dCoeff*(dVal - dMean0);
			if (dVal<0)
			{
				dVal = 0;
			}
			else if (dVal > 255)
			{
				dVal = 255;
			}
			unNormImg[i*iWidth + j] = (unsigned char)dVal;
		}
	}
	return 0;
}

//3.11.1ָ�Ƽ��߷������
//ucImg(ͼ������),fDire(���߷�������),iWidth(ͼ����),iHeight(ͼ��߶�)
int ImgDirection(unsigned char *ucImg, float *fDire, int iWidth, int iHeight)
{
	//�������
	const int SEMISIZ = 7;//���򴰿�����뾶
	int dx[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	int dy[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	float fx, fy;

	//�����ʼ��
	memset(fDire, 0, iWidth*iHeight * sizeof(float));
	//����ÿһ���صļ��߷���ֵ
	for (int y = SEMISIZ + 1; y<iHeight - SEMISIZ - 1; y++)//���б���(��ȥ��Ե)
	{
		for (int x = SEMISIZ + 1; x<iWidth - SEMISIZ - 1; x++)//���б���(��ȥ��Ե)
		{
			//�����Ե�ǰ����Ϊ���ĵ����򴰿������ڵ�ÿһ���ص��ݶ�
			for (int j = 0; j<SEMISIZ * 2 + 1; j++)
			{
				for (int i = 0; i<SEMISIZ * 2 + 1; i++)
				{
					int index1 = (y + j - SEMISIZ)*iWidth + x + i - SEMISIZ;
					int index2 = (y + j - SEMISIZ)*iWidth + x + i - SEMISIZ - 1;
					int index3 = (y + j - SEMISIZ - 1)*iWidth + x + i - SEMISIZ;
					dx[i][j] = int(ucImg[index1] - ucImg[index2]);
					dy[i][j] = int(ucImg[index1] - ucImg[index3]);
				}
			}

			//���㵱ǰ���صļ��߷���ֵ
			fx = 0.0;
			fy = 0.0;
			for (int j = 0; j<SEMISIZ * 2 + 1; j++)
			{
				for (int i = 0; i<SEMISIZ * 2 + 1; i++)
				{
					fx += 2 * dx[i][j] * dy[i][j];
					fy += (dx[i][j] * dx[i][j] - dy[i][j] * dy[i][j]);
				}
			}
			fDire[y*iWidth + x] = atan2(fx, fy);
		}
	}

	return 0;
}

//3.11.2���߷����ͨ�˲�
//fDire(���߷���ԭʼ����),fFitDire(���߷����˲��������),iWidth(ͼ����),iHeight(ͼ��߶�)
int DircLowPass(float * fDire, float * fFitDire, int iWidth, int iHeight)
{
	//�������
	const int DIR_FILTER_SIZE = 2;
	int blocksize = 2 * DIR_FILTER_SIZE + 1;
	int imgsize = iWidth*iHeight;

	float* filter = new float[blocksize*blocksize];//ʹ��5*5�˲���
	float* phix = new float[imgsize];
	float* phiy = new float[imgsize];
	float* phi2x = new float[imgsize];
	float* phi2y = new float[imgsize];

	//�����ʼ��
	memset(fFitDire, 0, sizeof(float)*iWidth*iHeight);

	//����5*5��˹��ͨ�˲���ģ��
	float tempSum = 0.0;
	for (int y = 0; y<blocksize; y++)
	{
		for (int x = 0; x<blocksize; x++)
		{
			filter[y*blocksize + x] = (float)(blocksize - (abs(DIR_FILTER_SIZE - x) + abs(DIR_FILTER_SIZE - y)));
			tempSum += filter[y*blocksize + x];
		}
	}

	for (int y = 0; y<blocksize; y++)
	{
		for (int x = 0; x<blocksize; x++)
		{
			filter[y*blocksize + x] /= tempSum;
		}
	}

	//��������ص�ķ�������ֵ������ֵ
	for (int y = 0; y<iHeight; y++)
	{
		for (int x = 0; x<iWidth; x++)
		{
			phix[y*iWidth + x] = cos(fDire[y*iWidth + x]);
			phiy[y*iWidth + x] = sin(fDire[y*iWidth + x]);
		}
	}

	//���������ؽ��з����ͨ�˲�
	memset(phi2x, 0, sizeof(float)*imgsize);
	memset(phi2y, 0, sizeof(float)*imgsize);
	float nx, ny;
	int val;
	for (int y = 0; y<iHeight - blocksize; y++)//���б���(��ȥ��Ե����)
	{
		for (int x = 0; x<iWidth - blocksize; x++)//���б���(��ȥ��Ե����)
		{
			//���Ե�ǰ����Ϊ���ĵ��˲������ڵ���������ֵ���м�Ȩ�ۼ�
			nx = 0.0;
			ny = 0.0;
			for (int j = 0; j<blocksize; j++)
			{
				for (int i = 0; i<blocksize; i++)
				{
					val = (x + i) + (j + y)*iWidth;
					nx += filter[j*blocksize + i] * phix[val];//�������Ҽ�Ȩ�ۼ�
					ny += filter[j*blocksize + i] * phiy[val];//�������Ҽ�Ȩ�ۼ�
				}
			}
			//���ۼӽ����Ϊ��ǰ���ص��µķ�������ֵ������ֵ
			val = x + y*iWidth;
			phi2x[val] = nx;
			phi2y[val] = ny;
		}
	}

	//���ݼ�Ȩ�ۼӽ������������صķ����˲����ֵ
	for (int y = 0; y<iHeight - blocksize; y++)
	{
		for (int x = 0; x<iWidth - blocksize; x++)
		{
			val = x + y*iWidth;
			fFitDire[val] = atan2(phi2y[val], phi2x[val])*0.5;
		}
	}

	delete[] phi2y;
	delete[] phi2x;
	delete[] phiy;
	delete[] phix;

	return 0;
}

//3.12.1ָ�Ƽ���Ƶ�ʼ���
//ucImg(Դͼ����),fDirection(���߷�������),fFrequency(����Ƶ�ʽ������),iWidth(ͼ����),iHeight(ͼ��߶�)
int Frequency(unsigned char * ucImg, float * fDirection, float * fFrequency, int iWidth, int iHeight)
{
	//���ڴ�С
	const int SIZE_L = 32;
	const int SIZE_W = 16;
	const int SIZE_L2 = 16;
	const int SIZE_W2 = 8;

	//���Ҳ���ֵ��
	int peak_pos[SIZE_L];
	int peak_cnt;
	float peak_freq;
	float Xsig[SIZE_L];

	//����
	float dir = 0.0;
	float cosdir = 0.0;
	float sindir = 0.0;
	float maxPeak, minPeak;

	//�����ʼ��
	float *frequency1 = new float[iWidth * iHeight];
	memset(fFrequency, 0, sizeof(float) * iWidth * iHeight);
	memset(frequency1, 0, sizeof(float) * iWidth * iHeight);

	int x, y;
	int d, k;
	int u, v;

	for (y = SIZE_L2; y<iHeight - SIZE_L2; y++)//���б���(��ȥ��Ե��)
	{
		for (x = SIZE_L2; x<iWidth - SIZE_L2; x++)//���б���(��ȥ��Ե��)
		{
			//��ǰ���صļ��߷���
			dir = fDirection[(y + SIZE_W2)*iWidth + (x + SIZE_W2)];
			cosdir = -sin(dir);
			sindir = cos(dir);

			//�����Ե�ǰ����Ϊ���ĵ�L*W���򴰿ڵķ�ֵ���У�X[0]...X[L-1]
			for (k = 0; k<SIZE_L; k++)
			{
				Xsig[k] = 0.0;
				for (d = 0; d<SIZE_W; d++)
				{
					u = (int)(x + (d - SIZE_W2)*cosdir + (k - SIZE_L2)*sindir);
					v = (int)(y + (d - SIZE_W2)*sindir + (k - SIZE_L2)*cosdir);
					//�߽�㴦��
					if (u<0)
					{
						u = 0;
					}
					else if (u > iWidth - 1)
					{
						u = iWidth - 1;
					}
					if (v<0)
					{
						v = 0;
					}
					else if (v > iHeight - 1)
					{
						u = iHeight - 1;
					}
					Xsig[k] += ucImg[u + v*iWidth];
				}
				Xsig[k] /= SIZE_W;
			}

			//ȷ����ֵ���б仯��Χ
			maxPeak = minPeak = Xsig[0];
			for (k = 0; k<SIZE_L; k++)
			{
				if (minPeak > Xsig[k])
				{
					minPeak = Xsig[k];
				}
				if (maxPeak < Xsig[k])
				{
					maxPeak = Xsig[k];
				}
			}

			//ȷ����ֵ��λ��
			peak_cnt = 0;
			if ((maxPeak - minPeak) > 64)
			{
				for (k = 0; k<SIZE_L; k++)
				{
					if ((Xsig[k - 1] < Xsig[k]) && (Xsig[k] >= Xsig[k + 1]))
					{
						peak_pos[peak_cnt++] = k;
					}
				}
			}

			//�����ֵ��ƽ�����
			peak_freq = 0.0;
			if (peak_cnt >= 2)
			{
				for (k = 0; k<peak_cnt; k++)
				{
					peak_freq += (peak_pos[k + 1] - peak_pos[k]);
				}

				peak_freq /= peak_cnt - 1;
			}

			//���㵱ǰ���ص�Ƶ��
			if (peak_freq < 3.0 || peak_freq > 25.0)
			{
				frequency1[x + y*iWidth] = 0.0;
			}
			else
			{
				frequency1[x + y*iWidth] = 1.0 / peak_freq;
			}
		}
	}

	//��Ƶ�ʽ��о�ֵ�˲�
	for (y = SIZE_L2; y<iHeight - SIZE_L2; y++)
	{
		for (x = SIZE_L2; x<iWidth - SIZE_L2; x++)
		{
			k = x + y*iWidth;//��ǰ����λ��(��Ƶ�������е������±�)
			peak_freq = 0.0;
			//ʹ�õ�ǰ����Ϊ���ĵ�5*5���򴰿ڽ��о�ֵ�˲�
			for (v = -2; v <= 2; v++)
			{
				for (u = -2; u <= 2; u++)
				{
					peak_freq += frequency1[(x + u) + (y + v)*iWidth];//��Ƶ���ۻ���					
				}
			}
			fFrequency[k] = peak_freq / 25;	//��ǰ����Ƶ�ʵ��ڴ�����Ƶ�ʾ�ֵ
		}
	}

	delete[] frequency1;

	return 0;
}

//3.13.1 ָ���������
//ʵ��ָ���������
int GetMask(unsigned char * ucImg, float * fDirection, float *fFrequency, unsigned char * ucMask, int iWidth, int iHeight)
{
	float freqMin = 1.0 / 25.0;
	float freqMax = 1.0 / 3.0;
	int x, y, k;
	int pos, posout;
	memset(ucMask, 0, iWidth*iHeight);
	for (y = 0; y < iHeight; y++)
	{
		for (x = 0; x < iWidth; x++)
		{
			pos = x + y*iWidth;
			posout = x + y*iWidth;
			ucMask[posout] = 0;
			if (fFrequency[pos] >= freqMin&&fFrequency[pos] <= freqMax)
			{
				ucMask[posout] = 225;
			}
		}
	}
	//�ڶ��������׶���������̬ѧ������ԭ��
	for (k = 0; k < 4; k++)
	{
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				//ǰ��������������ĸ����ڵ㶼���Ϊǰ���㣨��8λ����ֵ�����λ��Ϊ1���Ա��)
				if (ucMask[x + y*iWidth] == 0xFF)
				{
					ucMask[x - 1 + y*iWidth] |= 0x80;
					ucMask[x + 1 + y*iWidth] |= 0x80;
					ucMask[x + (y - 1)*iWidth] |= 0x80;
					ucMask[x + (y + 1)*iWidth] |= 0x80;
				}
			}
		}
		//=========�жϺ�����ǰ����============
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				//=======�����Ϊǰ��������ض���Ϊǰ����========
				if (ucMask[x + y*iWidth])
				{
					ucMask[x + y*iWidth] = 0xFF;

				}
			}
		}
	}//��������ȥ����Ե�͹����㣨������̬ѧ�ĸ�ʴԭ��
	for (k = 0; k < 12; k++)
	{
		//��Ǳ�����
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				if (ucMask[x + y*iWidth] == 0x0)
				{
					ucMask[x - 1 + y*iWidth] &= 0x80;
					ucMask[x + 1 + y*iWidth] &= 0x80;
					ucMask[x + (y - 1)*iWidth] &= 0x80;
					ucMask[x + (y + 1)*iWidth] &= 0x80;
				}
			}
		}
		//�жϺ����ñ�����
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				if (ucMask[x + y*iWidth] != 0xFF)
				{
					ucMask[x + y*iWidth] = 0x0;
				}
			}
		}
	}
	return 0;
}

//3.14 ָ��ͼ��Gabor�˲���ǿ
//ucImg(Դͼ����),fDirection(���߷�������),ucMask(��������),ucImgEnhanced���˲���ǿ�������),iWidth(ͼ����),iHeight(ͼ��߶�)
int GaborEnhance(unsigned char * ucImg, float *fDirection, float *fFrequency, unsigned char * ucMask, unsigned char *ucImgEnhanced, int iWidth, int iHeight)
{
	//�Զ������
	const float PI = 3.141592654;
	int i, j, u, v;
	int wg2 = 5;//ʹ��11*11��Gablor�˲�������߳�Ϊ5
	float sum, f, g;
	float x2, y2;
	float dx2 = 1.0 / (4.0*4.0);//��x�᷽��ĸ�˹���糣��
	float dy2 = 1.0 / (4.0*4.0);//��y�᷽��ĸ�˹���糣��

								//�����ʼ��
	memset(ucImgEnhanced, 0, iWidth*iHeight);

	//Gabor�˲�
	for (j = wg2; j<iHeight - wg2; j++)//���б�����ȥ����Ե���Σ�
	{
		for (i = wg2; i<iWidth - wg2; i++)//���б�����ȥ����Ե���Σ�
		{
			//����������
			if (ucMask[i + j*iWidth] == 0)//����Ϊ0��ʾ������
			{
				continue;
			}

			//��ȡ��ǰ���صķ����Ƶ��
			g = fDirection[i + j*iWidth];//��ǰ���صļ��߷���
			f = fFrequency[i + j*iWidth];//��ǰ���صļ���Ƶ��
			g += PI / 2;//��ֱ����

						//�Ե�ǰ���ؽ���Gabor�˲�
			sum = 0.0;
			for (v = -wg2; v <= wg2; v++)//�˲��������б���
			{
				for (u = -wg2, u <= wg2; u++;)//�˲��������б���
				{
					x2 = -u*sin(g) + v*cos(g);//x������ת
					y2 = u*cos(g) + v*sin(g);//y������ת
					sum += exp(-0.5*(x2*x2*dx2 + y2*y2*dy2)) *cos(2 * PI*x2*f)*ucImg[(i - u) + (j - v)*iWidth];//�������˲�ֵ�ۼ�
				}
			}
			//�߽�ֵ����
			if (sum>255.0)
			{
				sum = 255.0;
			}
			if (sum<0.0)
			{
				sum = 0.0;
			}
			//�õ���ǰ���ص��˲����
			ucImgEnhanced[i + j*iWidth] = (unsigned char)sum;
		}
	}

	return 0;
}

//3.16.1 ͼ���ֵ��
//ͼ���ֵ��
//ucImage(Դͼ����),ucBinImage(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�),uThreshold(��ֵ���Ҷ���ֵ)
int BinaryImg(unsigned char * ucImage, unsigned char * ucBinImage, int iWidth, int iHeight, unsigned char uThreshold)
{
	//�ֱ���ָ��Դͼ���ݺͽ�����ݵ�����ָ��
	unsigned char *pStart = ucImage, *pEnd = ucImage + iWidth*iHeight;
	unsigned char *pDest = ucBinImage;
	//��һ������������
	while (pStart<pEnd)
	{
		*pDest = *pStart > uThreshold ? 1 : 0;//��ֵ��
		pStart++;//Դͼ����ָ�����
		pDest++;//���ͼ����ָ�����
	}
	return 0;
}

//3.16.2 ��ֵͼ��ʾת��
//��ֵͼ��ʾת��([0,1]->[0,255])
//ucBinImg(Դͼ����),ucGrayImg(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�)
int BinaryToGray(unsigned char * ucBinImg, unsigned char * ucGrayImg, int iWidth, int iHeight)
{
	//�ֱ���ָ��Դͼ���ݺͽ�����ݵ�����ָ��
	unsigned char *pStart = ucBinImg, *pEnd = ucBinImg + iWidth*iHeight;
	unsigned char *pDest = ucGrayImg;

	//��һ������������
	while (pStart < pEnd)
	{
		*pDest = (*pStart) > 0 ? 255 : 0;//ȡֵת��([0,1]->[0,255])
		pStart++;//Դͼ����ָ�����
		pDest++;//���ͼ����ָ�����
	}
	return 0;
}

//3.17ָ��ͼ��ϸ��
//3.17.1 ͼ��ϸ���㷨
//ucBinedImg(ԭͼ����), ucThinnedImage(���ͼ����), iWidth(ͼ����), iHeight(ͼ��߶�), iIterativeLimit(����������)
int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit)
{
	//�������
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8, xp;
	unsigned char g1, g2, g3, g4;
	unsigned char b1, b2, b3, b4;
	unsigned char np1, np2, npm;
	unsigned char *pUp, *pDown, *pImg;
	int iDeletedPoints = 0;

	//�����ʼ��
	memcpy(ucThinnedImage, ucBinedImg, iWidth*iHeight);
	for (int it = 0; it<iIterativeLimit; it++)
	{
		iDeletedPoints = 0;//��ʼ�����ε�����ɾ����

						   //���ε����ڵ�һ�α�����ʹ������G1&G2&G3��
		for (int i = 1; i<iHeight - 1; i++)
		{
			//��ʼ������ָ��
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i*iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j<iWidth - 1; j++)//���б���
			{
				//��������ָ��
				pUp++;
				pImg++;	//��ǰ����(������������)
				pDown++;

				//����������(����������ֵΪ0)
				if (!*pImg)
				{
					continue;
				}

				//��ȡ3*3���򴰿�����9�����صĻҶ�ֵ
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//�ж�����G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//�ж�����G2
				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1>np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;

				//�ж�����G3&G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//����ж�(ʹ������G1&G2&G3)
				if (g1 && g2 &&g3)
				{
					ucThinnedImage[iWidth*i + j] = 0; //ɾ����ǰ����
					++iDeletedPoints;	//���ε�����ɾ��+1
				}
			}//end for(j) ��
		}//end for(i) ��

		 //���ͬ��
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//���ε����ڵڶ��α�����ʹ������G1&G2&G4��
		for (int i = 1; i<iHeight - 1; i++)	//���б���
		{
			//��ʼ������ָ��
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i*iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j<iWidth - 1; j++)//���б���
			{
				//��������ָ��
				pUp++;
				pImg++;	//��ǰ����(������������)
				pDown++;

				//����������(����������ֵΪ0)
				if (!*pImg)
				{
					continue;
				}

				//��ȡ3*3���򴰿�����9�����صĻҶ�ֵ
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//�ж�����G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//�ж�����G2
				np1 = x1 || x2;
				np1 += x3 || x4;
				np1 += x5 || x6;
				np1 += x7 || x8;
				np2 = x2 || x3;
				np2 += x4 || x5;
				np2 += x6 || x7;
				np2 += x8 || x1;

				npm = np1>np2 ? np2 : np1;
				g2 = npm >= 2 && npm <= 3;

				//�ж�����G3&G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//����ж�(ʹ������G1&G2&G4)
				if (g1 && g2 && g4)
				{
					ucThinnedImage[iWidth*i + j] = 0; //ɾ����ǰ����
					++iDeletedPoints;	//���ε�����ɾ��+1
				}
			}//end for(j) ��
		}//end for(i) ��

		 //���ͬ��
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//������ε������޵��ɾ�������ֹͣ
		if (iDeletedPoints == 0)
		{
			break;
		}

		//�����Ե�ض�
		for (int i = 0; i<iHeight; i++)
		{
			for (int j = 0; j<iWidth; j++)
			{
				if (i < 16)	//�ϱ�Ե
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (i >= iHeight - 16)
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (j < 16)	//���Ե
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (j >= iWidth - 16)	//�ұ�Ե
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
			}
		}
	}

	return 0;
}

//3.18.1 ָ��������ȡ
//ucThinImg(Դͼ����), ucMinuImg(���ͼ����),iWidth(ͼ����),iHeight(ͼ��߶�)
int Extract(unsigned char *ucThinImg, unsigned char *ucMinuImg, int iWidth, int iHeight)
{
	//�������
	unsigned char *pDest = ucMinuImg;//���ͼ����ָ��
	unsigned char *pUp, *pDown, *pImg;//Դͼ����ָ��
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8;//���ڵ�
	unsigned char nc;
	int iMinuCount = 0;
	//�����ʼ��(ȫ��Ϊ0����ʾ���Ƿ�������)
	memset(pDest, 0, iWidth * iHeight);
	//����Դͼ����ȡָ������
	for (int i = 1; i < iHeight - 1; i++) {
		//��ʼ������ָ��
		pUp = ucThinImg + (i - 1)*iWidth;
		pImg = ucThinImg + i*iWidth;
		pDown = ucThinImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++) {
			//��������ָ��
			pUp++;
			pImg++;
			pDown++;

			//����������
			if (!*pImg) {
				continue;
			}
			//��ȡ3*3���򴰿�������8���ڵ����صĻҶ�ֵ
			x6 = *(pUp - 1);
			x5 = *(pImg - 1);
			x4 = *(pDown - 1);

			x7 = *pUp;
			x3 = *pDown;

			x8 = *(pUp + 1);
			x1 = *(pImg + 1);
			x2 = *(pDown + 1);

			//ͳ�ư��ڵ��кڵ�����
			nc = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;

			//�������ж�
			if (nc == 1)//�˵�
			{
				pDest[i*iWidth + j] = 1;//���ͼ�ж�Ӧ���ص�Ӧ��Ϊ1����ʾ�˵㣩
				++iMinuCount;//����������+1
			}
			else if (nc == 3)//�ֲ��
			{
				pDest[i*iWidth + j] = 3;
				++iMinuCount;//����������+1
			}
		}

	}
	return iMinuCount;//����������������
}

//3.19.1 ָ����������
//����:minuData(����ͼ����),thinData(ϸ��ͼ����),minutiaes(����������),minuCount(����������),iWidth(ͼ����),iHeight(ͼ��߶�)
int  MinuFilter(unsigned char *minuData, unsigned char *thinData, MINUTIAE *minutiaes, int &minuCount, int iWidth, int iHeight)
{
	float *dir = new float[iWidth*iHeight];
	memset(dir, 0, iWidth*iHeight * sizeof(float));

	ImgDirection(thinData, dir, iWidth, iHeight);

	unsigned char *pImg;
	unsigned char val;
	int temp = 0;
	for (int i = 1; i <iHeight - 1; i++)
	{
		pImg = minuData + i*iWidth;
		for (int j = 0; j < iWidth - 1; j++)
		{
			++pImg;
			val = *pImg;
			if (val > 0) {
				minutiaes[temp].x = j + 1;
				minutiaes[temp].y = i + 1;
				minutiaes[temp].theta = dir[i*iWidth + j];
				minutiaes[temp].type = int(val);
				++temp;
			}
		}
	}
	delete[] dir;
	minuCount = CutEdge(minutiaes, minuCount, thinData, iWidth, iHeight);

	int *pFlag = new int[minuCount];
	memset(pFlag, 0, sizeof(int)*minuCount);

	int x1, x2, y1, y2, type1, type2;
	for (int i = 0; i < minuCount; i++) {
		x1 = minutiaes[i].x;
		y1 = minutiaes[i].y;
		type1 = minutiaes[i].type;
		for (int j = i + 1; j < minuCount; j++) {
			if (pFlag[j] == 1) {
				continue;
			}
			x2 = minutiaes[j].x;
			y2 = minutiaes[j].y;
			type2 = minutiaes[j].type;

			int r = (int)sqrt(float((y1 - y2)*(y1 - y2) + (x1 - x2)*(x1 - x2)));

			if (r <= 4) {
				if (type1 == type2) {
					if (type1 == 1) {
						pFlag[i] = pFlag[j] = 1;
					}
					else {
						pFlag[j] = 1;
					}
				}
				else if (type1 == 1) {
					pFlag[i] = 1;
				}
				else {
					pFlag[j] = 1;
				}
			}
		}
	}
	int newCount = 0;	//��Ч����������
	for (int i = 0; i < minuCount; i++) {
		if (pFlag[i] == 0) {
			memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));
			newCount++;
		}
	}
	delete[] pFlag;
	minuCount = newCount;
	return 0;
}

//3.19.2 ȥ����Ե������
//����:minutiaes(����������), count(��������Ŀ), ucImg(Դͼ����), iWidth(ͼ����), iHeight(ͼ��߶�)
int CutEdge(MINUTIAE *minutiaes, int count, unsigned char *ucImg, int iWidth, int iHeight)
{
	int minuCount = count;
	int x, y, type;
	bool del;

	int *pFlag = new int[minuCount];
	memset(pFlag, 0, sizeof(int)*minuCount);

	for (int i = 0; i < minuCount; i++) {
		y = minutiaes[i].y - 1;
		x = minutiaes[i].x - 1;
		type = minutiaes[i].type;

		del = true;

		if (x < iWidth / 2) {
			if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y)) {
				while (--x >= 0) {
					if (ucImg[x + y*iWidth] > 0) {
						del = false;
						break;
					}
				}
			}
			else {
				if (y > iHeight / 2) {
					while (++y < iHeight) {
						if (ucImg[x + y*iWidth] > 0) {
							del = false;
							break;
						}
					}
				}
				else {
					while (--y >= 0) {
						if (ucImg[x + y*iWidth] > 0) {
							del = false;
							break;
						}
					}
				}
			}
		}
		else {
			if (abs(iWidth / 2 - x) > abs(iHeight / 2 - y)) {
				while (++x < iWidth) {
					if (ucImg[x + y*iWidth] > 0) {
						del = false;
						break;
					}
				}
			}
			else {
				if (y > iHeight / 2) {
					while (++y < iHeight) {
						if (ucImg[x + y*iWidth] > 0) {
							del = false;
							break;
						}
					}
				}
				else {
					while (--y >= 0) {
						if (ucImg[x + y*iWidth] > 0) {
							del = false;
							break;
						}
					}
				}
			}
		}
		if (del) {
			pFlag[i] = 1;
			continue;
		}
	}
	int newCount = 0;
	for (int i = 0; i < minuCount; i++) {
		if (pFlag[i] == 0) {
			memcpy(&minutiaes[newCount], &minutiaes[i], sizeof(MINUTIAE));
			newCount++;
		}
	}
	delete pFlag;
	pFlag = NULL;
	return newCount;
}

//3.20.1 ��������ģ���ļ�
//����:minutiaes(����������)��count(����������),filename(ģ���ļ���)
int SaveMinutiae(MINUTIAE *minutiaes, int count, char *fileName)
{
	FILE *fp = fopen(fileName, "wb");
	if (!fp)
	{
		return -1;
	}
	const static int TemplateFileFlag = 0x3571027f;
	fwrite(&TemplateFileFlag, sizeof(int), 1, fp);
	fwrite(&count, sizeof(int), 1, fp);
	for (int i = 0; i < count; i++)
	{
		fwrite(&(minutiaes[i]), sizeof(MINUTIAE), 1, fp);
	}
	fclose(fp);
	return 0;
}

//3.20.2 
//�����ָ�Ʊ��
int GetNewIndexInDB()
{
	//��index�ļ��ж�ȡ���һ����¼�ı��
	int sNo = 0;
	char name[MAX_PATH] = { 0 }, srcFile[MAX_PATH] = { 0 }, mdlFile[MAX_PATH] = { 0 };
	FILE *index = fopen(DB_INDEX_TXT, "r");
	while (!feof(index)) {
		fscanf(index, "%d %s %s %s\n", &sNo, srcFile, mdlFile, name);
	}
	fclose(index);
	sNo = sNo + 1;
	return sNo;
}

//3.20.4 ��ʾָ�������
//���ַ���ת��Ϊ���ֽ��ַ���
char *ToChar(wchar_t *str)
{
	int num = WideCharToMultiByte(0, 0, str, -1, NULL, 0, NULL, false);
	char *chStr = new char[num];;
	WideCharToMultiByte(0, 0, str, -1, chStr, num, NULL, false);
	return chStr;
}

//3.23.1 �������������ڹ�ϵ
//�������������������߶���б��
//����: x1(�˵�1 ������), y1(�˵�1 ������), ����: x2(�˵�2 ������), y2(�˵�2 ������)
float Angle2Points(int x1, int y1, int x2, int y2)
{
	const float PI = 3.141592654;
	float diffY, diffX;
	float theta = 0.0;

	diffY = y2 - y1;
	diffX = x2 - x1;

	if (diffY<0 && diffX>0)
	{
		theta = atan2(-1 * diffY, diffX);
	}
	else if (diffY < 0 && diffX < 0)
	{
		theta = PI - atan2(-1 * diffY, -1 * diffX);
	}
	else if (diffY>0 && diffX<0)
	{
		theta = atan2(diffY, -1 * diffX);
	}
	else if (diffY>0 && diffX>0)
	{
		theta = PI - atan2(diffY, diffX);
	}
	else if (diffX == 0)
	{
		theta = PI / 2;
	}
	else
	{
		theta = 0.0;
	}

	return theta;
}

//�������������ڹ�ϵ
//������minutiae(����������),minuCount(��������Ŀ)
int BuildNabors(MINUTIAE *minutiae, int minuCount)
{
	const int MAX_NEIGHBOR_EACH = 10;
	int x1, x2, y1, y2;
	int *pFlag = new int[minuCount];

	for (int i = 0; i < minuCount; i++)
	{
		x1 = minutiae[i].x;
		y1 = minutiae[i].y;

		memset(pFlag, 0, sizeof(int)*minuCount);
		pFlag[i] = 1;

		minutiae[i].neibors = new NEIGHBOR[MAX_NEIGHBOR_EACH];
		if (minutiae[i].neibors == NULL)
		{
			return -1;
		}
		memset(minutiae[i].neibors, 0, sizeof(NEIGHBOR)*MAX_NEIGHBOR_EACH);

		for (int neighborNo = 0; neighborNo < MAX_NEIGHBOR_EACH; neighborNo++)
		{
			int  minDistance = 1000;
			int minNo = 0;

			for (int j = 0; j < minuCount; j++)
			{
				if (pFlag[j] == 1)
				{
					continue;
				}

				x2 = minutiae[j].x;
				y2 = minutiae[j].y;
				int r = (int)sqrt(float((y1 - y2)*(y1 - y2) + (x1 - x2)*(x1 - x2)));

				if (r < minDistance)
				{
					minNo = j;
					minDistance = r;
				}
			}

			pFlag[minNo] = 1;
			minutiae[i].neibors[neighborNo].x = minutiae[minNo].x;
			minutiae[i].neibors[neighborNo].y = minutiae[minNo].y;
			minutiae[i].neibors[neighborNo].type = minutiae[minNo].type;
			minutiae[i].neibors[neighborNo].Theta = Angle2Points(minutiae[minNo].x, minutiae[minNo].y, x1, y1);
			minutiae[i].neibors[neighborNo].Theta2Ridge = minutiae[minNo].theta - minutiae[i].theta;
			minutiae[i].neibors[neighborNo].ThetaThisNibor = minutiae[minNo].theta;
			minutiae[i].neibors[neighborNo].distance = minDistance;
		}
	}

	delete[] pFlag;
	return 0;
}

//3.23.2 ָ������ƥ�����ƶȼ���
//����:iWidth(ͼ����), iHeight(ͼ��߶�)��minutiae1(ָ��1������������)��count1(ָ��1������������)
//minutiae2(ָ��2������������)��count2(ָ��2������������)
float MinuSimilarity(int iWidth, int iHeight, MINUTIAE *minutiae1, int count1, MINUTIAE *minutiae2, int count2)
{
	const int MAX_SIMILAR_PAIR = 100;
	const int MAX_NEIGHBOR_EACH = 10;

	BuildNabors(minutiae1, count1);
	BuildNabors(minutiae2, count2);

	//��ʼ��������ƥ����
	int similarPair[MAX_SIMILAR_PAIR][2];
	memset(similarPair, 0, 100 * 2 * sizeof(int));

	//ѡ���׼�����Ͳο�����
	MINUTIAE *baseMinutiae;	//��׼����������
	MINUTIAE *refMinutiae;//�ο�����������

	int baseAccount, refAccount;
	if (count1 < count2)
	{
		baseMinutiae = minutiae1;
		baseAccount = count1;
		refMinutiae = minutiae2;
		refAccount = count2;
	}
	else
	{
		baseMinutiae = minutiae2;
		baseAccount = count2;
		refMinutiae = minutiae1;
		refAccount = count1;
	}

	NEIGHBOR *baseNeighbors = NULL;		//��׼������ڵ�ָ��
	NEIGHBOR *refNeighbors = NULL;		//�ο�������ڵ�ָ��
	int similarMinutiae = 0;		//	���Ƶ������
	float baseTheta, refTheta;		//��׼����Ͳο�����

									//������ƥ��
	for (int i = 0; i < baseAccount; i++)
	{
		//��ȡ��ǰ��׼����Ϣ
		baseNeighbors = baseMinutiae[i].neibors;	//��׼������ڵ�����
		baseTheta = baseMinutiae[i].theta;		//��׼��ļ��߷���

												//�ڲο�������Ѱ�ҵ�ǰ��׼�������ƵĲο���
		int refSimilarNo = 0;
		int maxSimilarNeibors = 0;

		for (int j = 0; j < refAccount; j++)
		{
			//�����뵱ǰ��׼�����Ͳ�ͬ�Ĳο���
			if (refMinutiae[j].type != baseMinutiae[i].type)
			{
				continue;
			}

			//��ȡ��ǰ�ο���Ϣ
			refNeighbors = refMinutiae[j].neibors;	//�ο�������ڵ�����
			refTheta = refMinutiae[j].theta;	//�ο���ļ��߷���

												//ͳ�����Ƶ�����
			int thisSimilarNeigbors = 0;
			for (int m = 0; m < MAX_NEIGHBOR_EACH; m++)
			{
				//�ڲο�������ڵ������в����뵱ǰ��׼��ĵ�ǰ���ڵ����Ƶ����ڵ�
				for (int n = 0; n < MAX_NEIGHBOR_EACH; n++)
				{
					//�������Ͳ�ͬ�����

					if (baseNeighbors[m].type != refNeighbors[n].type)
					{
						continue;
					}

					//�����������ڵ�֮��ľ����Ͳ�ͬ�ĽǶȲ�
					int dist = abs(int(baseNeighbors[m].distance - refNeighbors[n].distance));
					float theta1 = abs(float((baseNeighbors[m].Theta - baseTheta) - (refNeighbors[n].Theta - refTheta)));
					float theta2 = abs(float(baseNeighbors[m].Theta2Ridge - refNeighbors[n].Theta2Ridge));
					float theta3 = abs(float((baseNeighbors[m].Theta - baseNeighbors[m].ThetaThisNibor) - (refNeighbors[n].Theta - refNeighbors[n].ThetaThisNibor)));

					//��������С��ָ��ֵ
					if (dist<4 && theta1<0.15f && theta2<0.15f && theta3 < 0.15f)
					{
						++thisSimilarNeigbors;
						break;
					}
				}
			}

			//���3���������ڵ����ƣ�����Ϊ��ǰ��׼���뵱ǰ�ο������ƣ�����ƥ����
			if ((thisSimilarNeigbors >= MAX_NEIGHBOR_EACH * 3 / 10) && (similarMinutiae < MAX_SIMILAR_PAIR))
			{
				similarPair[similarMinutiae][0] = i;
				similarPair[similarMinutiae][1] = refSimilarNo;
				++similarMinutiae;
			}
		}
	}

	//��������ƥ�����ƶ�
	float similarity = similarMinutiae / 8.0f;
	//�����������������С��ָ��������ֵ
	similarity = similarMinutiae < 2 ? 0.0f : similarity;
	//���������������������ָ��������ֵ
	similarity = similarMinutiae > 8 ? 1.0f : similarity;

	//��������ƥ�����ƶ�
	return similarity;
}

//3.23.3 ��ȡ����ģ���ļ�
//����:fileName(����ģ���ļ���),minutiae(����������)
int ReadMinutiae(char *fileName, MINUTIAE **minutiae)
{
	FILE *fp = fopen(fileName, "rb");

	if (!fp)
	{
		return -1;
	}

	const static int TemplateFileFlag = 0x3571027f;
	int flag;
	fread(&flag, sizeof(int), 1, fp);
	if (flag != TemplateFileFlag)
	{
		return -2;
	}

	int account;
	fread(&account, sizeof(int), 1, fp);

	*minutiae = new MINUTIAE[account];//����������ṹ����
	if (*minutiae == NULL)
	{
		return -3;
	}
	for (int i = 0; i < account; i++)
	{
		fread(&((*minutiae)[i]), sizeof(MINUTIAE), 1, fp);
	}

	//�ر��ļ�
	fclose(fp);
	return account;
}

//3.24.1 ָ�ƿ�Ԥ��
//�ж�ָ�ƿ��Ƿ�Ϊ��
bool EmptyDB()
{
	//���ļ�
	char indexFile[MAX_PATH] = { DB_INDEX_TXT };//ָ�ƿ������ļ�
	FILE *pFile = fopen(indexFile, "r");
	if (!pFile)//�������ļ������ڻ�����쳣,��ָ�ƿ�Ϊ��
	{
		return true;
	}

	//�ж�ָ�ƿ��Ƿ�Ϊ��(�����ļ������Ƿ�Ϊ��)
	int size = filelength(fileno(pFile));//�ļ�����(���ֽ�Ϊ��λ)
	bool bEmpty = (size < 1) ? true : false;//�������ļ�����Ϊ��,��ָ�ƿ�Ϊ��

											//�ر��ļ�
	fclose(pFile);

	//�����жϽ��
	return bEmpty;
}

//3.25 ָ�ƵǼ�ȫ�̼���
//3.25.1 ָ�ƵǼǹ��̼���
//regName(�Ǽ�������),srcImgFile(Դͼ�ļ���),outImgFile(�������ͼ�ļ���),info(���ز����ɹ���ʧ����ʾ��Ϣ),bSaveImg(�Ƿ񱣴������м���ͼ��)
int Enroll(char *regName, char *srcImgFile, char *outImgFile, char *info, bool bSaveImg)
{
	//��0���������м����ļ������м��������ļ���
	char stepTxtFile1[MAX_PATH] = { STEP_TXT_1 };
	char stepTxtFile2[MAX_PATH] = { STEP_TXT_2 };
	char stepTxtFile3[MAX_PATH] = { STEP_TXT_3 };
	char stepTxtFile4[MAX_PATH] = { STEP_TXT_4 };
	char stepTxtFile5[MAX_PATH] = { STEP_TXT_5 };
	char stepTxtFile6[MAX_PATH] = { STEP_TXT_6 };
	char stepTxtFile7[MAX_PATH] = { STEP_TXT_7 };
	char stepTxtFile8[MAX_PATH] = { STEP_TXT_8 };
	char stepTxtFile9[MAX_PATH] = { STEP_TXT_9 };
	char stepTxtFile10[MAX_PATH] = { STEP_TXT_10 };
	char stepTxtFile11[MAX_PATH] = { STEP_TXT_11 };

	//�м���ͼ���ļ���
	char stepImgFile1[MAX_PATH] = { STEP_IMG_1 };
	char stepImgFile2[MAX_PATH] = { STEP_IMG_2 };
	char stepImgFile3[MAX_PATH] = { STEP_IMG_3 };
	char stepImgFile4[MAX_PATH] = { STEP_IMG_4 };
	char stepImgFile5[MAX_PATH] = { STEP_IMG_5 };
	char stepImgFile6[MAX_PATH] = { STEP_IMG_6 };
	char stepImgFile7[MAX_PATH] = { STEP_IMG_7 };
	char stepImgFile8[MAX_PATH] = { STEP_IMG_8 };
	char stepImgFile9[MAX_PATH] = { STEP_IMG_9 };
	char stepImgFile10[MAX_PATH] = { STEP_IMG_10 };
	char stepImgFile11[MAX_PATH] = { STEP_IMG_11 };
	char stepImgFile12[MAX_PATH] = { STEP_IMG_12 };

	//ָ������ģ���ļ���
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//��1��:ͼ������,��ȡԴͼ���ݺͲ�����Ϣ
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//��ȡԴͼ������Ϣ
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//��ȡԴͼ����
	memset(image2, 0, iWidth*iHeight);//���ͼ��ʼ��
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}

	//��2��:��ֵ�˲�
	MidFilter(image1, image2, iWidth, iHeight);//��ֵ�˲�
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//��3��:ֱ��ͼ���⻯
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�2���˲����ΪԴͼ����
	HistoNormalize(image1, image2, iWidth, iHeight); //ֱ��ͼ���⻯
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//��4��:�������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	float *tmpDirection = new float[iWidth*iHeight];
	float *direction = new float[iWidth*iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//�������
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//�����ͨ�˲�
	if (bSaveImg)
	{
		const int DIRECTION_SCALE = 100;//������ת������(�����ڽ����ʾ)
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction, DIRECTION_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//��5��:Ƶ�ʼ���
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	float *frequency = new float[iWidth*iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//Ƶ�ʼ���
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//Ƶ�ʽ��ת������(�����ڽ����ʾ)
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency, FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//��6��:�������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//�������
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//��7��:Gabor�˲���ǿ
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//��ǿ
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}
	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//��8��:��ֵ��
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�7��Gabor�˲����ΪԴͼ����
	BinaryImg(image1, image2, iWidth, iHeight, 128);//��ֵ��
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//��9��:ϸ��
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�8����ֵ�����ΪԴͼ����
	Thinning(image1, image2, iWidth, iHeight, 128);//ϸ��
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char *thin = new unsigned char[iWidth*iHeight];
	memcpy(thin, image2, sizeof(unsigned char)*iWidth*iHeight);//����ϸ�����

															   //��10��:������ȡ
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�9��ϸ�����ΪԴͼ����
	int minuCount = Extract(image1, image2, iWidth, iHeight);//������ȡ
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}

	//��11��:��������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�10��������ȡ���ΪԴͼ����
	MINUTIAE *minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);//��������
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//�������������ļ�
	if (true)
	{
		memset(image2, 0, iWidth*iHeight);
		for (int i = 0; i < minuCount; i++)
		{
			image2[(minutiaes[i].y - 1)*iWidth + (minutiaes[i].x - 1)] = 0xff;
		}
		SaveDataToImageFile(srcImgFile, stepImgFile11, image2);
		SaveDataToTextFile(stepTxtFile11, image2, iWidth, iHeight);
	}
	strcpy(outImgFile, stepImgFile11);//�����������ͼ�ļ���

									  //��12��:�������
	int sNo = GetNewIndexInDB();//������ݿ���ָ�Ƽ�¼���
	char dbImgFile[MAX_PATH] = { 0 };//���ݿ���Դͼ�ļ���
	char dbMdlFile[MAX_PATH] = { 0 };//���ݿ���ģ���ļ���
	sprintf(dbImgFile, "%s\\%d%s", DB_DIR, sNo, ".bmp");//����Դͼ�ļ���(�������ݿ��ļ�����,�Ա������)
	sprintf(dbMdlFile, "%s\\%d%s", DB_DIR, sNo, ".mdl");//����ģ���ļ���(�������ݿ��ļ�����,�Ա������)

	if (minuCount > 0)
	{
		//����ԭʼָ��ͼ��
		CopyFile(ToWideChar(srcImgFile), ToWideChar(dbImgFile), false);

		//����ָ������ģ���ļ�
		CopyFile(ToWideChar(stepMdlFile11), ToWideChar(dbMdlFile), false);

		//ָ�ƵǼ���Ϣд�����ݿ������ļ�
		FILE *index = fopen(DB_INDEX_TXT, "a");//׷��ģʽд��
		fprintf(index, "%d %s %s %s\n", sNo, dbImgFile, dbMdlFile, regName);
		fclose(index);
	}

	//�ͷſռ�
	delete[] image1;
	delete[] image2;
	delete[] minutiaes;

	sprintf(info, "ָ�ƵǼǳɹ�");
	return 0;
}

//3.26 ָ��ʶ����̼���
//3.26.1 ָ��ʶ����̼���
//srcImgFile(Դͼ�ļ���),outImgFile(ʶ����ͼ�ļ���,info(���ز����ɹ���ʧ����ʾ��Ϣ����bSaveImg(�Ƿ񱣴������м���ͼ��
int Identify(char *srcImgFile, char *outImgFile, char *info, bool bSaveImg)
{
	//��0���������м����ļ������м��������ļ���
	char stepTxtFile1[MAX_PATH] = { STEP_TXT_1 };
	char stepTxtFile2[MAX_PATH] = { STEP_TXT_2 };
	char stepTxtFile3[MAX_PATH] = { STEP_TXT_3 };
	char stepTxtFile4[MAX_PATH] = { STEP_TXT_4 };
	char stepTxtFile5[MAX_PATH] = { STEP_TXT_5 };
	char stepTxtFile6[MAX_PATH] = { STEP_TXT_6 };
	char stepTxtFile7[MAX_PATH] = { STEP_TXT_7 };
	char stepTxtFile8[MAX_PATH] = { STEP_TXT_8 };
	char stepTxtFile9[MAX_PATH] = { STEP_TXT_9 };
	char stepTxtFile10[MAX_PATH] = { STEP_TXT_10 };
	char stepTxtFile11[MAX_PATH] = { STEP_TXT_11 };

	//�м��������ļ���
	char stepImgFile1[MAX_PATH] = { STEP_IMG_1 };
	char stepImgFile2[MAX_PATH] = { STEP_IMG_2 };
	char stepImgFile3[MAX_PATH] = { STEP_IMG_3 };
	char stepImgFile4[MAX_PATH] = { STEP_IMG_4 };
	char stepImgFile5[MAX_PATH] = { STEP_IMG_5 };
	char stepImgFile6[MAX_PATH] = { STEP_IMG_6 };
	char stepImgFile7[MAX_PATH] = { STEP_IMG_7 };
	char stepImgFile8[MAX_PATH] = { STEP_IMG_8 };
	char stepImgFile9[MAX_PATH] = { STEP_IMG_9 };
	char stepImgFile10[MAX_PATH] = { STEP_IMG_10 };
	char stepImgFile11[MAX_PATH] = { STEP_IMG_11 };
	char stepImgFile12[MAX_PATH] = { STEP_IMG_12 };

	//ָ������ģ���ļ���
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//ָ�ƿ�Ԥ�죨�ж�ָ�ƿ��Ƿ�Ϊ�գ�
	EmptyDB();


	//��1����ͼ�����룬��ȡԴͼ���ݺͲ�����Ϣ
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//��ȡԴͼ������Ϣ
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//��ȡԴͼ����
	memset(image2, 0, iWidth*iHeight);//���ͼ��ʼ��
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}

	//��2������ֵ�˲�
	MidFilter(image1, image2, iWidth, iHeight);//��ֵ�˲�
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//��3����ֱ��ͼ���⻯
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�2���˲�ΪԴͼ����
	HistoNormalize(image1, image2, iWidth, iHeight);//ֱ��ͼ���⻯
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//��4�����������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե��������⻯���ΪԴͼ����
	float *tmpDirection = new float[iWidth*iHeight];
	float *direction = new float[iWidth*iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//�������	
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//�����ͨ�˲�
	if (bSaveImg)
	{
		const int DIRECTIOB_SCALE = 100;//������ת�������������ڽ����ʾ��
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction, DIRECTIOB_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//��5����Ƶ�ʼ���
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	float *frequency = new float[iWidth*iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//Ƶ�ʼ���
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//Ƶ�ʽ��ת�������������ڽ����ʾ��
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency, FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//��6�����������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե��������⻯���ΪԴͼ����
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//�������
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//��7����Gabor�˲���ǿ
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�3�����⻯���ΪԴͼ����
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//��ǿ
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}
	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//��8������ֵ��
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�7��Gabor�˲����ΪԴͼ����
	BinaryImg(image1, image2, iWidth, iHeight, 128);//��ֵ��
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//��9����ϸ��
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�8����ֵ�����ΪԴͼ����
	Thinning(image1, image2, iWidth, iHeight, 128);//ϸ��
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char *thin = new unsigned char[iWidth*iHeight];
	memcpy(thin, image2, sizeof(unsigned char)*iWidth*iHeight);//����ϸ�����

															   //��10����������ȡ
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//�Ե�9��ϸ�����ΪԴͼ����
	int minuCount = Extract(image1, image2, iWidth, iHeight);//������ȡ
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}


	//��11������������
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight); //�Ե�10��������ȡ���ΪԴͼ����
	MINUTIAE *minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//�������������ļ�
	if (bSaveImg)
	{
		memset(image2, 0, iWidth*iHeight);
		for (int i = 0; i<minuCount; i++)
		{
			image2[(minutiaes[i].y - 1)*iWidth + (minutiaes[i].x - 1)] = 0xff;
		}
		SaveDataToImageFile(srcImgFile, stepImgFile11, image2);
		SaveDataToTextFile(stepTxtFile11, image2, iWidth, iHeight);
	}
	delete[] image1;
	delete[] image2;

	//��12��������ʶ��
	int id = 0;
	char name[MAX_PATH] = { 0 }, src[MAX_PATH] = { 0 }, mdl[MAX_PATH] = { 0 };
	float maxSimilar = -1;
	int maxID = -1;
	char maxName[MAX_PATH] = { 0 }, maxSrc[MAX_PATH] = { 0 };
	MINUTIAE *minutiaes2 = NULL;
	int minuCount2 = 0;

	FILE *index = fopen(DB_INDEX_TXT, "r");//��ָ�ƿ������ļ�

	while (!feof(index))
	{
		//��ȡָ�Ƽ�¼��Ϣ��ָ�Ʊ��/ָ��ͼ���ļ���/����ģ���ļ���.�Ǽ���������
		fscanf(index, "%d %s %s %s\n", &id, src, mdl, name);

		//������ģ���ļ��ж�ȡָ������
		minuCount2 = ReadMinutiae(mdl, &minutiaes2);

		//����ƥ��
		float similar = MinuSimilarity(iWidth, iHeight, minutiaes, minuCount, minutiaes2, minuCount2);

		//�������ƶ���ߵ�ָ�Ƽ�¼��Ϣ
		if (similar > maxSimilar)
		{
			maxSimilar = similar;
			maxID = id;
			sprintf(maxName, name);
			sprintf(maxSrc, src);
		}
	}

	fclose(index);//�ر��ļ�

	delete[] minutiaes;
	delete[] minutiaes2;

	//����ʶ����
	const float SIMILAR_THRED = 0.1;//ƥ�����ƶȷ�ֵ��С�ڷ�ֵ����Ϊ��ƥ�䣩
	if (maxID<0 || maxSimilar < SIMILAR_THRED)
	{
		sprintf(info, "ʶ��ʧ��.");
		return -2;
	}

	strcpy(outImgFile, maxSrc);//����ʶ����ͼ���ļ���
	if (bSaveImg)
	{
		CopyFile(ToWideChar(maxSrc), ToWideChar(stepImgFile12), false);//����ʶ����ͼ��
	}
	sprintf(info, "ʶ��ɹ�. ʶ����������[%s],Ŀ��ָ��[%s],���ƶ�[%.2f].", maxName, maxSrc, maxSimilar);
	return 0;
}

//��ȡDebug��Release���ڵ�·��
CString GetModuleDir()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	return path;
}

//ɾ��ָ�����ļ���
void DeleteDirectory(CString strDir)
{
	if (strDir.IsEmpty())
	{
		RemoveDirectory(strDir);
		return;
	}

	//����ɾ���ļ������ļ��� 
	CFileFind   ff;
	BOOL bFound = ff.FindFile(strDir + _T("\\*"),0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//ȥ���ļ�(��)ֻ�������� 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			//�ݹ�ɾ�����ļ��� 
			DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());   //ɾ���ļ� 
		}
	}
	ff.Close();
	//Ȼ��ɾ�����ļ��� 
	RemoveDirectory(strDir);
}

//ɾ���ļ���
void DeleteAll()
{
	CString path;
	path = GetModuleDir();
	CString DBPath, TempPath;
	DBPath.Format(_T("%s\\%s"), path, CStringW(DB_DIR));
	TempPath.Format(_T("%s\\%s"), path, CStringW(TEMP_DIR));
	DeleteDirectory(DBPath);
	DeleteDirectory(TempPath);
}
