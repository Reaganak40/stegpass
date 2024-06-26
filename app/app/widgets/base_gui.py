
# ? Standard imports
from tkinter import *
from ctypes import windll
from tkinterdnd2 import TkinterDnD
from PIL import Image, ImageTk

# ? Project imports
from app.utils.utils import resize_image
from app.widgets.theme import THEME

# Credit to: https://github.com/Terranova-Python/Tkinter-Menu-Bar/blob/main/main.py

LGRAY = '#3e4042' # button color effects in the title bar (Hex color)
DGRAY = '#25292e' # window background color               (Hex color)
RGRAY = '#10121f' # title bar color                       (Hex color)

class BaseGui:
    """ Universal main window class for all windows in the application.
    """
    def __init__(self, **kwargs):
        # Set DPI awareness (makes the window look better on high resolution screens)
        try:
            from ctypes import windll
            windll.shcore.SetProcessDpiAwareness(1)
        except:
            pass
        
        self.root = TkinterDnD.Tk()
        self.root.overrideredirect(True)
        
        self.root.minimized = False # only to know if root is minimized
        self.root.maximized = False # only to know if root is maximized
        
        # Set window title
        title = kwargs.get("title", "StegPass")
        self.root.title(title)
        
        # Set window size
        if "size" in kwargs:
            self._center_geometry(kwargs["size"][0], kwargs["size"][1])
        else:
            self._center_geometry(THEME.WIDTH, THEME.HEIGHT)
            
        # Set window resizable
        self.resizable = kwargs.get("resizable", (True, True))
            
        # Implement custom title bar
        self.title_bar = Frame(self.root, bg=RGRAY, relief='raised', bd=0,highlightthickness=0)
        
        # put a close button on the title bar
        self.close_button = Button(self.title_bar, text='  ×  ', command=self.root.destroy,bg=RGRAY,padx=2,pady=2,font=("calibri", 13),bd=0,fg='white',highlightthickness=0)
        self.expand_button = Button(self.title_bar, text=' 🗖 ', command=self.maximize_me,bg=RGRAY,padx=2,pady=2,bd=0,fg='white',font=("calibri", 13),highlightthickness=0)
        self.minimize_button = Button(self.title_bar, text=' 🗕 ',command=self.minimize_me,bg=RGRAY,padx=2,pady=2,bd=0,fg='white',font=("calibri", 13),highlightthickness=0)
        self.title_bar_title = Label(self.title_bar, text=title, bg=RGRAY,bd=0,fg='white',font=("helvetica", 10),highlightthickness=0)
        self.title_bar_icon = Label(self.title_bar, bg=RGRAY,highlightthickness=0)
        
        # a frame for the main area of the window, this is where the actual app will go
        self.content = Frame(self.root, bg=DGRAY,highlightthickness=0)
        
        # pack the widgets
        self.title_bar.pack(fill=X)
        self.close_button.pack(side=RIGHT,ipadx=7,ipady=1)
        self.expand_button.pack(side=RIGHT,ipadx=7,ipady=1)
        self.minimize_button.pack(side=RIGHT,ipadx=7,ipady=1)
        self.title_bar_icon.pack(side=LEFT, padx=5)
        self.title_bar_title.pack(side=LEFT, padx=(0, 10))
        self.content.pack(expand=1, fill=BOTH) # replace this with your main Canvas/Frame/etc.
        
        # Bind title bar motion to the move window function
        self.title_bar.bind('<Button-1>', lambda e: self.get_pos(e)) # so you can drag the window from the title bar
        self.title_bar_title.bind('<Button-1>', lambda e: self.get_pos(e)) # so you can drag the window from the title 
        self.title_bar_icon.bind('<Button-1>', lambda e: self.get_pos(e)) # so you can drag the window from the icon

        # button effects in the title bar when hovering over buttons
        self.close_button.bind('<Enter>',lambda e: self.changex_on_hovering(e))
        self.close_button.bind('<Leave>', lambda e: self.returnx_to_normalstate(e))
        self.expand_button.bind('<Enter>', lambda e: self.change_size_on_hovering(e))
        self.expand_button.bind('<Leave>', lambda e: self.return_size_on_hovering(e))
        self.minimize_button.bind('<Enter>', lambda e: self.changem_size_on_hovering(e))
        self.minimize_button.bind('<Leave>', lambda e: self.returnm_size_on_hovering(e))
        
        # resize the window width
        self.resizex_widget = Frame(self.content,bg=DGRAY)
        if self.resizable[0]:
            self.resizex_widget.config(cursor='sb_h_double_arrow')
        
        # TODO: Find a better solution for resizing the window than using a packed widget.
        # currently does not show/work because width is 0, if >0 it will show and ruin the menubar
        self.resizex_widget.pack(side=RIGHT,ipadx=0,fill=Y)
        self.resizex_widget.bind("<B1-Motion>",lambda e: self.resizex(e))
        
        # resize the window height
        self.resizey_widget = Frame(self.content,bg=DGRAY)
        if self.resizable[1]:
            self.resizey_widget.config(cursor='sb_v_double_arrow')
        
        self.resizey_widget.pack(side=BOTTOM,ipadx=2,fill=X)
        self.resizey_widget.bind("<B1-Motion>",lambda e: self.resizey(e))
        
        # Add icon
        self.icon = kwargs.get("icon", None)
        
        if not self.resizable[0] and not self.resizable[1]:
            self.expand_button.config(state=DISABLED)
        
        # some settings
        self.root.bind("<FocusIn>", lambda e: self.deminimize(e)) # to view the window by clicking on the window icon on the taskbar
        self.root.after(10, self.set_appwindow) # to see the icon on the task bar
        self.root.after(15, self.update_icon) # to update the icon of the window
        
    def get_content(self) -> Frame:
        """ Returns the main content frame of the window.

        Returns:
            Frame: The main content frame, where the actual app will go.
        """
        return self.content
        
    def run(self):
        """ Starts the main loop of the application.
        """
        self.root.mainloop()
        
    def set_appwindow(self):
        """ To display the window icon on the taskbar, even when using root.overrideredirect(True).
        Some WindowsOS styles, required for task bar integration.
        """
        
        GWL_EXSTYLE = -20
        WS_EX_APPWINDOW = 0x00040000
        WS_EX_TOOLWINDOW = 0x00000080
        
        # Magic
        hwnd = windll.user32.GetParent(self.root.winfo_id())
        stylew = windll.user32.GetWindowLongW(hwnd, GWL_EXSTYLE)
        stylew = stylew & ~WS_EX_TOOLWINDOW
        stylew = stylew | WS_EX_APPWINDOW
        res = windll.user32.SetWindowLongW(hwnd, GWL_EXSTYLE, stylew)
    
        self.root.wm_withdraw()
        self.root.after(10, lambda: self.root.wm_deiconify())
        
    def minimize_me(self):
        """ Minimize the window.
        """
        self.root.attributes("-alpha",0) # so you can't see the window when is minimized
        self.root.minimized = True       

    def deminimize(self, event):
        """ Deminimize the window.

        Args:
            event (any): Event that triggered the deminimize.
        """
        if self.root.minimized:
            self.root.focus() 
        
        self.root.attributes("-alpha",1) # so you can see the window when is not minimized
        if self.root.minimized == True:
            self.root.minimized = False                                   

    def maximize_me(self):
        """ Maximize the window.
        """

        if self.root.maximized == False: # if the window was not maximized
            self.root.normal_size = self.root.geometry()
            self.expand_button.config(text=" 🗗 ")
            self.root.geometry(f"{self.root.winfo_screenwidth()}x{self.root.winfo_screenheight()}+0+0")
            self.root.maximized = not self.root.maximized 
            # now it's maximized
            
        else: # if the window was maximized
            self.expand_button.config(text=" 🗖 ")
            self.root.geometry(self.root.normal_size)
            self.root.maximized = not self.root.maximized
            # now it is not maximized
            
    def changex_on_hovering(self, event):
        self.close_button['bg']='red'
    
    def returnx_to_normalstate(self, event):
        self.close_button['bg']=RGRAY
        

    def change_size_on_hovering(self, event):
        if self.resizable[0] or self.resizable[1]:
            self.expand_button['bg']=LGRAY
        
        
    def return_size_on_hovering(self, event):
        self.expand_button['bg']=RGRAY
        

    def changem_size_on_hovering(self, event):
        self.minimize_button['bg']=LGRAY
        
        
    def returnm_size_on_hovering(self, event):
        self.minimize_button['bg']=RGRAY
        
    def get_pos(self, event):
        """ This is executed when the title bar is clicked to move the window

        Args:
            event (any): Event that triggered the move.
        """
        if self.root.maximized == False:
    
            xwin = self.root.winfo_x()
            ywin = self.root.winfo_y()
            startx = event.x_root
            starty = event.y_root

            ywin = ywin - starty
            xwin = xwin - startx
            
            def move_window(event): # runs when window is dragged
                #self.root.config(cursor="fleur")
                self.root.geometry(f'+{event.x_root + xwin}+{event.y_root + ywin}')


            def release_window(event): # runs when window is released
                pass
                #self.root.config(cursor="arrow")
                
            self.title_bar.bind('<B1-Motion>', move_window)
            self.title_bar.bind('<ButtonRelease-1>', release_window)
            
            self.title_bar_title.bind('<B1-Motion>', move_window)
            self.title_bar_title.bind('<ButtonRelease-1>', release_window)
            
            self.title_bar_icon.bind('<B1-Motion>', move_window)
            self.title_bar_icon.bind('<ButtonRelease-1>', release_window)
            
        else:
            self.expand_button.config(text=" 🗖 ")
            self.root.maximized = not self.root.maximized
    
    def resizex(self, event):
        if self.resizable[0] == False:
            return
        
        xwin = self.root.winfo_x()
        difference = (event.x_root - xwin) - self.root.winfo_width()
        
        if self.root.winfo_width() > 150 : # 150 is the minimum width for the window
            try:
                self.root.geometry(f"{ self.root.winfo_width() + difference }x{ self.root.winfo_height() }")
            except:
                pass
        else:
            if difference > 0: # so the window can't be too small (150x150)
                try:
                    self.root.geometry(f"{ self.root.winfo_width() + difference }x{ self.root.winfo_height() }")
                except:
                    pass
                
        self.resizex_widget.config(bg=DGRAY)
        
    def resizey(self, event):
        if self.resizable[1] == False:
            return
        
        ywin = self.root.winfo_y()
        difference = (event.y_root - ywin) - self.root.winfo_height()

        if self.root.winfo_height() > 150: # 150 is the minimum height for the window
            try:
                self.root.geometry(f"{ self.root.winfo_width()  }x{ self.root.winfo_height() + difference}")
            except:
                pass
        else:
            if difference > 0: # so the window can't be too small (150x150)
                try:
                    self.root.geometry(f"{ self.root.winfo_width()  }x{ self.root.winfo_height() + difference}")
                except:
                    pass

        self.root.resizex_widget.config(bg=DGRAY)
        
    def set_background_color(self, color):
        """ Set the background color of the window.

        Args:
            color (str): The color to set the background to.
        """
        self.content.config(bg=color)
        self.resizex_widget.config(bg=color)
        self.resizey_widget.config(bg=color)
        
    def set_config(self, **kwargs):
        """ Manually set the configuration of the window.
        """
        self.root.config(**kwargs)
        
    def set_title(self, title):
        """ Set the title of the window.

        Args:
            title (str): The title to set.
        """
        self.title_bar_title.config(text=title)
    
    def get_y_offset_for_origin(self):
        """ Get the y-offset to get the top of the window (after the title bar)
        """
        # hardcoded value for the title bar height
        return 45
    
    def center_window(self):
        """ Center the window on the screen.
        """
        # update the window to get the correct height
        self.root.update()
        
        # get the screen resolution
        scr_width, scr_height = self.root.winfo_screenwidth(), self.root.winfo_screenheight()
        
        # get the window resolution
        border_width = self.root.winfo_rootx() - self.root.winfo_x()
        title_height = self.root.winfo_rooty() - self.root.winfo_y()
        win_width = self.root.winfo_width() + border_width + border_width
        win_height = self.root.winfo_height() + title_height + border_width
        
        # calculate the position
        x = (scr_width - win_width) // 2
        y = (scr_height - win_height) // 2
        
        # place the window at the calculated position
        self.root.geometry("+%d+%d" % (x, y))
        
    def _center_geometry(self, width, height):
        """ Centers the window with the given width and height.

        Args:
            width (int): The width of the window.
            height (int): The height of the window.
        """
        # get the screen resolution
        scr_width, scr_height = self.root.winfo_screenwidth(), self.root.winfo_screenheight()
        
        # calculate the centered position offset
        offset_x = (scr_width - width) // 2
        offset_y = (scr_height - height) // 2
        
        # place the window at the calculated position
        self.root.geometry(f"{width}x{height}+{offset_x}+{offset_y}")
        
    def update_icon(self):
        """ Update the icon of the window.
        """
        if self.icon is not None:
            self.root.iconbitmap(self.icon)
            
        # add icon to the title bar
        image = Image.open(self.icon)
        resized_image = resize_image(image, self.get_y_offset_for_origin() - 10)
        
        # keep a reference to the image so it doesn't get garbage collected        
        self.icon_image = ImageTk.PhotoImage(resized_image)
        
        self.title_bar_icon.config(image=self.icon_image)
        
    def quit(self):
        """ Quit the application.
        """
        self.root.quit()