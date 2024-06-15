"""
StegPass - Password Manager Application
main.py - The main entry point for the application
"""
            
# ? Standard Imports
import os
import sys

# ? Project Imports
from app.gui import GuiApp
from app.login import LogInApp
from app.get_password import get_password, notify_user

from app.utils.config import setup_config
from app.utils.utils import show_error_message


if __name__ == '__main__':
    # Set the root directory path
    root_dir  = os.path.dirname(os.path.abspath(__file__))
    os.environ['ROOT_DIR'] = root_dir
    
    # Set up environment using config
    try:
        setup_config()
    except Exception as e:
        show_error_message(f"An error occurred while setting up the environment: {e}")
        sys.exit(1)
        
    # Launch the application according to the provided arguments
    # If no arguments are provided, launch the main application
    # --login [user]: Launch the login application
    # --password [image_path] [optional:username]: Retrieve a password from an image file
    if len(sys.argv) == 1:
        GuiApp()
        
    elif len(sys.argv) >= 2 and sys.argv[1] == '--login':
        default_user = None
        if len(sys.argv) == 3:
            default_user = sys.argv[2]
        print(LogInApp(default_user))
        
    elif len(sys.argv) >= 3 and sys.argv[1] == '--password':
        if len(sys.argv) == 3:
            exit_code, output = get_password(sys.argv[2])
        elif len(sys.argv) == 4:
            exit_code, output = get_password(sys.argv[2], sys.argv[3])
        
        if exit_code == 0:
            notify_user(output)
        elif exit_code == 4:
            show_error_message("Could not recover password.")
            sys.exit(1)
        else:
            show_error_message(output)
            sys.exit(1)
        
    else:
        show_error_message("Invalid arguments provided. Exiting...")
        sys.exit(1)