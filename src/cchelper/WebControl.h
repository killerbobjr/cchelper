#pragma once

#include <windows.h>
#include <exdisp.h>

#define NOTIMPLEMENTED __asm{ int 3 }; return E_NOTIMPL


struct CNullStorage
: public IStorage
{
  // IUnknown
  STDMETHODIMP QueryInterface(REFIID riid,void ** ppvObject);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);
  // IStorage
  STDMETHODIMP CreateStream(const WCHAR * pwcsName,DWORD grfMode,DWORD reserved1,DWORD reserved2,IStream ** ppstm);
  STDMETHODIMP OpenStream(const WCHAR * pwcsName,void * reserved1,DWORD grfMode,DWORD reserved2,IStream ** ppstm);
  STDMETHODIMP CreateStorage(const WCHAR * pwcsName,DWORD grfMode,DWORD reserved1,DWORD reserved2,IStorage ** ppstg);
  STDMETHODIMP OpenStorage(const WCHAR * pwcsName,IStorage * pstgPriority,DWORD grfMode,SNB snbExclude,DWORD reserved,IStorage ** ppstg);
  STDMETHODIMP CopyTo(DWORD ciidExclude,IID const * rgiidExclude,SNB snbExclude,IStorage * pstgDest);
  STDMETHODIMP MoveElementTo(const OLECHAR * pwcsName,IStorage * pstgDest,const OLECHAR* pwcsNewName,DWORD grfFlags);
  STDMETHODIMP Commit(DWORD grfCommitFlags);
  STDMETHODIMP Revert(void);
  STDMETHODIMP EnumElements(DWORD reserved1,void * reserved2,DWORD reserved3,IEnumSTATSTG ** ppenum);
  STDMETHODIMP DestroyElement(const OLECHAR * pwcsName);
  STDMETHODIMP RenameElement(const WCHAR * pwcsOldName,const WCHAR * pwcsNewName);
  STDMETHODIMP SetElementTimes(const WCHAR * pwcsName,FILETIME const * pctime,FILETIME const * patime,FILETIME const * pmtime);
  STDMETHODIMP SetClass(REFCLSID clsid);
  STDMETHODIMP SetStateBits(DWORD grfStateBits,DWORD grfMask);
  STDMETHODIMP Stat(STATSTG * pstatstg,DWORD grfStatFlag);
};

class WebControl;


struct CMyFrame
: public IOleInPlaceFrame
{
  WebControl* host;
  // IUnknown
  STDMETHODIMP QueryInterface(REFIID riid,void ** ppvObject);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);
  // IOleWindow
	STDMETHODIMP GetWindow(HWND FAR* lphwnd);
	STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);
  // IOleInPlaceUIWindow
  STDMETHODIMP GetBorder(LPRECT lprectBorder);
  STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS pborderwidths);
  STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS pborderwidths);
  STDMETHODIMP SetActiveObject(IOleInPlaceActiveObject *pActiveObject,LPCOLESTR pszObjName);
  // IOleInPlaceFrame
  STDMETHODIMP InsertMenus(HMENU hmenuShared,LPOLEMENUGROUPWIDTHS lpMenuWidths);
  STDMETHODIMP SetMenu(HMENU hmenuShared,HOLEMENU holemenu,HWND hwndActiveObject);
  STDMETHODIMP RemoveMenus(HMENU hmenuShared);
  STDMETHODIMP SetStatusText(LPCOLESTR pszStatusText);
  STDMETHODIMP EnableModeless(BOOL fEnable);
  STDMETHODIMP TranslateAccelerator(  LPMSG lpmsg,WORD wID);

};



struct CMySite
: public IOleClientSite,
  public IOleInPlaceSite
{
  WebControl* host;

  // IUnknown
  STDMETHODIMP QueryInterface(REFIID riid,void ** ppvObject);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);
  // IOleClientSite
  STDMETHODIMP SaveObject();
  STDMETHODIMP GetMoniker(DWORD dwAssign,DWORD dwWhichMoniker,IMoniker ** ppmk);
  STDMETHODIMP GetContainer(LPOLECONTAINER FAR* ppContainer);
  STDMETHODIMP ShowObject();
  STDMETHODIMP OnShowWindow(BOOL fShow);
  STDMETHODIMP RequestNewObjectLayout();
  // IOleWindow
	STDMETHODIMP GetWindow(HWND FAR* lphwnd);
	STDMETHODIMP ContextSensitiveHelp(BOOL fEnterMode);
	// IOleInPlaceSite methods
	STDMETHODIMP CanInPlaceActivate();
	STDMETHODIMP OnInPlaceActivate();
	STDMETHODIMP OnUIActivate();
	STDMETHODIMP GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,LPOLEINPLACEUIWINDOW FAR* lplpDoc,LPRECT lprcPosRect,LPRECT lprcClipRect,LPOLEINPLACEFRAMEINFO lpFrameInfo);
	STDMETHODIMP Scroll(SIZE scrollExtent);
	STDMETHODIMP OnUIDeactivate(BOOL fUndoable);
	STDMETHODIMP OnInPlaceDeactivate();
	STDMETHODIMP DiscardUndoState();
	STDMETHODIMP DeactivateAndUndo();
	STDMETHODIMP OnPosRectChange(LPCRECT lprcPosRect);
};


struct CMyContainer
: public IOleContainer
{
  // IUnknown
  STDMETHODIMP QueryInterface(REFIID riid,void ** ppvObject);
  STDMETHODIMP_(ULONG) AddRef(void);
  STDMETHODIMP_(ULONG) Release(void);
  // IParseDisplayName
  STDMETHODIMP ParseDisplayName(IBindCtx *pbc,LPOLESTR pszDisplayName,ULONG *pchEaten,IMoniker **ppmkOut);
  // IOleContainer
  STDMETHODIMP EnumObjects(DWORD grfFlags,IEnumUnknown **ppenum);
  STDMETHODIMP LockContainer(BOOL fLock);
};


class WebControl
{
	HWND m_hwnd;
	CNullStorage storage;
	CMySite site;
	CMyFrame frame;

	IOleObject* mpWebObject;

public:
	WebControl();
	~WebControl();

	HWND GetHandle() { return m_hwnd; }

	CMyFrame * GetFrame() { return &frame; }

	void CreateEmbeddedWebControl(HWND hwnd, const RECT& rect);
	void UnCreateEmbeddedWebControl(void);

	void Nav(BSTR url);

};


///////////////////////////////////////////////////////////////////////////////

