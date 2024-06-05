import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD
from PIL import Image, ImageTk

from gui.drag_n_drop import DragDropWidget
from gui.password_form import PasswordForm
from gui.theme import THEME

class Application(TkinterDnD.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("Password Manager")
        self.geometry(f"{THEME.WIDTH}x{THEME.HEIGHT}")
        self.resizable(False, False)
        
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass

        padding = 10  # Padding for the borders
        half_height = (THEME.HEIGHT - (3 * padding)) // 2  # Adjust for padding

        # Set the background color of the main window
        self.config(bg=THEME.BG)

        # Display the drag and drop widget at the top, taking up 50% of the height
        drag_drop_widget = DragDropWidget(self)
        drag_drop_widget.place(x=padding, y=padding, width=THEME.WIDTH - (2 * padding), height=half_height)

        # Show the PasswordForm below the drag and drop widget
        self.password_form = PasswordForm(self, bg='lightgrey')
        self.password_form.place(x=padding, y=half_height + (2 * padding), width=THEME.WIDTH - (2 * padding), height=half_height)

    def update_image_name(self, file_path):
        self.password_form.update_image_path(file_path)

if __name__ == "__main__":
    app = Application()
    app.mainloop()