
# ? Standard Imports
import os

# ? Project Imports
try:
    from app.utils.config import SP_BUILD_TYPE 
except ImportError:
    from config import SP_BUILD_TYPE

class TargetType:
    """ Enumerates the types of targets that can be selected
    """
    NOT_FOUND = 0 # Could not find the target
    BMP = 1       # For .bmp files
    
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
    
class UtilityFetcher:
    """ Fetches the path to the appropriate backend utility
    """
    
    BACKEND_NAMES = {
            TargetType.BMP : 'bmp-steg',
        }
    
    @staticmethod
    def fetch_path(target_type : int) -> str:
        """ Gets the path to the appropriate backend utility
        
        Args:
            target_type (int): The type of target
        
        Returns:
            str: The path to the backend utility, or None if the utility could not be found.
        """
        
        root_dir = os.environ.get('ROOT_DIR')
        if not root_dir:
            return None
        
        if target_type not in UtilityFetcher.BACKEND_NAMES:
            return None
        
        alias = UtilityFetcher.BACKEND_NAMES[target_type]
        if os.environ.get('SP_BUILD') == SP_BUILD_TYPE.DEBUG:
            return os.path.join(root_dir, f'..\\bin\\Release-windows-x86_64\\{alias}\\{alias}.exe')
        elif os.environ.get('SP_BUILD') == SP_BUILD_TYPE.RELEASE:
            return os.path.join(root_dir, f'..\\..\\utility\\{alias}.exe')
            
        return None
    
    @staticmethod
    def fetch_all_paths() -> list:
        """ Fetches the paths to all backend utilities
        
        Returns:
            list: A list of paths to the backend utilities, or None if the utilities could not be found.
        """
        root_dir = os.environ.get('ROOT_DIR')
        if not root_dir:
            return None
        
        build = os.environ.get('SP_BUILD')
        if not SP_BUILD_TYPE.IsValid(build):
            return None
        
        paths = []
        
        # Get the paths for each backend utility
        for alias in UtilityFetcher.BACKEND_NAMES.values():
            if build == SP_BUILD_TYPE.DEBUG:
                paths.append(os.path.join(root_dir, f'..\\bin\\Release-windows-x86_64\\{alias}\\{alias}.exe'))
            elif build == SP_BUILD_TYPE.RELEASE:
                paths.append(os.path.join(root_dir, f'..\\..\\utility\\{alias}.exe'))
     
        # Also include path to core dll
        if build == SP_BUILD_TYPE.DEBUG:
            # Its okay to use the bmp target type here since the core.dll is the same for all targets
            paths.append(os.path.join(root_dir, f'..\\bin\\Release-windows-x86_64\\{UtilityFetcher.BACKEND_NAMES[TargetType.BMP]}\\core.dll'))
        elif build == SP_BUILD_TYPE.RELEASE:
            paths.append(os.path.join(root_dir, f'..\\..\\utility\\core.dll'))
        
        return paths

if __name__ == '__main__':
    # Set the root directory path (up one directory from the current file's location)
    root_dir = os.path.abspath(os.path.join(os.path.dirname(os.path.abspath(__file__)), '../..'))
    os.environ['ROOT_DIR'] = root_dir
    
    from config import setup_config
    setup_config()
    
    for path in UtilityFetcher.fetch_all_paths():
        print(path)