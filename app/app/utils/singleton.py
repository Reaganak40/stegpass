"""
StegPass - Password Manager Application
singleton.py - Singleton decorator for classes
"""

def Singleton(cls):
    """ Singleton decorator for classes
    """
    instances = {}
    def get_instance(*args, **kwargs):
        if cls not in instances:
            instances[cls] = cls(*args, **kwargs)
        return instances[cls]
    return get_instance