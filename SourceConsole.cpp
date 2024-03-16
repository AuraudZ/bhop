#include "SourceConsole.h"


std::unique_ptr<SourceConsole> m_sourceConsole;

SourceConsole& SourceCon()
{
	return *m_sourceConsole;
}

SourceConsole::SourceConsole() {
	m_gameConsole = GetInterface<CGameConsole*>("client.dll","GameConsole004");

}


void SourceConsole::ColorPrint(const SourceColor& clr, const char* pMessage)
{
	if (!m_gameConsole->m_bInitialized)
	{
		return;
	}


	m_gameConsole->m_pConsole->m_pConsolePanel->ColorPrint(clr, pMessage);
} 



void SourceConsole::Print(const char* pMessage)
{
	if (!m_gameConsole->m_bInitialized)
	{
		return;
	}

	m_gameConsole->m_pConsole->m_pConsolePanel->Print(pMessage);
}

void SourceConsole::DPrint(const char* pMessage)
{
	if (!m_gameConsole->m_bInitialized)
	{
		return;
	}

	m_gameConsole->m_pConsole->m_pConsolePanel->DPrint(pMessage);
}