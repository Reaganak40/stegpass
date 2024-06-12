# ? Standard Imports
import random
import string
import tkinter as tk
from tkinter import ttk, filedialog

# Project Imports
from app.widgets.theme import THEME
from app.utils.user_manager import UserManager
from app.utils.password_creator import PasswordCreator

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
        self.original_path_to_image = None
        
        # Checkbox to save to the password folder
        self.save_to_password_folder_var = tk.IntVar()
        self.save_to_password_folder_checkbox = tk.Checkbutton(right_frame, text="Save to Password Folder", variable=self.save_to_password_folder_var, 
                                                 font=('Arial', 12), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                 activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.save_to_password_folder_checkbox.pack(pady=5, anchor='w')
        
        
        # Frame to group the user label and combobox
        user_frame = tk.Frame(left_frame)
        user_frame.config(bg=THEME.BG)
        user_frame.pack(pady=5, anchor='w')

        # Label for the user combobox
        user_label = tk.Label(user_frame, text="User:", font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        user_label.pack(side=tk.LEFT)
        
        # Get the list of users
        users = UserManager().get_users()
        
        # Combobox to select a user
        self.user_combobox = ttk.Combobox(user_frame, values=users, font=(THEME.FONT, 12), state='readonly', takefocus=False, width=22)
        self.user_combobox.pack(side=tk.LEFT, padx=5)
        
        # Bind an event to remove focus after selection
        self.user_combobox.bind("<<ComboboxSelected>>", self.on_user_selected)
        
        # Entry to enter the password
        self.password_entry = tk.Entry(left_frame, width=30, fg='grey', font=(THEME.FONT, 12))
        self.password_entry.pack(pady=5, anchor='w')
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.bind("<FocusIn>", self.clear_password_entry)
        self.password_entry.bind("<FocusOut>", self.restore_password_entry)
        self.password_entry.bind("<KeyRelease>", lambda e: self.check_if_ready_to_save())

        # Entry to confirm the password
        self.confirm_password_entry = tk.Entry(left_frame, width=30, fg='grey', font=(THEME.FONT, 12))
        self.confirm_password_entry.pack(pady=5, anchor='w')
        self.confirm_password_entry.insert(0, "Confirm Password")
        self.confirm_password_entry.bind("<FocusIn>", self.clear_confirm_password_entry)
        self.confirm_password_entry.bind("<FocusOut>", self.restore_confirm_password_entry)
        self.confirm_password_entry.bind("<KeyRelease>", lambda e: self.check_if_ready_to_save())
        
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
        
        # Disable button
        self.save_button.config(state='disabled')
        
    def clear_form(self):
        
        # will generate password or reset the password entry
        self.toggle_autogen_password()
        
        self.master.reset_image()
        self.update_image_path("")
        
        self.save_button.config(state='disabled')
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.set_default_user_combobox()
        self.clear_form()
        
    def set_default_user_combobox(self):
        """ Set the default value for the user combobox when the page is (re)loaded
        """
        users = UserManager().get_users()
        self.user_combobox.config(values=users)
        
        if len(users) == 0:
            self.user_combobox.set("No Users Found")
            self.user_combobox.config(state='disabled')
        elif len(users) == 1:
            self.user_combobox.set(users[0])
            self.user_combobox.config(state='disabled')
        else:
            self.user_combobox.set("Not Selected")
            self.user_combobox.config(state='readonly')
        
    def check_if_ready_to_save(self):
        if  self.user_combobox.get() != "Not Selected" and\
            self.user_combobox.get() != "No Users Found" and\
            self.password_entry.get() != "Enter Password" and\
            len(self.password_entry.get()) != 0 and\
            self.confirm_password_entry.get() != "Confirm Password" and\
            len(self.confirm_password_entry.get()) != 0 and\
            self.original_path_to_image is not None:
            self.save_button.config(state='normal')
        else:
            self.save_button.config(state='disabled')
            
    def on_user_selected(self, event):
        self.check_if_ready_to_save()
        self.remove_focus(event)
        
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
        if len(file_path) == 0:
            self.path_label.config(text="Image Source: No image loaded")
            self.original_path_to_image = None
            return
        
        # Truncate the file path if necessary
        truncated_path = self._truncate_path(file_path)
        self.path_label.config(text=f"Path: {truncated_path}")
        self.original_path_to_image = file_path
        self.check_if_ready_to_save()

    def _get_image_name(self):
        if self.original_path_to_image:
            return self.original_path_to_image.split('/')[-1]
        else:
            return None
        
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
            
            # set to normal to allow editing
            self.password_entry.config(state='normal')
            self.confirm_password_entry.config(state='normal')
            
            # clear the entries and insert the generated password
            self.password_entry.delete(0, tk.END)
            self.confirm_password_entry.delete(0, tk.END)
            
            
            # insert the generated password
            self.password_entry.insert(0, auto_password)
            self.confirm_password_entry.insert(0, auto_password)
            
            # disable the entries
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
        self.check_if_ready_to_save()

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
        self.check_if_ready_to_save()

    def save_password(self) -> bool:
        password = self.password_entry.get()
        confirm_password = self.confirm_password_entry.get()
        save_to_password_folder = self.save_to_password_folder_var.get()

        if password != confirm_password:
            print("Passwords do not match.")
            return False
            
        user_manager = UserManager()
        username = self.user_combobox.get()
        
        # open file dialog if save to password folder is not checked
        if not save_to_password_folder:
            image_name = self._get_image_name()
            path_to_save = filedialog.asksaveasfilename(defaultextension=".bmp", filetypes=[("BMP files", "*.bmp")], initialfile=image_name)
        else:
            path_to_save = user_manager.get_password_folder_path(username)
        
        # save password
        PasswordCreator().store_password(username, password, self.original_path_to_image, path_to_save)
        
        # clear the form
        self.clear_form()
        
        return True
            