
# ? Standard Imports
import os
import json

# ? Project Imports
from app.utils.utils import sha256_hash, convert_to_lowercase
from app.utils.config import SP_BUILD_TYPE
from app.utils.singleton import Singleton

USER_DATA_PATH = "data\\user_data.json"
PASSWORD_FOLDER_PATH = "data\\passwords"
UNIVERSAL_PEPPER = "chizom"

@Singleton
class UserManager:
    """ Used to create, delete, update and retrieve user data
    """
    def __init__(self):
        # Get the root directory of the currently executing script
        root_dir = os.environ.get("ROOT_DIR")
        if not root_dir:
            raise Exception("ROOT_DIR environment variable not set")
        
        # The build type will determine the path to the user data file
        build = os.environ.get("SP_BUILD")
        if not SP_BUILD_TYPE.IsValid(build):
            raise Exception(f"Invalid build type: {build}")
        
        if build == SP_BUILD_TYPE.RELEASE:
            root_dir = os.path.abspath(os.path.join(root_dir, '../..'))
            
        self.path_to_user_data = os.path.join(root_dir, USER_DATA_PATH)
        self.path_to_password_folder = os.path.join(root_dir, PASSWORD_FOLDER_PATH)
        
        # check if user data file exists (create directory and file if it doesn't)
        if not os.path.exists(self.path_to_user_data):
            os.makedirs(os.path.dirname(self.path_to_user_data), exist_ok=True)
            with open(self.path_to_user_data, 'w') as f:
                json.dump({}, f)
            self.user_data = {}
        else:
            with open(self.path_to_user_data, 'r') as f:
                self.user_data = json.load(f)
                
        self.add_user_callbacks = {}
        self.add_users_callbacks_to_remove = []
        
        self.set_active_user_callbacks = {}
        self.active_user = None
                
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
        
        self.user_data[username] = sha256_hash(master_password + UNIVERSAL_PEPPER)
        self.save_user_data()
        
        self.remove_listeners()        
        for callback in self.add_user_callbacks.values():
            callback(username)
        self.remove_listeners()
        
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
        
        return self.user_data[username] == sha256_hash(master_password + UNIVERSAL_PEPPER)
    
    def get_password_folder_path(self, username):
        """ Gets the path to the folder containing the passwords for a user

        Args:
            username (str): The username of the user

        Returns:
            str: The path to the folder containing the passwords for the user
        """
        path_to_user_folder = self.path_to_password_folder + "\\" + convert_to_lowercase(username)
        
        # create folder if it doesn't exist
        if not os.path.exists(path_to_user_folder):
            os.makedirs(path_to_user_folder)
            
        return path_to_user_folder
    
    def add_listener_on_add_user(self, id, callback):
        """ Adds a callback to be called when a user is added

        Args:
            callback (function): The function to call when a user is added, with the username as the only argument
        """
        self.add_user_callbacks[id] = callback
        
    def remove_listener_on_add_user(self, id):
        """ Removes a callback to be called when a user is added

        Args:
            id (int): The id of the callback to remove
        """
        if id in self.add_user_callbacks:
            self.add_users_callbacks_to_remove.append(id)
        
    def remove_listeners(self):
        """ Removes all listeners
        """
        for id in self.add_users_callbacks_to_remove:
            del self.add_user_callbacks[id]
        self.add_users_callbacks_to_remove = []
        
    def set_active_user(self, username):
        """ Sets the active user

        Args:
            username (str): The username of the active user
        """
        if self.check_user_exists(username):
            self.active_user = username
            
            for callback in self.set_active_user_callbacks.values():
                callback(username)
        
    def get_active_user(self):
        """ Gets the active user

        Returns:
            str: The username of the active user
        """
        return self.active_user
    
    def add_active_user_changed_listener(self, id, listener):
        """ Adds a listener for when the active user changes

        Args:
            listener (function): The function to call when the active user changes
        """
        self.set_active_user_callbacks[id] = (listener)
        
