# ? Standard Imports
import random
import string
import tkinter as tk
from tkinter import ttk

# Project Imports
from gui.theme import THEME
from utils.user_manager import UserManager

class PasswordForm(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self.times_reloaded = 0
        self.config(bd=2, relief=tk.FLAT, padx=10, pady=10, bg=THEME.BG)
        self.pack_propagate(False)  # Prevent the frame from resizing to fit its content
        
        # Create frames for the left and right columns
        two_columns_frame = tk.Frame(self)
        two_columns_frame.pack(fill=tk.BOTH, expand=True)
        two_columns_frame.config(bg=THEME.BG)
        
        max_column_width = (THEME.WIDTH - (2 * THEME.WINDOW_PADDING)) // 2
        left_frame = tk.Frame(two_columns_frame)
        right_frame = tk.Frame(two_columns_frame, width=max_column_width)

        left_frame.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        right_frame.pack(side=tk.RIGHT, fill=tk.BOTH, expand=False)
        right_frame.pack_propagate(False)  # Prevent the right frame from resizing to fit its content
        
        left_frame.config(bg=THEME.BG)
        right_frame.config(bg=THEME.BG)

        # Label to show the image path
        self.path_label = tk.Label(right_frame, text="Image Source: No image loaded", pady=10, font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, wraplength=max_column_width-20)
        self.path_label.pack(anchor="nw")
        
        # Checkbox to save to the password folder
        self.save_to_password_folder_var = tk.IntVar()
        self.save_to_password_folder_checkbox = tk.Checkbutton(right_frame, text="Save to Password Folder", variable=self.save_to_password_folder_var, 
                                                 font=('Arial', 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                 activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.save_to_password_folder_checkbox.pack(pady=5, anchor='w')
        
        # Get the list of users
        users = UserManager().get_users()
        
        # Frame to group the user label and combobox
        user_frame = tk.Frame(left_frame)
        user_frame.config(bg=THEME.BG)
        user_frame.pack(pady=5, anchor='w')

        # Label for the user combobox
        user_label = tk.Label(user_frame, text="User:", font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        user_label.pack(side=tk.LEFT)
        
        # Combobox to select a user
        self.user_combobox = ttk.Combobox(user_frame, values=users, font=(THEME.FONT, 12), state='readonly', takefocus=False, width=22)
        self.user_combobox.pack(side=tk.LEFT, padx=5)
        self.user_combobox.set("Not Selected")
        
        # Bind an event to remove focus after selection
        self.user_combobox.bind("<<ComboboxSelected>>", self.remove_focus)
        
        # Entry to enter the password
        self.password_entry = tk.Entry(left_frame, width=30, fg='grey', font=(THEME.FONT, 12))
        self.password_entry.pack(pady=5, anchor='w')
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.bind("<FocusIn>", self.clear_password_entry)
        self.password_entry.bind("<FocusOut>", self.restore_password_entry)

        # Entry to confirm the password
        self.confirm_password_entry = tk.Entry(left_frame, width=30, fg='grey', font=(THEME.FONT, 12))
        self.confirm_password_entry.pack(pady=5, anchor='w')
        self.confirm_password_entry.insert(0, "Confirm Password")
        self.confirm_password_entry.bind("<FocusIn>", self.clear_confirm_password_entry)
        self.confirm_password_entry.bind("<FocusOut>", self.restore_confirm_password_entry)
        
        # Autogenerate password checkbox
        self.auto_gen_var = tk.IntVar()
        self.auto_gen_checkbox = tk.Checkbutton(left_frame, text="Autogenerate Password", variable=self.auto_gen_var, command=self.toggle_autogen_password,
                                                font=('Arial', 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.auto_gen_checkbox.pack(pady=2, anchor='w')

        # Show password checkbox
        self.show_password_var = tk.IntVar()
        self.show_password_checkbox = tk.Checkbutton(left_frame, text="Show Password", variable=self.show_password_var, command=self.toggle_show_password,
                                                     font=('Arial', 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                    activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.show_password_checkbox.pack(pady=2, anchor='w')

        # Spacer frame to push the button to the bottom
        spacer_frame = tk.Frame(self)
        spacer_frame.pack(expand=True, fill=tk.BOTH)
        spacer_frame.config(bg=THEME.BG)
        
        # Button to save the password, centered in the bottom of the main frame
        self.save_button = tk.Button(self, text="Save Password", command=self.save_password, font=(THEME.FONT, 12), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.save_button.pack(side=tk.BOTTOM, pady=10)
        
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
            if self.show_password_var.get():
                self.password_entry.config(fg='black', show='')
            else:
                self.password_entry.config(fg='black', show='*')

    def restore_password_entry(self, event):
        if not self.password_entry.get():
            self.password_entry.insert(0, "Enter Password")
            self.password_entry.config(fg='grey', show='')

    def clear_confirm_password_entry(self, event):
        if self.confirm_password_entry.get() == "Confirm Password":
            self.confirm_password_entry.delete(0, tk.END)
            
            if self.show_password_var.get():
                self.confirm_password_entry.config(fg='black', show='')
            else:
                self.confirm_password_entry.config(fg='black', show='*')

    def restore_confirm_password_entry(self, event):
        if not self.confirm_password_entry.get():
            self.confirm_password_entry.insert(0, "Confirm Password")
            self.confirm_password_entry.config(fg='grey', show='')

    def update_image_path(self, file_path):
        # Truncate the file path if necessary
        truncated_path = self._truncate_path(file_path)
        self.path_label.config(text=f"Path: {truncated_path}")

    def _truncate_path(self, file_path):
        max_length = 40  # Set the max length for display
        if len(file_path) > max_length:
            truncated_path = '...' + file_path[-(max_length-3):]
            # Ensure it truncates at the nearest forward slash
            slash_index = truncated_path.find('/')
            if slash_index != -1:
                truncated_path = '...' + truncated_path[slash_index:]
            return truncated_path
        else:
            return file_path
        
    def toggle_autogen_password(self):
        if self.auto_gen_var.get():
            length_of_password = random.randint(12, 16)
            auto_password = self._generate_password(length_of_password)
            self.password_entry.delete(0, tk.END)
            self.confirm_password_entry.delete(0, tk.END)
            self.password_entry.insert(0, auto_password)
            self.confirm_password_entry.insert(0, auto_password)
            self.password_entry.config(state='disabled')
            self.confirm_password_entry.config(state='disabled')
            
            if self.show_password_var.get():
                self.password_entry.config(show='')
                self.confirm_password_entry.config(show='')
            else:
                self.password_entry.config(show='*')
                self.confirm_password_entry.config(show='*')
        else:
            self.password_entry.config(state='normal')
            self.confirm_password_entry.config(state='normal')
            self.password_entry.delete(0, tk.END)
            self.confirm_password_entry.delete(0, tk.END)
            self.restore_password_entry(None)
            self.restore_confirm_password_entry(None)

    def _generate_password(self, length=12):
        characters = string.ascii_letters + string.digits + string.punctuation
        password = ''.join(random.choice(characters) for _ in range(length))
        return password
    
    def toggle_show_password(self):
        show_password = self.show_password_var.get()
        if show_password or self.confirm_password_entry.get() == "Confirm Password":            
            self.password_entry.config(show='')
            self.confirm_password_entry.config(show='')
        else:
            self.password_entry.config(show='*')
            self.confirm_password_entry.config(show='*')
            
        self.master.focus()

    def save_password(self):
        password = self.password_entry.get()
        confirm_password = self.confirm_password_entry.get()
        save_as_copy = self.save_to_password_folder_var.get()

        if password == confirm_password:
            # Implement your password saving logic here
            print(f"Password saved. Save as copy: {'Yes' if save_as_copy else 'No'}")
        else:
            print("Passwords do not match.")