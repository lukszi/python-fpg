from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import atexit
import sys
import os
import subprocess

class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=''):
        Extension.__init__(self, name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

class CMakeBuild(build_ext):
    def run(self):
        # Apply patches first
        subprocess.check_call(['bash', 'apply_patches.sh'])

        try:
            subprocess.check_output(['cmake', '--version'])
        except OSError:
            raise RuntimeError(
                "CMake must be installed to build the following extensions: " +
                ", ".join(e.name for e in self.extensions))
        
        for ext in self.extensions:
            self.build_extension(ext)

        # atexit.register(self.cleanup_submodule)

    def cleanup_submodule(self):
        try:
            subprocess.check_call(['bash', 'apply_patches.sh', 'cleanup'])
        except subprocess.CalledProcessError:
            print("Warning: Failed to cleanup submodule changes")

    def build_extension(self, ext):
        extdir = os.path.abspath(os.path.dirname(self.get_ext_fullpath(ext.name)))
        
        cmake_args = ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                     '-DPYTHON_EXECUTABLE=' + sys.executable]

        cfg = 'Debug' if self.debug else 'Release'
        build_args = ['--config', cfg]

        cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
        build_args += ['--', '-j2']

        env = os.environ.copy()
        env['CXXFLAGS'] = '{} -DVERSION_INFO=\\"{}\\"'.format(
            env.get('CXXFLAGS', ''),
            self.distribution.get_version())
        
        if not os.path.exists(self.build_temp):
            os.makedirs(self.build_temp)
            
        subprocess.check_call(['cmake', ext.sourcedir] + cmake_args,
                            cwd=self.build_temp, env=env)
        subprocess.check_call(['cmake', '--build', '.'] + build_args,
                            cwd=self.build_temp)

setup(
    name='genpoly_fpg',
    version='1.0',
    description='Wrapper for the c++ fast polygon generator',
    long_description='',
    ext_modules=[CMakeExtension('genpoly_fpg')],
    cmdclass=dict(build_ext=CMakeBuild),
    zip_safe=False,
    python_requires='~=3.10',
)