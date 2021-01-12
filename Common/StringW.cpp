// String.cpp: implementation of the CMyStringW class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "StringW.h"

#pragma warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4996)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyStringW::CMyStringW()
    : m_nSize(0)
    , m_nLength(0)
    , m_pBuff(NULL)
    , m_pnRefCnt(NULL)
{

}

CMyStringW::CMyStringW(wchar_t ch, int nRepeat)
    : m_nSize(0)
    , m_nLength(0)
    , m_pBuff(NULL)
    , m_pnRefCnt(NULL)
{
    Alloc(nRepeat + 1);

    m_nLength = nRepeat;

    for (int i = 0; i < nRepeat; i++)
    {
        m_pBuff[i] = ch;
    }
}

CMyStringW::CMyStringW(const wchar_t *pszSrc)
    : m_nSize(0)
    , m_nLength(0)
    , m_pBuff(NULL)
    , m_pnRefCnt(NULL)
{
    AllocCopy(pszSrc);
}

CMyStringW::CMyStringW(const wchar_t *pszSrc, int nLength)
    : m_nSize(0)
    , m_nLength(0)
    , m_pBuff(NULL)
    , m_pnRefCnt(NULL)
{
    AllocCopy(pszSrc, nLength + 1);
}

CMyStringW::CMyStringW(const char *pszSrc)
{
    CMyStringW strSrc = FromMultiByte(pszSrc);
    m_nSize = strSrc.m_nSize;
    m_nLength = strSrc.m_nLength;
    m_pBuff = strSrc.m_pBuff;
    m_pnRefCnt = strSrc.m_pnRefCnt;
    AddRefCnt();
}

CMyStringW::CMyStringW(const char *pszSrc, int nLength)
{
    CMyStringW strSrc = FromMultiByte(pszSrc, nLength);
    m_nSize = strSrc.m_nSize;
    m_nLength = strSrc.m_nLength;
    m_pBuff = strSrc.m_pBuff;
    m_pnRefCnt = strSrc.m_pnRefCnt;
    AddRefCnt();
}

CMyStringW::CMyStringW(const CMyStringW& obj)
    : m_nSize(obj.m_nSize)
    , m_nLength(obj.m_nLength)
    , m_pBuff(obj.m_pBuff)
    , m_pnRefCnt(obj.m_pnRefCnt)
{
    AddRefCnt();
}

CMyStringW::~CMyStringW()
{
    Release();
}

//�ַ���ƴ��
CMyStringW& CMyStringW::Append(wchar_t ch)
{
    wchar_t sz[2] = { ch };

    return Append(sz);
}

//�ַ���ƴ��
CMyStringW& CMyStringW::Append(const wchar_t *pszSrc)
{
    if (pszSrc == NULL)
    {
        return *this;
    }

    int nLength = GetLength(pszSrc) + m_nLength;
    int nSize = nLength < m_nSize ? m_nSize : nLength + 1;

    CopyBeforeWrite(nSize);
    wcscat(m_pBuff, pszSrc);
    m_nLength = nLength;

    return *this;
}

//�ַ���ƴ��
CMyStringW& CMyStringW::Append(const char *pszSrc)
{
    CMyStringW strSrc = FromMultiByte(pszSrc);

    return Append(strSrc.m_pBuff);
}

//�ַ���ƴ��
CMyStringW& CMyStringW::Append(const CMyStringW& obj)
{
    if (this != &obj)
    {
        Append(obj.m_pBuff);
    }
    else
    {
        Append(CMyStringW(obj.m_pBuff).m_pBuff);
    }

    return *this;
}

//�ַ����Ƿ���ͬ
bool CMyStringW::IsEqual(wchar_t ch) const
{
    wchar_t sz[2] = { ch };

    return IsEqual(sz);
}

