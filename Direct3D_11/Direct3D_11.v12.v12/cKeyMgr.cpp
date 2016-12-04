#include "StdAfx.h"
#include "cKeyMgr.h"

cKeyMgr::cKeyMgr( void )
{
}

cKeyMgr::~cKeyMgr( void )
{
}

HRESULT cKeyMgr::Init( void )
{
	for ( int i = 0; i < MAX_KEY_NUM; i++ ) {
		m_bDownCheck[i] = false;
		m_bUpCheck[i] = false;
	}


	return S_OK;
}
void cKeyMgr::Release( void )
{

}

bool cKeyMgr::IsStayDown( int nVKey )
{
	return ( GetAsyncKeyState( nVKey ) & 0x8000 );
}

bool cKeyMgr::IsOnceDown( int nVKey )
{
	if ( GetAsyncKeyState( nVKey ) & 0x8000 )
	{
		if ( m_bDownCheck[nVKey] == false )
		{
			m_bDownCheck[nVKey] = true;
			return true;
		}
	}

	else {
		m_bDownCheck[nVKey] = false;
	}

	return false;
}

bool cKeyMgr::IsOnceUp( int nVKey )
{
	if ( GetAsyncKeyState( nVKey ) & 0x8000 )
	{
		if ( m_bUpCheck[nVKey] == false )
		{
			m_bUpCheck[nVKey] = true;
		}
	}
	else
	{
		if ( m_bUpCheck[nVKey] )
		{
			m_bUpCheck[nVKey] = false;
			return true;
		}
	}

	return false;
}
