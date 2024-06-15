"""
StegPass - Password Manager Application
singleton.py - Singleton decorator for classes
"""

# ? Standard Imports
import threading

def Singleton(cls):
    """ Singleton decorator for classes
    """
    instances = {}
    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return get_instance

# Singleton metaclass
class TSSingleton(type):
    """ Singleton metaclass for classes of which is thread-safe
    """
    _instances = {}
    _lock = threading.Lock()

    def __call__(cls, *args, **kwargs):
        with cls._lock:
            if cls not in cls._instances:
                cls._instances[cls] = super(TSSingleton, cls).__call__(*args, **kwargs)
        return cls._instances[cls]