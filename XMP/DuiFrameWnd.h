// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Build:   Oct/21/2013
// Author:  Alberl Lee
// Email:   316293804@qq.com
// Website: https://blog.csdn.net/qq316293804/article/details/14162539
//
// ��ܰ��ʾ��
// ������ÿ�춼�յ��ܶ������ʼ����ʼ����벻Ҫ������ȡ��Ӧ�����������ʼ����ֿ������Һ�QQ�Ĺ��˹��ܻ���ǿ��~O(��_��)O~
// ���������������ָ���ͼ��������������Ǽ���������ֱ�����������������ʣ�����ˡ���ظ�����������Ϊ���ú����С���Ҳ�ܿ����������~O(��_��)O~
//
// ʹ��Э�飺WTFPL
// �������쳯����Э�鶼�����ӣ������ܶ��˾�������أ������Բ����ˡ�DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE��Э��~O(��_��)O~
//
// ���밲ȫ�ԣ�
// ����ĿΪʾ����Ŀ��Ϊ�˷����ұ��룬û�������⣬���д��һЩ�򵥵ĺ������ܶ��߼��ж�Ҳֻ�Ǳ�֤����������ʵ��ʹ�������б�֤���밲ȫ~O(��_��)O~
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#pragma once
#include "duilib.h"
#include <deque>
#include "../_include/xmp/AVPlayer.h"
#include "PlayList.h"

enum enum_PlayMode
{
    EM_PLAY_MODE_SEQUENCE,      // ˳�򲥷�
    EM_PLAY_MODE_RANDOM,        // �������
    EM_PLAY_MODE_SINGLE_CIRCLE  // ����ѭ��
};

class CDuiFrameWnd: public CXMLWnd
{
public:
    explicit CDuiFrameWnd(LPCTSTR pszXMLName);
    ~CDuiFrameWnd();

    DUI_DECLARE_MESSAGE_MAP()
    virtual void InitWindow();
    virtual CControlUI* CreateControl(LPCTSTR pstrClassName);
    virtual void Notify(TNotifyUI& msg);
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void OnClick(TNotifyUI& msg);
    virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam); 

    void OnDropFiles(HWND hwnd, HDROP hDropInfo);
    void OnDisplayChange(HWND hwnd, UINT bitsPerPixel, UINT cxScreen, UINT cyScreen);
    void OnGetMinMaxInfo(HWND hwnd, LPMINMAXINFO lpMinMaxInfo);
    LRESULT OnPlaying(HWND hwnd, WPARAM wParam, LPARAM lParam);     // �ļ�ͷ��ȡ��ϣ���ʼ����
    LRESULT OnPosChanged(HWND hwnd, WPARAM wParam, LPARAM lParam);  // ���ȸı䣬�������������Ľ���
    LRESULT OnEndReached(HWND hwnd, WPARAM wParam, LPARAM lParam);  // �ļ��������
    bool    OnPosChanged(void* param);                              // ���ȸı䣬�û������ı����
    bool    OnVolumeChanged(void* param);                           // �����ı�

    void Play(LPCTSTR pszPath);                                     // ����·��ΪpszPath���ļ�
    void Play(bool bPlay);                                          // ���Ż���ͣ
    void Stop();                                                    // ֹͣ
    void OpenFileDialog();                                          // ���ļ�����
    void ShowPlaylist(bool bShow);                                  // ��ʾ�����б�
    CDuiString GetNextPath(bool bNext);                             // ��ȡ��һ������·����bNextΪtrue������һ����Ϊfalse������һ��
    void AddFiles(const std::vector<string_t> &vctString, bool bInit = false);    // ����ļ��������б�

private:
    CAVPlayer       m_cAVPlayer;        // ��������
    CDuiString      m_strPath;          // ��ǰ�ļ���·��
    CSliderUI       *m_pSliderPlay;     // �ļ����Ž���
    CLabelUI        *m_pLabelTime;      // �ļ�����ʱ��
    WINDOWPLACEMENT m_OldWndPlacement;  // ���洰��ԭ����λ��
    bool            m_bFullScreenMode;  // �Ƿ���ȫ��ģʽ
    int             m_iPlaylistIndex;   // ��ǰ�����ļ���tree�ؼ�����±�  treeview�ؼ�Ŀǰ�ܲ����ƣ�������m_iPlaylistIndex���������
    CPlaylist       m_cPlayList;        // �洢�����б��·��
    enum_PlayMode   m_emPlayMode;       // ����ģʽ
    std::deque<unsigned>   m_queRand;   // �������ģʽ���ļ����

    void ReadConfig(LPCTSTR pszPath);
    void WriteConfig(LPCTSTR pszPath);

    void ShowPlayButton(bool bShow);                                // ��ʾ���Ű�ť
    void ShowPlayWnd(bool bShow);                                   // ��ʾ���Ŵ���
    void ShowControlsForPlay(bool bShow);                           // ����ʼ���Ż�ֹͣʱ����ʾ������һЩ�ؼ�

    void AdaptWindowSize(UINT cxScreen);                            // ������Ļ�ֱ����Զ��������ڴ�С
    void FullScreen(bool bFull);                                    // ȫ��
    void GetPlaylistInfo(int &iIndexTreeStart, int &iFileCount);    // ��ȡ�����б���Tree�ؼ��е���ʼλ��(���û���ļ����򷵻�-1),�Լ��ļ�����
    int  GetPlaylistIndex(int iIndexTree);                          // ����Tree�ؼ����±������Playlist�е��±�
};