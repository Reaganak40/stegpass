
# ? Standard Imports
import tkinter as tk

# ? Project Imports
from app.widgets.theme import THEME
from app.widgets.user_selector import UserSelector
from app.pages.add_user import AddUserWindow

def create_cascade_menu(parent, menu_items):
    popup = tk.Menu(parent, tearoff=0)
    
    #Adding Menu Items
    for item in menu_items:
        popup.add_command(label=item)
    
    return popup
      
class PopupMenu(tk.Menu):
    """ A popup menu that can be added to menu bar for cascading menus.
    """
    
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        self.config(tearoff=0)
        
    def on_menu_click(self, event):
         # Get the absolute position of the widget on the screen
        widget_abs_x = event.widget.winfo_rootx()
        widget_abs_y = event.widget.winfo_rooty()
        
        # Get the width and height of the widget
        widget_width = event.widget.winfo_width()
        widget_height = event.widget.winfo_height()
        
        spawn_pos_x = widget_abs_x + 5
        spawn_pos_y = widget_abs_y + widget_height + 5
        
        try:
            self.tk_popup(spawn_pos_x, spawn_pos_y, 0)
        finally:
            self.grab_release()

class MenuBar(tk.Frame):
    """ A custom menu bar widget, which can be integrated into the title bar or tk.Frame objects.
    """
    
    def __init__(self, master, **kwargs):
        super().__init__(master, **kwargs)
        self.config(bg=THEME.MENU_BAR_COLOR_BG, height=30)
        
        self.user_selector = UserSelector(self)
        self.user_selector.pack(side=tk.RIGHT, padx=5, fill=tk.Y)
        
    def AddButton(self, label_text) -> tk.Label:
        """ Add a button to the menu bar.

        Args:
            label_text (str): The text to display on the button.
        
        Returns:
            tk.Label: The label widget that was created.

        """
        label = tk.Label(self, text=label_text, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR, padx=10, disabledforeground=THEME.PRIMARY_COLOR_LIGHTEN)
        label.pack(side=tk.LEFT)
        
        label.bind("<Enter>", lambda e: label.config(bg=THEME.PRIMARY_COLOR_LIGHTEN))
        label.bind("<Leave>", lambda e: label.config(bg=THEME.PRIMARY_COLOR))
        
        return label

        
    def AddCascade(self, label_text : str, popup_menu : PopupMenu):
        """ Add a cascading menu to the menu bar.

        Args:
            label_text (str): The text to display on the menu.
            popup_menu (PopupMenu): The popup menu object to display when the menu is clicked.
        """
        label = tk.Label(self, text=label_text, bg=THEME.PRIMARY_COLOR, fg=THEME.TEXT_COLOR, padx=10)
        label.pack(side=tk.LEFT)
        
        label.bind("<Enter>", lambda e: label.config(bg=THEME.PRIMARY_COLOR_LIGHTEN))
        label.bind("<Leave>", lambda e: label.config(bg=THEME.PRIMARY_COLOR))
        label.bind("<Button-1>", popup_menu.on_menu_click)
    
    def default_height(self):
        return 30 - 4 # account for the padding on top and bottom