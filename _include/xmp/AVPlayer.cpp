#include "AVPlayer.h"
#include <cmath>
#include <vector>
#include "../vlc/vlc.h"
#pragma comment(lib, "../_lib/vlc/libvlc.lib")
#pragma comment(lib, "../_lib/vlc/libvlccore.lib")


CAVPlayer::CAVPlayer(void) :
m_pVLC_Inst(NULL),
m_pVLC_Player(NULL),
m_hWnd(NULL),
m_pfnPlaying(NULL),
m_pfnPosChanged(NULL),
m_pfnEndReached(NULL)
{
}

CAVPlayer::~CAVPlayer(void)
{
    Release();
}

void CAVPlayer::Init()
{
    if (! m_pVLC_Inst)
    {
        m_pVLC_Inst = libvlc_new(0, NULL);
    }
}

void CAVPlayer::Release()
{
    Stop();

    if (m_pVLC_Inst)
    {
        libvlc_release (m_pVLC_Inst);
        m_pVLC_Inst   = NULL;
    }
}

bool CAVPlayer::Play(const std::string &strPath)
{
    if (! m_pVLC_Inst)
    {
        Init();
    }

    if(strPath.empty() || ! m_pVLC_Inst)
    {
        return false;
    }

    // ��֤��ַ�������ַ�����Ǳ��ص�ַ
    bool bURL = false;
    std::vector<std::string> vctURL;

    vctURL.push_back("http"); 
    vctURL.push_back("https"); 
    vctURL.push_back("ftp");
    vctURL.push_back("rstp");

    for (unsigned i = 0; i < vctURL.size(); i++)
    {
        // ʵ��ʹ�����жϴ�Сд
        if (! strPath.compare(0, vctURL[i].size(), vctURL[i]))
        {
            bURL = true;
            break;
        }
    }

    Stop();

    bool bRet = false;
    libvlc_media_t *m;

    if (bURL)
    {
        // ʾ�������ַ��http://zhangmenshiting.baidu.com/data2/music/1203175/51597064800128.mp3?xcode=cb97ecc9d44ba29122881c69e505a43849f04e8309ecc859
        m = libvlc_media_new_location(m_pVLC_Inst, strPath.c_str());
    } 
    else
    {
        m = libvlc_media_new_path(m_pVLC_Inst, strPath.c_str());
    }

    if (m)
    {
        if (m_pVLC_Player = libvlc_media_player_new_from_media(m))
        {
            libvlc_media_player_set_hwnd(m_pVLC_Player, m_hWnd);
            libvlc_media_player_play(m_pVLC_Player);

            // �¼�����
            libvlc_event_manager_t *vlc_evt_man = libvlc_media_player_event_manager(m_pVLC_Player);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPlaying, ::OnVLC_Event, this);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerPositionChanged, ::OnVLC_Event, this);
            libvlc_event_attach(vlc_evt_man, libvlc_MediaPlayerEndReached, ::OnVLC_Event, this);
            bRet = true;
        }

        libvlc_media_release(m);
    }

    return bRet;
}

void CAVPlayer::Stop()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_stop (m_pVLC_Player);      /* Stop playing */    
        libvlc_media_player_release (m_pVLC_Player);   /* Free the media_player */
        m_pVLC_Player = NULL;
    }
}

void CAVPlayer::Play()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_play(m_pVLC_Player);
    }
}

void CAVPlayer::Pause()
{
    if (m_pVLC_Player)
    {
        libvlc_media_player_pause(m_pVLC_Player);
    }
}

void CAVPlayer::Volume(int iVol)
{
    if (iVol < 0)
    {
        return;
    }

    if (m_pVLC_Player)
    {
        libvlc_audio_set_volume(m_pVLC_Player,int(iVol * 1.3)); // ����ŵ�100���о���Ѹ�׵�100����30������������1.3������
    }
}

void CAVPlayer::VolumeIncrease()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)ceil(iVol * 1.1));
    }
}

void CAVPlayer::VolumeReduce()
{
    if (m_pVLC_Player)
    {
        int iVol = libvlc_audio_get_volume(m_pVLC_Player);
        Volume((int)floor(iVol * 0.9));
    }
}

int CAVPlayer::GetPos()
{   
    if (m_pVLC_Player)
    {
        return (int)(1000 * libvlc_media_player_get_position(m_pVLC_Player));
    }

    return 0;
}

void CAVPlayer::SeekTo(int iPos)
{
    if (iPos < 0 || iPos > 1000)
    {
        return;
    }

    if (m_pVLC_Player)
    {
        libvlc_media_player_set_position(m_pVLC_Player, iPos/(float)1000.0);
    }
}

void CAVPlayer::SeekForward()
{
    //int iPos = GetPos();
    //SeekTo((int)ceil(iPos * 1.1));

    // һ�ο���5��
    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) + 5000;

        if (i_time > GetTotalTime())
        {
            i_time = GetTotalTime();
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void CAVPlayer::SeekBackward()
{
    //int iPos = GetPos();
    //SeekTo((int)floor(iPos * 0.9));

    if (m_pVLC_Player)
    {
        libvlc_time_t i_time = libvlc_media_player_get_time(m_pVLC_Player) - 5000;

        if (i_time < 0)
        {
            i_time = 0;
        }

        libvlc_media_player_set_time(m_pVLC_Player, i_time);
    }
}

void CAVPlayer::SetHWND( HWND hwnd )
{
    if (::IsWindow(hwnd))
    {
        m_hWnd = hwnd;
    }
}

HWND CAVPlayer::GetHWND()
{
    return m_hWnd;
}

bool CAVPlayer::IsOpen()
{
    return NULL != m_pVLC_Player;
}

bool CAVPlayer::IsPlaying()
{
    if (m_pVLC_Player)
    {
        return (1 == libvlc_media_player_is_playing(m_pVLC_Player));
    }

    return false;
}

__int64 CAVPlayer::GetTotalTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_length(m_pVLC_Player);
    }

    return 0;
}

__int64 CAVPlayer::GetTime()
{
    if (m_pVLC_Player)
    {
        return libvlc_media_player_get_time(m_pVLC_Player);
    }

    return 0;
}

int CAVPlayer::GetVolume()
{
    if (m_pVLC_Player)
    {
        return libvlc_audio_get_volume(m_pVLC_Player);
    }

    return 0;
}

void CAVPlayer::SetCallbackPlaying( pfnCallback pfn )
{
    m_pfnPlaying = pfn;
}

void CAVPlayer::SetCallbackPosChanged( pfnCallback pfn )
{
    m_pfnPosChanged = pfn;
}

void CAVPlayer::SetCallbackEndReached( pfnCallback pfn )
{
    m_pfnEndReached = pfn;
}

void OnVLC_Event( const libvlc_event_t *event, void *data )
{
    CAVPlayer *pAVPlayer = (CAVPlayer *) data;
    pfnCallback pfn = NULL;

    if (! pAVPlayer)
    {
        return;
    }

    switch(event->type)
    {
    case libvlc_MediaPlayerPlaying:
        pfn = pAVPlayer->m_pfnPlaying;
        break;
    case libvlc_MediaPlayerPositionChanged:
        pfn = pAVPlayer->m_pfnPosChanged;
        break;
    case libvlc_MediaPlayerEndReached:
        pfn = pAVPlayer->m_pfnEndReached;
        break;
    default:
        break;
    }

    if (pfn)
    {
        pfn(data);  // �˻ص����������Դ�����������������data�⣬����event�ĸ�����Ϣ����event->u.media_player_position_changed.new_position���ȵȣ���������չ��
    }   
}
