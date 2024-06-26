"""
StegPass - Password Manager Application
utils.py - Utility functions for the application
"""

# ? Standard Imports
import hashlib
import ctypes
from ctypes import wintypes
import os
import subprocess
import sys
import shutil
from PIL import Image


# ? Project Imports
from app.utils.config import SP_BUILD_TYPE

def sha256_hash(password) -> str:
    """ Hashes a password using the SHA-256 algorithm

    Args:
        password (str): The password to hash

    Returns:
        str: The hashed password
    """
    
    # Convert the password string to bytes
    password_bytes = password.encode('utf-8')

    # Create a SHA-256 hash object
    sha256_hash_obj = hashlib.sha256()

    # Update the hash object with the password bytes
    sha256_hash_obj.update(password_bytes)

    # Get the hexadecimal representation of the hash
    hashed_password = sha256_hash_obj.hexdigest()

    return hashed_password

def is_valid_sha256_hash(hash) -> bool:
    """ Determines if a string is a valid SHA-256 hash

    Args:
        hash (str): The string to check

    Returns:
        bool: True if the string is a valid SHA-256 hash, False otherwise
    """
    return len(hash) == 64 and all(c in '0123456789abcdefABCDEF' for c in hash)

def convert_to_lowercase(username : str) -> str:
    """ Converts a username to lowercase

    Args:
        username (str): The username to convert

    Returns:
        str: The username in lowercase
    """
    return username.lower()

def show_error_message(message : str):
    """ Displays an error message dialog

    Args:
        message (str): The message to display
    """
    # Constants from the Windows API
    MB_OK = 0x00000000
    MB_ICONERROR = 0x00000010
    ctypes.windll.user32.MessageBoxW(0, message, "StegPass", MB_OK | MB_ICONERROR)
    
def fork_to_login(username = None) -> str:
    """ Launches the login application

    Args:
        username (str): The user to log in as

    Returns:
        str: The hash of the master password. None if the user cancels the login process.
    """
    # Get root directory path
    root_dir = os.environ.get("ROOT_DIR")
    if not root_dir:
        raise Exception("ROOT_DIR environment variable not set")
    
    build = os.environ.get('SP_BUILD')
    if not SP_BUILD_TYPE.IsValid(build):
        return None
    
    if build == SP_BUILD_TYPE.DEBUG:
        args = [sys.executable, os.path.join(root_dir, 'main.py'), '--login']
    elif build == SP_BUILD_TYPE.RELEASE:
        args = [os.path.join(root_dir, '..\\StegPass.exe'), '--login']
    
    if username is not None:
        args.append(username)
        
    output = subprocess.check_output(args).decode('utf-8').strip()
    
    if output == "None":
        return None
    
    return output

def copy_file(src, dst) -> bool:
    """ Copies a file from the source to the destination

    Args:
        src (str): The path to the source file
        dst (str): The path to the destination file

    Returns:
        bool: True if the file was copied successfully, False otherwise
    """
    
    # ensure absolute paths
    src = os.path.abspath(src)
    dst = os.path.abspath(dst)
    
    try:
        # Ensure the source file exists
        if not os.path.isfile(src):
            show_error_message(f"File {src} does not exist.")
            return False

        # Copy the file
        shutil.copy2(src, dst)
    except Exception as e:
        show_error_message(f"An error occurred while copying the file: {e}")
        return False
    
    return True

def run_subprocess(command : list[str]):
    """ Runs a subprocess and captures the output

    Args:
        command (list[str]): The command to run

    Returns:
        int: The exit code of the subprocess
    
    Raises:
        Exception: If the command is unable to run
    """
    # Run the subprocess and capture the output
    CREATE_NO_WINDOW = 0x08000000
    result = subprocess.run(command, capture_output=True, text=True, shell=False, creationflags=CREATE_NO_WINDOW)
    
    # Get the standard output
    stdout = result.stdout
    
    # Get the exit code
    exit_code = result.returncode
    
    if exit_code != 0:
        if 'is not recognized as an internal or external command' in result.stderr:
            raise Exception('Bad command')
    
    return stdout, exit_code

def get_path_to_icon():
    """ Returns the path to the application icon

    Returns:
        str: The path to the application icon
    """
    # Get root directory path
    root_dir = os.environ.get("ROOT_DIR")
    if not root_dir:
        raise Exception("ROOT_DIR environment variable not set")
    
    build = os.environ.get('SP_BUILD')
    if not SP_BUILD_TYPE.IsValid(build):
        return None
    
    if build == SP_BUILD_TYPE.DEBUG:
        return os.path.abspath(os.path.join(root_dir, 'res/icon.ico'))
    elif build == SP_BUILD_TYPE.RELEASE:
        return os.path.abspath(os.path.join(root_dir, 'icon.ico'))

def resize_image(image, max_height):
    """ Resizes an image while maintaining the aspect ratio

    Args:
        image (PIL.Image): The image to resize
        max_height (int): The maximum height of the resized image

    Returns:
        PIL.Image: The resized image
    """
    
    # Get the original dimensions of the image
    width, height = image.size
    
    # Calculate the new width and height while maintaining the aspect ratio
    if height > max_height:
        new_height = max_height
        new_width = int((new_height / height) * width)
    else:
        new_width, new_height = width, height
    
    # Resize the image
    resized_image = image.resize((new_width, new_height), Image.LANCZOS)
    return resized_image