//�ַ����Ƿ���ͬ
bool CMyStringW::IsEqual(const wchar_t *pszSrc) const
{
    if (pszSrc == NULL)
    {
        return false;
    }

    return wcscmp(m_pBuff, pszSrc) == 0;
}

//�ַ����Ƿ���ͬ
bool CMyStringW::IsEqual(const CMyStringW& obj) const
{
    return IsEqual(obj.m_pBuff);
}

//�ַ����Ƚ�
int CMyStringW::Compare(wchar_t ch) const
{
    wchar_t sz[2] = { ch };

    return Compare(sz);
}

//�ַ����Ƚ�
int CMyStringW::Compare(const wchar_t *pszSrc) const
{
    if (pszSrc == NULL)
    {
        return -1;
    }

    return wcscmp(m_pBuff, pszSrc);
}

//�ַ����Ƚ�
int CMyStringW::Compare(const CMyStringW& obj) const
{
    return Compare(obj.m_pBuff);
}



//�Ƚ��ַ���
int CMyStringW::CompareNoCase(char ch) const
{
    char sz[2] = { ch };

    return CompareNoCase(sz);
}

//�Ƚ��ַ���
int CMyStringW::CompareNoCase(const wchar_t *pszSrc) const
{
    if (pszSrc == NULL)
    {
        return -1;
    }

    return wcsicmp(m_pBuff, pszSrc);
}

//�Ƚ��ַ���
int CMyStringW::CompareNoCase(const CMyStringW& obj) const
{
    return CompareNoCase(obj.m_pBuff);
}

//�Ƿ�����Ӵ�
bool CMyStringW::Contain(wchar_t ch) const
{
    wchar_t sz[2] = { ch };

    return Contain(sz);
}

//�Ƿ�����Ӵ�
bool CMyStringW::Contain(const wchar_t *pszSub) const
{
    if (pszSub == NULL)
    {
        return true;
    }

    return wcsstr(m_pBuff, pszSub) != NULL;
}

//�Ƿ�����Ӵ�
bool CMyStringW::Contain(const CMyStringW& obj) const
{
    return Contain(obj.m_pBuff);
}

//�����ַ�
int CMyStringW::Find(wchar_t ch) const
{
    wchar_t sz[2] = { ch };

    return Find(sz);
}

//�����Ӵ�
int CMyStringW::Find(const wchar_t *pszSub) const
{
    wchar_t* psz = wcsstr(m_pBuff, pszSub);

    return psz != NULL ? psz - m_pBuff : -1;
}

//�����ַ�
int CMyStringW::Find(wchar_t ch, int nStart) const
{
    wchar_t sz[2] = { ch };

    return Find(sz, nStart);
}

//�����Ӵ�
int CMyStringW::Find(const wchar_t *pszSub, int nStart) const
{
    wchar_t* psz = wcsstr(m_pBuff + nStart, pszSub);

    return psz != NULL ? psz - m_pBuff : -1;
}

//��������ַ�
int CMyStringW::ReverseFind(wchar_t ch) const
{
    for (int i = m_nLength - 1; i >= 0; i--)
    {
        if (m_pBuff[i] == ch)
        {
            return i;
        }
    }

    return -1;
}

//תΪ��д
CMyStringW& CMyStringW::Upper()
{
    if (m_pBuff == NULL)
    {
        return *this;
    }

    CopyBeforeWrite();

    wcsupr(m_pBuff);

    return *this;
}

//תΪСд
CMyStringW& CMyStringW::Lower()
{
    if (m_pBuff == NULL)
    {
        return *this;
    }

    CopyBeforeWrite();

    wcslwr(m_pBuff);

    return *this;
}

//�Ƴ��ַ���
CMyStringW& CMyStringW::Remove(int nIndex, int nCount)
{
    if (nIndex < 0 || nIndex >= m_nLength || nCount < 0)
    {
        return *this;
    }

    CopyBeforeWrite();

    if (nCount == 0 || nIndex + nCount >= m_nLength)
    {
        m_nLength = nIndex;
        m_pBuff[nIndex] = L'\0';
    }
    else
    {
        m_nLength -= nCount;
        for (int i = nIndex; i <= m_nLength; i++)
        {
            m_pBuff[i] = m_pBuff[i + nCount];
        }
    }

    return *this;
}

