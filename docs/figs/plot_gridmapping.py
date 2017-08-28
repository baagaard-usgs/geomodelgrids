#!/usr/bin/env python

import numpy
import matplotlib.pyplot as pyplot

z_top = 0.0
z_bot = -2000.0
grids = (
    {'z_top': 0.0, 'num_z': 11, 'res_horiz': 100.0, 'res_vert': 25.0, 'color': 'ltred'},
    {'z_top': -250.0, 'num_z': 11, 'res_horiz': 200.0, 'res_vert': 50.0, 'color': 'ltblue'},
    {'z_top': -750.0, 'num_z': 11, 'res_horiz': 500.0, 'res_vert': 125.0, 'color': 'orange'},
    )

topo = numpy.array([0.0, 50.0, 100.0, 150.0, 180.0, 200.0, 190.0, 120.0, 90.0, 50.0, 30.0, 20.0, 10.0, 0.0, 0.0, 0.0, 20.0, 50.0, 70.0, 30.0, 20.0])
xmin = 0.0
xmax = grids[0]['res_horiz']*(topo.shape[-1]-1)

def physical_grid(ax):
    for grid in grids:
        gridX = numpy.arange(xmin, xmax+0.5*grid['res_horiz'], grid['res_horiz'])
        skip = (topo.shape[-1]-1) / (gridX.shape[-1]-1)
        gridTopo = topo[::skip]

        z0 = grid['z_top']
        z1 = grid['z_top'] - grid['res_vert']*(grid['num_z']-1)
        gridZ = numpy.arange(z0, z1-grid['res_vert'], -grid['res_vert'])

        x2 = numpy.zeros((gridX.shape[-1], gridZ.shape[-1],))
        z2 = numpy.zeros(x2.shape)
        for iz,z in enumerate(gridZ):
            x2[:,iz] = gridX
            z2[:,iz] = z_bot + (gridTopo-z_bot)/(z_top-z_bot)*(z-z_bot)
        
        ax.plot(x2, z2, 'o', color=grid['color'], ms=4, zorder=3)
        ax.plot(x2, z2, '-', lw=0.5, color='fg', zorder=2)
        ax.plot(x2.T, z2.T, '-', lw=0.5, color='fg', zorder=1)
        ax.set_xlim(xmin-100, xmax+100)
        ax.set_ylim(z_bot, numpy.max(topo))
        ax.set_aspect('equal', 'datalim')
        ax.set_title("Physical Grid")
    return


def topological_grid(ax):
    for grid in grids:
        gridX = numpy.arange(xmin, xmax+0.5*grid['res_horiz'], grid['res_horiz'])
        z0 = grid['z_top']
        z1 = grid['z_top'] - grid['res_vert']*(grid['num_z']-1)
        gridZ = numpy.arange(z0, z1-grid['res_vert'], -grid['res_vert'])
        x2,z2 = numpy.meshgrid(gridX, gridZ)
        
        ax.plot(x2, z2, 'o', lw=0, color=grid['color'], ms=4, zorder=3)
        ax.plot(x2, z2, '-', lw=0.5, color='fg', zorder=2)
        ax.plot(x2.T, z2.T, '-', lw=0.5, color='fg', zorder=1)
        ax.set_xlim(xmin-100, xmax+100)
        ax.set_ylim(z_bot, numpy.max(topo))
        ax.set_aspect('equal', 'datalim')
        ax.set_title("Topological Grid")
    return


from basemap.Figure import Figure
figure = Figure()
figure.open(6.5, 3.5, margins=((0.6, 0.5, 0.2), (0.5, 0, 0.2)))

ax = figure.axes(1, 2, 1, 1)
physical_grid(ax)

ax = figure.axes(1, 2, 1, 2)
topological_grid(ax)

figure.figure.savefig("gridmapping.png")
#pyplot.show()
