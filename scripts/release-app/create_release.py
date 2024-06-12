import configparser
import os
import sys
import subprocess
import shutil

ROOT_DIR = os.path.dirname(os.path.abspath(__file__))

COMMAND = """



"""

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
    path_to_icon = os.path.abspath(os.path.join(ROOT_DIR, '../../app/res/icon.ico'))
    
    # verify paths
    if not os.path.exists(path_to_main_py):
        print('Error: main.py not found')
        sys.exit(1)
        
    if not os.path.exists(path_to_icon):
        print('Error: icon.ico not found')
        sys.exit(1)
        
    build_command = f'python -m PyInstaller --noconfirm --onedir --console --icon "{path_to_icon}"\
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
    
if __name__ == '__main__':
    
    config = configparser.ConfigParser()
    config.read('config.ini')
    version = f'v{config.get("app", "version")}'
    
    release_dir = create_release_folder(version)
    build_app(release_dir)