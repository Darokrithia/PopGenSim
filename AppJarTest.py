from appJar import gui


def runGenancesim(button):
    print("Run the program, etc, with the following params:")
    print("Chromosome Length: ", genancesimGUI.getEntry("Chromosome Length"))
    print("Population Size: ", genancesimGUI.getEntry("Population Size"))
    print("Generations: ", genancesimGUI.getEntry("Generations"))
    print("Chromosome Length: ", genancesimGUI.getEntry("Crossover Rate"))
    print("Selection Mode:", genancesimGUI.getRadioButton("selectionMode"))
    print("Verbose Mode On:", genancesimGUI.getCheckBox("Verbose Mode"))
    print("Percentages Only Mode On:", genancesimGUI.getCheckBox("Percentages Only"))
    print("Stop if all dgnomes are identical?:", genancesimGUI.getCheckBox("Stop if all dgnomes are identical"))
    print("Additional processing may be needed for these.")
    print("It may also be a decent idea to have a function called whenever any of the integer-only labelEntries are edited.")
    print("We may be able to force the user to only input integers that way (instead of truncating floats)")
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


