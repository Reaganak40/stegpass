# ? Standard Imports
import configparser
import os

class SP_BUILD_TYPE:
    """ Enumerates the types of builds that can be created
    """
    DEBUG = '0'
    RELEASE = '1'
    
    @staticmethod
    def IsValid(build_type : str) -> bool:
        """ Checks if the given build type is valid
        
        Args:
            build_type (str): The build type to check
        
        Returns:
            bool: True if the build type is valid, False otherwise
        """
        return build_type == SP_BUILD_TYPE.RELEASE or build_type == SP_BUILD_TYPE.DEBUG

def setup_config():
    """ Sets up the environment variables needed for the application from the config file.
    """
    root_dir = os.environ.get('ROOT_DIR')
    if not root_dir:
        raise Exception("ROOT_DIR environment variable not set.")
    
    # get path to config file and verify it exists
    path_to_config = os.path.join(root_dir, 'config.ini')
    if not os.path.exists(path_to_config):
        raise Exception("Config file not found.")
    
    # read the config file
    config = configparser.ConfigParser()
    config.read(path_to_config)
    
    # set the environment variables
    os.environ['SP_VERSION'] = config.get('app', 'version')
    os.environ['SP_BUILD'] = config.get('app', 'build')
    
    if os.environ['SP_BUILD'] != SP_BUILD_TYPE.RELEASE and os.environ['SP_BUILD'] != SP_BUILD_TYPE.DEBUG:
        raise Exception(f"Invalid build type in config file: {os.environ['SP_BUILD']}")