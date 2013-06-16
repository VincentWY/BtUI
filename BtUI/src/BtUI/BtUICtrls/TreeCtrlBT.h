/************************************************************************/
/*TreeCtrlBT.h                                                          */
/************************************************************************/

#pragma once
#include <map>
#include <vector>
#include "ScrollBarEx.h"
#include "BaseCtrl.h"
using namespace std;

//宏定义函数：用于释放GDI对象
#define RELEASE_GDI_RESOURCE(gdiResource) if(gdiResource.GetSafeHandle()){gdiResource.DeleteObject();}

typedef enum tagItemImgSt_t
{
	item_std,
	item_alt
}ItemImgSt_t;

//项所包含的相关信息
typedef struct tagItemMsg
{	
	HTREEITEM hItem;			//项的句柄
	CxImage   ItemImg;
	CxImage   ItemImgAlt;		//切换图片
	CxImage   ItemBack;
	ItemImgSt_t st;			//当前项状态
	BOOL	  bDraw;			//该项是否需要绘制

	tagItemMsg()
	{
		hItem   = NULL;
		st  = item_std;
		bDraw = TRUE;
	}
	
	~tagItemMsg()
	{

	}
	
}ITEM_MSG;
typedef map<HTREEITEM,ITEM_MSG*>	ITEMMSG_MAP;

class BTUI_API CTreeCtrlBT : public CTreeCtrl,
					public CBTBaseCtrl
{
// Construction
public:
	CTreeCtrlBT();
	virtual ~CTreeCtrlBT();
public:
	virtual void UpdateCtrl(BYTE* buf);
	
public:

	//设置背景位图
	BOOL SetBackgroudImg(LPCTSTR lpszRecourceName);

	CString GetItemTextBT(HTREEITEM hItem);
	void SetImgChangeAuto(BOOL mark);//废除
	void SetTreeItemSameHeight(BOOL mark = TRUE);
	void ResetItemImg(HTREEITEM hItem, ItemImgSt_t st);//变换节点图标
	ItemImgSt_t GetItemImgSt(HTREEITEM hItem);
	//设置某一项位图
	void SetItemImg(HTREEITEM hItem, LPCTSTR lpszImg, LPCTSTR lpszImgAlt = L"");

	//设置滚动条贴图
	void SetScrollBarImg(CString strimg);
	void SetScrollBarImg(UINT uID);
	void SetScrollBarImg(LPCTSTR strimg);

	//设置项的默认图
	void SetDefaultItemImg(CString strimg, CString strimgAlt = L"");
	void SetDefaultItemImg(LPCTSTR strimg, LPCTSTR strimgAlt = L"");

	//设置展开收缩的图表
	//void SetExpandBitmap(UINT nIdExpand, UINT nIdCollapse, COLORREF crTransparent = RGB(255,255,255));
	void SetExpandPng(LPCTSTR lpszExpandAndCollapse);

	//设置选中项的背景以及鼠标move背景
	void SetSelectAndMovePng(LPCTSTR lpszSel, LPCTSTR lpszMove);

	//设置某一项的背景
	void SetItemBackImg(LPCTSTR lpszBack, LPCTSTR lpszBackAlt);

	void SetSeparateColor(COLORREF clr, BOOL mark = TRUE);

	void SetFontNum(int parent = 20, int child = 20);//设置字号
	
	//插入新的项目
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, int heightTime = 1, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"", BOOL bDraw = TRUE);
	//HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"", BOOL bDraw = TRUE);
	//插入新项目，且高度为普通的一倍
	HTREEITEM InsertItemForHeight(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"");

	//开启横线
	void EnableRowLine( BOOL bEnable = TRUE );
	//是否开启横线
	BOOL IsEnableRowLine();
	
	//开启根节点背景
	void EnableRootBk( BOOL bEnable = TRUE );
	//是否开启根节点背景
	BOOL IsEnableRootBk();

	//获取某一项信息
	ITEM_MSG* GetItemMsg(HTREEITEM hItem);

	BOOL RemoveAllItem();

	void InitCreateTree(HMODULE hResHandle);//控件为动态创建，必须调用此函数初始化，参数为资源句柄
