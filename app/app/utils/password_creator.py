
# ? Standard Imports
import os
import configparser
from shlex import quote

# Project Imports
from app.utils.utils import fork_to_login, copy_file, show_error_message, run_subprocess
from app.utils.utility_fetcher import TargetType, UtilityFetcher
from app.utils.user_manager import UserManager

class PasswordCreator:
    
    def store_password(self, username, new_password, src, dest) -> bool:
        """ Stores a password in an image file
        
        Args:
            username (str): The username of the user
            new_password (str): The password to store
            src (str): The path to the source image file
            dest (str): The path to the destination image file
        
        Returns:
            bool: True if the password was stored successfully, false otherwise
        """
        user_hash = UserManager().get_user_pass_hash(username)
        if user_hash is None:
            return False
        
        # Step 1: Get the target type and check if it is supported
        file_type = TargetType.GetTargetType(src)
        if file_type == TargetType.NOT_FOUND:
            show_error_message('Encountered an error while copying the file: File type not supported.')
            return False
            
        if not os.path.isdir(dest):
            image_name = os.path.basename(dest)
            dest = os.path.dirname(dest)
        else:
            image_name = os.path.basename(src)
        
        # Step 2: Copy original image to destination
        if not os.path.samefile(src, os.path.join(dest, image_name)):
            copy_file(src, dest)
            
        # Step 3: Call backend utility to store the password in the image
        dest = os.path.join(dest, image_name)
        
        # get path to appropriate backend utility
        path_to_utility = UtilityFetcher.fetch_path(file_type)
        if not path_to_utility:
            show_error_message('Encountered an error while storing the password: Utility not found.')
            return False
        
        # ensure that the password is properly quoted to escape special characters
        new_password = new_password.replace('"', '\\"')
    
        try:
            command = [path_to_utility, '-s', dest, new_password, '-h', user_hash]
            stdout, exit_code = run_subprocess(command)
        except Exception as e:
            show_error_message(f"An error occurred while storing the password: {e}")
            return False
        
        if exit_code != 0:
            print(command)
            show_error_message(f"A utility error occurred while storing the password:\noutput:{stdout}\nExit Code:{exit_code}")
            return False
        
        return True
        
            
            
            
        
        
        