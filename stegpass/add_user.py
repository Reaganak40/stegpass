import os
import tkinter as tk
from tkinterdnd2 import TkinterDnD

from gui.theme import THEME
from gui.add_user_form import AddUserForm

class AddUserWindow(TkinterDnD.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("StegPass - Add User")
        self.geometry(f"{THEME.WIDTH}x{THEME.HEIGHT}")
        self.resizable(False, False)
        
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass

        padding = 10
        
        # Set the background color of the main window
        self.config(bg=THEME.BG)
        
        one_forth_height = (THEME.HEIGHT - (3 * padding)) // 4
        
        # Show the AddUserForm in the center of the window
        self.add_user_form = AddUserForm(self)
        self.add_user_form.place(x=padding, y=one_forth_height, width=THEME.WIDTH - (2 * padding), height=one_forth_height * 2)

if __name__ == "__main__":
    # Set the root directory path
    root_dir  = os.path.dirname(os.path.abspath(__file__))
    os.environ['ROOT_DIR'] = root_dir
    
    app = AddUserWindow()
    app.mainloop()