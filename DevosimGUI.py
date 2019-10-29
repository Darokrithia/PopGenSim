from appJar import gui
from appJar.appjar import ItemLookupError
import subprocess

import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

import re

class PlotCounter:
    plotNumber = 1  # A simple counter to differentiate the titles of plot windows.

class Generation:
    def __init__(self, number):
        self.number = number # This generation's number (i.e 0, 1, 1000).
        self.dgnomeValues = dict() # The element[N] is the Nth Dgnome's allele value array.
        self.dgnomeAncestries = dict() # The element[N] is the Nth Dgnome's ancestry percentage array.
        self.dgnomeAncestryPercentages = dict()
        self.percentDiversity = None # Not always defined, but it's a floating point when it is.

# Returns an array of all the generations, in order.
def processDevosimOutputLines(outputLines):
    outputLines.pop(0) # remove the first line because we'll already know what those values should be.
    generationArray = []
    currentGeneration = None # Start with a None generation.

    floatRegex = re.compile(r'\d+\.\d+')  # Compile the regex for finding floating point units in a line of output.
    intRegex = re.compile(r'\d+')  # Compile the regex for finding integers in a line of output.

    for index, line in enumerate(outputLines):
        if line == '':
            continue  # Skip over blank lines.
        # A generation starts with a line of the form "Generation X:". This case parses that line.
        # We expect this to be the first case matched; this will probably break if I'm wrong.
        if re.search("^Generation \d+:$", line):
            if currentGeneration is not None:
                generationArray.append(currentGeneration) # Append the current generation to the array.
            reTemp = re.findall(r'\d+', line)
            currentGenerationInt = list(map(int, reTemp))[0]
            currentGeneration = Generation(currentGenerationInt) # "Start" a new generation.

        # Match this pattern:
        # Degnome X allele values:
        # and then operate on the following line of
        # (some amount of: floatingPointNumber\t)
        if re.search("Degnome \d+ allele values:$", line):
            reTemp = re.findall(r'\d+', line)
            currentDgInt = list(map(int, reTemp))[0]
            dgValueLine = outputLines[index + 1] # The values for this dgnome's alleles.
            currentGeneration.dgnomeValues[currentDgInt] = [float(i) for i in floatRegex.findall(dgValueLine)]

        # Match this pattern:
        # Degnome X ancestries:
        # and then operate on the following line of
        # (some amount of: int\t)
        if re.search("Degnome \d+ ancestries:$", line):
            reTemp = re.findall(r'\d+', line)
            currentDgInt = list(map(int, reTemp))[0]
            dgValueLine = outputLines[index + 1] # The values for this dgnome's alleles.
            currentGeneration.dgnomeAncestries[currentDgInt] = [int(i) for i in intRegex.findall(dgValueLine)]
            # Inside this pattern, match this pattern with the line after dgValueLine:
            # (floatingPoint% Degnome \d+\t)+
            if re.search("\d+\.\d+% Degnome \d+", outputLines[index + 2]):
                reTemp = re.findall(r'\d+\t', outputLines[index + 2])
                intArray = list(map(int, reTemp))
                floatArray = [float(i) for i in floatRegex.findall(outputLines[index + 2])]
                zippedArray = list(zip(intArray, floatArray))
                currentGeneration.dgnomeAncestryPercentages[currentDgInt] = zippedArray
                # print(outputLines[index + 2])
                # print(zippedArray)

        # Match this pattern:
        # Percent diversity: floatingPoint
        if re.search("Percent diversity: \d+\.\d+$", line):
            reTemp = floatRegex.findall(line)
            currentGeneration.percentDiversity = list(map(float, reTemp))[0]

    generationArray.append(currentGeneration)  # Append the last generation to the array.

    return generationArray


def previousChart(button):

    return


def nextChart(button):

    return


