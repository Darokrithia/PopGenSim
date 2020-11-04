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
    def __init__(self, number, chromosomeLength, population):
        self.number = number  # This generation's number (i.e 0, 1, 1000).
        self.dgnomeValues = dict()  # The element[N] is the Nth dgnome's allele value array.
        self.dgnomeAncestries = dict()  # The element[N] is the Nth dgnome's ancestry percentage array.
        self.dgnomeAncestryPercentages = dict()
        self.percentDiversity = None  # Not always defined, but it's a floating point when it is.
        self.chromosomeLength = chromosomeLength
        self.population = population

    def __repr__(self):
        return "Generation (stores parsed data for a simulated generation)"

    def __str__(self):
        return f"Generation {self.number}:\n" \
               f"self.dgnomeValues: {self.dgnomeValues}\n" \
               f"self.dgnomeAncestries: {self.dgnomeAncestries}\n" \
               f"self.dgnomeAncestryPercentages: {self.dgnomeAncestryPercentages}\n" \
               f"self.percentDiversity: {self.percentDiversity}\n"


class ChartWindow:
    allWindows = dict()  # A dictionary of (self.number : ChartWindow) pairs.
    dgGraphOptions = ["Dgnome Values", "Dgnome Ancestries", "Dgnome Ancestry Percentages"]

    def __init__(self, name, number, generation, figure):
        self.name = name  # The name/identifier per AppJar.
        self.number = number  # the PlotNumber when this was initialized.
        self.generation = generation  # The generation object this window charts.
        self.fig = figure  # A reference to this window's figure.
        self.optionBoxChoice = None
        self.displayIndex = 0  # This keep track of which sub-element we're graphing (i.e the Nth dgnome ancestry)
        self.previousOptionBoxChoice = None # If this is equal to optionBoxChoice, we shouldn't recalculate chart data.

    def updateOptionBoxChoice(self, optionBox):
        self.optionBoxChoice = devosimGUI.getOptionBox(optionBox)

# Returns an array of all the generations, in order.
def processDevosimOutputLines(outputLines):

    generationArray = []
    currentGeneration = None # Start with a None generation.

    floatRegex = re.compile(r'-?\d+\.\d+')  # Compile the regex for finding floating point units in a line of output.
    intRegex = re.compile(r'-?\d+')  # Compile the regex for finding integers in a line of output.

    firstLine = outputLines.pop(0)
    numbersInFirstLine = [int(i) for i in intRegex.findall(firstLine)] # Get all the numbers in the first line.
    print(numbersInFirstLine)
    # The first number will be the chromosome length and the second will be the population.
    chromosomeLength = numbersInFirstLine[0]
    population = numbersInFirstLine[1]

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
            currentGeneration = Generation(currentGenerationInt, chromosomeLength, population) # "Start" a new generation.

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
            if re.search("-?\d+\.\d+% Degnome \d+", outputLines[index + 2]):
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

def findWindowFromWidget(widgetName):
    reTemp = re.findall(r'\d+', widgetName)
    wInt = list(map(int, reTemp))[0]
    return ChartWindow.allWindows[wInt]

def previousChart(button):
    window = findWindowFromWidget(button)
    window.displayIndex = window.displayIndex - 1  # Decrement displayIndex. Overflow will be handled in updateFigure.
    updateFigure(window)  # Assuming this exists for simplicity, for now.
    return


def nextChart(button):
    window = findWindowFromWidget(button)
    window.displayIndex = window.displayIndex + 1  # Increment displayIndex. Overflow will be handled in updateFigure.
    updateFigure(window)  # Assuming this exists for simplicity, for now.
    return

def processDataForFigure(inputData, dgnumber, population, chromosomeLength, desiredGraph):
    # Not all of the parameters are used yet... but they may be necessary in the future.
    x = []  # x should be a 1-d array of numbers.
    y = []  # y should be a 2-d array of numbers, whose length along the first dimension is equal to x's length
    xlabel = "X Axis Description"
    print(inputData)
    if desiredGraph == ChartWindow.dgGraphOptions[0]:
        # x should just be a list from 0 to (chromosomeLength-1).
        # each entry in y should be the corresponding array of that dgnome's allele values.
        x = list(range(chromosomeLength))
        for k, v in inputData:
            y.append(v)
        xlabel = "Allele Number" # X corresponds to the allele number from which the value was extracted.
    elif desiredGraph == ChartWindow.dgGraphOptions[1]:
        # x should be a list from 0 to (population-1), though this will have the same effect.
        # each entry in y should be the corresponding ancestry array.
        x = list(range(chromosomeLength))
        for k, v in inputData:
            y.append(v)
        xlabel = "Allele Number" # X corresponds to the allele number from which the value was extracted.
    elif desiredGraph == ChartWindow.dgGraphOptions[2]:
        # The input data is a dictionary of number keys and (int, float) tuple values.
        # We want an x array of the int values and a y array of the float values.
        for i in range(len(inputData)):
            y.append([0] * len(inputData))
        for dgnomeNumber, v in inputData:
            x.append(dgnomeNumber)
            for fromDgnomeNumber, percentage in v:
                y[dgnomeNumber][fromDgnomeNumber] = percentage
        xlabel = "Dgnome of Origin"
        # If we contain alleles originating from dgnome X, the value of Y at X is the percent of that allele's ocurrence
        # For example, if 4/10 of our alleles are from dgnome 5 (and thus have a value of 5), x=5 has a y=40.0.

    return x, y, xlabel

