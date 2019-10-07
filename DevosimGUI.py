from appJar import gui
from appJar.appjar import ItemLookupError
import subprocess

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, NavigationToolbar2Tk)
import seaborn as sns


class PlotCounter:
    plotNumber = 1  # A simple counter to differentiate the titles of plot windows.


# This function is called when we press the Go! button.

def createNewChartWindow():
    # Create a new chart window.
    devosimGUI.startSubWindow("Plot " + f"{PlotCounter.plotNumber}")

    # Typical dark mode setup...
    devosimGUI.setFg("white")
    devosimGUI.setBg("black")

    # Add the figure we just made.
    fig = devosimGUI.addPlotFig("Figure " + f"{PlotCounter.plotNumber}")

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
        outputString = subprocess.check_output(settingsStringArray)

    except FileNotFoundError:
        devosimGUI.errorBox("Error launching ./devosim",
                            "./devosim was not found in this directory. Make sure you compiled it and that it's in the same directory as this program!")
        return
    except subprocess.CalledProcessError:
        devosimGUI.errorBox("Error running ./devosim",
                            "./devosim returned/exited with an error.")
        return

    print(outputString)

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