// Operations
public:

private:
	void Calculate();
	
	//void GradientFillRect( CDC *pDC, CRect &rect, COLORREF col_from, COLORREF col_to, bool vert_grad );
	void GradientFillRect(CDC *pDC, CRect &rect, BOOL sel = TRUE);
	//void DrawBackgroudBitmap(CDC* pDc);
	void DrawBackgroudImg(CDC* pDc);
	void DrawItem( CDC* pDC );
	void DrawRowLine(CPoint ptBegin, CPoint ptEnd, CDC *pDc = NULL);
	void DrawExpand(CRect rect, int state, CDC *pDc = NULL);
	//void DrawItemBitmap(CRect rect, HTREEITEM hItem, CDC *pDc = NULL);
	void DrawItemBack(CRect rc, CDC *pDC = NULL);
	void DrawItemImg(CRect rect, HTREEITEM hItem, CDC *pDc = NULL);

	void DrawItemText(CRect rect, HTREEITEM hItem, bool bselected,CDC *pDc = NULL);

	int CalclateTotalHeight();
	int CalculateVisableItemCount();
	void GetList();
	void DestroyList();
	HTREEITEM HitTestEx( CPoint pt);
	void GetToDrawItemList(HTREEITEM hItem);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeCtrlBT)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeCtrlBT)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	ITEMMSG_MAP		m_itmemsg_map;		//保存所有相的相关信息
	ITEM_MSG*       m_pCurDrawItemMsg;	//当前正在绘制的项信息

	CRect			m_rect;				// The client rect when drawing
	int				m_h_offset;			// 0... -x (scroll offset)
	int				m_h_size;			// width of unclipped window
	int				m_v_offset;			// 0... -y (scroll offset)
	int				m_v_size;			// height of unclipped window

	//CList<HTREEITEM* ,HTREEITEM*> m_TreeItemList;
	//CBitmap			m_bmpBackgroud;		//背景位图
	CxImage			m_imgBackground;
	//CImageList		m_ImageList;		//展开收缩的图标
	std::vector<CxImage>  m_vecImgList;
	int				m_nImageHeight;		//展开收缩图标高度
	int				m_nImageWidth;		//展开收缩图标宽度
	int				m_nItemHeight;		//单元项目的高度

	bool			m_bEnableRowLine;	//单元项分开的横线 true:显示   false:反之
	bool			m_bEnableRootBk;	//根节点项的背景   true:显示   flase:反之
	int             m_nCellWidth;		//单元项图标文本的间距
	
	COLORREF		m_clrRootText;		//非叶子节点文本颜色
	COLORREF		m_clrTextNormal;	//正常情况下字体颜色
	COLORREF		m_clrTextSelect;	//选择情况下字体颜色
	
	COLORREF		m_clrRoot_from;		//根节点渐变色
	COLORREF		m_clrRoot_to;		//根节点渐变色
	COLORREF		m_clrLine;			//分割线的颜色
	
	CFont			m_FontRoots;		//根节点字体
	CFont			m_FontChild;		//子节点字体

	HTREEITEM		m_hItemSelect;		//当前选中的项目句柄
	HTREEITEM       m_hItemMouseMove;	//当前鼠标所在的句柄
	CPoint			m_ptOldMouse;		//上一次鼠标所在的位置

	CxImage			m_imgScrollBar;
	CxImage			m_imgSel;			//选中项背景
	CxImage			m_imgMove;			//鼠标经过背景
	CxImage			m_imgDefaultItem;
	CxImage			m_imgDefaultItemAlt;
	CxImage			m_imgDefaultItemBack;
	CxImage			m_imgDefaultItemBackAlt;
	BOOL			m_bCurItemBackAlt;

	HMODULE			m_hResHandle;		//资源句柄


	CScrollBarEx	m_HScroll;
	CScrollBarEx	m_VScroll;

	CString			m_strScrollImg;

	BOOL			m_dCreate;
	BOOL			m_bAutoChange;//是否自动切换状态图标
	BOOL			m_bSameHeight;//节点是否等高，不等高时所有组节点高度相同，非组节点高度一致
	BOOL			m_bSeparate;//是否显示分割线

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
