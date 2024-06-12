import tkinter as tk
from app.widgets.theme import THEME
from app.utils.user_manager import UserManager

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
        self.username_entry.bind("<Return>", self.on_enter)
        
        # Entry to enter the master password
        self.master_password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.master_password_entry.pack(pady=5, anchor='s')
        self.master_password_entry.insert(0, "Master Password")
        self.master_password_entry.bind("<FocusIn>", self.clear_master_password_entry)
        self.master_password_entry.bind("<FocusOut>", self.restore_master_password_entry)
        self.master_password_entry.bind("<Return>", self.on_enter)
        
        # Entry to confirm the master password
        self.confirm_master_password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.confirm_master_password_entry.pack(pady=5, anchor='s')
        self.confirm_master_password_entry.insert(0, "Confirm Master Password")
        self.confirm_master_password_entry.bind("<FocusIn>", self.clear_confirm_master_password_entry)
        self.confirm_master_password_entry.bind("<FocusOut>", self.restore_confirm_master_password_entry)
        self.confirm_master_password_entry.bind("<Return>", self.on_enter)
        
        # Message label to display error messages
        self.message_label = tk.Label(self, text="", font=(THEME.FONT, 12), fg='red', bg=THEME.BG)
        self.message_label.pack(pady=5, anchor='s')
        
        # Spacer frame to push the button to the bottom
        spacer_frame = tk.Frame(self)
        spacer_frame.pack(expand=True)
        
        # Button to add user
        self.add_user_button = tk.Button(self, text="Add User", command=self.add_user, font=(THEME.FONT, 12), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.add_user_button.pack(pady=10, anchor='s')
    
    def on_enter(self, event = None):
        self.add_user()
    
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
            
    def clear_message_label(self):
        self.message_label.config(text="")
        
    def show_error_message(self, message):
        self.message_label.config(text=message, fg='red')
        
    def show_success_message(self, message):
        self.message_label.config(text=message, fg='green')
        
    def add_user(self):
        # check if username is entered
        username = self.username_entry.get()
        if (username == "Username" and self.username_entry.cget('fg') == 'grey') or len(username) == 0:
            self.show_error_message("Please enter a username")
            return
        
        # check if master password is entered
        master_password = self.master_password_entry.get()
        if master_password == "Master Password" and self.master_password_entry.cget('fg') == 'grey' or len(master_password) == 0:
            self.show_error_message("Please enter a master password")
            return
        
        # confirm password matches
        password1 = self.master_password_entry.get()
        password2 = self.confirm_master_password_entry.get()
        
        if password1 != password2:
            self.show_error_message("Passwords do not match")
            return

        user_manager = UserManager()
        if user_manager.add_user(username, master_password):
            self.clear_form()
            self.show_success_message("User added successfully!")
            self.master.focus()
        else:
            self.show_error_message("User already exists!")
    
    def clear_form(self):
        self.username_entry.delete(0, tk.END)
        self.username_entry.insert(0, "Username")
        self.username_entry.config(fg='grey')
        
        self.master_password_entry.delete(0, tk.END)
        self.master_password_entry.insert(0, "Master Password")
        self.master_password_entry.config(fg='grey', show='')

        self.confirm_master_password_entry.delete(0, tk.END)
        self.confirm_master_password_entry.insert(0, "Confirm Master Password")
        self.confirm_master_password_entry.config(fg='grey', show='')
    
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.clear_form()
        self.clear_message_label()
        self.master.focus()
            
        