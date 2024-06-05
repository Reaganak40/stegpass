import tkinter as tk
from tkinter import filedialog
from tkinterdnd2 import DND_FILES, TkinterDnD
from PIL import Image, ImageTk

class DragDropWidget(tk.Frame):
    NO_SELECT_COLOR = "#D3D3D3"
    SELECTED_BG_COLOR = "#1E1E1E"
    
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        # Configure the frame with the master widget's background color
        self.config(bd=2, relief=tk.SUNKEN, bg=self.NO_SELECT_COLOR)
        self.pack_propagate(False)
        
        # Create a label to display the instructions with the master widget's background color
        self.instruction_label = tk.Label(self, text="Drag BMP file here or click to browse", pady=20, bg=self.NO_SELECT_COLOR)
        self.instruction_label.pack()

        # Create a label to display the image with the master widget's background color
        self.image_label = tk.Label(self, bg=self.NO_SELECT_COLOR)
        self.image_label.pack(expand=True)

        # Register drop target
        self.drop_target_register(DND_FILES)
        self.dnd_bind('<<Drop>>', self.drop)

        # Bind left mouse button click to open file dialog
        self.bind("<Button-1>", self.open_file_dialog)
        self.image_label.bind("<Button-1>", self.open_file_dialog)

    def drop(self, event):
        file_path = event.data.strip('{}')  # Strip curly braces for paths with spaces
        if file_path.lower().endswith('.bmp'):
            self.load_image(file_path)
            self.master.update_image_name(file_path)
        else:
            print("Only BMP files are supported.")

    def load_image(self, file_path):
        image = Image.open(file_path)
        
        # Get the widget's size
        widget_width = self.winfo_width()
        widget_height = self.winfo_height()
        
        # Calculate the new size maintaining the aspect ratio
        aspect_ratio = min(widget_width / image.width, widget_height / image.height)
        new_size = (int(image.width * aspect_ratio), int(image.height * aspect_ratio))
        
        # Resize the image
        resized_image = image.resize(new_size, Image.LANCZOS)
        
        # Convert the image to a format Tkinter can display
        photo = ImageTk.PhotoImage(resized_image)
        
        # Update the label with the new image
        self.image_label.config(image=photo)
        self.image_label.image = photo  # Keep a reference to avoid garbage collection
        self.instruction_label.pack_forget()  # Remove instruction label when an image is loaded
        
        # Change background color to black
        self.update_background_color(self.SELECTED_BG_COLOR)
        
    def update_background_color(self, color):
        self.config(bg=color)
        self.image_label.config(bg=color)

    def open_file_dialog(self, event=None):
        file_path = filedialog.askopenfilename(filetypes=[("BMP files", "*.bmp")])
        if file_path:
            self.load_image(file_path)
            self.master.update_image_name(file_path)