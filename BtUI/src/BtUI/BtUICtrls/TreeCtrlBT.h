/************************************************************************/
/*TreeCtrlBT.h                                                          */
/************************************************************************/

#pragma once
#include <map>
#include <vector>
#include "ScrollBarEx.h"
#include "BaseCtrl.h"
using namespace std;

//�궨�庯���������ͷ�GDI����
#define RELEASE_GDI_RESOURCE(gdiResource) if(gdiResource.GetSafeHandle()){gdiResource.DeleteObject();}

typedef enum tagItemImgSt_t
{
	item_std,
	item_alt
}ItemImgSt_t;

//���������������Ϣ
typedef struct tagItemMsg
{	
	HTREEITEM hItem;			//��ľ��
	CxImage   ItemImg;
	CxImage   ItemImgAlt;		//�л�ͼƬ
	CxImage   ItemBack;
	ItemImgSt_t st;			//��ǰ��״̬
	BOOL	  bDraw;			//�����Ƿ���Ҫ����

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

	//���ñ���λͼ
	BOOL SetBackgroudImg(LPCTSTR lpszRecourceName);

	CString GetItemTextBT(HTREEITEM hItem);
	void SetImgChangeAuto(BOOL mark);//�ϳ�
	void SetTreeItemSameHeight(BOOL mark = TRUE);
	void ResetItemImg(HTREEITEM hItem, ItemImgSt_t st);//�任�ڵ�ͼ��
	ItemImgSt_t GetItemImgSt(HTREEITEM hItem);
	//����ĳһ��λͼ
	void SetItemImg(HTREEITEM hItem, LPCTSTR lpszImg, LPCTSTR lpszImgAlt = L"");

	//���ù�������ͼ
	void SetScrollBarImg(CString strimg);
	void SetScrollBarImg(UINT uID);
	void SetScrollBarImg(LPCTSTR strimg);

	//�������Ĭ��ͼ
	void SetDefaultItemImg(CString strimg, CString strimgAlt = L"");
	void SetDefaultItemImg(LPCTSTR strimg, LPCTSTR strimgAlt = L"");

	//����չ��������ͼ��
	//void SetExpandBitmap(UINT nIdExpand, UINT nIdCollapse, COLORREF crTransparent = RGB(255,255,255));
	void SetExpandPng(LPCTSTR lpszExpandAndCollapse);

	//����ѡ����ı����Լ����move����
	void SetSelectAndMovePng(LPCTSTR lpszSel, LPCTSTR lpszMove);

	//����ĳһ��ı���
	void SetItemBackImg(LPCTSTR lpszBack, LPCTSTR lpszBackAlt);

	void SetSeparateColor(COLORREF clr, BOOL mark = TRUE);

	void SetFontNum(int parent = 20, int child = 20);//�����ֺ�
	
	//�����µ���Ŀ
	HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, int heightTime = 1, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"", BOOL bDraw = TRUE);
	//HTREEITEM InsertItemEx(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"", BOOL bDraw = TRUE);
	//��������Ŀ���Ҹ߶�Ϊ��ͨ��һ��
	HTREEITEM InsertItemForHeight(HTREEITEM hParent, LPCTSTR lpszItem, LPCTSTR lpszImg = _T(""), LPCTSTR lpszImgAlt = _T(""), LPCTSTR lpszBack = L"");

	//��������
	void EnableRowLine( BOOL bEnable = TRUE );
	//�Ƿ�������
	BOOL IsEnableRowLine();
	
	//�������ڵ㱳��
	void EnableRootBk( BOOL bEnable = TRUE );
	//�Ƿ������ڵ㱳��
	BOOL IsEnableRootBk();

	//��ȡĳһ����Ϣ
	ITEM_MSG* GetItemMsg(HTREEITEM hItem);

	BOOL RemoveAllItem();

	void InitCreateTree(HMODULE hResHandle);//�ؼ�Ϊ��̬������������ô˺�����ʼ��������Ϊ��Դ���
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
	ITEMMSG_MAP		m_itmemsg_map;		//����������������Ϣ
	ITEM_MSG*       m_pCurDrawItemMsg;	//��ǰ���ڻ��Ƶ�����Ϣ

	CRect			m_rect;				// The client rect when drawing
	int				m_h_offset;			// 0... -x (scroll offset)
	int				m_h_size;			// width of unclipped window
	int				m_v_offset;			// 0... -y (scroll offset)
	int				m_v_size;			// height of unclipped window

	//CList<HTREEITEM* ,HTREEITEM*> m_TreeItemList;
	//CBitmap			m_bmpBackgroud;		//����λͼ
	CxImage			m_imgBackground;
	//CImageList		m_ImageList;		//չ��������ͼ��
	std::vector<CxImage>  m_vecImgList;
	int				m_nImageHeight;		//չ������ͼ��߶�
	int				m_nImageWidth;		//չ������ͼ����
	int				m_nItemHeight;		//��Ԫ��Ŀ�ĸ߶�

	bool			m_bEnableRowLine;	//��Ԫ��ֿ��ĺ��� true:��ʾ   false:��֮
	bool			m_bEnableRootBk;	//���ڵ���ı���   true:��ʾ   flase:��֮
	int             m_nCellWidth;		//��Ԫ��ͼ���ı��ļ��
	
	COLORREF		m_clrRootText;		//��Ҷ�ӽڵ��ı���ɫ
	COLORREF		m_clrTextNormal;	//���������������ɫ
	COLORREF		m_clrTextSelect;	//ѡ�������������ɫ
	
	COLORREF		m_clrRoot_from;		//���ڵ㽥��ɫ
	COLORREF		m_clrRoot_to;		//���ڵ㽥��ɫ
	COLORREF		m_clrLine;			//�ָ��ߵ���ɫ
	
	CFont			m_FontRoots;		//���ڵ�����
	CFont			m_FontChild;		//�ӽڵ�����

	HTREEITEM		m_hItemSelect;		//��ǰѡ�е���Ŀ���
	HTREEITEM       m_hItemMouseMove;	//��ǰ������ڵľ��
	CPoint			m_ptOldMouse;		//��һ��������ڵ�λ��

	CxImage			m_imgScrollBar;
	CxImage			m_imgSel;			//ѡ�����
	CxImage			m_imgMove;			//��꾭������
	CxImage			m_imgDefaultItem;
	CxImage			m_imgDefaultItemAlt;
	CxImage			m_imgDefaultItemBack;
	CxImage			m_imgDefaultItemBackAlt;
	BOOL			m_bCurItemBackAlt;

	HMODULE			m_hResHandle;		//��Դ���


	CScrollBarEx	m_HScroll;
	CScrollBarEx	m_VScroll;

	CString			m_strScrollImg;

	BOOL			m_dCreate;
	BOOL			m_bAutoChange;//�Ƿ��Զ��л�״̬ͼ��
	BOOL			m_bSameHeight;//�ڵ��Ƿ�ȸߣ����ȸ�ʱ������ڵ�߶���ͬ������ڵ�߶�һ��
	BOOL			m_bSeparate;//�Ƿ���ʾ�ָ���

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
