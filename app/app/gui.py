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
from app.widgets.menu_bar import MenuBar, PopupMenu

class ManageUserWindow(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        label = tk.Label(self, text="Manage User Page", font=('Arial', 16))
        label.pack(pady=20)
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        pass
      
def GuiApp():
    """ Main function for the GUI application
    """
    app = BaseGui(
        resizable = (False, False),
        icon = get_path_to_icon()
    )
    
    content = app.get_content()
    app.set_background_color(THEME.BG)
    
    def show_add_user_page():
        show_page('add_user')

    def show_manage_user_page():
        show_page('manage_user')

    def show_add_password_page():
        show_page('add_password')
    
    # Create a menu bar
    menu_bar = MenuBar(content)
    menu_bar.pack(side=tk.TOP, fill=tk.X)
    
    # Cascade menu to view pages    
    file_popup = PopupMenu(content)
    file_popup.add_command(label="Add User", command=show_add_user_page)
    file_popup.add_command(label="Manage User", command=show_manage_user_page)
    file_popup.add_command(label="Add Password", command=show_add_password_page)
    menu_bar.AddCascade("View", file_popup) 
    
    # Add button and handlers for getting password
    get_password_button = menu_bar.AddButton("Get Password")
    
    def on_get_password_start():
        get_password_button.config(state="disabled")
    
    def on_get_password_end():
        get_password_button.config(state="normal")
        
    def handle_get_password():
        task_thread = Thread(target=open_password_form, args=(on_get_password_start, on_get_password_end))
        task_thread.start()
    
    get_password_button.bind("<Button-1>", lambda e: handle_get_password())
    
    # Create frames for each page
    pages = {}
    pages['add_user'] = AddUserWindow(content)
    pages['manage_user'] = ManageUserWindow(content)
    pages['add_password'] = AddPasswordWindow(content)
    
    # Titles for each page
    titles = {
        'add_user': 'StegPass - Add User',
        'manage_user': 'StegPass - Manage User',
        'add_password': 'StegPass - Add Password'
    }
    
    # Place each frame in the same location; only one will be visible at a time
    y_offset = menu_bar.default_height()
    
    for page in pages.values():
        page.place(x=0, y=y_offset, width=THEME.WIDTH, height=THEME.HEIGHT-y_offset-app.get_y_offset_for_origin())
        
    def show_page(page_name):
        for name, page in pages.items():
            if name == page_name:
                page.tkraise()
                page.on_page_reload()
                app.set_title(titles[page_name])  # Update the window title
            else:
                page.lower()
        
    # Show the initial page
    show_add_user_page()

    app.run()
    
if __name__ == '__main__':
    raise Exception("This file is not meant to be run on its own. Please run app/main.py instead.")