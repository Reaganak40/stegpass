import configparser
import os
import sys
import subprocess
import shutil

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
PATH_TO_ICON = os.path.abspath(os.path.join(ROOT_DIR, '../../app/res/icon.ico'))

def get_config() -> configparser.ConfigParser:
    """ Gets the config file for the application.

    Returns:
        configparser.ConfigParser: The config file for the application.
    """
    path_to_config_file = os.path.join(ROOT_DIR, '../../app/config.ini')
    
    # verify the path
    if not os.path.exists(path_to_config_file):
        print('Error: config.ini not found')
        sys.exit(1)
    
    config = configparser.ConfigParser()
    config.read(path_to_config_file)
    return config

def get_utility_paths():
    """ Gets the paths to the utility executables needed for the app.

    Returns:
        list: A list of paths to the utility executables, or None if the utilities could not be found.
    """
    path_to_utility_fetcher = os.path.abspath(os.path.join(ROOT_DIR, '../../app/app/utils/utility_fetcher.py'))
    
    # verify the path
    if not os.path.exists(path_to_utility_fetcher):
        print('Error: utility_fetcher.py not found')
        sys.exit(1)
        
    # execute the script to get the paths
    output = subprocess.check_output([sys.executable, path_to_utility_fetcher]).decode('utf-8').strip()
    
    paths = []
    for line in output.split('\n'):
        if line.endswith('\r'):
            line = line[:-1]
        paths.append(os.path.abspath(line))
        
    # verify the existence of each path
    for path in paths:
        if not os.path.exists(path):
            print(f'Error: {path} not found')
            return None

    return paths
    
def copy_utility_files(release_dir):
    """ Copies the utility executables to the release folder.
    """
    path_to_utility_folder = os.path.join(release_dir, 'utility')
    utility_paths = get_utility_paths()
    
    if not utility_paths:
        print('Error: Utility paths not found')
        sys.exit(1)
    
    # create the utility folder
    os.makedirs(path_to_utility_folder)
    
    # copy each utility to the utility folder
    for path in utility_paths:
        print(f'Copying utility at {path}...')
        shutil.copy2(path, path_to_utility_folder)

def create_release_folder(version):
    """ Creates a new release folder with the given version number.

    Args:
        version (str): The version number of the release.
    """
    print(f'Creating release folder for version {version}')
    
    # create the releases folder if not exists
    release_dir = os.path.join(ROOT_DIR, 'releases')
    if not os.path.exists(release_dir):
        os.makedirs(release_dir)
        
    # create the release folder
    release_dir = os.path.join(release_dir, version)
    
    # if it already exists, delete it
    if os.path.exists(release_dir):
        print('Warning: Release folder already exists. Deleting it...')
        shutil.rmtree(release_dir, ignore_errors=True)
        
    os.makedirs(release_dir)
    
    return release_dir
    
def build_app(release_dir):
    """ Calls the build script to build the app via pyinstaller.

    Args:
        version (str): The version number of the release.
    """
    
    # get paths needed for pyinstaller command
    path_to_main_py = os.path.abspath(os.path.join(ROOT_DIR, '../../app/main.py'))
    
    # verify paths
    if not os.path.exists(path_to_main_py):
        print('Error: main.py not found')
        sys.exit(1)
        
    if not os.path.exists(PATH_TO_ICON):
        print('Error: icon.ico not found')
        sys.exit(1)
        
    build_command = f'python -m PyInstaller --noconfirm --onedir --windowed --icon "{PATH_TO_ICON}"\
        --hidden-import "tkinterdnd2" --hidden-import "tkdnd" --collect-all "tkinterdnd2"  {path_to_main_py}'
    
    print(f'Building app with pyinstaller...')
    # Execute the process with the specified working directory
    process = subprocess.Popen(build_command, cwd=release_dir)

    # Wait for the process to finish
    process.wait()
    
    # Clean up: remove the build folder and the spec file
    print('Cleaning up build files...')
    shutil.rmtree(os.path.join(release_dir, 'build'), ignore_errors=True)
    os.remove(os.path.join(release_dir, 'main.spec'))
    
    # Move the executable up one level and rename dist to app
    print("Reformatting app folder...")
    path_to_exe = os.path.join(release_dir, 'dist/main/main.exe')
    path_to_internal = os.path.join(release_dir, 'dist/main/_internal')
    
    app_dir = os.path.join(release_dir, 'app')
    os.makedirs(app_dir)
    
    new_internal_path = os.path.join(app_dir, '_internal')
    os.makedirs(new_internal_path)
    
    # gather all files in the internal folder
    all_files = os.listdir(path_to_internal)
    
    # iterate on all files to move them to the new internal folder
    for f in all_files:
        src_path = os.path.join(path_to_internal, f)
        dst_path = os.path.join(new_internal_path, f)
        os.rename(src_path, dst_path)
        
    # move the executable to the app folder
    shutil.move(path_to_exe, app_dir)
        
    # delete the old internal folder
    os.rmdir(path_to_internal)
    
    # delete dist folder
    shutil.rmtree(os.path.join(release_dir, 'dist'), ignore_errors=True)
    
    # rename exe to stegpass
    os.rename(os.path.join(app_dir, 'main.exe'), os.path.join(app_dir, 'stegpass.exe'))
    
def save_release_config(release, config):
    """ Saves the release config file.

    Args:
        release (str): The path to the release folder.
        config (configparser.ConfigParser): The config file for the application.
    """
    # Set build to be release (0 - debug, 1 - release)
    print("Updating config to release build...")
    config.set('app', 'build', '1')
    
    print("Saving config to release folder...")
    save_path = os.path.join(release, 'app\\_internal\\config.ini')
    
    with open(save_path, "w") as f:
        config.write(f)
        
def move_changelog(release):
    """ Moves the changelog to the release folder.

    Args:
        release (str): The path to the release folder.
    """
    print("Moving changelog to release folder...")
    shutil.copy2(os.path.join(ROOT_DIR, '..\\..\\CHANGELOG.md'), os.path.join(release, 'CHANGELOG.md'))
        
def copy_icon(release):
    """ Copies the icon to the release folder.

    Args:
        release (str): The path to the release folder.
    """
    print("Copying icon to release folder...")
    shutil.copy2(PATH_TO_ICON, os.path.join(release, 'app\\_internal\\icon.ico'))
    
if __name__ == '__main__':
    
    config = get_config()
    version = f'v{config.get("app", "version")}'
    
    release_dir = create_release_folder(version)
    build_app(release_dir)
    copy_utility_files(release_dir)
    save_release_config(release_dir, config)
    copy_icon(release_dir)
    move_changelog(release_dir)
    
else:
    print('create_release.py should be run as the main script.')
    sys.exit(1)