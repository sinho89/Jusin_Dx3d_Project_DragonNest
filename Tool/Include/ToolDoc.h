
// ToolDoc.h : CToolDoc Ŭ������ �������̽�
//


#pragma once


class CToolDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CToolDoc();
	DECLARE_DYNCREATE(CToolDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CToolDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


