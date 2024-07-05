#include "pch.h"
#include "Pages.hpp"
#include "Widgets.hpp"
#include "Window.hpp"

/***************************************************************
* PAGE MANAGEMENT
***************************************************************/
static sp::Page activePage = sp::Page::PageAddUser;

void sp::SetActivePage(Page page)
{
	activePage = page;
}

sp::Page sp::GetActivePage()
{
    return activePage;
}

/***************************************************************
* PAGE DRAWING
***************************************************************/

void sp::DrawAddUserPage()
{
    /***************************
	*   -- Page Prologue --
	****************************/

	// title for 'add user' page
	SetWindowTitle("Stegpass - Add User");

	// set the main font
	ImFont* openSans = sp::FontManager::GetFont("OpenSans", 20.0f);
	ImGui::PushFont(openSans);

	// standard menu bar for all pages
	sp::DrawMenuBar();

	/***************************
	*   -- Page Content --
	****************************/
    sp::DrawAddUserForm();

	/***************************
	*   -- Page Epilogue --
	****************************/
	ImGui::PopFont();
}

void sp::DrawAddPasswordPage()
{

    /***************************
    *   -- Page Prologue --
    ****************************/ 

    // title for 'add password' page
    SetWindowTitle("Stegpass - Add Password");

    // set the main font
    ImFont* openSans = sp::FontManager::GetFont("OpenSans", 20.0f);
    ImGui::PushFont(openSans);

    // standard menu bar for all pages
    sp::DrawMenuBar();

    /***************************
    *   -- Page Content --
	****************************/
    sp::DrawAddPasswordForm();
    sp::DrawImageViewer();

    /***************************
    *   -- Page Epilogue --
    ****************************/
    ImGui::PopFont();
}
