import tkinter as tk
from gui.theme import THEME
from utils.user_manager import UserManager

class AddUserForm(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self.config(bd=2, relief=tk.FLAT, padx=10, pady=10, bg=THEME.BG)
        self.pack_propagate(False)  # Prevent the frame from resizing to fit its content
        
        # Entry to enter the username
        self.username_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.username_entry.pack(pady=5, anchor='s')
        self.username_entry.insert(0, "Username")
        self.username_entry.bind("<FocusIn>", self.clear_username_entry)
        self.username_entry.bind("<FocusOut>", self.restore_username_entry)
        
        # Entry to enter the master password
        self.master_password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.master_password_entry.pack(pady=5, anchor='s')
        self.master_password_entry.insert(0, "Master Password")
        self.master_password_entry.bind("<FocusIn>", self.clear_master_password_entry)
        self.master_password_entry.bind("<FocusOut>", self.restore_master_password_entry)
        
        # Entry to confirm the master password
        self.confirm_master_password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.confirm_master_password_entry.pack(pady=5, anchor='s')
        self.confirm_master_password_entry.insert(0, "Confirm Master Password")
        self.confirm_master_password_entry.bind("<FocusIn>", self.clear_confirm_master_password_entry)
        self.confirm_master_password_entry.bind("<FocusOut>", self.restore_confirm_master_password_entry)
        
        # Spacer frame to push the button to the bottom
        spacer_frame = tk.Frame(self)
        spacer_frame.pack(expand=True)
        
        # Button to add user
        self.add_user_button = tk.Button(self, text="Add User", command=self.add_user, font=(THEME.FONT, 12), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.add_user_button.pack(pady=10, anchor='s')
        
    def clear_username_entry(self, event = None):
        if self.username_entry.get() == "Username":
            self.username_entry.delete(0, tk.END)
            self.username_entry.config(fg='black')
            
    def restore_username_entry(self, event = None):
        if not self.username_entry.get():
            self.username_entry.insert(0, "Username")
            self.username_entry.config(fg='grey')
            
    def clear_master_password_entry(self, event = None):
        if self.master_password_entry.get() == "Master Password":
            self.master_password_entry.delete(0, tk.END)
            self.master_password_entry.config(fg='black', show='*')
            
    def restore_master_password_entry(self, event = None):
        if not self.master_password_entry.get():
            self.master_password_entry.insert(0, "Master Password")
            self.master_password_entry.config(fg='grey', show='')
            
    def clear_confirm_master_password_entry(self, event = None):
        if self.confirm_master_password_entry.get() == "Confirm Master Password":
            self.confirm_master_password_entry.delete(0, tk.END)
            self.confirm_master_password_entry.config(fg='black', show='*')
        
    def restore_confirm_master_password_entry(self, event = None):
        if not self.confirm_master_password_entry.get():
            self.confirm_master_password_entry.insert(0, "Confirm Master Password")
            self.confirm_master_password_entry.config(fg='grey', show='')
        
    def add_user(self):
        # check if username is entered
        username = self.username_entry.get()
        if username == "Enter Username":
            print("Please enter a username")
            return
        
        # check if master password is entered
        master_password = self.master_password_entry.get()
        if master_password == "Enter Master Password":
            print("Please enter a master password")
            return
        
        # confirm password matches
        password1 = self.master_password_entry.get()
        password2 = self.confirm_master_password_entry.get()
        
        if password1 != password2:
            print("Passwords do not match")
            return

        user_manager = UserManager()
        if not user_manager.add_user(username, master_password):
            print("User already exists")
    
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.username_entry.delete(0, tk.END)
        self.username_entry.insert(0, "Username")
        self.username_entry.config(fg='grey')
        
        self.master_password_entry.delete(0, tk.END)
        self.master_password_entry.insert(0, "Master Password")
        self.master_password_entry.config(fg='grey', show='')
        
        self.confirm_master_password_entry.delete(0, tk.END)
        self.confirm_master_password_entry.insert(0, "Confirm Master Password")
        self.confirm_master_password_entry.config(fg='grey', show='')
        
        self.master.focus()
            
        