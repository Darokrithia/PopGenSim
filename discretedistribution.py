##discrete distribution as horizontal bar chart 
## maybe create as a dictionary -- the degnome number ex: degnome 1 is key and the percentages are values???

import matplotlib.pyplot as plt
from matplotlib.animation import ArtistAnimation
import numpy as np
from matplotlib import cm
from matplotlib.colors import ListedColormap, LinearSegmentedColormap

file = open("test1.txt","r")
lines = file.readlines() 

i = 0; #line num increment 
percentagenum = 100
count = 0 #to help print which Degnome Ancestry is displayed 
numframes = 30
num = 0  #to get info from l ine 1 ("-p") 

t = 1 #start counter -- can find line "Degnome n ancestries"

images=[] #dunno since ive never done animation -- okay saves all plot graphs into images
listnames = [] #for the top 
dictionary = {}
listofallpercentages = []
k = 0

for line in lines:
    newlistfortuples = []
    if num == 0:
        degnomenum = int(line) #degnome number -- ex: test1.txt = 25
        for numbers in range(degnomenum):
            listnames.append('Generation ' + str(numbers))
        """
        for things in listnames:
            print(things,"\n")
        """
        dictionary = dict.fromkeys(listnames,0)
        #print(dictionary)
        num = 1
        
        
    x = [] #degnome number 
    y = [] #the percentage 
    
    line = line.strip() #removes extra \n at end 
    
    i=i+1
    if i == (6*t): #want to go through this line and find all numbers -- so happens to be every mult of 6 b/c of how it is written
        
        t=t+1
        #print(line)
        newline=line.split("\t") #how formatted in the code, spaces for whitespace = a tab length
            
        for things in newline:
            tuples = ()
            posofchar = things.find('%') 
            posofperiod = things.find('.')
            locationofstart = posofchar+2
            newsubstringdegnum = things[locationofstart:] #prints Degnome 2
            newsubstringpercentnum = things[:posofperiod] #prints 5 [not includeing .000000% so we can make into an int] 
            #print(newsubstringpercentnum) #okay finds each specific percentage
            y.append(int(newsubstringpercentnum))
            
            for num in range(degnomenum):
                degnomename = "Degnome "+str(num)
                if degnomename == newsubstringdegnum:
                    x.append(int(num))
                    tuples = tuples + (int(num),)
                    tuples = tuples + (int(newsubstringpercentnum),)
                 

            newlistfortuples.append(tuples)
        
        listofallpercentages.append(y)
        """
        for x in newlistfortuples:
            print(x)
        """
        degname = 'Generation ' + str(k)
        if degname in dictionary:
            dictionary[degname] = newlistfortuples
        
        k = k+1
        
print(dictionary)
print(listofallpercentages)
    
def graphing(dictionary, listnames):
    """
    """
graphing(dictionary,listnames)
plt.show()

