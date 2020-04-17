import os
import platform
import sys

def linuxInstall():
	os.system('sudo apt-get install libgsl-dev')
	if(sys.version[0] == '3'):
		os.system('pip3 install appjar')
		os.system('pip3 install seaborn')
	else:
		os.system('pip install appjar')
		os.system('pip install seaborn')



def macInstall():
	os.system('brew install gsl')
	if(sys.version[0] == '3'):
		os.system('pip3 install appjar')
		os.system('pip3 install seaborn')
	else:
		os.system('pip install appjar')
		os.system('pip install seaborn')


if(platform.system() == 'Linux'):
	linuxInstall()
elif(platform.system() == 'Darwin'):
	macInstall()
else:
	print("INVALID OS")