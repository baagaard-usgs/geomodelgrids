#!/usr/bin/env python

import numpy
import matplotlib.pyplot as pyplot
import matplotlib_extras



Z_TOP = 0.0
Z_BOT = -2000.0
GRIDS = (
    {'z_top': 0.0, 'num_z': 11, 'res_horiz': 100.0, 'res_vert': 25.0},
    {'z_top': -250.0, 'num_z': 11, 'res_horiz': 200.0, 'res_vert': 50.0},
    {'z_top': -750.0, 'num_z': 11, 'res_horiz': 500.0, 'res_vert': 125.0},
    )

FIG_SIZE = (7.0, 3.5)

TOPO = numpy.array([0.0, 50.0, 100.0, 150.0, 180.0, 200.0, 190.0, 120.0, 90.0, 50.0, 30.0, 20.0, 10.0, 0.0, 0.0, 0.0, 20.0, 50.0, 70.0, 30.0, 20.0])
X_MIN = 0.0
X_MAX = GRIDS[0]['res_horiz']*(TOPO.shape[-1]-1)




def physical_grid(ax):
    color_order = pyplot.rcParams['axes.prop_cycle'].by_key()['color']
    for i_grid, grid in enumerate(GRIDS):
        gridX = numpy.arange(X_MIN, X_MAX+0.5*grid['res_horiz'], grid['res_horiz'])
        skip = (TOPO.shape[-1]-1) / (gridX.shape[-1]-1)
        gridTopo = TOPO[::skip]

        z0 = grid['z_top']
        z1 = grid['z_top'] - grid['res_vert']*(grid['num_z']-1)
        gridZ = numpy.arange(z0, z1-0.5*grid['res_vert'], -grid['res_vert'])

        x2 = numpy.zeros((gridX.shape[-1], gridZ.shape[-1],))
        z2 = numpy.zeros(x2.shape)
        for iz,z in enumerate(gridZ):
            x2[:,iz] = gridX
            z2[:,iz] = Z_BOT + (gridTopo-Z_BOT)/(Z_TOP-Z_BOT)*(z-Z_BOT)

        color_grid = color_order[i_grid]
        ax.plot(x2, z2, 'o', color=color_grid, ms=4, zorder=3)
        ax.plot(x2, z2, '-', lw=0.5, color=color_grid, zorder=2)
        ax.plot(x2.T, z2.T, '-', lw=0.5, color=color_grid, zorder=1)
        ax.set_xlim(X_MIN-100, X_MAX+100)
        ax.set_ylim(Z_BOT, numpy.max(TOPO))
        ax.set_aspect('equal', 'datalim')
        ax.set_title("Physical Grid")
    return


def topological_grid(ax):
    color_order = pyplot.rcParams['axes.prop_cycle'].by_key()['color']
    for i_grid, grid in enumerate(GRIDS):
        gridX = numpy.arange(X_MIN, X_MAX+0.5*grid['res_horiz'], grid['res_horiz'])
        z0 = grid['z_top']
        z1 = grid['z_top'] - grid['res_vert']*(grid['num_z']-1)
        gridZ = numpy.arange(z0, z1-grid['res_vert'], -grid['res_vert'])
        x2,z2 = numpy.meshgrid(gridX, gridZ)
        
        color_grid = color_order[i_grid]
        ax.plot(x2, z2, 'o', lw=0, color=color_grid, ms=4, zorder=3)
        ax.plot(x2, z2, '-', lw=0.5, color=color_grid, zorder=2)
        ax.plot(x2.T, z2.T, '-', lw=0.5, color=color_grid, zorder=1)
        ax.set_xlim(X_MIN-100, X_MAX+100)
        ax.set_ylim(Z_BOT, numpy.max(TOPO))
        ax.set_aspect('equal', 'datalim')
        ax.set_title("Logical Grid")
    return


pyplot.style.use("size-presentation")
pyplot.style.use("color-lightbg")

figure = pyplot.figure(figsize=FIG_SIZE)
rect_factory = matplotlib_extras.axes.RectFactory(figure, nrows=1, ncols=2, margins=((0.7, 0.9, 0.1), (0.30, 0, 0.25)))
ax = figure.add_axes(rect_factory.rect(row=1, col=1))
physical_grid(ax)

ax = figure.add_axes(rect_factory.rect(row=1, col=2))
topological_grid(ax)

figure.savefig("gridmapping.pdf")
