
// FingerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Finger.h"
#include "FingerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFingerDlg 对话框



CFingerDlg::CFingerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FINGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

//各个控件的绑定关系
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

// CFingerDlg 消息处理程序

BOOL CFingerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	InitControl();	//初始化控件

	InitDatabase();	//创建并初始化指纹库

	FingerConnect();	//自动连接指纹识别器


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFingerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFingerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//绘制指纹识别器接受到的图像
void CFingerDlg::OnImageReceivedZkfpengx(BOOL * AImageValid)
{
	HDC hdc = this->GetDC()->m_hDC;	//获得显示设备上下文环境的语柄
	
	//int x = 80;	//图像绘制区左上角横坐标
	//int y = 180;	

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_IMG_1);//IDC_picture为picture控件ID
	pWnd->GetClientRect(&rect);//rc为控件的大小。
	int width = rect.Width()-2;	//图像绘制区宽度
	int height = rect.Height()-2;	//图像绘制区高度
	int x = 41;	//图像绘制区左上角横坐标
	int y = 71;		//图像绘制区左上角纵坐标

	m_zkfpEng.PrintImageAt(int(hdc), x, y, width, height);	//绘制图像
}

/********************************************
*											*
*			CFingerprintDlg类成员函数		*
*											*											
********************************************/

//指纹器登记
void CFingerDlg::OnBnClickedBtnRegister()
{
	//设置右侧Picture为空白
	char emptyImgFile[MAX_PATH] = "empty.bmp";
	ShowImageInCtrl(m_picCtrl2, emptyImgFile);
	//清空信息
	m_staticInfo.SetWindowText(ToWideChar(""));

	//初始化操作结果信息
	char info[MAX_PATH] = { 0 };

	//获取界面输入内容(登记人姓名)
	UpdateData(true);

	//实时采集图像
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));
	
	//指纹登记
	bool bSaveImg = false;	//是否保存结果中间图像
	char outImgFile[MAX_PATH] = { 0 };
	if (m_name.IsEmpty())
	{
		MessageBox(_T("登记人姓名为空."), _T("提示"));//显示提示信息
		return;
	}

	Enroll(ToChar(m_name.GetBuffer()), srcImgFile, outImgFile, info, bSaveImg);

	//显示操作结果信息
	m_staticInfo.SetWindowText(ToWideChar(info));

	//显示图像
	ShowImageInCtrl(m_picCtrl1, srcImgFile);
	ShowImageInCtrl(m_picCtrl2, outImgFile);
}

//指纹器识别
void CFingerDlg::OnBnClickedBtnIdentify()
{
	//设置右侧Picture为空白
	char emptyImgFile[MAX_PATH] = "empty.bmp";
	ShowImageInCtrl(m_picCtrl2, emptyImgFile);
	//清空信息
	m_staticInfo.SetWindowText(ToWideChar(""));

	//初始化操作结果信息
	char info[MAX_PATH] = { 0 };

	//获取界面输入内容(登记人姓名)
	UpdateData(true);
	
	//实时采集图像
	char srcImgFile[MAX_PATH] = "capt.bmp";
	m_zkfpEng.SaveBitmap(ToWideChar(srcImgFile));

	//指纹识别
	bool bSaveImg = false;	//是否保存结果中间图像
	char outImgFile[MAX_PATH] = { 0 };

	if (Identify(srcImgFile, outImgFile, info, bSaveImg) < 0)
	{
		//显示操作结果信息
		m_staticInfo.SetWindowText(ToWideChar(info));
	}
	else
	{
		//显示操作结果信息
		m_staticInfo.SetWindowText(ToWideChar(info));
		//显示图像
		ShowImageInCtrl(m_picCtrl1, srcImgFile);
		ShowImageInCtrl(m_picCtrl2, outImgFile);
	}
}

//指纹识别器自动连接
void CFingerDlg::FingerConnect()
{
	if (m_zkfpEng.InitEngine() == 0)//启动指纹采集器
	{
		//MessageBox(_T("采集器连接成功."), _T("提示"));//显示提示信息
		m_staticInfo.SetWindowText(ToWideChar("采集器连接成功."));	//显示提示信息
	}
	else
	{
		MessageBox(_T("采集器连接失败,\n请检测采集器是否连接."), _T("提示"));//显示提示信息
		DeleteAll();		 //删除文件夹
		OnOK();	//关闭系统界面
	}
}

//初始化各个控件位置及大小
void CFingerDlg::InitControl()
{
	CWnd *pWnd1, *pWnd2;
	pWnd1 = GetDlgItem(IDC_STATIC_IMG_1); //获取控件指针
	pWnd1->SetWindowPos(NULL, 40, 70, 160, 200, SWP_NOZORDER); //把按钮移到窗口的(50,120)处,宽高（280,350）

	pWnd1 = GetDlgItem(IDC_STATIC_IMG_2); //获取控件指针
	pWnd1->SetWindowPos(NULL, 280, 70, 160, 200, SWP_NOZORDER); //把按钮移到窗口的(50,120)处,宽高（280,350）
}

//显示指纹库信息
void CFingerDlg::OnBnClickedBtnDatabase()
{
	char info[MAX_PATH] = { 0 };	//	初始化显示信息

	//判断指纹库是否为空
	if (EmptyDB())
	{
		//MessageBox(_T("指纹库为空！"), _T("提示"));
		m_staticInfo.SetWindowText(ToWideChar("指纹库信息为空."));	//显示指纹库信息
	}
	else
	{
		GetDatabaseInfo(info);	//获取指纹库信息
		m_staticInfo.SetWindowText(ToWideChar(info));	//显示指纹库信息
	}
}

//退出指纹识别系统
void CFingerDlg::OnBnClickedBtnExit()
{
	//删除文件夹
	DeleteAll();
	OnOK();	//关闭系统界面
}

/********************************************
*											*
*				全局函数					*
*											*
********************************************/

//显示图片
//参数：picCtrl(图像控件)，filename(图像文件路径)
int ShowImageInCtrl(CStatic &picCtrl, char *filename)
{
	//载入图像
	CImage image;
	HRESULT hResult = image.Load(ToWideChar(filename));

	//获得图像参数
	int width = image.GetWidth();//图像高度
	int height = image.GetHeight();//图像高度

								   //设置显示区域
	CRect rect;//定义矩形对象
	picCtrl.GetClientRect(&rect);//获得picture控件所在的矩形区域
	CDC *pDc = picCtrl.GetWindowDC();//获得picture控件的设备环境句柄
	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//设置为图拉伸模式

													//显示图像
	image.StretchBlt(pDc->m_hDC, rect, SRCCOPY);//将图像画到Picture控件表示的矩形区域

												//更新控件显示
	picCtrl.Invalidate(false);

	//释放变量空间
	image.Destroy();
	picCtrl.ReleaseDC(pDc);//释放picture控件的设备环境句柄

	return 0;
}

//多字节字符串转换为Unicode宽字符串
wchar_t *ToWideChar(char *str)
{
	int num = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
	wchar_t *wideStr = new wchar_t[num];
	MultiByteToWideChar(0, 0, str, -1, wideStr, num);
	return wideStr;
}

