"""
StegPass - Password Manager Application
add_password.py - Gui form for adding a password
"""

# ? Standard Imports
import os
import tkinter as tk
import random
import string
from tkinter import ttk, filedialog

# ? Project Imports
from app.widgets.drag_n_drop import DragDropWidget
from app.widgets.theme import THEME
from app.utils.user_manager import UserManager
from app.utils.password_creator import PasswordCreator

class AddPasswordWindow(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        # Set the background color of the main window
        self.config(bg=THEME.BG, padx=THEME.WINDOW_PADDING, pady=THEME.WINDOW_PADDING)
        
        # Define frames for page layout
        left_column = tk.Frame(self, bg=THEME.BG)
        right_column = tk.Frame(self, bg=THEME.BG)
        
        left_column.pack(fill='both', expand=True, side='left')
        right_column.pack(fill='both', expand=True, side='right')
        
        # Label to show the image path
        max_column_width = (THEME.WIDTH - (2 * THEME.WINDOW_PADDING)) // 2
        self.path_label = tk.Label(right_column, text="Image Source: No image loaded", pady=10, font=(THEME.FONT, 10), bg=THEME.BG, fg=THEME.TEXT_COLOR, wraplength=max_column_width-20)
        self.path_label.pack(side="top", pady=(THEME.WINDOW_PADDING, 0), anchor='nw')
        self.original_path_to_image = None
        
        # Display the drag and drop widget at the top, taking up 50% of the height and 50% of the width, x-centered
        self.drag_drop_widget = DragDropWidget(right_column)
        self.drag_drop_widget.pack(fill='x', expand=True, side='top', anchor='n')
        self.drag_drop_widget.set_on_load_image_listener(self.update_image_path)
        
        # needed padding to align the password entry with the drag drop widget
        y_padding_aligned = THEME.WINDOW_PADDING + self.path_label.winfo_reqheight()
        
        # Entry to enter the password
        self.password_entry = tk.Entry(left_column, width=30, fg='grey', font=(THEME.FONT, 10))
        self.password_entry.pack(side='top', pady=(y_padding_aligned, 5), anchor='w')
        self.password_entry.insert(0, "Enter Password")
        self.password_entry.bind("<FocusIn>", self.clear_password_entry)
        self.password_entry.bind("<FocusOut>", self.restore_password_entry)

        # Entry to confirm the password
        self.confirm_password_entry = tk.Entry(left_column, width=30, fg='grey', font=(THEME.FONT, 10))
        self.confirm_password_entry.pack(side='top', pady=5, anchor='w')
        self.confirm_password_entry.insert(0, "Confirm Password")
        self.confirm_password_entry.bind("<FocusIn>", self.clear_confirm_password_entry)
        self.confirm_password_entry.bind("<FocusOut>", self.restore_confirm_password_entry)
        
        # Autogenerate password checkbox
        self.auto_gen_var = tk.IntVar()
        self.auto_gen_checkbox = tk.Checkbutton(left_column, text="Autogenerate Password", variable=self.auto_gen_var, command=self.toggle_autogen_password,
                                                font=(THEME.FONT, 10), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.auto_gen_checkbox.pack(side='top', pady=5, anchor='w')

        # Show password checkbox
        self.show_password_var = tk.IntVar()
        self.show_password_checkbox = tk.Checkbutton(left_column, text="Show Password", variable=self.show_password_var, command=self.toggle_show_password,
                                                     font=(THEME.FONT, 10), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                    activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.show_password_checkbox.pack(side='top', pady=2, anchor='w')
        
        # Checkbox to save to the password folder
        spacing_to_bottom = 120
        self.save_to_password_folder_var = tk.IntVar()
        self.save_to_password_folder_checkbox = tk.Checkbutton(left_column, text="Save to Password Folder", variable=self.save_to_password_folder_var, 
                                                 font=(THEME.FONT, 10), bg=THEME.BG, fg=THEME.TEXT_COLOR, selectcolor=THEME.BG,
                                                 activebackground=THEME.BG, activeforeground=THEME.TEXT_COLOR)
        self.save_to_password_folder_checkbox.pack(side='top', pady=(2, spacing_to_bottom), anchor='w')
        
        # Button to save the password, centered in the bottom of the main frame
        self.add_password_button = tk.Button(left_column, text="Add Password", command=self.save_password, font=(THEME.FONT, 10), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.add_password_button.pack(side='top', pady=10)
        
        # Disable button
        self.add_password_button.bind("<Enter>", lambda e: self.add_password_button.config(bg=THEME.PRIMARY_COLOR_LIGHTEN))
        self.add_password_button.bind("<Leave>", lambda e: self.add_password_button.config(bg=THEME.PRIMARY_COLOR))
        
        # info label to let the user know which user the password will be created for
        user_manager = UserManager()
        active_user = user_manager.get_active_user()
        self.info_label = tk.Label(left_column, text=f"* Password will be added for user '{active_user}'", font=(THEME.FONT, 8), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        self.info_label.pack(side='top', pady=(5,2), anchor='w')
        user_manager.add_active_user_changed_listener('add_password', lambda e: self.info_label.config(text=f"* Password will be added for user '{e}'"))
        
        # error label if the form is not ready to save
        self.error_label = tk.Label(left_column, text="", font=(THEME.FONT, 8), bg=THEME.BG, fg='red')
        self.error_label.pack(side='top', pady=0, anchor='w')
        
        # Bind the resize event to the frame
        right_column.bind("<Configure>", lambda e: self.drag_drop_widget.config(height=e.width))
        

    def check_if_ready_to_save(self):
        """ Check if the form is ready to save

        Returns:
            str: An error message if the form is not ready to save
        """
        if self.original_path_to_image is None:
            return "* Please select an image"
        
        if self.password_entry.get() == "Enter Password" and self.password_entry.cget("fg") == "grey":
            return "* Please enter a password"
        
        if self.confirm_password_entry.get() == "Confirm Password" and self.confirm_password_entry.cget("fg") == "grey":
            return "* Please confirm the password"
        
        if self.password_entry.get() != self.confirm_password_entry.get():
            return "* Passwords do not match"
        
        # check for password requirements
        password = self.password_entry.get()
        
        if len(password) < 6:
            return "* Password must be at least 6 characters long"
        
        if '\0' in password:
            return "* Password cannot contain null characters"    
        
        return ""
            
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
            
    def update_image_path(self, file_path):
        """ Update the image name label with the file path

        Args:
            file_path (str): The path to the image file
        """
        if len(file_path) == 0:
            self.path_label.config(text="Image Source: No image loaded")
            self.original_path_to_image = None
            return
        
        # Truncate the file path if necessary
        truncated_path = self._truncate_path(file_path)
        self.path_label.config(text=f"Path: {truncated_path}")
        self.original_path_to_image = file_path
        
    def reset_image(self):
        self.drag_drop_widget.clear_image()
        
    def clear_password_entry(self, event):
        """ Clear the password entry when the user clicks on it
        """
        if self.password_entry.get() == "Enter Password":
            self.password_entry.delete(0, tk.END)
            if self.show_password_var.get():
                self.password_entry.config(fg='black', show='')
            else:
                self.password_entry.config(fg='black', show='*')

    def restore_password_entry(self, event):
        """ Restore the password entry when the user clicks away from it
        """
        if not self.password_entry.get():
            self.password_entry.insert(0, "Enter Password")
            self.password_entry.config(fg='grey', show='')

    def clear_confirm_password_entry(self, event):
        """ Clear the confirm password entry when the user clicks on it
        """
        if self.confirm_password_entry.get() == "Confirm Password":
            self.confirm_password_entry.delete(0, tk.END)
            
            if self.show_password_var.get():
                self.confirm_password_entry.config(fg='black', show='')
            else:
                self.confirm_password_entry.config(fg='black', show='*')

    def restore_confirm_password_entry(self, event):
        """ Restore the confirm password entry when the user clicks away from it
        """
        if not self.confirm_password_entry.get():
            self.confirm_password_entry.insert(0, "Confirm Password")
            self.confirm_password_entry.config(fg='grey', show='')
            
    def toggle_autogen_password(self):
        """ Toggle the autogenerate password checkbox
        """
        
        if self.auto_gen_var.get():
            length_of_password = random.randint(16, 24)
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
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        pass
        
    def clear_form(self):
        
        # will generate password or reset the password entry
        self.toggle_autogen_password()
        
        # clear the image (should call the listener to clear the path label)
        self.reset_image()
        
        self.error_label.config(text="")
        
        self.master.focus_set()
        
    
    def save_password(self) -> bool:
        """ Save the password to the password folder or a specified location

        Returns:
            bool: True if the password was saved successfully, False otherwise
        """
        error = self.check_if_ready_to_save()
        if error:
            self.error_label.config(text=error)
            return False
        
        password = self.password_entry.get()
        save_to_password_folder = self.save_to_password_folder_var.get()

        user_manager = UserManager()
        username = user_manager.get_active_user()
        
        # open file dialog if save to password folder is not checked
        if not save_to_password_folder:
            image_name = self._get_image_name()
            path_to_save = filedialog.asksaveasfilename(defaultextension=".bmp", filetypes=[("BMP files", "*.bmp")], initialfile=image_name)
        else:
            path_to_save = user_manager.get_password_folder_path(username)
            
        if len(path_to_save) == 0:
            return False
        
        # save password
        PasswordCreator().store_password(username, password, self.original_path_to_image, path_to_save)
        
        # clear the form
        self.clear_form()
        
        return True