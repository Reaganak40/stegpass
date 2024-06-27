"""
StegPass - Password Manager Application
user_selector.py - Widget for selecting a user from a list of users
"""

# ? Standard Imports
import tkinter as tk
from tkinter import ttk

# ? Project Imports
from app.utils.user_manager import UserManager
from app.widgets.theme import THEME

class UserSelector(tk.Frame):
    """ Select active user for the application, placed on the menu bar.
    """
    
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self.config(bg=THEME.MENU_BAR_COLOR_BG)
        user_manager = UserManager()
        users = user_manager.get_users()
        
        if len(users) == 0:
            self.user_label = tk.Label(self, text="No Users Found", font=(THEME.FONT, 8, "italic"), fg='grey', bg=THEME.MENU_BAR_COLOR_BG)
            self.user_label.pack(side=tk.LEFT, padx=0)
            self.user_combobox = None
        else:
            self.user_label = tk.Label(self, text="User", font=(THEME.FONT, 8), fg='black', bg=THEME.MENU_BAR_COLOR_BG)
            self.user_label.pack(side=tk.LEFT, padx=0)
            
            # Combobox to select a user
            self.user_combobox = ttk.Combobox(self, values=users, font=(THEME.FONT, 8), state='readonly', takefocus=False, width=15,
                                              height=8, background=THEME.MENU_BAR_COLOR_BG)
            self.user_combobox.pack(side=tk.LEFT, padx=(3, 0))
            self.user_combobox.set(users[0])
            user_manager.set_active_user(users[0])
            
            # Bind an event to remove focus after selection
            self.user_combobox.bind("<<ComboboxSelected>>", self.on_user_selected)
            self.user_combobox.bind("<FocusIn>", self.defocus)
            
        user_manager.add_listener_on_add_user('user_selector', lambda e: self.on_user_added())
    
    def on_user_selected(self, event):
        UserManager().set_active_user(event.widget.get())
        self.master.focus()

    def defocus(self, event):
        event.widget.master.focus_set()
    
    def on_user_added(self):
        """ Callback when a new user is added
        """
        user_manager = UserManager()
        users = user_manager.get_users()
        
        if self.user_combobox is None:
            self.user_label.config(text="User", fg='black', font=(THEME.FONT, 8))
            self.user_combobox = ttk.Combobox(self, values=users, font=(THEME.FONT, 8), state='readonly', takefocus=False, width=15,
                                              height=8, background=THEME.MENU_BAR_COLOR_BG)
            self.user_combobox.pack(side=tk.LEFT, padx=(3, 0))
            self.user_combobox.set(users[0])
            user_manager.set_active_user(users[0])
            
            # Bind an event to remove focus after selection
            self.user_combobox.bind("<<ComboboxSelected>>", self.on_user_selected)
            self.user_combobox.bind("<FocusIn>", self.defocus)
            
        else:
            curr_user = self.user_combobox.get()
            self.user_combobox.config(values=users)
            self.user_combobox.set(curr_user)
        
            
            