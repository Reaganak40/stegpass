import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD
from PIL import Image, ImageTk

from gui.drag_n_drop import DragDropWidget

class THEME:
    BG = "#3e3e42"
    WIDTH = 800
    HEIGHT = 600

class Application(TkinterDnD.Tk):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.title("Drag and Drop BMP Loader")
        self.geometry(f"{THEME.WIDTH}x{THEME.HEIGHT}")
        self.resizable(False, False)

        padding = 10  # Padding for the borders

        # Set the background color of the main window
        self.config(bg=THEME.BG)

        # Show name of file on the left with a background color
        self.image_name_label = tk.Label(self, text="No image loaded", padx=20, pady=20, bg='lightgrey')
        self.image_name_label.place(x=padding, y=padding, width=(THEME.WIDTH // 2) - (2 * padding), height=(THEME.HEIGHT // 2) - (2 * padding))

        # Display the drag and drop widget on the right, taking up 50% of the height
        drag_drop_widget = DragDropWidget(self)
        drag_drop_widget.place(x=(THEME.WIDTH // 2) + padding, y=padding, width=(THEME.WIDTH // 2) - (2 * padding), height=(THEME.HEIGHT // 2) - (2 * padding))

    def update_image_name(self, file_path):
        self.image_name_label.config(text=file_path)

if __name__ == "__main__":
    app = Application()
    app.mainloop()