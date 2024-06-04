import tkinter as tk
from tkinterdnd2 import DND_FILES, TkinterDnD
from PIL import Image, ImageTk

class DragDropWidget(tk.Frame):
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        
        # Get the background color from the master widget
        master_bg = master.cget('bg')
        
        # Configure the frame with the master widget's background color
        self.config(bd=2, relief=tk.SUNKEN, bg=master_bg)
        self.pack_propagate(False)
        
        # Create a label to display the instructions with the master widget's background color
        self.instruction_label = tk.Label(self, text="Drag BMP file here", pady=20, bg=master_bg)
        self.instruction_label.pack()

        # Create a label to display the image with the master widget's background color
        self.image_label = tk.Label(self, bg=master_bg)
        self.image_label.pack(expand=True)

        # Register drop target
        self.drop_target_register(DND_FILES)
        self.dnd_bind('<<Drop>>', self.drop)

    def drop(self, event):
        file_path = event.data.strip('{}')  # Strip curly braces for paths with spaces
        if file_path.lower().endswith('.bmp'):
            self.load_image(file_path)
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