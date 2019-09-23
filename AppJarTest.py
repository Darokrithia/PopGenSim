from appJar import gui
import subprocess

def runGenancesim(button):
    chromosomeLengthInt = int(genancesimGUI.getEntry("Chromosome Length"))
    populationSizeInt = int(genancesimGUI.getEntry("Population Size"))
    generationsInt = int(genancesimGUI.getEntry("Generations"))
    crossoverRateInt = int(genancesimGUI.getEntry("Crossover Rate"))


    selectionModeString = ""
    print("Selection Mode:", genancesimGUI.getRadioButton("selectionMode"))
    ## What happens if neither -s or -u are specified?

    verboseModeString = ""
    if genancesimGUI.getCheckBox("Verbose Mode"):
        verboseModeString = "-v"

    percentagesOnlyModeString = ""
    if genancesimGUI.getCheckBox("Percentages Only"):
        percentagesOnlyModeString = "-r"


    stopOnIdenticalDgnomesString = ""
    if genancesimGUI.getCheckBox("Stop if all dgnomes are identical"):
        stopOnIdenticalDgnomesString = "-b"

    print("It may also be a decent idea to have a function called whenever any of the integer-only labelEntries are edited.")
    print("We may be able to force the user to only input integers that way (instead of truncating floats)")

    try:
        print(f"-c {chromosomeLengthInt}")
        subprocess.call(["./genancesim",
                         "-c", f"{chromosomeLengthInt}", #f-strings require Python 3.6, I think?
                         "-p", f"{populationSizeInt}",
                         "-g", f"{generationsInt}",
                         "-o", f"{crossoverRateInt}" # ,
                         # selectionModeString,
                         # verboseModeString,
                         # percentagesOnlyModeString,
                         # stopOnIdenticalDgnomesString
                        ])
    except FileNotFoundError:
        print("./genancesim was not found in this directory. Make sure you compiled it!")

    return

## For this general test GUI, I'll just configure it to look like it
## could support all of ./genancesim's command line arguments.

# This is also my first Python program ever! Hello, world!

# Create the master GUI object.
genancesimGUI = gui("GenAnceSim GUI Test")

# Mac OS X Dark Mode quirkiness requires this setup. That's right -- I'm making Dark Mode first.
genancesimGUI.setFg("white")
genancesimGUI.setBg("black")
# Mac OS X seemed to force the text to be white if it was in Dark Mode, though, which didn't play well with a white background...


# Now, set up all the widgets.

# Since AppJar is based on TKInter, it uses the TKInter Grid.
# These numerical arguments in order are:
# Column Number, Row Number, Column Span (how wide), Row Span (How tall).
# If omitted, they're set to a default.
genancesimGUI.addNumericLabelEntry("Chromosome Length", 0, 0) # -c
genancesimGUI.addNumericLabelEntry("Population Size", 0, 1) # -p
genancesimGUI.addNumericLabelEntry("Generations", 1, 0) # -g
genancesimGUI.addNumericLabelEntry("Crossover Rate", 1, 1) #-o
genancesimGUI.addRadioButton("selectionMode", "Selection", 0, 2) # -s -- What's the default behavior of this, by the way?
genancesimGUI.addRadioButton("selectionMode", "All-contribute", 1, 2) # -u
genancesimGUI.addCheckBox("Verbose Mode", 2, 0) # -v
genancesimGUI.addCheckBox("Percentages Only", 2, 1) # -r -- Not like I'm sure this needs to be in a final GUI version...
genancesimGUI.addCheckBox("Stop if all dgnomes are identical", 2, 2) # -b

# Now let's add some default values; the same ones the program has itself.

genancesimGUI.setEntry("Chromosome Length", "10")
genancesimGUI.setEntry("Population Size", "10")
genancesimGUI.setEntry("Generations", "1000")
genancesimGUI.setEntry("Crossover Rate", "2")

# Now let's add the placeholder text (which is called "default" for some reason) for those entry fields.

genancesimGUI.setEntryDefault("Chromosome Length", "Chromosome Length")
genancesimGUI.setEntryDefault("Population Size", "PopulationSize")
genancesimGUI.setEntryDefault("Generations", "Generations")
genancesimGUI.setEntryDefault("Crossover Rate", "Crossover Rate")

# Now, here comes the Run button.
genancesimGUI.addButton("Go!", runGenancesim, 3, 0, 3, 1)

# And this "runs" the thing.
genancesimGUI.go()


