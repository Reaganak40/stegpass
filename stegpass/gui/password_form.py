import tkinter as tk
from tkinter import ttk

from gui.theme import THEME
from utils.user_manager import UserManager

class PasswordForm(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self.config(bd=2, relief=tk.FLAT, padx=10, pady=10, bg=THEME.BG)
        self.pack_propagate(False)  # Prevent the frame from resizing to fit its content

        # Label to show the image path
        self.path_label = tk.Label(self, text="Image Source: No image loaded", pady=10, font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        self.path_label.pack(anchor="w")
        
        # Get the list of users
        users = UserManager().get_users()
        
        # Frame to group the user label and combobox
        user_frame = tk.Frame(self)
        user_frame.config(bg=THEME.BG)
        user_frame.pack(pady=5, anchor='w')

        # Label for the user combobox
        user_label = tk.Label(user_frame, text="User:", font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        user_label.pack(side=tk.LEFT)

        # Combobox to select a user
        self.user_combobox = ttk.Combobox(user_frame, values=users, font=(THEME.FONT, 12), state='readonly', takefocus=False)
        self.user_combobox.pack(side=tk.LEFT, padx=5)
        self.user_combobox.set("Not Selected")
        
        # Bind an event to remove focus after selection
        self.user_combobox.bind("<<ComboboxSelected>>", self.remove_focus)

        # Entry to enter the password
        self.password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.password_entry.pack(pady=5, anchor='w')
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.bind("<FocusIn>", self.clear_password_entry)
        self.password_entry.bind("<FocusOut>", self.restore_password_entry)

        # Entry to confirm the password
        self.confirm_password_entry = tk.Entry(self, width=30, fg='grey', font=(THEME.FONT, 12))
        self.confirm_password_entry.pack(pady=5, anchor='w')
        self.confirm_password_entry.insert(0, "Confirm Password")
        self.confirm_password_entry.bind("<FocusIn>", self.clear_confirm_password_entry)
        self.confirm_password_entry.bind("<FocusOut>", self.restore_confirm_password_entry)

        # Checkbox to save to the password folder
        self.save_to_password_folder_var = tk.IntVar()
        self.save_to_password_folder_checkbox = tk.Checkbutton(self, text="Save to Password Folder", variable=self.save_to_password_folder_var, 
                                                 font=('Arial', 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                 activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.save_to_password_folder_checkbox.pack(pady=5, anchor='w')

        # Spacer frame to push the button to the bottom
        spacer_frame = tk.Frame(self)
        spacer_frame.pack(expand=True)

        # Button to save the password
        self.save_button = tk.Button(self, text="Save Password", command=self.save_password, font=(THEME.FONT, 12), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.save_button.pack(pady=10, anchor='s')
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        users = UserManager().get_users()
        self.user_combobox['values'] = users
        self.user_combobox.set("Not Selected")
        
        self.password_entry.delete(0, tk.END)
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.config(fg='grey', show='')
        
        self.confirm_password_entry.delete(0, tk.END)
        self.confirm_password_entry.insert(0, "Confirm Password")
        self.confirm_password_entry.config(fg='grey', show='')
        
        self.save_to_password_folder_var.set(0)
        
    def remove_focus(self, event):
        self.master.focus()

    def clear_password_entry(self, event):
        if self.password_entry.get() == "Enter Password":
            self.password_entry.delete(0, tk.END)
            self.password_entry.config(fg='black', show='*')

    def restore_password_entry(self, event):
        if not self.password_entry.get():
            self.password_entry.insert(0, "Enter Password")
            self.password_entry.config(fg='grey', show='')

    def clear_confirm_password_entry(self, event):
        if self.confirm_password_entry.get() == "Confirm Password":
            self.confirm_password_entry.delete(0, tk.END)
            self.confirm_password_entry.config(fg='black', show='*')

    def restore_confirm_password_entry(self, event):
        if not self.confirm_password_entry.get():
            self.confirm_password_entry.insert(0, "Confirm Password")
            self.confirm_password_entry.config(fg='grey', show='')

    def update_image_path(self, file_path):
        self.path_label.config(text=f"Image Source: {file_path}")

    def save_password(self):
        password = self.password_entry.get()
        confirm_password = self.confirm_password_entry.get()
        save_as_copy = self.save_to_password_folder_var.get()

        if password == confirm_password:
            # Implement your password saving logic here
            print(f"Password saved. Save as copy: {'Yes' if save_as_copy else 'No'}")
        else:
            print("Passwords do not match.")