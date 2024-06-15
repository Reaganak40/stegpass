"""
StegPass - Password Manager Application
login.py - GUI to handle user logins
"""

# ? Standard Imports
import os
import sys
import tkinter as tk
from tkinter import ttk
from tkinterdnd2 import TkinterDnD

# Project Imports
from app.widgets.base_gui import BaseGui
from app.widgets.theme import THEME
from app.utils.user_manager import UserManager
from app.utils.utils import sha256_hash, show_error_message, get_path_to_icon

def LogInApp(default_user : str = None) -> str:
    """ Launches the login application
    
    Args:
        default_user (str): The user to log in as. None if no default user is provided.

    Returns:
        str: The hash of the master password. None if the user cancels the login process.
    """
    app = BaseGui(
        size = (450, 275),
        resizable = (False, False),
        icon = get_path_to_icon(),
        title = "StegPass - Login"
    )
    content = app.get_content()
    content.config(relief='solid', bd=1)    
    
    user_manager = UserManager()
    
    # No password hash yet
    password_hash = None
    
    # Create and configure the select user combobox
    if default_user is None:
        if user_manager.count_users() == 0:
            # NOTE: This is a bug. The user should not be forked to login if there are no users.
            show_error_message("No users found. Please create a user first.")
            return None
        
        # if no default user, use combo box to select user from list
        users = user_manager.get_users()
        user_combobox = ttk.Combobox(content, values=users, font=(THEME.FONT, 10), state='readonly', takefocus=False, width=25)
        user_combobox.set("Select User")
    else:
        if not user_manager.check_user_exists(default_user):
            show_error_message(f"The default user '{default_user}' was not found.")
            return None
        
        # still use the combobox but disable and show the default user
        user_combobox = ttk.Combobox(content, values=[default_user], font=(THEME.FONT, 10), takefocus=False, width=25, foreground="grey", state='disabled')
        user_combobox.set(default_user)
    user_combobox.pack(side='top', padx=5, pady = 20, anchor='s')
    
    # Password entry
    password_entry = tk.Entry(content, width=28, fg='grey', font=(THEME.FONT, 10))
    password_entry.pack(side='top', pady=(0, 5), anchor='s')
    password_entry.insert(0, "Enter Password")
    
    def toggle_password_visibility():
        """ Toggle the visibility of the password in the entry field
        """
        if show_password.get() == 1 or password_entry.get() == "Enter Password":
            password_entry.config(show='')
        else:
            password_entry.config(show='*')
    
    # Checkbox to show password
    show_password = tk.IntVar()
    show_password_checkbox = tk.Checkbutton(content, text="Show Password", variable=show_password, command=toggle_password_visibility, 
                                                    font=(THEME.FONT, 10), bg=THEME.BG, activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR, selectcolor=THEME.BG, fg=THEME.TEXT_COLOR)
    show_password_checkbox.pack(side='top', pady = 5, padx=90, anchor='w')
    
    def clear_password_entry():
        """ Clear the password entry field of default text
        """
        if password_entry.get() == "Enter Password":
            password_entry.delete(0, tk.END)
            
            if show_password.get() == 1:
                password_entry.config(fg='black', show='')
            else:
                password_entry.config(fg='black', show='*')
    
    def restore_password_entry():
        """ Restore the default text to the password entry field
        """
        if not password_entry.get():
            password_entry.insert(0, "Enter Password")
            password_entry.config(fg='grey', show='')
    
    password_entry.bind("<FocusIn>", lambda e: clear_password_entry())
    password_entry.bind("<FocusOut>", lambda e: restore_password_entry())
    
    # Bad login label (don't show at start)
    bad_login_label = tk.Label(content, text="", font=(THEME.FONT, 10), fg='red', bg=THEME.BG)
    bad_login_label.pack(side='top', pady=(0, 5), anchor='s')

    def attempt_login():
        selected_user = user_combobox.get()
        if selected_user == "Select User":
            bad_login_label.config(text="** Select a user to login **")
            return
        
        password = password_entry.get()
        if (password == "Enter Password" and password_entry.cget('fg') == 'grey') or len(password) == 0:
            bad_login_label.config(text="** Enter a password to login **")
            return
        
        if not UserManager().check_password(selected_user, password):
            bad_login_label.config(text="** Incorrect Password **")
            return

        nonlocal password_hash
        password_hash = sha256_hash(password)
        app.quit()
        
    # Button to submit (login)
    login_button = tk.Button(content, text="Login", command=attempt_login, font=(THEME.FONT, 12), width=15, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
    login_button.pack(side=tk.BOTTOM, pady=(0, 15))
    login_button.bind("<Enter>", lambda e: login_button.config(bg=THEME.PRIMARY_COLOR_LIGHTEN))
    login_button.bind("<Leave>", lambda e: login_button.config(bg=THEME.PRIMARY_COLOR))
    
    # Allow pressing enter to login
    password_entry.bind("<Return>", lambda e: attempt_login())
    
    app.run()
    return password_hash

if __name__ == '__main__':
    raise Exception("This file is not meant to be run on its own. Please run app/main.py instead.")