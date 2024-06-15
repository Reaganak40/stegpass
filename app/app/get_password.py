"""
StegPass - Password Manager Application
get_password.py - handles password retrieval from an image file
"""

# ? Standard Imports
import os
import pyperclip
import time
from win10toast import ToastNotifier
import ctypes
from tkinter import filedialog

# ? Project Imports
from app.utils.utils import is_valid_sha256_hash, run_subprocess, fork_to_login, get_path_to_icon, show_error_message
from app.utils.utility_fetcher import TargetType, UtilityFetcher
from app.utils.config import SP_BUILD_TYPE
from app.utils.user_manager import UserManager

def open_password_form(on_start, on_end):
    """ Opens a file dialog to select an image file and retrieves a password from it

    Args:
        on_start (func): The function to call when the password retrieval process starts
        on_end (func): The function to call when the password retrieval process ends
    """
    on_start()
    
    user_manager = UserManager()
    active_user = user_manager.get_active_user()
    if active_user is not None:
        password_folder = user_manager.get_password_folder_path(active_user)
    else:
        password_folder = ""
    
    # Create a file dialog to select an image file
    image_path = filedialog.askopenfilename(defaultextension=".bmp", filetypes=[("BMP files", "*.bmp")], initialdir=password_folder)
    
    if not image_path:
        on_end()
        return
    
    # Try to get the password from the image file    
    exit_code, output = get_password(image_path, active_user)
    on_end()
    
    if exit_code == 0:
        notify_user(output)
    elif exit_code == 4:
        show_error_message("Could not recover password.")
    else:
        show_error_message(f'Encountered unexpected utility error: {output}\nExit code: {exit_code}')

def get_password(image_path, username = None) -> tuple[int, str]:
    """ Retrieves a password from an image file
    
    Args:
        image_path (str): The path to the image file
        username (str): The username of the user, if not provided, use fork to login to choose a user
        
    Returns:
        tuple[int, str]: A tuple containing the exit code and the password (none when exit code is not 0).
        
        EXIT CODES:
        -1: Unknown/Unexpected utility error
        0:  Success
        1:  Not enough arguments
        2:  Invalid argument(s)
        3:  BMP file does not exist
        4:  Could not recover password
        5:  Unsupported target type
        6:  Bad hash
        7:  Bad config
    """
    # check if image path is relative or absolute
    if not os.path.isabs(image_path):
        build = os.environ.get('SP_BUILD')
        
        if not SP_BUILD_TYPE.IsValid(build):
            return 7, f"Invalid build type: {build}"
        
        if build == SP_BUILD_TYPE.DEBUG:
            image_path = os.path.join(os.environ['ROOT_DIR'], image_path)
        
        # on release build, the image path is relative where the app is executed (don't need to change it)
        
    # Validate the image path and user hash
    if not os.path.exists(image_path):
        return 3, f"Error: The file '{image_path}' does not exist."
    
    
    if username is None:
        user_hash = fork_to_login()
    else:
        user_hash = UserManager().get_user_pass_hash(username)
        
    if user_hash is None:
        return 4, "Could not recover password: Failed to login."
    
    if not is_valid_sha256_hash(user_hash):
        return 6, "Error: The user hash is not a valid SHA-256 hash."
    
    target_type = TargetType.GetTargetType(image_path)
    if target_type == TargetType.NOT_FOUND:
        return 5, "Error: Unsupported target type."
    
    stdout, exit_code = run_subprocess(f'{UtilityFetcher.fetch_path(target_type)} -g "{image_path}" -h {user_hash}')
    
    if exit_code == -1:
        return -1, f'Encountered unexpected utility error: {stdout}'
    
    return exit_code, stdout

def notify_user(password : str):
    """ Notifies the user of the password, copies it to the clipboard, and clears the clipboard after 30 seconds

    Args:
        password (str): The password to notify the user of
    """
    
    # use secure-copy utility to copy the password to the clipboard
    stdout, exit = run_subprocess(f'{UtilityFetcher.fetch_path(TargetType.SECURE_COPY)} "{password}"')
    if exit != 0:
        show_error_message(f"An error occurred while copying the password to the clipboard.")
        return
    
    ToastNotifier().show_toast(
        "Password Copied to Clipboard",
        "It will be cleared in 30 seconds.",
        duration = 10,
        icon_path = get_path_to_icon(),
        threaded=True
    )
    
    # wait 30 seconds before clearing the clipboard
    time.sleep(30)
    if password == pyperclip.paste():
        pyperclip.copy(f" ")

if __name__ == '__main__':
    raise Exception("get_password.py is not meant to be run directly.")