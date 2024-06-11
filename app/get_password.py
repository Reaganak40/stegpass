# ? Standard Imports
import os
import sys

# Project Imports
from utils.utils import is_valid_sha256_hash, run_subprocess, fork_to_login
from utils.utility_fetcher import TargetType, fetch_utility_path

def get_password(image_path, user_hash) -> tuple[int, str]:
    """ Retrieves a password from an image file
    
    Args:
        image_path (str): The path to the image file
        user_hash (str): The hash of the user's master password
        
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
    """
    target_type = TargetType.GetTargetType(image_path)
    if target_type == TargetType.NOT_FOUND:
        return 5, None
    
    stdout, exit_code = run_subprocess(f'{fetch_utility_path(target_type)} -g "{image_path}" -h {user_hash}')
    return exit_code, stdout if exit_code == 0 else None
    
if __name__ == '__main__':
    # Set the root directory path
    root_dir  = os.path.dirname(os.path.abspath(__file__))
    os.environ['ROOT_DIR'] = root_dir
    
    # Check for the correct number of arguments
    if len(sys.argv) == 1 or len(sys.argv) > 3:
        print("Usage: python get_password.py <image_path> <optional:user_hash>")
    
    # Argument 1 should always be the image path
    image_path = sys.argv[1]
    
    # Argument 2 is optional, if no user hash is provided, fork to login
    if len(sys.argv) == 3:
        user_hash = sys.argv[2]
    else:
        user_hash = fork_to_login()
        if not user_hash:
            print("Cannot retrieve password without a user hash. Exiting.")
            sys.exit(1)
    
    # Validate the image path and user hash
    if not os.path.exists(image_path):
        print(f"Error: The file '{image_path}' does not exist.")
        sys.exit(1)
        
    if not is_valid_sha256_hash(user_hash):
        print("Error: The user hash is not a valid SHA-256 hash.")
        sys.exit(1)
    
    # Retrieve the password
    result, password = get_password(image_path, user_hash)
    if result != 0:
        print("Failed to retrieve the password: Error code", result)
        sys.exit(1)
    
    print(password)
    sys.exit(0)