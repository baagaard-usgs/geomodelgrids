# Batch Python Objects 

## BatchGenerator2D

**Full name**: geomodelgrids.create.utils.batch.BatchGenerator2D

Iterator for batches of points for 2D domains.

```{code-block} python
for batch in BatchGenerator2D(num_x, num_y, max_nvalues):
    # Use batch
```

### Data Members

+ **num_x** *(int)* Number of points in x direction.
+ **num_y** *(int)* Number of points in y direction.
+ **bnum_x** *(int)* Number of points in x direction for current batch.
+ **bnum_y** *(int)* Number of points in y direction for current batch.
+ **nbatch_x** *(int)* Number of batches in x direction.
+ **nbatch_y** *(int)* Number of batches in y direction.
+ **ix** *(int)* Batch index in x direction.
+ **iy** *(int)* Batch index in y direction.
+ **x_range** *(tuple)* Starting and ending indices in x direction of current batch.
+ **y_range** *(tuple)* Starting and ending indices in y direction of current batch.

### Methods

+ [BatchGenerator2D(num_x, num_y, max_nvalues)](py-api-create-utils-batch2d-constructor)
+ [\_\_str\_\_()](py-api-create-utils-batch2d-str)
+ [\_\_iter\_\_()](py-api-create-utils-batch2d-iter)
+ [\_\_next\_\_()](py-api-create-utils-batch2d-next)

(py-api-create-utils-batch2d-constructor)=
#### BatchGenerator2D(num_x, num_y, max_nvalues=None)

Constructor.

+ **num_x** *(int)* Number of points in x direction.
+ **num_y** *(int)* Number of points in y direction.
+ **max_nvalues** *(int)* Maximum number of points in a batch.

(py-api-create-utils-batch2d-str)=
#### \_\_str\_\_()

Get string representation.

+ **returns** Batch as a string.

(py-api-create-utils-batch2d-iter)=
#### \_\_iter\_\_()

Iteration.

+ **returns** Batch object.

(py-api-create-utils-batch2d-next)=
#### \_\_next\_\_()

Get next batch.

+ **returns** Batch object with next batch.

## BatchGenerator3D

**Full name**: geomodelgrids.create.utils.batch.BatchGenerator3D

Iterator for batches of points for 3D domains.

```{code-block} python
for batch in BatchGenerator3D(num_x, num_y, num_z, max_nvalues):
    # Use batch
```

### Data Members

+ **num_x** *(int)* Number of points in x direction.
+ **num_y** *(int)* Number of points in y direction.
+ **num_z** *(int)* Number of points in z direction.
+ **bnum_x** *(int)* Number of points in x direction for current batch.
+ **bnum_y** *(int)* Number of points in y direction for current batch.
+ **bnum_z** *(int)* Number of points in z direction for current batch.
+ **nbatch_x** *(int)* Number of batches in x direction.
+ **nbatch_y** *(int)* Number of batches in y direction.
+ **nbatch_z** *(int)* Number of batches in z direction.
+ **ix** *(int)* Batch index in x direction.
+ **iy** *(int)* Batch index in y direction.
+ **iz** *(int)* Batch index in z direction.
+ **x_range** *(tuple)* Starting and ending indices in x direction of current batch.
+ **y_range** *(tuple)* Starting and ending indices in y direction of current batch.
+ **z_range** *(tuple)* Starting and ending indices in z direction of current batch.

### Methods

+ [BatchGenerator3D(num_x, num_y, num_z, max_nvalues)](py-api-create-utils-batch3d-constructor)
+ [\_\_str\_\_()](py-api-create-utils-batch3d-str)
+ [\_\_iter\_\_()](py-api-create-utils-batch3d-iter)
+ [\_\_next\_\_()](py-api-create-utils-batch3d-next)

(py-api-create-utils-batch3d-constructor)=
#### BatchGenerator3D(num_x, num_y, num_z, max_nvalues=None)

Constructor.

+ **num_x** *(int)* Number of points in x direction.
+ **num_y** *(int)* Number of points in y direction.
+ **num_z** *(int)* Number of points in z direction.
+ **max_nvalues** *(int)* Maximum number of points in a batch.

(py-api-create-utils-batch3d-str)=
#### \_\_str\_\_()

Get string representation.

+ **returns** Batch as a string.

(py-api-create-utils-batch3d-iter)=
#### \_\_iter\_\_()

Iteration.

+ **returns** Batch object.

(py-api-create-utils-batch3d-next)=
#### \_\_next\_\_()

Get next batch.

+ **returns** Batch object with next batch.
