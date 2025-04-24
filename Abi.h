#pragma once
class Abi
{
public:
	Abi(int type, int lv) {
		this->lv = lv;
		this->type = type;
	}
	int lv, type;
	void Draw(CDC* pDC, CPoint pos);
};

/*
	1.兔宝无限生
	兔子繁殖上限增加至(13/19/25)
*/