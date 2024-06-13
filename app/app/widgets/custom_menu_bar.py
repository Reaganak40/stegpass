import tkinter as tk

def create_cascade_menu(parent, menu_items):
    popup = tk.Menu(parent, tearoff=0)
    
    #Adding Menu Items
    for item in menu_items:
        popup.add_command(label=item)
    
    return popup

def on_menu_click(event, menu):
    # display the popup menu
   try:
      menu.tk_popup(event.x_root, event.y_root, 0)
   finally:
      #Release the grab
      menu.grab_release()

class CustomMenuBar(tk.Frame):
    
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        self.config(bg="lightgray", height=30)
        
        file_menu_label = tk.Label(self, text="File", bg="lightgray", padx=10)
        file_menu_label.pack(side=tk.LEFT)
        
        edit_menu_label = tk.Label(self, text="Edit", bg="lightgray", padx=10)
        edit_menu_label.pack(side=tk.LEFT)
        
        help_menu_label = tk.Label(self, text="Help", bg="lightgray", padx=10)
        help_menu_label.pack(side=tk.LEFT)
        
        file_menu_label.bind("<Enter>", lambda e: file_menu_label.config(bg="gray"))
        file_menu_label.bind("<Leave>", lambda e: file_menu_label.config(bg="lightgray"))
        
        edit_menu_label.bind("<Enter>", lambda e: edit_menu_label.config(bg="gray"))
        edit_menu_label.bind("<Leave>", lambda e: edit_menu_label.config(bg="lightgray"))
        
        help_menu_label.bind("<Enter>", lambda e: help_menu_label.config(bg="gray"))
        help_menu_label.bind("<Leave>", lambda e: help_menu_label.config(bg="lightgray"))
        
        file_menu = create_cascade_menu(self, ["New", "Open", "Save", "Exit"])
        file_menu_label.bind("<Button-1>", lambda e: on_menu_click(e, file_menu))
        