def updateFigure(window):
    sns.set(context="talk", style="white", palette="muted") # Future feature idea: make these user-set

    # Clear the figure of anything it may be currently showing.
    window.fig.clear()

    # We'll display the [window.displayIndex]-th element, but it should be modulo the size of the array we're using.
    window.displayIndex = window.displayIndex % window.generation.population
    # Correct the displayIndex, in case it's too big or negative.

    # This will possibly be different under some circumstances, but for now:
    dgnumber = window.displayIndex

    switch = { # A replacement for a switch block:
        ChartWindow.dgGraphOptions[0]: window.generation.dgnomeValues.items(),
        ChartWindow.dgGraphOptions[1]: window.generation.dgnomeAncestries.items(),
        ChartWindow.dgGraphOptions[2]: window.generation.dgnomeAncestryPercentages.items(),  # Not yet enabled
    }
    # Choose the input data based on the window's option box choice.
    inputData = switch.get(window.optionBoxChoice)
    #print(inputData)
    # Process the data in another procedure.
    x, y, xlabel = processDataForFigure(inputData,
                                dgnumber,
                                window.generation.population,
                                window.generation.chromosomeLength,
                                window.optionBoxChoice)
    #print(x)
    #print(y)

    axis1 = window.fig.add_subplot(111)
    sns.barplot(x=x, y=y[window.displayIndex], palette="rocket", ax=axis1)
    axis1.axhline(0, color="k", clip_on=False)
    axis1.set_xlabel(xlabel)
    axis1.set_ylabel(f"Generation {window.generation.number}, Dgnome {dgnumber}\n{window.optionBoxChoice}")

    # Finalizing the plot:
    sns.despine(bottom=True)
    plt.setp(window.fig.axes)
    plt.tight_layout(h_pad=2)

    # Finally, tell AppJar that the plot was updated:
    devosimGUI.refreshPlot(f"Figure {window.number}")

    return

def optionBoxChanged(optionBox):
    window = findWindowFromWidget(optionBox)
    window.updateOptionBoxChoice(optionBox) # Update the internal option box choice.
    window.displayIndex = 0 # Reset the displayIndex.
    updateFigure(window) # Assuming this exists for simplicity, for now.
    return

def createNewChartWindowForGeneration(generation):
    # generation : A Generation object

    # Create a new chart window.
    window = ChartWindow(f"Plot {PlotCounter.plotNumber}: Generation {generation.number}", PlotCounter.plotNumber, generation, None)

    devosimGUI.startSubWindow(window.name)

    # Typical dark mode setup...
    devosimGUI.setFg("white")
    devosimGUI.setBg("black")

    # Add a figure.
    fig = devosimGUI.addPlotFig(f"Figure {window.number}", colspan=3)
    window.fig = fig # The window's figure can now be updated.

    # Now, create the previous and next buttons, and between them, a drop-down menu.
    devosimGUI.addButton(f"prev{window.number}", previousChart, 1, 0)
    devosimGUI.setButtonImage(f"prev{window.number}", "arrow-prev.gif")

    # The idea with the menu box is to allow the user to choose, for example, a generation. We'll see where this goes.
    devosimGUI.addOptionBox(f"optionBox{window.number}",
                            ChartWindow.dgGraphOptions,
                            1, 1, callFunction=True)
    devosimGUI.setOptionBoxChangeFunction(f"optionBox{window.number}", optionBoxChanged)

    devosimGUI.addButton(f"next{window.number}", nextChart, 1, 2)
    devosimGUI.setButtonImage(f"next{window.number}", "arrow-next.gif")

    # End the subwindow definition.
    devosimGUI.stopSubWindow()

    # Draw the figure.
    window.updateOptionBoxChoice(f"optionBox{window.number}")
    updateFigure(window)

    #Add myself to the dictionary of all windows.
    ChartWindow.allWindows[window.number] = window

    # Finally, show the window, and increment the plotNumber.
    devosimGUI.showSubWindow(f"Plot {window.number}: Generation {generation.number}")
    PlotCounter.plotNumber = PlotCounter.plotNumber + 1

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
        print(outputString)
    except FileNotFoundError:
        devosimGUI.errorBox("Error launching ./devosim",
                            "./devosim was not found in this directory. "
                            "Make sure you compiled it and that it's in the same directory as this program!")
        return
    except subprocess.CalledProcessError:
        devosimGUI.errorBox("Error running ./devosim",
                            "./devosim returned/exited with an error.")
        return

    outputLines = outputString.splitlines()

    # Creates a strange number of generations when verbose mode is on, but otherwise, it's getting there...
    generationArray = processDevosimOutputLines(outputLines)

    createNewChartWindowForGeneration(generationArray.pop())

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

# For when we make the icon buttons: set the image folder to the "res" folder in this directory.
devosimGUI.setImageLocation("./res")

# Now, here comes the Run button.
devosimGUI.addButton("Go!", runDevosim, 4, 0, 3, 1)

# And this "runs" the thing.
devosimGUI.go()
