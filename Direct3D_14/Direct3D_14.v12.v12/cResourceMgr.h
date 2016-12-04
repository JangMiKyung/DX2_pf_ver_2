#pragma once
#include "cSingletonBase.h"
#include <map>
#include <string>

template<typename T, typename A>
class cResourceMgr : public cSingletonBase<A>
{
protected:
	typedef std::map<std::string, T> MAP_RESOURCE;

	MAP_RESOURCE m_ResourceTable; //���ҽ� ���̺�

public:
	cResourceMgr();
	~cResourceMgr();

	//���� ��η� ���ҽ��� �ҷ��´�.
	T GetResource(std::string filePath, void* pParam = NULL);

	//Ư�� ���ҽ� ����
	void RemoveResource(std::string filePath);

	//��� ���ҽ� ����
	void ClearResource();

protected:

	//�ε� ��ĸ� ������
	virtual T LoadResource(std::string filePath, void* pParam = NULL) = 0;

	//���� ��ĸ� ������
	virtual void ReleaseResource(T data) = 0;

};

template<typename T, typename A>
cResourceMgr<T, A>::cResourceMgr(void)
{

}

template<typename T, typename A>
cResourceMgr<T, A>::~cResourceMgr(void)
{
	//��� ���ҽ� �ϴ�
	this->ClearResource();
}



template<typename T, typename A>
T cResourceMgr<T, A>::GetResource(std::string filePath, void* pParam /*= NULL*/)
{
	std::map<std::string, T>::iterator pFind;

	//�̹� �ε��Ǿ����� Ȯ��
	pFind = m_ResourceTable.find(filePath);

	//�ش� filePath �� �ʿ� �߰� �Ǿ����� �ʴٸ�...
	if (pFind == m_ResourceTable.end())
	{
		//LoadResource �Լ��� ���ο� Resource �ε�
		T newResource = LoadResource(filePath, pParam);

		//���ҽ� �ε��� ���еǾ��ٸ� 0����
		if (newResource == 0)
			return 0;
		
		//Map Table �� �߰�
		m_ResourceTable.insert(std::make_pair(filePath, newResource));

		return newResource;
	}

	return pFind->second;
}

template<typename T, typename A>
void cResourceMgr<T, A>::RemoveResource(std::string filePath)
{
	std::map<std::string, T>::iterator pFind;

	//�̹� �ε��Ǿ����� Ȯ��
	pFind = m_ResourceTable.find(filePath);

	//�����Ѵٸ�...
	if (pFind != m_ResourceTable.end())
	{
		//ã�� ���ҽ� ����
		ReleaseResource(pFind->second);

		//Iterator �� ����Ű�� ���� ����
		m_ResourceTable.erase(pFind);
	}
}

template<typename T, typename A>
void cResourceMgr<T, A>::ClearResource()
{
	std::map<std::string, T>::iterator pIter;

	for (pIter = m_ResourceTable.begin(); pIter != m_ResourceTable.end(); ++pIter)
	{
		ReleaseResource(pIter->second);
	}

	//Table
	m_ResourceTable.clear();
}