def createNewChartWindow():
    # Create a new chart window.
    devosimGUI.startSubWindow(f"Plot {PlotCounter.plotNumber}")

    # Typical dark mode setup...
    devosimGUI.setFg("white")
    devosimGUI.setBg("black")

    # Add the figure we just made.
    fig = devosimGUI.addPlotFig(f"Figure {PlotCounter.plotNumber}", colspan=3)

    # When making the icon buttons, first set the image folder to the "res" folder in this directory.
    devosimGUI.setImageLocation("./res")

    # Now, create the previous and next buttons, and between them, a drop-down menu.
    devosimGUI.addButton(f"prev{PlotCounter.plotNumber}", previousChart, 1, 0)
    devosimGUI.setButtonImage(f"prev{PlotCounter.plotNumber}", "arrow-prev.gif")

    # The idea with the menu box is to allow the user to choose, for example, a generation. We'll see where this goes.
    devosimGUI.addOptionBox(f"optionBox{PlotCounter.plotNumber}", ["- This is a grayed-out line -",
                                                                   "This Menu", "Is a test", "but doesn't",
                                                                   "actually do anything right now"], 1, 1)

    devosimGUI.addButton(f"next{PlotCounter.plotNumber}", nextChart, 1, 2)
    devosimGUI.setButtonImage(f"next{PlotCounter.plotNumber}", "arrow-next.gif")

    # End the subwindow definition.
    devosimGUI.stopSubWindow()

    # The snippet of code below is a modified version of what's on seaborn.pydata.ord/examples/color_palettes.html

    # For now, this will just statically create a new chart window.
    sns.set(context="talk", style="white", palette="muted")
    random = np.random.RandomState(10)

    # Here's the sequential data example:
    x = np.array(["A", "B", "C", "D", "E", "F", "G", "H", "I", "J"])

    # Add the first subplot.
    # This number means "3 by 1 grid, 1st subplot".
    axis1 = fig.add_subplot(311)

    y1 = np.arange(1, 11)
    sns.barplot(x=x, y=y1, palette="rocket", ax=axis1)
    axis1.axhline(0, color="k", clip_on=False)
    axis1.set_ylabel("Sequential")

    # Here's the diverging data example:

    axis2 = fig.add_subplot(312)

    y2 = y1 - 5.5
    sns.barplot(x=x, y=y2, palette="vlag", ax=axis2)
    axis2.axhline(0, color="k", clip_on=False)
    axis2.set_ylabel("Diverging")

    # Here's the randomly reordered (Qualitative) example:

    axis3 = fig.add_subplot(313)

    y3 = random.choice(y1, len(y1), replace=False)
    sns.barplot(x=x, y=y3, palette="deep", ax=axis3)
    axis3.axhline(0, color="k", clip_on=False)
    axis3.set_ylabel("Qualitative")

    # Finalizing the plot:
    sns.despine(bottom=True)
    plt.setp(fig.axes, yticks=[])
    plt.tight_layout(h_pad=2)

    # Finally, show the window, and increment the plotNumber.
    devosimGUI.showSubWindow("Plot " + f"{PlotCounter.plotNumber}")
    PlotCounter.plotNumber = PlotCounter.plotNumber + 1

    # The takeaway is that we can do basically whatever we want here, as long as we use the figure returned to us
    # by appJar.

    return

# This function is called when we press the Go! button.

