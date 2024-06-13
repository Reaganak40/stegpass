"""
StegPass - Password Manager Application
app.py - Main GUI for the StegPass application
"""

# ? Standard Imports
import tkinter as tk
from tkinter import ttk
from tkinterdnd2 import TkinterDnD
from threading import Thread

# ? Project Imports
from app.widgets.theme import THEME
from app.pages.add_user import AddUserWindow
from app.pages.add_password import AddPasswordWindow
from app.utils.utils import get_path_to_icon
from app.get_password import open_password_form

class ManageUserWindow(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        label = tk.Label(self, text="Manage User Page", font=('Arial', 16))
        label.pack(pady=20)
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        pass

class Application(TkinterDnD.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("StegPass - Save Password")
        self.geometry(f"{THEME.WIDTH}x{THEME.HEIGHT}")
        self.resizable(False, False)
        self.iconbitmap(get_path_to_icon())
        
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass

        # Create a menu bar
        self.menu_bar = tk.Menu(self)
        self.config(menu=self.menu_bar)

        # Add options to the menu bar
        file_menu = tk.Menu(self.menu_bar, tearoff=0)
        self.menu_bar.add_cascade(label="View", menu=file_menu)
        file_menu.add_command(label="Add User", command=self.show_add_user_page)
        file_menu.add_command(label="Manage User", command=self.show_manage_user_page)
        file_menu.add_command(label="Add Password", command=self.show_add_password_page)
        self.menu_bar.add_command(label="Get Password", command=self.handle_get_password)

        # Create frames for each page
        self.pages = {}
        self.pages['add_user'] = AddUserWindow(self)
        self.pages['manage_user'] = ManageUserWindow(self)
        self.pages['add_password'] = AddPasswordWindow(self)
        
        # Titles for each page
        self.titles = {
            'add_user': 'StegPass - Add User',
            'manage_user': 'StegPass - Manage User',
            'add_password': 'StegPass - Add Password'
        }

        # Place each frame in the same location; only one will be visible at a time
        for page in self.pages.values():
            page.place(x=0, y=0, width=THEME.WIDTH, height=THEME.HEIGHT)

        # Show the initial page
        self.show_add_user_page()

    def show_add_user_page(self):
        self._show_page('add_user')

    def show_manage_user_page(self):
        self._show_page('manage_user')

    def show_add_password_page(self):
        self._show_page('add_password')
    
    def handle_get_password(self):
        task_thread = Thread(target=open_password_form, args=(self.on_get_password_start, self.on_get_password_end))
        task_thread.start()
    
    def on_get_password_start(self):
        self.menu_bar.entryconfig("Get Password", state="disabled")
    
    def on_get_password_end(self):
        self.menu_bar.entryconfig("Get Password", state="normal")

    def _show_page(self, page_name):
        for name, page in self.pages.items():
            if name == page_name:
                page.tkraise()
                page.on_page_reload()
                self.title(self.titles[page_name])  # Update the window title
            else:
                page.lower()
                
def GuiApp():
    """ Main function for the GUI application
    """
    app = Application()
    app.mainloop()
    
if __name__ == '__main__':
    raise Exception("This file is not meant to be run on its own. Please run app/main.py instead.")