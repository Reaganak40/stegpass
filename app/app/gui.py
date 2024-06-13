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
from app.widgets.base_gui import BaseGui
from app.get_password import open_password_form
from app.pages.add_user import AddUserWindow
from app.pages.add_password import AddPasswordWindow
from app.utils.utils import get_path_to_icon
from app.widgets.theme import THEME
from app.widgets.custom_menu_bar import CustomMenuBar

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
        
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass
        
        self.title("StegPass - Save Password")
        self.geometry(f"{THEME.WIDTH}x{THEME.HEIGHT}+75+75")
        self.resizable(False, False)
        
        #self.iconbitmap(get_path_to_icon())
        
        # Add custom title bar
        self.overrideredirect(True) # turns off title bar, geometry
        self.minimized = False # only to know if root is minimized
        self.maximized = False # only to know if root is maximized
        title_bar = TitleBar(self)
        title_bar.pack(fill=tk.X)
        
        # a frame for the main area of the window, this is where the actual app will go
        self.content = tk.Frame(self, bg={THEME.BG},highlightthickness=0)
        self.content.pack(expand=1, fill=tk.BOTH)

        # Create a menu bar
        self.menu_bar = tk.Menu(self.content)
        self.config(menu=self.content.menu_bar)

        # Add View menu
        file_menu = tk.Menu(self.content.menu_bar, tearoff=0)
        self.content.menu_bar.add_cascade(label="View", menu=file_menu)
        file_menu.add_command(label="Add User", command=self.show_add_user_page)
        file_menu.add_command(label="Manage User", command=self.show_manage_user_page)
        file_menu.add_command(label="Add Password", command=self.show_add_password_page)
        self.content.menu_bar.add_command(label="Get Password", command=self.handle_get_password)

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
        self.content.menu_bar.entryconfig("Get Password", state="disabled")
    
    def on_get_password_end(self):
        self.content.menu_bar.entryconfig("Get Password", state="normal")

    def _show_page(self, page_name):
        for name, page in self.pages.items():
            if name == page_name:
                page.tkraise()
                page.on_page_reload()
                self.title(self.titles[page_name])  # Update the window title
            else:
                page.lower()
      
def dummy_action():
    pass
        
def create_custom_menu_bar(parent_frame):
    menu_bar_frame = tk.Frame(parent_frame, bg="lightgray", height=30)
    menu_bar_frame.pack(side=tk.TOP, fill=tk.X)
    
    file_menu_label = tk.Label(menu_bar_frame, text="File", bg="lightgray", padx=10)
    file_menu_label.pack(side=tk.LEFT)
    
    edit_menu_label = tk.Label(menu_bar_frame, text="Edit", bg="lightgray", padx=10)
    edit_menu_label.pack(side=tk.LEFT)
    
    help_menu_label = tk.Label(menu_bar_frame, text="Help", bg="lightgray", padx=10)
    help_menu_label.pack(side=tk.LEFT)

    file_menu_label.bind("<Button-1>", lambda e: dummy_action())
    edit_menu_label.bind("<Button-1>", lambda e: dummy_action())
    help_menu_label.bind("<Button-1>", lambda e: dummy_action())

    return menu_bar_frame
      
def GuiApp():
    """ Main function for the GUI application
    """
    app = BaseGui(
        resizable = (False, False),
        icon = get_path_to_icon()
    )
    
    content = app.get_content()
    app.set_background_color(THEME.BG)
    
    # Create a menu bar
    menu_bar = CustomMenuBar(content)
    menu_bar.pack(side=tk.TOP, fill=tk.X)
    
    app.run()
    
if __name__ == '__main__':
    raise Exception("This file is not meant to be run on its own. Please run app/main.py instead.")