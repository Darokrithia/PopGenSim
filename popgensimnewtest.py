import matplotlib.pyplot as plt
from matplotlib.animation import ArtistAnimation

# have it be able to make in diff data --> put ./devosim -c [num] --- have the use input this similar to how they would be doing it on term
# import different test cases for all these and put into text file 

"""
userinput = input("Enter the info you want to see: ")

letters = []

for let in userinput:
    if let.isdigit():
        letters.append(let)
        
number = ""
if len(letters) == 0:
    number = 30
else:    
    for things in letters:
        print(things)
        number = number+things
        
"""     

file = open("test1.txt","r")  ## what to change this to when coming out just from data? --> not text file
lines = file.readlines() 

i = 0; #line num increment 
percentagenum = 100
count = 0 #to help print which Degnome Ancestry is displayed 
numframes = 30
num = 0  #to get info from line 1 ("-p") 

t = 1 #start counter -- can find line "Degnome n ancestries"
#using t to see which multiple of 6 is being used (ex: 6*t if t = 1 is using Desgnome 0 since first degnome)

images=[] #dunno since ive never done animation -- okay saves all plot graphs into images
fig,ax =  plt.subplots()

for line in lines:
    if num == 0:
        degnomenum = int(line) #gets -p val which data is based on 
        num = 1
        
    x = [] #degnome number 
    y = [] #the percentage 
    
    line = line.strip() #removes extra \n at end 
    
    i=i+1
    if i == (6*t): #want to go through this line and find all numbers -- so happens to be every mult of 6 b/c of how it is written
        #NEED TO FIND WAY TO MAKE IT FIND UNDER STRING CONDITION BUT IDK HOW TO ITERATE THAT
        
        t=t+1
        print(line)
        newline=line.split("\t") #how formatted in the code, spaces for whitespace = a tab length, replace doesnt do anyhting lmao
        
        #print(newline) #because split, newline is now a list -- has correct info
        
        #for things in newline:
            #print (things) #prints 5.000000% Degnome 2
            
        for things in newline:
            posofchar = things.find('%') 
            posofperiod = things.find('.')
            #print(posofchar)
            locationofstart = posofchar+2
            newsubstringdegnum = things[locationofstart:] #prints Degnome 2
            #print(newsubstring) #okay finds each specific degnome
            newsubstringpercentnum = things[:posofperiod] #prints 5 [not includeing .000000% so we can make into an int] 
            #print(newsubstringpercentnum) #okay finds each specific percentage
            y.append(int(newsubstringpercentnum))
            
            for num in range(degnomenum):
                degnomename = "Degnome "+str(num)
                if degnomename == newsubstringdegnum:
                    #print(degnomename) #okay getting correct same output
                    x.append(int(num))
                           
        bargraph = plt.bar(x,y, align = 'center',alpha=.5) 
        plt.xlabel('Degnomes')
        plt.ylabel('Percentages')   
        title = 'Degnome '+str(count)+ ' Ancestries'
        plt.title(title)
        
        #ttl = plt.text(0.5, 1.01, title, horizontalalignment='center', verticalalignment='bottom',transform=ax.transAxes )       
        
        #images.append([bargraph,ttl])
        images.append(bargraph)
        
        
        #figure = 'DegnomeNewtest'+str(count)+'.png'
        #plt.savefig(figure) #will save graph to this file here 
        
        count = count+1
        
        
        #anim = save("polygensimtester.gif", writer = "ffmpeg", fps = None, dpi = None, codec = None, bitrate = None, extra_args=None, metadata=None, extra_anim = None, savefig_kwargs = None)
      
anim = ArtistAnimation(fig, images, interval=500, repeat = False)  #higher interval = slower graph changes
plt.show()     
       


