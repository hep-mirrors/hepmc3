import os
import setuptools
import sys
from setuptools import find_packages, setup, Extension
from setuptools.command.build_ext import build_ext as build_ext_orig
from subprocess import check_output
#https://stackoverflow.com/questions/42585210/extending-setuptools-extension-to-use-cmake-in-setup-py
class CMakeExtension(Extension):
    def __init__(self, name, sources=[]):
        Extension.__init__(self,name = name, sources = sources)

def get_hepmc3_version():
  line = '#define HEPMC3_VERSION_CODE 3002000'
  current=os.getcwd()
  with open(current+'/include/HepMC3/Version.h') as f:
    line = next((l for l in f if 'HEPMC3_VERSION_CODE' in l and '#define ' in l), None)
  number=int(line.split(' ')[2]) 
  return str(number/1000000)+'.'+str((number%1000000)/1000)+'.'+str((number%1000))


class build_ext(build_ext_orig):
    def get_ctest_exe(self):
		return "ctest"
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
    def get_cmake_python_flags(self):
        pv=sys.version_info
        return '-DHEPMC3_PYTHON_VERSIONS='+str(pv[0])+'.'+str(pv[1])

        
    def run(self):
        for ext in self.extensions:
            self.build_cmake(ext)
        build_ext_orig.run(self)

    def build_cmake(self, ext):
        cwd=os.getcwd()
        cmake_exe=self.get_cmake_exe()
        ctest_exe=self.get_ctest_exe()
        cmake_args = [
         'CMakeLists.txt','-DHEPMC3_BUILD_EXAMPLES:BOOL=OFF','-DHEPMC3_INSTALL_INTERFACES:BOOL=OFF','-DHEPMC3_ENABLE_SEARCH:BOOL=ON',
         '-DHEPMC3_BUILD_DOCS:BOOL=OFF','-DHEPMC3_ENABLE_TEST:BOOL=ON', 
         '-DHEPMC3_ENABLE_PYTHON:BOOL=ON', '-DHEPMC3_ENABLE_ROOTIO:BOOL=OFF' , self.get_cmake_python_flags()]
        self.spawn([cmake_exe, str(cwd)] + cmake_args)
        if not self.dry_run:
            build_args = [  ]
            self.spawn([cmake_exe, '--build', '.'] + build_args)
            self.spawn([ctest_exe,  '.'])
        os.chdir(str(cwd))
	
	
setuptools.setup(
     name='HepMC3',  
     version=get_hepmc3_version(),
     author="HepMC3 Developers",
     author_email="hepmc-dev@cern.ch",
     description="HepMC3 library and Python bindings for HepMC3",
     long_description="#HepMC3\nOfficial python bindings for the HepMC3 library",
     long_description_content_type="text/markdown",
     url="https://gitlab.cern.ch/hepmc/HepMC3",
     license = "GPLv3",
     platforms=['any'],
     include_package_data = True,
     packages=setuptools.find_packages(),
     
     data_files=[
     ('lib64',              ['outputs/lib64/libHepMC3.so.3', 'outputs/lib64/libHepMC3.so.3','outputs/lib64/libHepMC3search.so.3', 'outputs/lib64/libHepMC3search.so.3']),      
     ('bin',                ['outputs/bin/HepMC3-config']),       
     ('share/HepMC3/cmake', ['outputs/share/HepMC3/cmake/HepMC3Config-version.cmake', 'outputs/share/HepMC3/cmake/HepMC3Config.cmake']),       
     ('include/HepMC3',[
     'include/HepMC3/WriterAsciiHepMC2.h','include/HepMC3/WriterHEPEVT.h','include/HepMC3/Units.h','include/HepMC3/HEPEVT_Wrapper.h','include/HepMC3/GenCrossSection.h','include/HepMC3/GenRunInfo.h','include/HepMC3/WriterAscii.h','include/HepMC3/WriterPlugin.h','include/HepMC3/Setup.h','include/HepMC3/GenVertex.h','include/HepMC3/FourVector.h','include/HepMC3/PrintStreams.h','include/HepMC3/GenEvent.h','include/HepMC3/ReaderHEPEVT.h','include/HepMC3/Print.h','include/HepMC3/LHEF.h','include/HepMC3/GenParticle_fwd.h',
     'include/HepMC3/Reader.h','include/HepMC3/GenPdfInfo_fwd.h','include/HepMC3/GenParticle.h','include/HepMC3/GenCrossSection_fwd.h','include/HepMC3/LHEFAttributes.h',
     'include/HepMC3/AssociatedParticle.h','include/HepMC3/ReaderLHEF.h','include/HepMC3/GenPdfInfo.h','include/HepMC3/HepMC3.h','include/HepMC3/Errors.h',
     'include/HepMC3/GenHeavyIon.h','include/HepMC3/Writer.h','include/HepMC3/ReaderFactory.h','include/HepMC3/ReaderAsciiHepMC2.h',
     'include/HepMC3/Version.h','include/HepMC3/Attribute.h','include/HepMC3/GenHeavyIon_fwd.h','include/HepMC3/ReaderAscii.h',
     'include/HepMC3/GenVertex_fwd.h','include/HepMC3/ReaderPlugin.h','search/include/HepMC3/Filter.h', 'search/include/HepMC3/Relatives.h', 'search/include/HepMC3/AttributeFeature.h', 'search/include/HepMC3/FilterAttribute.h', 'search/include/HepMC3/Feature.h', 'search/include/HepMC3/Selector.h'
      ]),
     ('include/HepMC3/Data',['include/HepMC3/Data/GenEventData.h','include/HepMC3/Data/GenParticleData.h','include/HepMC3/Data/GenVertexData.h','include/HepMC3/Data/GenRunInfoData.h'])
                  ],
     
     classifiers=[
         "Programming Language :: Python ",
         "Operating System :: OS Independent",
     ],
     ext_modules=[CMakeExtension('pyHepMC3')],
      cmdclass={
        'build_ext': build_ext,
    }
 )
