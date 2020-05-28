try:
 from ctypes import cdll
 libCore = cdll.LoadLibrary("libCore.so")
from .pyHepMC3rootIO import *
