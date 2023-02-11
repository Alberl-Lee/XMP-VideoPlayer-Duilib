#pragma once
#include <vector>
#include "DefineUnicode.h"
using namespace duilib;
using namespace std;

// �����б��ļ�������ֻ֧�ּ򵥵�M3U�����б����ڿ����PLS���б��ʽ��֧��
// ����ֻ�Ǽ�ʾ����VLC�ڲ��Ѿ�ʵ���˲����б��ܣ��������Ķ��ĵ�
class CPlaylist
{
public:
    CPlaylist(void);
    ~CPlaylist(void);

    bool ReadFile (const string_t &strPath);    // ��ȡ�����б��ļ���m_vctPath      
    bool WriteFile(const string_t &strPath);    // ����m_vctPath���ɲ����б�      
    bool ReadFile (const string_t &strPath, vector<string_t> &vctPlaylist);      // ��ȡ�����б��ļ���vctPlaylist
    bool WriteFile(const string_t &strPath, vector<string_t> &vctPlaylist);      // ����vctPlaylist���ɲ����б�
    void Add(string_t strPath);                 // ���·�����б�β��
    vector<string_t> GetPlaylist();             // ��ȡ�����б�����·��
    string_t GetPlaylist(unsigned uIndex);      // ��ȡ�����б�ָ���±��·��

private:
    vector<string_t> m_vctPath;                 // ��Ų����б��·��
};

