# ? Standard Imports
import os
import tkinter as tk
from tkinter import ttk

# Project Imports
from gui.theme import THEME
from gui.add_user_form import AddUserForm

class AddUserWindow(tk.Frame):
    """ Page for adding a new user to the system
    """
    
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)

        # Set the background color of this page
        self.config(bg=THEME.BG)
        
        one_forth_height = (THEME.HEIGHT - (3 * THEME.WINDOW_PADDING)) // 4
        
        # Show the AddUserForm in the center of the window
        self.add_user_form = AddUserForm(self)
        self.add_user_form.place(x=THEME.WINDOW_PADDING, y=one_forth_height, width=THEME.WIDTH - (2 * THEME.WINDOW_PADDING), height=one_forth_height * 2)
    
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.add_user_form.on_page_reload()