
# ? Standard Imports
import os
import configparser

# Project Imports
from utils.utils import fork_to_login, copy_file, show_error_message, run_subprocess
from utils.utility_fetcher import TargetType, fetch_utility_path

class PasswordCreator:
    
    def store_password(self, username, new_password, src, dest) -> int:
        """ Stores a password in an image file
        
        Args:
            username (str): The username of the user
            new_password (str): The password to store
            src (str): The path to the source image file
            dest (str): The path to the destination image file
        
        Returns:
            int: 0 if the password was stored successfully, 1 otherwise
        """
        user_env_name = username + '_HASH'
        
        # Check if the user hash is in the environment variables, otherwise prompt the user to log in
        if user_env_name not in os.environ:
            user_hash = fork_to_login(username)
            if user_hash is None:
                return 1
            os.environ[user_env_name] = user_hash
        else:
            user_hash = os.environ[user_env_name]
            
        # Step 1: Get the target type and check if it is supported
        file_type = TargetType.GetTargetType(src)
        if file_type == TargetType.NOT_FOUND:
            show_error_message('Encountered an error while copying the file: File type not supported.')
            return 1
            
        # Step 2: Copy original image to destination
        if src != dest:
            if not copy_file(src, dest):
                return 1
            
        # Step 3: Call backend utility to store the password in the image
        image_name = os.path.basename(src)
        dest = os.path.join(dest, image_name)
        print(dest)
        
        # get path to appropriate backend utility
        path_to_utility = fetch_utility_path(file_type)
        if not path_to_utility:
            show_error_message('Encountered an error while storing the password: Utility not found.')
            return 1
    
        try:
            command = f'{path_to_utility} -s "{dest}" "{new_password}" -h {user_hash}'
            stdout, exit_code = run_subprocess(command)
        except Exception as e:
            show_error_message(f"An error occurred while storing the password: {e}")
            return 1
        
        if exit_code != 0:
            show_error_message(f"An error occurred while storing the password: {stdout}")
            return 1
        
        return 0
        
            
            
            
        
        
        