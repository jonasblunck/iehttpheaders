#pragma once

class CAutoMutex
{
  const CRITICAL_SECTION& m_cs;
public:
  CAutoMutex(const CRITICAL_SECTION& cs) : m_cs(cs)
  {
    EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&m_cs));
  }

  ~CAutoMutex()
  {
    LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&m_cs));
  }
};