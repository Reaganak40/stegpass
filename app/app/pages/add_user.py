# ? Standard Imports
import os
import tkinter as tk
from tkinter import ttk

# Project Imports
from app.widgets.theme import THEME
from app.widgets.add_user_form import AddUserForm

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
        self.user_added_listeners = []
    
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.add_user_form.on_page_reload()
        
    def add_user_added_listener(self, callback):
        """ Called by the AddUserForm when a user is successfully added
        """
        self.user_added_listeners.append(callback)
        
    def on_user_added(self):
        """ Called by the AddUserForm when a user is successfully added
        """
        for listener in self.user_added_listeners:
            listener()