#include "pch.h"
#include "Abi.h"

void Abi::Draw(CDC* pDC, CPoint tpos)
{

    CFont Big;
    Big.CreateFont(
        -50,                 // 字体高度（负值表示使用逻辑单位）
        0,                   // 字体宽度
        0,                   // 字符倾斜角度
        0,                   // 字符旋转角度
        FW_BOLD,             // 字体粗细（FW_BOLD 表示粗体）
        FALSE,               // 是否为斜体
        FALSE,               // 是否为下划线
        FALSE,               // 是否为删除线
        ANSI_CHARSET,        // 字符集
        OUT_DEFAULT_PRECIS,  // 输出精度
        CLIP_DEFAULT_PRECIS, // 剪裁精度
        DEFAULT_QUALITY,     // 输出质量
        DEFAULT_PITCH | FF_SWISS, // 字符间距和字体族
        _T("Arial")          // 字体名称
    );
    
    CFont Small;
    Small.CreateFont(
        -20,                 // 字体高度（负值表示使用逻辑单位）
        0,                   // 字体宽度
        0,                   // 字符倾斜角度
        0,                   // 字符旋转角度
        FW_BOLD,             // 字体粗细（FW_BOLD 表示粗体）
        FALSE,               // 是否为斜体
        FALSE,               // 是否为下划线
        FALSE,               // 是否为删除线
        ANSI_CHARSET,        // 字符集
        OUT_DEFAULT_PRECIS,  // 输出精度
        CLIP_DEFAULT_PRECIS, // 剪裁精度
        DEFAULT_QUALITY,     // 输出质量
        DEFAULT_PITCH | FF_SWISS, // 字符间距和字体族
        _T("Arial")          // 字体名称
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
        pDC->TextOutW(pos.x, pos.y+50, _T("兔宝无限生"));
        pDC->SelectObject(&Small);
        pos.x += 300; pos.y += 30;
        pDC->SetTextColor(RGB(0, 0, 0));
        pDC->TextOutW(pos.x, pos.y, _T("兔子繁殖上限增加至"));
        Size = pDC->GetTextExtent(_T("兔子繁殖上限增加至")); pos.x += Size.cx;

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
        pDC->TextOutW(pos.x, pos.y, _T("。"));
        Size = pDC->GetTextExtent(_T("。")); pos.x += Size.cx;

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

