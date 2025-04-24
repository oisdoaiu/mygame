#include "pch.h"
#include "Abi.h"

void Abi::Draw(CDC* pDC, CPoint tpos)
{

    CFont Big;
    Big.CreateFont(
        -50,                 // ����߶ȣ���ֵ��ʾʹ���߼���λ��
        0,                   // ������
        0,                   // �ַ���б�Ƕ�
        0,                   // �ַ���ת�Ƕ�
        FW_BOLD,             // �����ϸ��FW_BOLD ��ʾ���壩
        FALSE,               // �Ƿ�Ϊб��
        FALSE,               // �Ƿ�Ϊ�»���
        FALSE,               // �Ƿ�Ϊɾ����
        ANSI_CHARSET,        // �ַ���
        OUT_DEFAULT_PRECIS,  // �������
        CLIP_DEFAULT_PRECIS, // ���þ���
        DEFAULT_QUALITY,     // �������
        DEFAULT_PITCH | FF_SWISS, // �ַ�����������
        _T("Arial")          // ��������
    );
    
    CFont Small;
    Small.CreateFont(
        -20,                 // ����߶ȣ���ֵ��ʾʹ���߼���λ��
        0,                   // ������
        0,                   // �ַ���б�Ƕ�
        0,                   // �ַ���ת�Ƕ�
        FW_BOLD,             // �����ϸ��FW_BOLD ��ʾ���壩
        FALSE,               // �Ƿ�Ϊб��
        FALSE,               // �Ƿ�Ϊ�»���
        FALSE,               // �Ƿ�Ϊɾ����
        ANSI_CHARSET,        // �ַ���
        OUT_DEFAULT_PRECIS,  // �������
        CLIP_DEFAULT_PRECIS, // ���þ���
        DEFAULT_QUALITY,     // �������
        DEFAULT_PITCH | FF_SWISS, // �ַ�����������
        _T("Arial")          // ��������
    );
    CFont* pOldFont = pDC->SelectObject(&Big);
    CSize Size;

    if (lv == 1) {
        pDC->SetTextColor(RGB(6, 197, 38));
    }
    if (lv == 2) {
        pDC->SetTextColor(RGB(255, 185, 15));
    }
    if (lv == 3) {
        pDC->SetTextColor(RGB(255, 0, 0));
    }

    CPoint pos = tpos;
    pos.x += 20;
    if (type == 1) {
        pDC->TextOutW(pos.x, pos.y+50, _T("�ñ�������"));
        pDC->SelectObject(&Small);
        pos.x += 300; pos.y += 30;
        pDC->SetTextColor(RGB(0, 0, 0));
        pDC->TextOutW(pos.x, pos.y, _T("���ӷ�ֳ����������"));
        Size = pDC->GetTextExtent(_T("���ӷ�ֳ����������")); pos.x += Size.cx;

        pDC->SetTextColor(RGB(6, 197, 38));
        pDC->TextOutW(pos.x, pos.y, _T("13"));
        Size = pDC->GetTextExtent(_T("13")); pos.x += Size.cx;
        pDC->SetTextColor(RGB(0, 0 ,0));
        pDC->TextOutW(pos.x, pos.y, _T("/"));
        Size = pDC->GetTextExtent(_T("/")); pos.x += Size.cx;
        pDC->SetTextColor(RGB(255, 185, 15));
        pDC->TextOutW(pos.x, pos.y, _T("19"));
        Size = pDC->GetTextExtent(_T("19")); pos.x += Size.cx;
        pDC->SetTextColor(RGB(0, 0, 0));
        pDC->TextOutW(pos.x, pos.y, _T("/"));
        Size = pDC->GetTextExtent(_T("/")); pos.x += Size.cx;
        pDC->SetTextColor(RGB(255, 0, 0));
        pDC->TextOutW(pos.x, pos.y, _T("25"));
        Size = pDC->GetTextExtent(_T("25")); pos.x += Size.cx;
        pDC->SetTextColor(RGB(0, 0, 0));
        pDC->TextOutW(pos.x, pos.y, _T("��"));
        Size = pDC->GetTextExtent(_T("��")); pos.x += Size.cx;

    }

    pos = tpos;
    pDC->SelectObject(pOldFont);
    CBrush* pOldBrush = nullptr;
    CBrush brush;
    brush.CreateStockObject(NULL_BRUSH);
    pOldBrush = pDC->SelectObject(&brush);
    CRect rect(pos.x + 15, pos.y + 40, pos.x + 280, pos.y + 120);
    pDC->Rectangle(rect);
    rect = CRect(pos.x + 300, pos.y + 20, pos.x + 1400, pos.y + 140);
    pDC->Rectangle(rect);
    pDC->SelectObject(pOldBrush);
}

