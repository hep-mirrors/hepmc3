import os
import setuptools
from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig
from subprocess import check_output
#https://stackoverflow.com/questions/42585210/extending-setuptools-extension-to-use-cmake-in-setup-py
class CMakeExtension(Extension):
    def __init__(self, name, sources=[]):
        Extension.__init__(self,name = name, sources = sources)

class build_ext(build_ext_orig):
    def get_cmake_exe(self):

        vg='0'
        cmakeg_exe=""
        outg = check_output(["cmake", "--version"])
        outgsplit=outg.split()
        if len(outgsplit)>2: 
           vg=outgsplit[2]
           if int(vg[0])>=3: 
            cmakeg_exe="cmake"
        if (cmakeg_exe!=""): return cmakeg_exe

        v3='0'
        cmake3_exe=""
        out3 = check_output(["cmake3", "--version"])
        out3split=out3.split()
        if len(out3split)>2:
         v3=out3split[2]
         if int(v3[0])==3: 
           cmake3_exe="cmake3"
        if (cmake3_exe!=""): return cmake3_exe
        return "foo "
        
    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        build_ext_orig.run(self)

    def build_cmake(self, ext):
        cwd=os.getcwd()
        cmake_exe=self.get_cmake_exe()
        cmake_args = [ 'CMakeLists.txt',  '-DUSE_INSTALLED_HEPMC3=ON' ]
        self.spawn([cmake_exe, str(cwd)] + cmake_args)
        if not self.dry_run:
            build_args = [  ]
            self.spawn([cmake_exe, '--build', '.'] + build_args)
        os.chdir(str(cwd))

setuptools.setup(
     name='pyHepMC3',  
     version='3.2.0',
     author="HepMC3 Developers",
     author_email="hepmc-dev@cern.ch",
     description="Python bindings for HepMC3",
     long_description="# pyHepMC3\nOfficial python bindings for the HepMC3 library",
     long_description_content_type="text/markdown",
     url="https://gitlab.cern.ch/hepmc/HepMC3",
     license = "GPLv3",
     platforms=['any'],
     packages=setuptools.find_packages(),
     
     classifiers=[
         "Programming Language :: Python ",
         "License :: OSI Approved :: GPLv3",
         "Operating System :: OS Independent",
     ],
     setup_requires=["cmake"],
     ext_modules=[CMakeExtension('pyHepMC3')],
      cmdclass={
        'build_ext': build_ext,
    }
 )