//3.5.2 获取图像参数信息
//获取图像的高度，宽度，深度信息
int ReadBMPImgFilePara(char *filename, int &width, int &height, int &depth)
{
	CImage image;//载入图像
	HRESULT hResult = image.Load(ToWideChar(filename));
	if (FAILED(hResult) || image.IsNull())	//图片损坏或是文件不存在等因素均可导致加载失败
	{
		return -1;
	}

	//获得图像参数
	width = image.GetWidth();//获得以像素为单位的图像宽度
	height = image.GetHeight();//获得以像素为单位的图像高度
	depth = image.GetBPP();//获得以像素为单位的图像深度
	if (depth != 8) //仅支持8位位图
	{
		return -2;
	}

	image.Destroy();//释放变量空间

	return 0;
}

//3.5.3 从图文件读取数据
//获取图像位图数据
int ReadBMPImgFileData(char *filename, unsigned char *data)
{
	CImage image;//载入位图
	HRESULT hResult = image.Load(ToWideChar(filename));
	if (FAILED(hResult) || image.IsNull())	//图片损坏或是文件不存在等因素均可导致加载失败
	{
		return -1;
	}

	//获得图像参数
	int width = image.GetWidth();//获得以像素为单位的图像宽度
	int height = image.GetHeight();//获得以像素为单位的图像高度
	int depth = image.GetBPP();//获得以像素为单位的图像深度（每个像素的位数）
	if (depth != 8) //仅支持8位位图
	{
		return -2;
	}

	memset(data, 0, width*height);	//初始化结果数组

									//读取图像数据
	int pitch = image.GetPitch();	//每行行首像素的地址偏移量
	unsigned char* pData1 = (unsigned char*)image.GetBits();	//图像位图数据区地址
	unsigned char* pData2 = data;
	unsigned char gray = 0;

	unsigned char *pRow1, *pRow2, *pPix1, *pPix2;
	for (int y = 0; y < height; y++)	//逐行扫描
	{
		pRow1 = pData1 + pitch*y;
		pRow2 = pData2 + width*y;
		for (int x = 0; x < width; x++)	//逐列扫描
		{
			//获取源图像灰度值
			pPix1 = pRow1 + x;
			gray = *pPix1;

			//保存到结果数组
			pPix2 = pRow2 + x;
			*pPix2 = gray;
		}//end for(x)
	}//end for (y)

	image.Destroy();
	return 0;
}

//3.5.4 用文本文件保存数据
//保存位图数据到文本文件
//dstFile(目标文件)，data(无符号字符数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, unsigned char *data, int width, int height)
{
	//打开目标文件
	ofstream fout(dstFile, ios::out);//使用覆盖写入方法
	if (!fout)
	{
		return -1;
	}

	//按指定格式向文件写入数据2
	int space = 5;	//每个无符号字符类型数据均用5个字符位等宽存储
	for (int i = 0; i < height*width; i++)	//遍历数组
	{
		fout << setw(space) << int(data[i]);//等宽写入数据
		if (i % width == (width - 1))//行尾
		{
			fout << endl;//换行
		}
	}
	//关闭文件
	fout.close();
	return 0;
}

//保存位图数据到文本文
//dstFile(目标文件)，data(浮点型数据数组)，width(图像宽度)，height(图像高度)
int SaveDataToTextFile(char *dstFile, float *data, int width, int height)
{
	//打开目标文件
	ofstream fout(dstFile, ios::out);//使用覆盖写入方法
	if (!fout)
	{
		return -1;
	}
	//按指定格式向文件写入数据
	int preci = 6;	//每个浮点型数据均保留6位小数位
	int space = 16;	//每个浮点型数据均用16个字符位等宽存储

	fout.precision(preci);
	for (int i = 0; i < height*width; i++)
	{
		fout << " " << setw(space) << data[i];	//等宽写入数据
		if (i%width == (width - 1))	//行尾
		{
			fout << endl;	//换行
		}
	}
	fout.close();	//关闭文件

	return 0;
}

//3.5.5	所需要的方法
//将CString类型转换为char[]类型
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

//3.6.1	创建指纹库和索引文件
//创建并初始化指纹库
void InitDatabase()
{
	_mkdir(TEMP_DIR);	//创建临时文件夹
	_mkdir(DB_DIR);		//创建指纹库文件夹
	InitFile(DB_INDEX_TXT);		//创建索引文件
}
//创建并初始化文件
void InitFile(char * filename)
{
	FILE *index = fopen(filename, "w");	//若文件不存在则创建，若已存在则清除其内容
	fclose(index);	//关闭文件
}

//3.6.2	遍历指纹库
//获取指纹库统计信息
void GetDatabaseInfo(char * info)
{
	//遍历指纹库
	int	pNo = 0;	//指纹序号
	char name[MAX_PATH] = { 0 };	//登记人姓名
	char srcFile[MAX_PATH] = { 0 };	//指纹图像文件
	char mdlFile[MAX_PATH] = { 0 };	//指纹特征文件

	FILE *index = fopen(DB_INDEX_TXT, "r");		//打开索引文件
	while (!feof(index))
	{
		fscanf(index, "%d %s %s %s\n", &pNo, srcFile, mdlFile, name);
	}
	fclose(index);	//关闭文件

	//统计指纹库信息
	sprintf(info, "当前指纹库中共有 %d 条记录", pNo);

}

//3.8.1	从文本文件读取数据
//从文本文件读取无符号自负类型的图像位图数据
//srcFile(源文件),data(无符号字符型数据数组),iWidth(图像宽度),iHeight(图像高度)
int ReadDataFromTextFile(char * srcFile, unsigned char * data, int iWidth, int iHeight)
{
	//打开源文件
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail());
	{
		return -1;
	}

	//读取数据
	int d = 0;
	for (int i = 0; i < iHeight *iWidth; i++)
	{
		fin >> d;
		data[i] = (unsigned char)d;
	}

	//关闭文件
	fin.close();

	return 0;
}

int ReadDataFromTextFile(char * srcFile, float * data, int iWidth, int iHeight)
{
	//打开源文件
	ifstream fin(srcFile, ios::in);
	if (!fin)//or if(fin.fail());
	{
		return -1;
	}

	//读取数据
	for (int i = 0; i < iHeight*iWidth; i++)
	{
		fin >> data[i];
	}

	//关闭文件
	fin.close();

	return 0;
}