//�����ַ���
CMyStringW& CMyStringW::Insert(int nIndex, const wchar_t *pszSrc)
{
    if (pszSrc == NULL || *pszSrc == '\0' || nIndex < 0)
    {
        return *this;
    }

    int nLength = GetLength(pszSrc), i = 0;

    CopyBeforeWrite(m_nLength + nLength + 1);

    nIndex = nIndex < m_nLength ? nIndex : m_nLength;

    for (i = m_nLength; i >= nIndex; i--)
    {
        m_pBuff[i + nLength] = m_pBuff[i];
    }

    for (i = 0; i < nLength; i++)
    {
        m_pBuff[i + nIndex] = pszSrc[i];
    }

    m_nLength += nLength;

    return *this;
}

//�滻�ַ���
CMyStringW& CMyStringW::Replace(const wchar_t *pszOld, const wchar_t *pszNew)
{
    int nOldLength = GetLength(pszOld);

    if (m_nLength == 0 || nOldLength == 0)
    {
        return *this;
    }

    int nNewLength = GetLength(pszNew);

    int nCount = 0;
    wchar_t *pszStart = m_pBuff;
    wchar_t *pszTarget = NULL;

    while ((pszTarget = wcsstr(pszStart, pszOld)) != NULL)
    {
        nCount++;
        pszStart = pszTarget + nOldLength;
    }

    if (nCount > 0)
    {
        CopyBeforeWrite(m_nLength + (nNewLength - nOldLength) * nCount + 1);

        pszStart = m_pBuff;

        while ((pszTarget = wcsstr(pszStart, pszOld)) != NULL)
        {
            int nLength = m_nLength - (pszTarget - m_pBuff) - nOldLength + 1;

            memmove(pszTarget + nNewLength,
                pszTarget + nOldLength,
                nLength * sizeof(wchar_t));
            memmove(pszTarget, pszNew, nNewLength * sizeof(wchar_t));

            m_nLength += nNewLength - nOldLength;
            pszStart = pszTarget + nNewLength;
        }
    }

    return *this;
}

//�ַ����ָ�
int CMyStringW::Split(const wchar_t *pszSplit, CMyList<CMyStringW>& lstStrings)
{
    lstStrings.RemoveAll();

    int nSplitLength = GetLength(pszSplit);

    if (m_nLength == 0 || nSplitLength == 0)
    {
        return lstStrings.GetCount();
    }

    wchar_t *pszStart = m_pBuff;
    wchar_t *pszTarget = NULL;

    while ((pszTarget = wcsstr(pszStart, pszSplit)) != NULL)
    {
        int nLength = pszTarget - pszStart;

        CMyStringW strItem;
        strItem.AllocCopy(pszStart, nLength + 1);
        lstStrings.AddTail(strItem);

        pszStart = pszTarget + nSplitLength;
    }

    if (pszStart[0] != L'\0')
    {
        lstStrings.AddTail(CMyStringW(pszStart));
    }

    return lstStrings.GetCount();
}

//��ʽ���ַ���
CMyStringW& CMyStringW::Format(const wchar_t *pszFmt, ...)
{
    va_list argList = NULL;
    va_start(argList, pszFmt);

    const int MAXSIZE = 256;
    wchar_t szBuff[MAXSIZE] = { 0 };
    _vsnwprintf(szBuff, MAXSIZE - 1, pszFmt, argList);

    va_end(argList);

    AllocCopy(szBuff);

    return *this;
}

