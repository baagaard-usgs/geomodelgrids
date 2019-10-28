# The geogrid_info command

The `geogrid_info` command line program is used to print information
about a model to stdout.

## Synopsis

Optional command line arguments are in square brackets.

```
geogrid_info [-h] [-b] [-c] [-v] -f FILE_0,...,FILE_M
```

### Required arguments

* **-f FILE_0,...,FILE_M** Names of `M` model files to query. The
  information is printed for each model listed.

### Optional arguments

* **-h** Print help information to stdout and exit.
* **-b** Print block grid information.
* **-c** Print coordinate system information.
* **-v** Print names and units of values stored in the model.