//3.8.2	图像中值滤波
//参数：ucImg(原图数据),ucDstImg(结果图像数据)，iWidth(图像宽度),iHeight(图像高度)
int MidFilter(unsigned char * ucImg, unsigned char * ucDstImg, int iWidth, int iHeight)
{
	//Step1:结果图像数据初始化
	memset(ucDstImg, 0, iWidth* iHeight);

	//Step2: 中心区域滤波(使用3*3邻域)
	unsigned char* pUp, *pDown, *pImg;//用来确定3*3淋雨的3个图像数据指针(下文简称"淋浴指针")
	unsigned char x[9];//3*3邻域图像数据数组
	for (int i = 1; i < iHeight - 1; i++)//遍历第2行到倒数第2行
	{
		//初始化淋浴指针
		pUp = ucImg + (i - 1)*iWidth;
		pImg = ucImg + i*iWidth;
		pDown = ucImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++)//遍历第2列到倒数第2列
		{
			//移动淋浴指针
			pUp++;
			pImg++;
			pDown++;

			//获取3*3邻域数据
			x[0] = *(pUp - 1);
			x[1] = *(pImg - 1);
			x[2] = *(pDown - 1);

			x[3] = *pUp;
			x[4] = *pImg;
			x[5] = *pDown;

			x[6] = *(pUp + 1);
			x[7] = *(pImg + 1);
			x[8] = *(pDown + 1);

			//数组排序
			Sort(x, 9);

			//结果图像数据取邻域中值
			*(ucDstImg + i*iWidth + j) = x[4];
		}
	}

	//Step:3:第1行和最后1行滤波(使用2*3邻域)

	//第1行
	pDown = ucImg + iWidth;//邻域指针初始化
	for (int j = 1; j < iWidth - 1; j++)//第1行遍历第2列到倒数第2列
	{
		//获取2*3邻域数据
		x[0] = *(ucImg + j - 1);
		x[1] = *(ucImg + j);
		x[2] = *(ucImg + j + 1);

		x[3] = *(pDown + j - 1);
		x[4] = *(pDown + j);
		x[5] = *(pDown + j + 1);

		//数组排序
		Sort(x, 6);

		//接果取中值
		*(ucDstImg + j) = x[3];
	}

	//最后1行(倒数第1行)
	pUp = ucImg + iWidth*(iHeight - 2);//邻域指针初始化
	pDown = ucImg + iWidth*(iHeight - 1);//邻域指针初始化
	for (int j = 1; j<iWidth - 1; j++)//最后一行遍历第2列到倒数第2列
	{
		//获取2*3邻域数据
		x[0] = *(pDown + j - 1);
		x[1] = *(pDown + j);
		x[2] = *(pDown + j + 1);

		x[3] = *(pUp + j - 1);
		x[4] = *(pUp + j);
		x[5] = *(pUp + j + 1);

		//数组排序
		Sort(x, 6);

		//结果取中值
		*(ucDstImg + iWidth*(iHeight - 1) + j) = x[3];
	}

	//Step4:4个焦点铝箔(使用2*2邻域)

	//左上角点
	x[0] = *(ucImg);//获取2*2邻域数据
	x[1] = *(ucImg + 1);
	x[2] = *(ucImg + iWidth);
	x[3] = *(ucImg + iWidth + 1);
	Sort(x, 4);//数组排序
	*(ucDstImg) = x[2];

	//右上角点
	x[0] = *(ucImg + iWidth - 1);//获取2*2邻域数据
	x[1] = *(ucImg + iWidth - 2);
	x[2] = *(ucImg + 2 * iWidth - 1);
	x[3] = *(ucImg + 2 * iWidth - 2);
	Sort(x, 4);//数组排序
	*(ucDstImg + iWidth - 1) = x[2];//结果取中值

									//左下角点
	x[0] = *(ucImg + (iHeight - 1)*iWidth);//获取2*2邻域数据
	x[1] = *(ucImg + (iHeight - 2)*iWidth);
	x[2] = *(ucImg + (iHeight - 1)*iWidth + 1);
	x[3] = *(ucImg + (iHeight - 2)*iWidth + 1);
	Sort(x, 4);//数组排序
	*(ucDstImg + (iHeight - 1)*iWidth) = x[2];//结果取中值

											  //右下角点
	x[0] = *(ucImg + (iHeight - 0)*iWidth - 1);//获取2*2邻域数据
	x[1] = *(ucImg + (iHeight - 1)*iWidth - 1);
	x[2] = *(ucImg + (iHeight - 0)*iWidth - 2);
	x[3] = *(ucImg + (iHeight - 1)*iWidth - 2);
	Sort(x, 4);//数组排序
	*(ucDstImg + (iHeight - 0)*iWidth - 1) = x[2];//结果取中值
	return 0;
}

//使用冒泡法对叔组进行升序排序
//参数:data(数组),dsize(数组长度)
void Sort(unsigned char* data, int dsize)
{
	unsigned char temp = 0;
	for (int i = 0; i < dsize; i++)
	{
		for (int j = dsize - 1; j > i; j--)
		{
			if (data[j] < data[j - 1])//升序排序
			{
				temp = data[j];
				data[j] = data[j - 1];
				data[j - 1] = temp;
			}
		}
	}
}

//3.8.3
//保存数据到BMP图像文件
int SaveDataToImageFile(char *srcFile, char *dstFile, unsigned char *data)
{
	//文件复制
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//写入数据
	WriteBMPImgFile(dstFile, &data);

	return 0;
}

//保存数据到图像文件
//srcFile(源文件名),dstFile(目标文件名),data(待保存数据数组),scale(转换比例)
int SaveDataToImageFile(char *srcFile, char *dstFile, float *data, float scale)
{
	//读取图像参数
	int iWidth, iHeight, iDepth;//图像宽度、高度、深度
	ReadBMPImgFilePara(srcFile, iWidth, iHeight, iDepth);

	//文件复制
	CopyFile(ToWideChar(srcFile), ToWideChar(dstFile), false);

	//数据转换
	unsigned char *tmpData = new unsigned char[iWidth * iHeight];
	for (int i = 0; i<int(iWidth*iHeight); i++)
	{
		tmpData[i] = unsigned char((scale * data[i]));
	}

	//写入数据
	WriteBMPImgFile(dstFile, &tmpData);

	//释放内存空间
	delete[] tmpData;

	return 0;
}

//将位图数据写入BMP图像文件
//dstFileName(目标文件名),pusImgData(待保存数据数组)
int WriteBMPImgFile(char *dstFileName, unsigned char **pusImgData)
{
	//打开文件
	FILE *fp = fopen(dstFileName, "r+b");
	if (!fp)
	{
		return -1;
	}

	//读取图像参数信息(用于定位数据区写入数据)
	int imgType, iWidth, iHeight;//图像宽度、高度、深度
	int iStartPos = 0; 	//位图数据区起始地址
	fseek(fp, 10L, SEEK_SET);
	fread((char *)(&iStartPos), 4, 1, fp);//获取位图数据区起始地址(以字节为单位)
	fseek(fp, 18L, SEEK_SET);
	fread((char *)(&iWidth), 4, 1, fp);//获取图像宽度(以像素为单位)
	fread((char *)(&iHeight), 4, 1, fp);//获取图像高度(以像素为单位)

	unsigned short temp;
	fseek(fp, 28L, SEEK_SET);
	fread((char *)(&temp), 2, 1, fp);//获取图像深度(每个像素的位数，以位为单位)

	imgType = temp;
	if (imgType != 8)	//仅支持8位的BMP位图图像
	{
		return -2;
	}

	//向数据区写入数据
	unsigned char *usImgData = *pusImgData;	//待保存数据数组地址
	int iWidthInFile = 0;	//文件每行像素宽度、长度
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
		fseek(fp, iStartPos, SEEK_SET);	//定位行首地址
		fwrite((usImgData + i*iWidth), 1, iWidth, fp);	//写入一行数据
		iStartPos += iWidthInFile;	//调整行首地址
	}

	//关闭文件
	fclose(fp);

	return 0;
}

