# ? Standard Imports
import os

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
    
def fetch_utility_path(target_type : int) -> str:
    """ Gets the path to the appropriate backend utility
    
    Args:
        target_type (int): The type of target
    
    Returns:
        str: The path to the backend utility, or None if the utility could not be found.
    """
    root_dir = os.environ.get('ROOT_DIR')
    if not root_dir:
        return None
    
    backend_paths = {
        'bmp' : '..\\bin\\Release-windows-x86_64\\bmp-steg\\bmp-steg.exe',
    }
        
    if target_type == TargetType.BMP:
        return os.path.join(root_dir, backend_paths['bmp'])
        
    return None
