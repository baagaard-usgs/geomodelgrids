# generate_topoxy.py

Create xy grid file with points for topography

1. Create top level grid (only xy coordinates). The order must match that in the block grid space.

# generate_blocksxyz.py

Create xyz grid files, one per block with points in physical space

1. Load topography file

2. Loop over blocks

    a. Compute xy coordinates for block

	b. Loop over z values in grid space

		Compute z values in physical space


