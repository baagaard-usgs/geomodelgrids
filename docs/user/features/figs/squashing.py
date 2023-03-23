#!/usr/bin/env python3

import numpy
import matplotlib.pyplot as pyplot
import matplotlib_extras

FIG_SIZE = (7.0, 4.0)
MARGINS = ((0.7, 0.7, 0.2), (0.5, 0, 0.3))

maxTopo = 3.0
minTopo = -0.5
squashMin = -15.0
minModel = -40.0

levels = (-5.0, -10.0, -25.0, -35.0)
amplitude = 1.0
x = numpy.arange(0.0, 50.0, 5.0)
numX = len(x)


def squash(y, yTop, squashMin):
    """Compute elevation in squashed model from elevation of 'normal' model."""
    mask = y > squashMin
    yFlat = mask * (squashMin / (squashMin-yTop) * (y-yTop)) + ~mask*y
    return yFlat


def generateModel():
    yContours = []
    yTop = numpy.random.uniform(minTopo, maxTopo, numX)
    yContours.append(yTop)

    for yAvg in levels:
        minY = yAvg - amplitude
        maxY = yAvg + amplitude
        y = numpy.random.uniform(minY, maxY, numX)
        yContours.append(y)

    yBot = minModel*numpy.ones(numX)
    yContours.append(yBot)
    return yContours


def plotModel(ax, x, y, title):
    for i in range(len(y)-1):
        ax.fill_between(x, y[i], y[i+1])

    numX = len(x)
    ax.plot(x, squashMin*numpy.ones(numX), "k--", lw=3)
    ax.text(10, squashMin-0.5, "Min. squash elev.", va="top")
    ax.set_xlabel("Horizontal Distance")
    ax.set_ylabel("Elevation")
    ax.set_xlim(min(x), max(x))
    ax.set_ylim(minModel, maxTopo)
    ax.set_title(title)
    ax.spines[["top", "right"]].set_visible(False)


yModel = generateModel()
ySquashed = [squash(y, yModel[0], squashMin) for y in yModel]

pyplot.style.use("size-presentation")
pyplot.style.use("color-lightbg")

figure = pyplot.figure(figsize=FIG_SIZE)
rect_factory = matplotlib_extras.axes.RectFactory(figure, ncols=2, margins=MARGINS)
ax = figure.add_axes(rect_factory.rect(col=1))
plotModel(ax, x, yModel, "Model in Physical Space")
ax = figure.add_axes(rect_factory.rect(col=2))
plotModel(ax, x, ySquashed, "Squashed Model")
ax.set_ylabel("")

figure.savefig("squashing.png")
figure.savefig("squashing.pdf")