//3.9.1图像直方图均衡化
//ucImg(源图数据），ucNormImg(结果图数据,iWidth(图像宽度），iHeight(图像高度）
int HistoNormalize(unsigned char* ucImg, unsigned char* unNormImg, int iWidth, int iHeight)
{
	//构建源图灰度直方图
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

//3.11.1指纹脊线方向计算
//ucImg(图像数据),fDire(脊线方向数据),iWidth(图像宽度),iHeight(图像高度)
int ImgDirection(unsigned char *ucImg, float *fDire, int iWidth, int iHeight)
{
	//定义变量
	const int SEMISIZ = 7;//领域窗口区域半径
	int dx[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	int dy[SEMISIZ * 2 + 1][SEMISIZ * 2 + 1];
	float fx, fy;

	//结果初始化
	memset(fDire, 0, iWidth*iHeight * sizeof(float));
	//计算每一像素的脊线方向值
	for (int y = SEMISIZ + 1; y<iHeight - SEMISIZ - 1; y++)//逐行遍历(除去边缘)
	{
		for (int x = SEMISIZ + 1; x<iWidth - SEMISIZ - 1; x++)//逐列遍历(除去边缘)
		{
			//计算以当前像素为中心的邻域窗口区域内的每一像素的梯度
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

			//计算当前像素的脊线方向值
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

//3.11.2脊线方向低通滤波
//fDire(脊线方向原始数据),fFitDire(脊线方向滤波结果数据),iWidth(图像宽度),iHeight(图像高度)
int DircLowPass(float * fDire, float * fFitDire, int iWidth, int iHeight)
{
	//定义变量
	const int DIR_FILTER_SIZE = 2;
	int blocksize = 2 * DIR_FILTER_SIZE + 1;
	int imgsize = iWidth*iHeight;

	float* filter = new float[blocksize*blocksize];//使用5*5滤波器
	float* phix = new float[imgsize];
	float* phiy = new float[imgsize];
	float* phi2x = new float[imgsize];
	float* phi2y = new float[imgsize];

	//结果初始化
	memset(fFitDire, 0, sizeof(float)*iWidth*iHeight);

	//设置5*5高斯低通滤波器模板
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

	//计算各像素点的方向正弦值和余弦值
	for (int y = 0; y<iHeight; y++)
	{
		for (int x = 0; x<iWidth; x++)
		{
			phix[y*iWidth + x] = cos(fDire[y*iWidth + x]);
			phiy[y*iWidth + x] = sin(fDire[y*iWidth + x]);
		}
	}

	//对所有像素进行方向低通滤波
	memset(phi2x, 0, sizeof(float)*imgsize);
	memset(phi2y, 0, sizeof(float)*imgsize);
	float nx, ny;
	int val;
	for (int y = 0; y<iHeight - blocksize; y++)//逐行遍历(除去边缘区段)
	{
		for (int x = 0; x<iWidth - blocksize; x++)//逐列遍历(除去边缘区段)
		{
			//对以当前像素为中心的滤波窗口内的所有像素值进行加权累加
			nx = 0.0;
			ny = 0.0;
			for (int j = 0; j<blocksize; j++)
			{
				for (int i = 0; i<blocksize; i++)
				{
					val = (x + i) + (j + y)*iWidth;
					nx += filter[j*blocksize + i] * phix[val];//方向余弦加权累计
					ny += filter[j*blocksize + i] * phiy[val];//方向正弦加权累计
				}
			}
			//将累加结果作为当前像素的新的方向正弦值和余弦值
			val = x + y*iWidth;
			phi2x[val] = nx;
			phi2y[val] = ny;
		}
	}

	//根据加权累加结果，计算各像素的方向滤波结果值
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

//3.12.1指纹脊线频率计算
//ucImg(源图数据),fDirection(脊线方向数据),fFrequency(脊线频率结果数据),iWidth(图像宽度),iHeight(图像高度)
int Frequency(unsigned char * ucImg, float * fDirection, float * fFrequency, int iWidth, int iHeight)
{
	//窗口大小
	const int SIZE_L = 32;
	const int SIZE_W = 16;
	const int SIZE_L2 = 16;
	const int SIZE_W2 = 8;

	//正弦波峰值点
	int peak_pos[SIZE_L];
	int peak_cnt;
	float peak_freq;
	float Xsig[SIZE_L];

	//方向
	float dir = 0.0;
	float cosdir = 0.0;
	float sindir = 0.0;
	float maxPeak, minPeak;

	//结果初始化
	float *frequency1 = new float[iWidth * iHeight];
	memset(fFrequency, 0, sizeof(float) * iWidth * iHeight);
	memset(frequency1, 0, sizeof(float) * iWidth * iHeight);

	int x, y;
	int d, k;
	int u, v;

	for (y = SIZE_L2; y<iHeight - SIZE_L2; y++)//逐行遍历(除去边缘区)
	{
		for (x = SIZE_L2; x<iWidth - SIZE_L2; x++)//逐列遍历(除去边缘区)
		{
			//当前像素的脊线方向
			dir = fDirection[(y + SIZE_W2)*iWidth + (x + SIZE_W2)];
			cosdir = -sin(dir);
			sindir = cos(dir);

			//计算以当前像素为中心的L*W邻域窗口的幅值序列，X[0]...X[L-1]
			for (k = 0; k<SIZE_L; k++)
			{
				Xsig[k] = 0.0;
				for (d = 0; d<SIZE_W; d++)
				{
					u = (int)(x + (d - SIZE_W2)*cosdir + (k - SIZE_L2)*sindir);
					v = (int)(y + (d - SIZE_W2)*sindir + (k - SIZE_L2)*cosdir);
					//边界点处理
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

			//确定幅值序列变化范围
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

			//确定峰值点位置
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

			//计算峰值点平均间距
			peak_freq = 0.0;
			if (peak_cnt >= 2)
			{
				for (k = 0; k<peak_cnt; k++)
				{
					peak_freq += (peak_pos[k + 1] - peak_pos[k]);
				}

				peak_freq /= peak_cnt - 1;
			}

			//计算当前像素的频率
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

	//对频率进行均值滤波
	for (y = SIZE_L2; y<iHeight - SIZE_L2; y++)
	{
		for (x = SIZE_L2; x<iWidth - SIZE_L2; x++)
		{
			k = x + y*iWidth;//当前像素位置(在频率数组中的数组下标)
			peak_freq = 0.0;
			//使用当前像素为中心的5*5邻域窗口进行均值滤波
			for (v = -2; v <= 2; v++)
			{
				for (u = -2; u <= 2; u++)
				{
					peak_freq += frequency1[(x + u) + (y + v)*iWidth];//求频率累积和					
				}
			}
			fFrequency[k] = peak_freq / 25;	//当前像素频率等于窗口内频率均值
		}
	}

	delete[] frequency1;

	return 0;
}

//3.13.1 指纹掩码计算
//实现指纹掩码计算
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
	//第二步：填充孔洞（利用形态学的膨胀原理）
	for (k = 0; k < 4; k++)
	{
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				//前景点的上下左右四个相邻点都标记为前景点（将8位像素值的最高位设为1予以标记)
				if (ucMask[x + y*iWidth] == 0xFF)
				{
					ucMask[x - 1 + y*iWidth] |= 0x80;
					ucMask[x + 1 + y*iWidth] |= 0x80;
					ucMask[x + (y - 1)*iWidth] |= 0x80;
					ucMask[x + (y + 1)*iWidth] |= 0x80;
				}
			}
		}
		//=========判断和设置前景点============
		for (y = 1; y < iHeight - 1; y++)
		{
			for (x = 1; x < iWidth - 1; x++)
			{
				//=======将标记为前景点的像素都设为前景点========
				if (ucMask[x + y*iWidth])
				{
					ucMask[x + y*iWidth] = 0xFF;

				}
			}
		}
	}//第三步：去除边缘和孤立点（利用形态学的腐蚀原理）
	for (k = 0; k < 12; k++)
	{
		//标记背景点
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
		//判断和设置背景点
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

//3.14 指纹图像Gabor滤波增强
//ucImg(源图数据),fDirection(脊线方向数据),ucMask(掩码数据),ucImgEnhanced（滤波增强结果数据),iWidth(图像宽度),iHeight(图像高度)
int GaborEnhance(unsigned char * ucImg, float *fDirection, float *fFrequency, unsigned char * ucMask, unsigned char *ucImgEnhanced, int iWidth, int iHeight)
{
	//自定义变量
	const float PI = 3.141592654;
	int i, j, u, v;
	int wg2 = 5;//使用11*11的Gablor滤波器。半边长为5
	float sum, f, g;
	float x2, y2;
	float dx2 = 1.0 / (4.0*4.0);//沿x轴方向的高斯包络常量
	float dy2 = 1.0 / (4.0*4.0);//沿y轴方向的高斯包络常量

								//结果初始化
	memset(ucImgEnhanced, 0, iWidth*iHeight);

	//Gabor滤波
	for (j = wg2; j<iHeight - wg2; j++)//逐行遍历（去除边缘区段）
	{
		for (i = wg2; i<iWidth - wg2; i++)//逐行遍历（去除边缘区段）
		{
			//跳过背景点
			if (ucMask[i + j*iWidth] == 0)//掩码为0表示背景点
			{
				continue;
			}

			//获取当前像素的方向和频率
			g = fDirection[i + j*iWidth];//当前像素的脊线方向
			f = fFrequency[i + j*iWidth];//当前像素的脊线频率
			g += PI / 2;//垂直方向

						//对当前像素进行Gabor滤波
			sum = 0.0;
			for (v = -wg2; v <= wg2; v++)//滤波窗口逐行遍历
			{
				for (u = -wg2, u <= wg2; u++;)//滤波窗口逐行遍历
				{
					x2 = -u*sin(g) + v*cos(g);//x坐标旋转
					y2 = u*cos(g) + v*sin(g);//y坐标旋转
					sum += exp(-0.5*(x2*x2*dx2 + y2*y2*dy2)) *cos(2 * PI*x2*f)*ucImg[(i - u) + (j - v)*iWidth];//窗口内滤波值累加
				}
			}
			//边界值处理
			if (sum>255.0)
			{
				sum = 255.0;
			}
			if (sum<0.0)
			{
				sum = 0.0;
			}
			//得到当前像素的滤波结果
			ucImgEnhanced[i + j*iWidth] = (unsigned char)sum;
		}
	}

	return 0;
}

//3.16.1 图像二值化
//图像二值化
//ucImage(源图数据),ucBinImage(结果图数据),iWidth(图像宽度),iHeight(图像高度),uThreshold(二值化灰度阈值)
int BinaryImg(unsigned char * ucImage, unsigned char * ucBinImage, int iWidth, int iHeight, unsigned char uThreshold)
{
	//分别定义指向源图数据和结果数据的数据指针
	unsigned char *pStart = ucImage, *pEnd = ucImage + iWidth*iHeight;
	unsigned char *pDest = ucBinImage;
	//逐一遍历所有像素
	while (pStart<pEnd)
	{
		*pDest = *pStart > uThreshold ? 1 : 0;//二值化
		pStart++;//源图数据指针后移
		pDest++;//结果图数据指针后移
	}
	return 0;
}

//3.16.2 二值图显示转换
//二值图显示转换([0,1]->[0,255])
//ucBinImg(源图数据),ucGrayImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int BinaryToGray(unsigned char * ucBinImg, unsigned char * ucGrayImg, int iWidth, int iHeight)
{
	//分别定义指向源图数据和结果数据的数据指针
	unsigned char *pStart = ucBinImg, *pEnd = ucBinImg + iWidth*iHeight;
	unsigned char *pDest = ucGrayImg;

	//逐一遍历所有像素
	while (pStart < pEnd)
	{
		*pDest = (*pStart) > 0 ? 255 : 0;//取值转换([0,1]->[0,255])
		pStart++;//源图数据指针后移
		pDest++;//结果图数据指针后移
	}
	return 0;
}

//3.17指纹图像细化
//3.17.1 图像细化算法
//ucBinedImg(原图数据), ucThinnedImage(结果图数据), iWidth(图像宽度), iHeight(图像高度), iIterativeLimit(最大迭代次数)
int Thinning(unsigned char * ucBinedImg, unsigned char * ucThinnedImage, int iWidth, int iHeight, int iIterativeLimit)
{
	//定义变量
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8, xp;
	unsigned char g1, g2, g3, g4;
	unsigned char b1, b2, b3, b4;
	unsigned char np1, np2, npm;
	unsigned char *pUp, *pDown, *pImg;
	int iDeletedPoints = 0;

	//结果初始化
	memcpy(ucThinnedImage, ucBinedImg, iWidth*iHeight);
	for (int it = 0; it<iIterativeLimit; it++)
	{
		iDeletedPoints = 0;//初始化本次迭代所删除点

						   //本次迭代内第一次遍历（使用条件G1&G2&G3）
		for (int i = 1; i<iHeight - 1; i++)
		{
			//初始化邻域指针
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i*iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j<iWidth - 1; j++)//逐列遍历
			{
				//调整邻域指针
				pUp++;
				pImg++;	//当前像素(窗口中心像素)
				pDown++;

				//跳过背景点(背景点像素值为0)
				if (!*pImg)
				{
					continue;
				}

				//获取3*3邻域窗口所有9个像素的灰度值
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//判断条件G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//判断条件G2
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

				//判断条件G3&G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//组合判断(使用条件G1&G2&G3)
				if (g1 && g2 &&g3)
				{
					ucThinnedImage[iWidth*i + j] = 0; //删除当前像素
					++iDeletedPoints;	//本次迭代所删点+1
				}
			}//end for(j) 列
		}//end for(i) 行

		 //结果同步
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//本次迭代内第二次遍历（使用条件G1&G2&G4）
		for (int i = 1; i<iHeight - 1; i++)	//逐行遍历
		{
			//初始化邻域指针
			pUp = ucBinedImg + (i - 1)*iWidth;
			pImg = ucBinedImg + i*iWidth;
			pDown = ucBinedImg + (i + 1)*iWidth;

			for (int j = 1; j<iWidth - 1; j++)//逐列遍历
			{
				//调整邻域指针
				pUp++;
				pImg++;	//当前像素(窗口中心像素)
				pDown++;

				//跳过背景点(背景点像素值为0)
				if (!*pImg)
				{
					continue;
				}

				//获取3*3邻域窗口所有9个像素的灰度值
				x6 = *(pUp - 1);
				x5 = *(pImg - 1);
				x4 = *(pDown - 1);

				x7 = *pUp;
				xp = *pImg;
				x3 = *pDown;

				x8 = *(pUp + 1);
				x1 = *(pImg + 1);
				x2 = *(pDown + 1);

				//判断条件G1
				b1 = !x1 && (x2 == 1 || x3 == 1);
				b2 = !x3 && (x4 == 1 || x5 == 1);
				b3 = !x5 && (x6 == 1 || x7 == 1);
				b4 = !x7 && (x8 == 1 || x1 == 1);

				g1 = (b1 + b2 + b3 + b4) == 1;

				//判断条件G2
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

				//判断条件G3&G4
				g3 = (x1 && (x2 || x3 || !x8)) == 0;
				g4 = (x5 && (x6 || x7 || !x4)) == 0;

				//组合判断(使用条件G1&G2&G4)
				if (g1 && g2 && g4)
				{
					ucThinnedImage[iWidth*i + j] = 0; //删除当前像素
					++iDeletedPoints;	//本次迭代所删点+1
				}
			}//end for(j) 列
		}//end for(i) 行

		 //结果同步
		memcpy(ucBinedImg, ucThinnedImage, iWidth*iHeight);

		//如果本次迭代已无点可删，则迭代停止
		if (iDeletedPoints == 0)
		{
			break;
		}

		//清除边缘地段
		for (int i = 0; i<iHeight; i++)
		{
			for (int j = 0; j<iWidth; j++)
			{
				if (i < 16)	//上边缘
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (i >= iHeight - 16)
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (j < 16)	//左边缘
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
				else if (j >= iWidth - 16)	//右边缘
				{
					ucThinnedImage[i*iWidth + j] = 0;
				}
			}
		}
	}

	return 0;
}

//3.18.1 指纹特征提取
//ucThinImg(源图数据), ucMinuImg(结果图数据),iWidth(图像宽度),iHeight(图像高度)
int Extract(unsigned char *ucThinImg, unsigned char *ucMinuImg, int iWidth, int iHeight)
{
	//定义变量
	unsigned char *pDest = ucMinuImg;//结果图数据指针
	unsigned char *pUp, *pDown, *pImg;//源图邻域指针
	unsigned char x1, x2, x3, x4, x5, x6, x7, x8;//八邻点
	unsigned char nc;
	int iMinuCount = 0;
	//结果初始化(全设为0，表示都是非特征点)
	memset(pDest, 0, iWidth * iHeight);
	//遍历源图以提取指纹特征
	for (int i = 1; i < iHeight - 1; i++) {
		//初始化邻域指针
		pUp = ucThinImg + (i - 1)*iWidth;
		pImg = ucThinImg + i*iWidth;
		pDown = ucThinImg + (i + 1)*iWidth;

		for (int j = 1; j < iWidth - 1; j++) {
			//调整邻域指针
			pUp++;
			pImg++;
			pDown++;

			//跳过背景点
			if (!*pImg) {
				continue;
			}
			//获取3*3邻域窗口内所有8个邻点像素的灰度值
			x6 = *(pUp - 1);
			x5 = *(pImg - 1);
			x4 = *(pDown - 1);

			x7 = *pUp;
			x3 = *pDown;

			x8 = *(pUp + 1);
			x1 = *(pImg + 1);
			x2 = *(pDown + 1);

			//统计八邻点中黑点数量
			nc = x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8;

			//特征点判断
			if (nc == 1)//端点
			{
				pDest[i*iWidth + j] = 1;//结果图中对应像素点应设为1（表示端点）
				++iMinuCount;//特征点数量+1
			}
			else if (nc == 3)//分叉点
			{
				pDest[i*iWidth + j] = 3;
				++iMinuCount;//特征点数量+1
			}
		}

	}
	return iMinuCount;//返回特征点数量；
}

//3.19.1 指纹特征过滤
//参数:minuData(特征图数据),thinData(细化图数据),minutiaes(特征点数组),minuCount(特征点数量),iWidth(图像宽度),iHeight(图像高度)
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
	int newCount = 0;	//有效特征点数量
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

//3.19.2 去除边缘特征点
//参数:minutiaes(特征点数组), count(特征点数目), ucImg(源图数据), iWidth(图像宽度), iHeight(图像高度)
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

//3.20.1 保存特征模板文件
//参数:minutiaes(特征点数组)，count(特征点数量),filename(模板文件名)
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
//获得新指纹编号
int GetNewIndexInDB()
{
	//从index文件中读取最后一条记录的编号
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

//3.20.4 显示指纹入库结果
//宽字符串转换为多字节字符串
char *ToChar(wchar_t *str)
{
	int num = WideCharToMultiByte(0, 0, str, -1, NULL, 0, NULL, false);
	char *chStr = new char[num];;
	WideCharToMultiByte(0, 0, str, -1, chStr, num, NULL, false);
	return chStr;
}

//3.23.1 构建特征点相邻关系
//计算连接两个坐标点的线段倾斜度
//参数: x1(端点1 横坐标), y1(端点1 纵坐标), 参数: x2(端点2 横坐标), y2(端点2 纵坐标)
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

//构建特征点相邻关系
//参数：minutiae(特征点数组),minuCount(特征点数目)
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

//3.23.2 指纹特征匹配相似度计算
//参数:iWidth(图像宽度), iHeight(图像高度)，minutiae1(指纹1的特征点数组)，count1(指纹1的特征点数量)
//minutiae2(指纹2的特征点数组)，count2(指纹2的特征点数量)
float MinuSimilarity(int iWidth, int iHeight, MINUTIAE *minutiae1, int count1, MINUTIAE *minutiae2, int count2)
{
	const int MAX_SIMILAR_PAIR = 100;
	const int MAX_NEIGHBOR_EACH = 10;

	BuildNabors(minutiae1, count1);
	BuildNabors(minutiae2, count2);

	//初始化特征点匹配结果
	int similarPair[MAX_SIMILAR_PAIR][2];
	memset(similarPair, 0, 100 * 2 * sizeof(int));

	//选择基准特征和参考特征
	MINUTIAE *baseMinutiae;	//基准特征点数组
	MINUTIAE *refMinutiae;//参考特征点数组

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

	NEIGHBOR *baseNeighbors = NULL;		//基准点的相邻点指针
	NEIGHBOR *refNeighbors = NULL;		//参考点的相邻点指针
	int similarMinutiae = 0;		//	相似点对数量
	float baseTheta, refTheta;		//基准方向和参考方向

									//特征点匹配
	for (int i = 0; i < baseAccount; i++)
	{
		//获取当前基准点信息
		baseNeighbors = baseMinutiae[i].neibors;	//基准点的相邻点数组
		baseTheta = baseMinutiae[i].theta;		//基准点的脊线方向

												//在参考数组中寻找当前基准点最相似的参考点
		int refSimilarNo = 0;
		int maxSimilarNeibors = 0;

		for (int j = 0; j < refAccount; j++)
		{
			//跳过与当前基准点类型不同的参考点
			if (refMinutiae[j].type != baseMinutiae[i].type)
			{
				continue;
			}

			//获取当前参考信息
			refNeighbors = refMinutiae[j].neibors;	//参考点的相邻点数组
			refTheta = refMinutiae[j].theta;	//参考点的脊线方向

												//统计相似点数量
			int thisSimilarNeigbors = 0;
			for (int m = 0; m < MAX_NEIGHBOR_EACH; m++)
			{
				//在参考点的相邻点数组中查找与当前基准点的当前相邻点相似的相邻点
				for (int n = 0; n < MAX_NEIGHBOR_EACH; n++)
				{
					//跳过类型不同的相点

					if (baseNeighbors[m].type != refNeighbors[n].type)
					{
						continue;
					}

					//计算两个相邻点之间的距离差和不同的角度差
					int dist = abs(int(baseNeighbors[m].distance - refNeighbors[n].distance));
					float theta1 = abs(float((baseNeighbors[m].Theta - baseTheta) - (refNeighbors[n].Theta - refTheta)));
					float theta2 = abs(float(baseNeighbors[m].Theta2Ridge - refNeighbors[n].Theta2Ridge));
					float theta3 = abs(float((baseNeighbors[m].Theta - baseNeighbors[m].ThetaThisNibor) - (refNeighbors[n].Theta - refNeighbors[n].ThetaThisNibor)));

					//如果距离差小于指阈值
					if (dist<4 && theta1<0.15f && theta2<0.15f && theta3 < 0.15f)
					{
						++thisSimilarNeigbors;
						break;
					}
				}
			}

			//如果3对以上相邻点相似，则认为当前基准点与当前参考点相似，保存匹配结果
			if ((thisSimilarNeigbors >= MAX_NEIGHBOR_EACH * 3 / 10) && (similarMinutiae < MAX_SIMILAR_PAIR))
			{
				similarPair[similarMinutiae][0] = i;
				similarPair[similarMinutiae][1] = refSimilarNo;
				++similarMinutiae;
			}
		}
	}

	//计算特征匹配相似度
	float similarity = similarMinutiae / 8.0f;
	//如果相似特征点数量小于指定下线阈值
	similarity = similarMinutiae < 2 ? 0.0f : similarity;
	//如果相似特征点数量大于指定上线阈值
	similarity = similarMinutiae > 8 ? 1.0f : similarity;

	//返回特征匹配相似度
	return similarity;
}

//3.23.3 读取特征模板文件
//参数:fileName(特征模板文件名),minutiae(特征点数组)
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

	*minutiae = new MINUTIAE[account];//创建特征点结构数组
	if (*minutiae == NULL)
	{
		return -3;
	}
	for (int i = 0; i < account; i++)
	{
		fread(&((*minutiae)[i]), sizeof(MINUTIAE), 1, fp);
	}

	//关闭文件
	fclose(fp);
	return account;
}

//3.24.1 指纹库预检
//判断指纹库是否为空
bool EmptyDB()
{
	//打开文件
	char indexFile[MAX_PATH] = { DB_INDEX_TXT };//指纹库索引文件
	FILE *pFile = fopen(indexFile, "r");
	if (!pFile)//若索引文件不存在或存在异常,则指纹库为空
	{
		return true;
	}

	//判断指纹库是否为空(索引文件内容是否为空)
	int size = filelength(fileno(pFile));//文件长度(以字节为单位)
	bool bEmpty = (size < 1) ? true : false;//若索引文件内容为空,则指纹库为空

											//关闭文件
	fclose(pFile);

	//返回判断结果
	return bEmpty;
}

//3.25 指纹登记全程集成
//3.25.1 指纹登记过程集成
//regName(登记人姓名),srcImgFile(源图文件名),outImgFile(特征结果图文件名),info(返回操作成功或失败提示信息),bSaveImg(是否保存所有中间结果图像)
int Enroll(char *regName, char *srcImgFile, char *outImgFile, char *info, bool bSaveImg)
{
	//第0步：设置中间结果文件名、中间结果数据文件名
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

	//中间结果图像文件名
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

	//指纹特征模板文件名
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//第1步:图像载入,读取源图数据和参数信息
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//读取源图参数信息
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//读取源图数据
	memset(image2, 0, iWidth*iHeight);//结果图初始化
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}

	//第2步:中值滤波
	MidFilter(image1, image2, iWidth, iHeight);//中值滤波
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//第3步:直方图均衡化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第2步滤波结果为源图数据
	HistoNormalize(image1, image2, iWidth, iHeight); //直方图均衡化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//第4步:方向计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	float *tmpDirection = new float[iWidth*iHeight];
	float *direction = new float[iWidth*iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//方向计算
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//方向低通滤波
	if (bSaveImg)
	{
		const int DIRECTION_SCALE = 100;//方向结果转换比例(仅用于结果显示)
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction, DIRECTION_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//第5步:频率计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	float *frequency = new float[iWidth*iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//频率计算
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//频率结果转换比例(仅用于结果显示)
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency, FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//第6步:掩码计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//掩码计算
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//第7步:Gabor滤波增强
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//增强
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}
	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//第8步:二值化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第7步Gabor滤波结果为源图数据
	BinaryImg(image1, image2, iWidth, iHeight, 128);//二值化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//第9步:细化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第8步二值化结果为源图数据
	Thinning(image1, image2, iWidth, iHeight, 128);//细化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char *thin = new unsigned char[iWidth*iHeight];
	memcpy(thin, image2, sizeof(unsigned char)*iWidth*iHeight);//保存细化结果

															   //第10步:特征提取
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第9步细化结果为源图数据
	int minuCount = Extract(image1, image2, iWidth, iHeight);//特征提取
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}

	//第11步:特征过滤
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第10步特征提取结果为源图数据
	MINUTIAE *minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);//特征过滤
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//保存特征数据文件
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
	strcpy(outImgFile, stepImgFile11);//保存特征结果图文件名

									  //第12步:特征入库
	int sNo = GetNewIndexInDB();//获得数据库新指纹记录编号
	char dbImgFile[MAX_PATH] = { 0 };//数据库中源图文件名
	char dbMdlFile[MAX_PATH] = { 0 };//数据库中模板文件名
	sprintf(dbImgFile, "%s\\%d%s", DB_DIR, sNo, ".bmp");//设置源图文件名(存入数据库文件夹内,以编号命名)
	sprintf(dbMdlFile, "%s\\%d%s", DB_DIR, sNo, ".mdl");//设置模板文件名(存入数据库文件夹内,以编号命名)

	if (minuCount > 0)
	{
		//保存原始指纹图像
		CopyFile(ToWideChar(srcImgFile), ToWideChar(dbImgFile), false);

		//保存指纹特征模板文件
		CopyFile(ToWideChar(stepMdlFile11), ToWideChar(dbMdlFile), false);

		//指纹登记信息写入数据库索引文件
		FILE *index = fopen(DB_INDEX_TXT, "a");//追加模式写入
		fprintf(index, "%d %s %s %s\n", sNo, dbImgFile, dbMdlFile, regName);
		fclose(index);
	}

	//释放空间
	delete[] image1;
	delete[] image2;
	delete[] minutiaes;

	sprintf(info, "指纹登记成功");
	return 0;
}

//3.26 指纹识别过程集成
//3.26.1 指纹识别过程集成
//srcImgFile(源图文件名),outImgFile(识别结果图文件名,info(返回操作成功或失败提示信息），bSaveImg(是否保存所有中间结果图像）
int Identify(char *srcImgFile, char *outImgFile, char *info, bool bSaveImg)
{
	//第0步：设置中间结果文件名、中间结果数据文件名
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

	//中间结果数据文件名
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

	//指纹特征模板文件名
	char stepMdlFile11[MAX_PATH] = { STEP_IMG_11_MDL };

	//指纹库预检（判断指纹库是否为空）
	EmptyDB();


	//第1步：图像载入，读取源图数据和参数信息
	int iWidth, iHeight, iDepth;
	ReadBMPImgFilePara(srcImgFile, iWidth, iHeight, iDepth);//读取源图参数信息
	unsigned char *image1 = new unsigned char[iWidth*iHeight];
	unsigned char *image2 = new unsigned char[iWidth*iHeight];
	ReadBMPImgFileData(srcImgFile, image1);//读取源图数据
	memset(image2, 0, iWidth*iHeight);//结果图初始化
	if (bSaveImg)
	{
		CopyFile(ToWideChar(srcImgFile), ToWideChar(stepImgFile1), false);
		SaveDataToTextFile(stepTxtFile1, image1, iWidth, iHeight);
	}

	//第2步：中值滤波
	MidFilter(image1, image2, iWidth, iHeight);//中值滤波
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile2, image2);
		SaveDataToTextFile(stepTxtFile2, image2, iWidth, iHeight);
	}

	//第3步：直方图均衡化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第2步滤波为源图数据
	HistoNormalize(image1, image2, iWidth, iHeight);//直方图均衡化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile3, image2);
		SaveDataToTextFile(stepTxtFile3, image2, iWidth, iHeight);
	}

	//第4步：方向计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第三步均衡化结果为源图数据
	float *tmpDirection = new float[iWidth*iHeight];
	float *direction = new float[iWidth*iHeight];
	ImgDirection(image1, tmpDirection, iWidth, iHeight);//方向计算	
	DircLowPass(tmpDirection, direction, iWidth, iHeight);//方向低通滤波
	if (bSaveImg)
	{
		const int DIRECTIOB_SCALE = 100;//方向结果转换比例（仅用于结果显示）
		SaveDataToImageFile(srcImgFile, stepImgFile4, direction, DIRECTIOB_SCALE);
		SaveDataToTextFile(stepTxtFile4, direction, iWidth, iHeight);
	}
	delete[] tmpDirection;

	//第5步：频率计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	float *frequency = new float[iWidth*iHeight];
	Frequency(image1, direction, frequency, iWidth, iHeight);//频率计算
	if (bSaveImg)
	{
		const int FREQUENCY_SCALE = 1000;//频率结果转换比例（仅用于结果显示）
		SaveDataToImageFile(srcImgFile, stepImgFile5, frequency, FREQUENCY_SCALE);
		SaveDataToTextFile(stepTxtFile5, frequency, iWidth, iHeight);
	}

	//第6步：掩码计算
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第三步均衡化结果为源图数据
	unsigned char *mask = new unsigned char[iWidth*iHeight];
	GetMask(image1, direction, frequency, mask, iWidth, iHeight);//掩码计算
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile6, mask);
		SaveDataToTextFile(stepTxtFile6, mask, iWidth, iHeight);
	}

	//第7步：Gabor滤波增强
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第3步均衡化结果为源图数据
	GaborEnhance(image1, direction, frequency, mask, image2, iWidth, iHeight);//增强
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile7, image2);
		SaveDataToTextFile(stepTxtFile7, image2, iWidth, iHeight);
	}
	delete[] direction;
	delete[] frequency;
	delete[] mask;

	//第8步：二值化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第7步Gabor滤波结果为源图数据
	BinaryImg(image1, image2, iWidth, iHeight, 128);//二值化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile8, image2);
		SaveDataToTextFile(stepTxtFile8, image2, iWidth, iHeight);
	}

	//第9步：细化
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第8步二值化结果为源图数据
	Thinning(image1, image2, iWidth, iHeight, 128);//细化
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile9, image2);
		SaveDataToTextFile(stepTxtFile9, image2, iWidth, iHeight);
	}
	unsigned char *thin = new unsigned char[iWidth*iHeight];
	memcpy(thin, image2, sizeof(unsigned char)*iWidth*iHeight);//保存细化结果

															   //第10步：特征提取
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight);//以第9步细化结果为源图数据
	int minuCount = Extract(image1, image2, iWidth, iHeight);//特征提取
	if (bSaveImg)
	{
		SaveDataToImageFile(srcImgFile, stepImgFile10, image2);
		SaveDataToTextFile(stepTxtFile10, image2, iWidth, iHeight);
	}


	//第11步：特征过滤
	memcpy(image1, image2, sizeof(unsigned char)*iWidth*iHeight); //以第10步特征提取结果为源图数据
	MINUTIAE *minutiaes = new MINUTIAE[minuCount];
	memset(minutiaes, sizeof(MINUTIAE), minuCount);
	MinuFilter(image1, thin, minutiaes, minuCount, iWidth, iHeight);
	delete[] thin;
	SaveMinutiae(minutiaes, minuCount, stepMdlFile11);//保存特征数据文件
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

	//第12步：特征识别
	int id = 0;
	char name[MAX_PATH] = { 0 }, src[MAX_PATH] = { 0 }, mdl[MAX_PATH] = { 0 };
	float maxSimilar = -1;
	int maxID = -1;
	char maxName[MAX_PATH] = { 0 }, maxSrc[MAX_PATH] = { 0 };
	MINUTIAE *minutiaes2 = NULL;
	int minuCount2 = 0;

	FILE *index = fopen(DB_INDEX_TXT, "r");//打开指纹库索引文件

	while (!feof(index))
	{
		//读取指纹记录信息（指纹编号/指纹图像文件名/特征模板文件名.登记人姓名）
		fscanf(index, "%d %s %s %s\n", &id, src, mdl, name);

		//从特征模板文件中读取指纹特征
		minuCount2 = ReadMinutiae(mdl, &minutiaes2);

		//特征匹配
		float similar = MinuSimilarity(iWidth, iHeight, minutiaes, minuCount, minutiaes2, minuCount2);

		//保存相似度最高的指纹记录信息
		if (similar > maxSimilar)
		{
			maxSimilar = similar;
			maxID = id;
			sprintf(maxName, name);
			sprintf(maxSrc, src);
		}
	}

	fclose(index);//关闭文件

	delete[] minutiaes;
	delete[] minutiaes2;

	//返回识别结果
	const float SIMILAR_THRED = 0.1;//匹配相似度阀值（小于阀值则认为不匹配）
	if (maxID<0 || maxSimilar < SIMILAR_THRED)
	{
		sprintf(info, "识别失败.");
		return -2;
	}

	strcpy(outImgFile, maxSrc);//保存识别结果图像文件名
	if (bSaveImg)
	{
		CopyFile(ToWideChar(maxSrc), ToWideChar(stepImgFile12), false);//复制识别结果图像
	}
	sprintf(info, "识别成功. 识别结果：姓名[%s],目标指纹[%s],相似度[%.2f].", maxName, maxSrc, maxSimilar);
	return 0;
}

//获取Debug或Release所在的路径
CString GetModuleDir()
{
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);

	return path;
}

//删除指定的文件夹
void DeleteDirectory(CString strDir)
{
	if (strDir.IsEmpty())
	{
		RemoveDirectory(strDir);
		return;
	}

	//首先删除文件及子文件夹 
	CFileFind   ff;
	BOOL bFound = ff.FindFile(strDir + _T("\\*"),0);
	while (bFound)
	{
		bFound = ff.FindNextFile();
		if (ff.GetFileName() == _T(".") || ff.GetFileName() == _T(".."))
			continue;
		//去掉文件(夹)只读等属性 
		SetFileAttributes(ff.GetFilePath(), FILE_ATTRIBUTE_NORMAL);
		if (ff.IsDirectory())
		{
			//递归删除子文件夹 
			DeleteDirectory(ff.GetFilePath());
			RemoveDirectory(ff.GetFilePath());
		}
		else
		{
			DeleteFile(ff.GetFilePath());   //删除文件 
		}
	}
	ff.Close();
	//然后删除该文件夹 
	RemoveDirectory(strDir);
}

//删除文件夹
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