//����ռ�
void CMyStringW::Alloc(int nSize)
{
    Release();

    m_nSize = nSize;
    m_nLength = 0;
    m_pBuff = new wchar_t[m_nSize];
    memset(m_pBuff, 0, m_nSize * sizeof(wchar_t));

    AddRefCnt();
}

//���俽��
void CMyStringW::AllocCopy(const wchar_t *pszBuff, int nSize)
{
    int nLength = GetLength(pszBuff);

    if (nSize <= 0)
    {
        nSize = nLength < nSize ? nSize : nLength + 1;
    }

    if (GetRefCnt() > 1 || nSize > m_nSize)
    {
        Release();

        m_nSize = nSize;
        m_pBuff = new wchar_t[m_nSize];

        AddRefCnt();
    }

    m_nLength = nLength < m_nSize ? nLength : m_nSize - 1;

    wcsncpy(m_pBuff, pszBuff, m_nLength);
    m_pBuff[m_nLength] = L'\0';
}

//дʱ����
void CMyStringW::CopyBeforeWrite(int nSize)
{
    nSize = nSize > 0 ? nSize : m_nSize;

    if (GetRefCnt() > 1 || nSize > m_nSize)
    {
        int nLength = m_nLength < nSize ? m_nLength : nSize - 1;
        wchar_t *pszBuff = new wchar_t[nSize];
        wcsncpy(pszBuff, m_pBuff, nLength);
        pszBuff[nLength] = L'\0';
        Release();

        m_nSize = nSize;
        m_nLength = nLength;
        m_pBuff = pszBuff;
        AddRefCnt();
    }
}

//תΪ����
int CMyStringW::ToInt(const wchar_t *pszSrc)
{
    int nValue = 0;

    swscanf(pszSrc, L"%d", &nValue);

    return nValue;
}

//תΪ����
int CMyStringW::ToInt(const CMyStringW& obj)
{
    return ToInt(obj.m_pBuff);
}

//תΪ������
double CMyStringW::ToFloat(const wchar_t *pszSrc)
{
    double dblValue = 0;

    swscanf(pszSrc, L"%lf", &dblValue);

    return dblValue;
}

//תΪ������
double CMyStringW::ToFloat(const CMyStringW& obj)
{
    return ToFloat(obj.m_pBuff);
}

//תΪʮ������
int CMyStringW::ToHex(const CMyStringW& obj)
{
    return ToHex(obj.m_pBuff);
}

//תΪʮ������
int CMyStringW::ToHex(const wchar_t *pszSrc)
{
    return wcstoul(pszSrc, NULL, 16);
}

#ifdef WIN32

#include <Windows.h>

CMyStringW CMyStringW::FromMultiByte(const char *pszSrc, int nBytes)
{
    CMyStringW strDest;

    if (pszSrc == NULL)
    {
        return strDest;
    }

    int nSize = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        pszSrc,
        nBytes,
        NULL,
        0);

    if (nSize <= 0)
    {
        return strDest;
    }

    strDest.Alloc(nSize);
    strDest.m_nLength = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        pszSrc,
        nBytes,
        strDest.m_pBuff,
        nSize);
    strDest.m_nLength--;

    return strDest;
}
#else // Linux Version

#endif // WIN32


wchar_t* wcsistr(const wchar_t * str1, const wchar_t * str2)
{
    wchar_t *cp = (wchar_t *)str1;
    wchar_t *s1, *s2;

    if (!*str2)
        return((wchar_t *)str1);

    while (*cp)
    {
        s1 = cp;
        s2 = (wchar_t *)str2;

        while (*s1 && *s2)
        {
            wchar_t ch1 = *s1, ch2 = *s2;
            if (iswascii(*s1) && iswupper(*s1)) ch1 = towlower(*s1);
            if (iswascii(*s2) && iswupper(*s2)) ch2 = towlower(*s2);

            if (ch1 - ch2 == 0) s1++, s2++;
            else break;
        }

        if (!*s2)
            return(cp);

        cp++;
    }

    return(NULL);
}