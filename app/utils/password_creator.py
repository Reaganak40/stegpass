
# ? Standard Imports
import os
import configparser

# Project Imports
from utils.utils import fork_to_login, copy_file, show_error_message, run_subprocess

class TargetType:
    """ Enumerates the types of targets that can be selected
    """
    NOT_FOUND = 0
    BMP = 1
    
    @staticmethod
    def GetTargetType(filepath) -> int:
        """ Determines the type of target
        
        Args:
            filepath (str): The path to the target
        
        Returns:
            int: The type of target
        """
        extension = os.path.splitext(filepath)[1]
        
        if extension in ['.bmp']:
            return TargetType.BMP
        return TargetType.NOT_FOUND

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
        path_to_utility = self._get_backend_utility(file_type)
        if not path_to_utility:
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
        
    def _get_backend_utility(self, target_type : int) -> str:
        """ Gets the path to the appropriate backend utility
        
        Args:
            target_type (int): The type of target
        
        Returns:
            str: The path to the backend utility
        """
        root_dir = os.environ.get('ROOT_DIR')
        if not root_dir:
            show_error_message('Encountered an error while copying the file: ROOT_DIR environment variable not set.')
            return None
        
        # read from config.ini to get the path to the backend utility
        config = configparser.ConfigParser()
        config.read(os.path.join(root_dir, 'config.ini'))
        
        if target_type == TargetType.BMP and 'bmp' in config:
            return os.path.join(root_dir, config.get('bmp', 'path'))
        
        show_error_message('Encountered an error while copying the file: Invalid config file.')
        return None
    
        
            
            
            
        
        
        