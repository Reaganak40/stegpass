#include "pch.h"
#include "User.hpp"

sp::UserManager* sp::UserManager::m_instance = nullptr;

void sp::UserManager::Init()
{
	if (m_instance == nullptr)
	{
		m_instance = new UserManager();
	}
}

void sp::UserManager::Destroy()
{
	if (m_instance != nullptr)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}
