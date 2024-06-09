# ? Standard Imports
import os
import sys
import tkinter as tk
from tkinter import ttk
from tkinterdnd2 import TkinterDnD

# Project Imports
from gui.theme import THEME
from utils.user_manager import UserManager
from utils.utils import sha256_hash, show_error_message

class Application(TkinterDnD.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("StegPass - Login")
        self.geometry(f"{450}x{225}")
        self.resizable(False, False)
        self.config(bg=THEME.BG)
        
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass
        
        # No password hash yet
        self.password_hash = None
        
        # Check for default user
        users = UserManager().get_users()
        self.selected_user = args[0]
        
        # If no default user, use combo box to select user from list
        if self.selected_user is None:
            
            # Combobox to select a user
            self.user_combobox = ttk.Combobox(self, values=users, font=(THEME.FONT, 12), state='readonly', takefocus=False, width=25)
            self.user_combobox.set("Select User")
            
            # Bind an event to remove focus after selection
            self.user_combobox.bind("<<ComboboxSelected>>", self.on_user_selected)
        else:
            
            if self.selected_user not in users:
                show_error_message(f"The user '{self.selected_user}' was not found. Please select a user from the list.")
                exit(-1)
            
            # still use the combobox but disable and show the default user
            self.user_combobox = ttk.Combobox(self, values=[self.selected_user], font=(THEME.FONT, 12), takefocus=False, width=25,
                                              foreground="grey", state='disabled')
            self.user_combobox.set(self.selected_user)
        
        self.user_combobox.pack(padx=5, pady = 20, anchor='s')
        
        # Password entry
        self.password_entry = tk.Entry(self, width=28, fg='grey', font=(THEME.FONT, 12))
        self.password_entry.pack(pady=5, anchor='s')
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.bind("<FocusIn>", self.clear_password_entry)
        self.password_entry.bind("<FocusOut>", self.restore_password_entry)
        self.password_entry.bind("<KeyRelease>", lambda e: self.check_if_ready_to_submit())
        
        # Checkbox to show password
        self.show_password = tk.IntVar()
        self.show_password_checkbox = tk.Checkbutton(self, text="Show Password", variable=self.show_password, command=self.toggle_password_visibility, 
                                                     font=(THEME.FONT, 12), bg=THEME.BG, activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR, selectcolor=THEME.BG, fg=THEME.TEXT_COLOR)
        self.show_password_checkbox.pack(pady = 5, padx=90, anchor='w')
        
        # Incorrect password label (don't show at start)
        self.incorrect_password_label = tk.Label(self, text="** Incorrect Password **", font=(THEME.FONT, 12), fg='red', bg=THEME.BG, disabledforeground=THEME.BG)
        self.incorrect_password_label.pack(pady=5, anchor='s')
        self.incorrect_password_label.config(state='disabled')
        
        # Button to submit (login)
        self.login_button = tk.Button(self, text="Login", command=self.attempt_login, font=(THEME.FONT, 12), width=15, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.login_button.pack(side=tk.BOTTOM, pady=10)
        
        # Disable button
        self.login_button.config(state='disabled')
    
    
    def on_user_selected(self, event):
        self.selected_user = self.user_combobox.get()
        self.check_if_ready_to_submit()
        
    def get_password_hash(self):
        return self.password_hash
    
    def clear_password_entry(self, event = None):
        if self.password_entry.get() == "Enter Password":
            self.password_entry.delete(0, tk.END)
            
            if self.show_password.get() == 1:
                self.password_entry.config(fg='black', show='')
            else:
                self.password_entry.config(fg='black', show='*')
    
    def restore_password_entry(self, event = None):
        if not self.password_entry.get():
            self.password_entry.insert(0, "Enter Password")
            self.password_entry.config(fg='grey', show='')
            
    def toggle_password_visibility(self):
        if self.show_password.get() == 1 or self.password_entry.get() == "Enter Password":
            self.password_entry.config(show='')
        else:
            self.password_entry.config(show='*')
    
    def check_if_ready_to_submit(self):
        if self.password_entry.get() != "Enter Password" and len(self.password_entry.get()) > 0 and self.user_combobox.get() != "Select User":
            self.login_button.config(state='normal')
        else:
            self.login_button.config(state='disabled')
    
    def attempt_login(self):
        if not UserManager().check_password(self.selected_user, self.password_entry.get()):
            self.incorrect_password_label.config(state='normal')
            return

        self.password_hash = sha256_hash(self.password_entry.get())
        self.quit()


def LogInApp(default_user : str) -> str:
    """ Launches the login application
    
    Args:
        default_user (str): The user to log in as. None if no default user is provided.

    Returns:
        str: The hash of the master password. None if the user cancels the login process.
    """
    
    # Set the root directory path
    root_dir  = os.path.dirname(os.path.abspath(__file__))
    os.environ['ROOT_DIR'] = root_dir
    
    app = Application(default_user)
    app.mainloop()
    
    return app.get_password_hash()

if __name__ == '__main__':
    # check for default user in args
    default_user = None
    if len(sys.argv) > 1:
        default_user = sys.argv[1]
    
    print(LogInApp(default_user))