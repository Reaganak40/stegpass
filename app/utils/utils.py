import hashlib
import ctypes
from ctypes import wintypes
import os
import subprocess
import sys
import shutil

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
    
def fork_to_login(username) -> str:
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
    
    # Launch the login application
    return subprocess.check_output([sys.executable, os.path.join(root_dir, 'login.py'), username]).decode('utf-8').strip()

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
    