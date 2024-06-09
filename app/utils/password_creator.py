
# ? Standard Imports
import os
import subprocess
import sys

# Project Imports
from utils.utils import fork_to_login, copy_file

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
            
        print(user_hash)
            
        # Step 1: Copy original image to destination
        if src != dest:
            if not copy_file(src, dest):
                return 1
            
            
            
        
        
        