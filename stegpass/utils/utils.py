import hashlib

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