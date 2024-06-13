import os
import tkinter as tk

from app.widgets.drag_n_drop import DragDropWidget
from app.widgets.password_form import PasswordForm
from app.widgets.theme import THEME

class AddPasswordWindow(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        height_of_widget = self.winfo_height()

        half_height = (THEME.HEIGHT - (3 * THEME.WINDOW_PADDING)) // 2  # Adjust for padding
        half_width = (THEME.WIDTH - (3 * THEME.WINDOW_PADDING)) // 2

        # Set the background color of the main window
        self.config(bg=THEME.BG)

        # Display the drag and drop widget at the top, taking up 50% of the height and 50% of the width, x-centered
        self.drag_drop_widget = DragDropWidget(self, width=half_width)
        self.drag_drop_widget.pack_propagate(False)
        self.drag_drop_widget.pack(fill='y', expand=True, side='top', padx=THEME.WINDOW_PADDING, pady=THEME.WINDOW_PADDING)

        # Show the PasswordForm below the drag and drop widget
        self.password_form = PasswordForm(self, bg='lightgrey')
        self.password_form.pack(fill='both', expand=True, side='bottom', padx=THEME.WINDOW_PADDING, pady=(0, THEME.WINDOW_PADDING), anchor='center')

    def update_image_name(self, file_path):
        self.password_form.update_image_path(file_path)
        
    def reset_image(self):
        self.drag_drop_widget.clear_image()
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.password_form.on_page_reload()