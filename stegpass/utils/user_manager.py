import os
import json

from utils.utils import sha256_hash, convert_to_lowercase

USER_DATA_PATH = "\\data\\user_data.json"
class UserManager:
    """ Used to create, delete, update and retrieve user data
    """
    
    UNIVERSAL_PEPPER = "chizom"
    
    def __init__(self):
        # Get the root directory of the currently executing script
        root_dir = os.environ.get("ROOT_DIR")
        
        # Path to the user data file
        self.path_to_user_data = root_dir + USER_DATA_PATH
    
        # check if user data file exists (create directory and file if it doesn't)
        if not os.path.exists(self.path_to_user_data):
            os.makedirs(os.path.dirname(self.path_to_user_data), exist_ok=True)
            with open(self.path_to_user_data, 'w') as f:
                json.dump({}, f)
            self.user_data = {}
        else:
            with open(self.path_to_user_data, 'r') as f:
                self.user_data = json.load(f)
                
    def save_user_data(self):
        """ Saves the user data to the user data file
        """
        with open(self.path_to_user_data, 'w') as f:
            json.dump(self.user_data, f)
                
    def check_user_exists(self, username) -> bool:
        """ Checks if a user exists in the user data

        Args:
            username (str): The username to check

        Returns:
            bool: True if the user exists, False otherwise
        """
        return username in self.user_data
    
    def add_user(self, username, master_password) -> bool:
        """ Adds a user to the user data

        Args:
            username (str): The username of the user
            master_password (str): The master password of the user

        Returns:
            bool: True if the user was added successfully, False otherwise
        """
        username = convert_to_lowercase(username)
        
        if self.check_user_exists(username):
            return False
        
        self.user_data[username] = sha256_hash(master_password + UserManager.UNIVERSAL_PEPPER)
        self.save_user_data()
        return True
        
    def delete_user(self, username) -> bool:
        """ Deletes a user from the user data

        Args:
            username (str): The username of the user to delete

        Returns:
            bool: True if the user was deleted successfully, False otherwise
        """
        username = convert_to_lowercase(username)
        
        if not self.check_user_exists(username):
            return False
        
        del self.user_data[username]
        self.save_user_data()
        return True
    
    def get_users(self) -> list:
        """ Gets a list of all the users in the user data

        Returns:
            list: A list of all the users in the user data
        """
        return list(self.user_data.keys())
    
    def check_password(self, username, master_password) -> bool:
        """ Checks if a password is correct for a given user

        Args:
            username (str): The username of the user
            master_password (str): The password to check

        Returns:
            bool: True if the password is correct, False otherwise
        """
        username = convert_to_lowercase(username)
        
        if not self.check_user_exists(username):
            return False
        
        return self.user_data[username] == sha256_hash(master_password + UserManager.UNIVERSAL_PEPPER)
        
