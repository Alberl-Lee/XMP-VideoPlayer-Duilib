#pragma once
#include "duilib.h"

class CMenuWnd: public CXMLWnd
{
public:
    explicit CMenuWnd(LPCTSTR pszXMLName);

protected:
    virtual ~CMenuWnd();   // ˽�л����������������˶���ֻ��ͨ��new�����ɣ�������ֱ�Ӷ���������ͱ�֤��delete this�������

public:
    void Init(CPaintManagerUI *pOwnerPM, POINT ptPos);
    virtual void    OnFinalMessage(HWND hWnd);
    virtual LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnKillFocus   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void    Notify(TNotifyUI& msg);

private:
    CPaintManagerUI *m_pOwnerPM;
};