def runDevosim(button):
    # These entry fields return floats, but we need these parameters to be ints.
    chromosomeLengthInt = int(devosimGUI.getEntry("Chromosome Length"))
    populationSizeInt = int(devosimGUI.getEntry("Population Size"))
    generationsInt = int(devosimGUI.getEntry("Generations"))
    crossoverRateInt = int(devosimGUI.getEntry("Crossover Rate"))
    mutationRateInt = int(devosimGUI.getEntry("Mutation Rate"))
    mutationEffectInt = int(devosimGUI.getEntry("Mutation Effect"))

    executableNameString = "./devosim"  # Platform-dependent

    settingsStringArray = [executableNameString,
                           "-c", f"{chromosomeLengthInt}",  # f-strings require Python 3.6, I think?
                           "-p", f"{populationSizeInt}",
                           "-g", f"{generationsInt}",
                           "-o", f"{crossoverRateInt}",
                           "-m", f"{mutationRateInt}",
                           "-e", f"{mutationEffectInt}"
                           ]

    selectionModeString = devosimGUI.getRadioButton("selectionMode")
    # Append no flag for Random Selection Mode
    if selectionModeString == "Selective Pressure":
        settingsStringArray.append("-s")
    elif selectionModeString == "Uniform Selection":
        settingsStringArray.append("-u")

    if devosimGUI.getCheckBox("Verbose Mode"):
        settingsStringArray.append("-v")

    if devosimGUI.getCheckBox("Percentages Only"):
        settingsStringArray.append("-r")

    if devosimGUI.getCheckBox("Stop if all dgnomes are identical"):
        settingsStringArray.append("-b")

    try:
        # subprocess.call(settingsStringArray)
        outputString = subprocess.check_output(settingsStringArray, encoding="utf-8")

    except FileNotFoundError:
        devosimGUI.errorBox("Error launching ./devosim",
                            "./devosim was not found in this directory. Make sure you compiled it and that it's in the same directory as this program!")
        return
    except subprocess.CalledProcessError:
        devosimGUI.errorBox("Error running ./devosim",
                            "./devosim returned/exited with an error.")
        return

    outputLines = outputString.splitlines()

    # Creates a strange number of generations when verbose mode is on, but otherwise, it's getting there...
    generationArray = processDevosimOutputLines(outputLines)

    try:
        devosimGUI.showSubWindow("Console Output")
    except ItemLookupError:
        # In the case of an ItemLookupError, then we couldn't find the output window.
        # Let's make it.
        devosimGUI.startSubWindow("Console Output")
        devosimGUI.setFg("white")
        devosimGUI.setBg("black")
        devosimGUI.addScrolledTextArea("output", colspan=1, rowspan=2)
        devosimGUI.stopSubWindow()
        devosimGUI.showSubWindow("Console Output")

    devosimGUI.clearTextArea("output")
    devosimGUI.setTextArea("output", outputString)

    return


# For this general test GUI, I'll just configure it to look like it
# could support all of ./devosim's command line arguments.

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
devosimGUI.addNumericLabelEntry("Mutation Rate", 0, 2)  # -m
devosimGUI.addNumericLabelEntry("Mutation Effect", 1, 2)  # -e

devosimGUI.addRadioButton("selectionMode", "Random Selection", 2, 0)  # no tag
devosimGUI.addRadioButton("selectionMode", "Selective Pressure", 2, 1)  # -s
devosimGUI.addRadioButton("selectionMode", "Uniform Selection", 2, 2)  # -u

devosimGUI.addCheckBox("Verbose Mode", 3, 0)  # -v
devosimGUI.addCheckBox("Percentages Only", 3, 1)  # -r
# -- Not like I'm sure this needs to be in a final GUI version...
devosimGUI.addCheckBox("Stop if all dgnomes are identical", 3, 2)  # -b

# Some systems (My Linux computer) have the text near buttons change color when "activated" (moused over).
# Naturally, this color defaults to black. Let's make it the same color as the text is normally...

devosimGUI.setRadioButtonActiveFg("selectionMode", "white")  # Good for all 3 buttons!
devosimGUI.setCheckBoxActiveFg("Verbose Mode", "white")
devosimGUI.setCheckBoxActiveFg("Percentages Only", "white")
devosimGUI.setCheckBoxActiveFg("Stop if all dgnomes are identical", "white")

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

# To demonstrate the use of Seaborn+Matplotlib+Appjar, this button will create a new window with a figure in it.
devosimGUI.addButton("Show Sample Chart", createNewChartWindow, 5, 0, 3, 1)

# And this "runs" the thing.
devosimGUI.go()
