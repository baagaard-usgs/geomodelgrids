# The geogrid_info command

The `geogrid_info` command line program is used to print information
about a model to stdout.

## Synopsis

Optional command line arguments are in square brackets.

```
geogrid_info [--help] [--description] [--blocks] [--coordsys] [--values] --models=FILE_0,...,FILE_M
```

### Required arguments

* **--models=FILE_0,...,FILE_M** Names of `M` model files to query. The
  information is printed for each model listed.

### Optional arguments

* **--help** Print help information to stdout and exit.
* **--description** Print model description.
* **--blocks** Print block grid information.
* **--coordsys** Print coordinate system information.
* **--values** Print names and units of values stored in the model.
