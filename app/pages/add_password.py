import os
import tkinter as tk

from gui.drag_n_drop import DragDropWidget
from gui.password_form import PasswordForm
from gui.theme import THEME

class AddPasswordWindow(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)

        half_height = (THEME.HEIGHT - (3 * THEME.WINDOW_PADDING)) // 2  # Adjust for padding
        half_width = (THEME.WIDTH - (3 * THEME.WINDOW_PADDING)) // 2

        # Set the background color of the main window
        self.config(bg=THEME.BG)

        # Display the drag and drop widget at the top, taking up 50% of the height and 50% of the width, x-centered
        self.drag_drop_widget = DragDropWidget(self)
        self.drag_drop_widget.place(x=THEME.WINDOW_PADDING + half_width / 2, y=THEME.WINDOW_PADDING, width=half_width, height=half_height)

        # Show the PasswordForm below the drag and drop widget
        self.password_form = PasswordForm(self, bg='lightgrey')
        self.password_form.place(x=THEME.WINDOW_PADDING, y=half_height + (2 * THEME.WINDOW_PADDING), width=THEME.WIDTH - (2 * THEME.WINDOW_PADDING), height=half_height)

    def update_image_name(self, file_path):
        self.password_form.update_image_path(file_path)
        
    def reset_image(self):
        self.drag_drop_widget.clear_image()
        
    def on_page_reload(self):
        """ Called by master when this page is (re)loaded
        """
        self.password_form.on_page_reload()