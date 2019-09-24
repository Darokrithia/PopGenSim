from appJar import gui
import subprocess

# This function is called when we press the Go! button.
def runDevosim(button):
    # These entry fields return floats, but we need these parameters to be ints.
    chromosomeLengthInt = int(devosimGUI.getEntry("Chromosome Length"))
    populationSizeInt = int(devosimGUI.getEntry("Population Size"))
    generationsInt = int(devosimGUI.getEntry("Generations"))
    crossoverRateInt = int(devosimGUI.getEntry("Crossover Rate"))
    mutationRateInt = int(devosimGUI.getEntry("Mutation Rate"))
    mutationEffectInt = int(devosimGUI.getEntry("Mutation Effect"))

    settingsStringArray = ["./devosim",
                           "-c", f"{chromosomeLengthInt}",  # f-strings require Python 3.6, I think?
                           "-p", f"{populationSizeInt}",
                           "-g", f"{generationsInt}",
                           "-o", f"{crossoverRateInt}",
                           "-m", f"{mutationRateInt}",
                           "-e", f"{mutationEffectInt}"
                           ]

    selectionModeString = ""
    print("Selection Mode:", devosimGUI.getRadioButton("selectionMode"))
    ## What happens if neither -s or -u are specified?

    if devosimGUI.getCheckBox("Verbose Mode"):
        settingsStringArray.append("-v")

    if devosimGUI.getCheckBox("Percentages Only"):
        settingsStringArray.append("-r")

    if devosimGUI.getCheckBox("Stop if all dgnomes are identical"):
        settingsStringArray.append("-b")

    print(
        "It may also be a decent idea to have a function called whenever any of the integer-only labelEntries are edited.")
    print("We may be able to force the user to only input integers that way (instead of truncating floats)")

    try:
        print(f"-c {chromosomeLengthInt}")
        subprocess.call(settingsStringArray)
    except FileNotFoundError:
        devosimGUI.warningBox("Error launching ./devosim",
                                 "./devosim was not found in this directory. Make sure you compiled it and that it's in the same directory as this program!")

    return


## For this general test GUI, I'll just configure it to look like it
## could support all of ./devosim's command line arguments.

# This is also my first Python program ever! Hello, world!

# Create the master GUI object.
devosimGUI = gui("DevoSim GUI Test")

# Mac OS X Dark Mode quirkiness requires this setup. That's right -- I'm making Dark Mode first.
devosimGUI.setFg("white")
devosimGUI.setBg("black")
# Mac OS X seemed to force the text to be white if it was in Dark Mode, though, which didn't play well with a white
# background...


# Now, set up all the widgets.

# Since AppJar is based on TKInter, it uses the TKInter Grid.
# These numerical arguments in order are:
# Column Number, Row Number, Column Span (how wide), Row Span (How tall).
# (y coord)      (x coord)
# If omitted, they're set to a default.
devosimGUI.addNumericLabelEntry("Chromosome Length", 0, 0)  # -c
devosimGUI.addNumericLabelEntry("Population Size", 0, 1)  # -p
devosimGUI.addNumericLabelEntry("Generations", 1, 0)  # -g
devosimGUI.addNumericLabelEntry("Crossover Rate", 1, 1)  # -o
devosimGUI.addNumericLabelEntry("Mutation Rate", 0, 2) # -m
devosimGUI.addNumericLabelEntry("Mutation Effect", 1, 2) # -e

devosimGUI.addRadioButton("selectionMode", "Random Selection", 2, 0) # no tag
devosimGUI.addRadioButton("selectionMode", "Selective Pressure", 2, 1)  # -s
devosimGUI.addRadioButton("selectionMode", "Uniform Selection", 2, 2)  # -u

devosimGUI.addCheckBox("Verbose Mode", 3, 0)  # -v
devosimGUI.addCheckBox("Percentages Only", 3, 1)  # -r
# -- Not like I'm sure this needs to be in a final GUI version...
devosimGUI.addCheckBox("Stop if all dgnomes are identical", 3, 2)  # -b

# Now let's add some default values; the same ones the program has itself.

devosimGUI.setEntry("Chromosome Length", "10")
devosimGUI.setEntry("Population Size", "10")
devosimGUI.setEntry("Generations", "1000")
devosimGUI.setEntry("Crossover Rate", "2")
devosimGUI.setEntry("Mutation Rate", "0")
devosimGUI.setEntry("Mutation Effect", "2")

# Now let's add the placeholder text (which is called "default" for some reason) for those entry fields.

devosimGUI.setEntryDefault("Chromosome Length", "Chromosome Length")
devosimGUI.setEntryDefault("Population Size", "PopulationSize")
devosimGUI.setEntryDefault("Generations", "Generations")
devosimGUI.setEntryDefault("Crossover Rate", "Crossover Rate")
devosimGUI.setEntryDefault("Mutation Rate", "Mutation Rate")
devosimGUI.setEntryDefault("Mutation Effect", "Mutation Effect")

# Now, here comes the Run button.
devosimGUI.addButton("Go!", runDevosim, 4, 0, 3, 1)

# And this "runs" the thing.
devosimGUI.go()