import tkinter as tk
from gui.theme import THEME

class PasswordForm(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        self.config(bd=2, relief=tk.FLAT, padx=10, pady=10, bg=THEME.BG)
        self.pack_propagate(False)  # Prevent the frame from resizing to fit its content

        # Label to show the image path
        self.path_label = tk.Label(self, text="Image Source: No image loaded", pady=10, font=(THEME.FONT, 12), bg=THEME.BG, fg=THEME.TEXT_COLOR)
        self.path_label.pack(anchor="w")

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

        # Checkbox to save as a copy
        self.save_copy_var = tk.IntVar()
        self.save_copy_checkbox = tk.Checkbutton(self, text="Save as a copy", variable=self.save_copy_var, font=(THEME.FONT, 12))
        self.save_copy_checkbox.pack(pady=5, anchor='w')

        # Spacer frame to push the button to the bottom
        spacer_frame = tk.Frame(self)
        spacer_frame.pack(expand=True)

        # Button to save the password
        self.save_button = tk.Button(self, text="Save Password", command=self.save_password, font=(THEME.FONT, 12), width=20, height=2, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR)
        self.save_button.pack(pady=10, anchor='s')

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
        save_as_copy = self.save_copy_var.get()

        if password == confirm_password:
            # Implement your password saving logic here
            print(f"Password saved. Save as copy: {'Yes' if save_as_copy else 'No'}")
        else:
            print("Passwords do not match.")