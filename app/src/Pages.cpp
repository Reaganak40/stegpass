#include "pch.h"
#include "Pages.hpp"
#include "Widgets.hpp"
#include "Window.hpp"

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
    

    /***************************
    *   -- Page Epilogue --
    ****************************/
    ImGui::PopFont();
}
