# MetaData Example

We provide a complete example Python script for accessing model metadata with comments describing each step.
The code can be run in the `tests` directory of the GeoModelGrids source tree.

:::{tip}
The [`geomodelgrids_info`](../../apps/info.md) command line program generates a more complete description of the model metadata, including the details about the surfaces and blocks.
:::

```{code-block} python
# We use the `geomodelgrids` Python modules, so we must import them before use.
import geomodelgrids

# Model to examine.
FILENAME = "data/one-block-topo.h5"

# Create a model object and open the file as read only.
model = geomodelgrids.Model()
model.open(FILENAME, model.READ)

# Load the metadata but not the model contents.
model.load_metadata()

# Get and print model values, units, and data layout
print(f"Model values: {model.value_names}")
print(f"      units: {model.value_units}")
print(f"Data layout: {model.data_layout}")

# Get and print model dimensions, origin, y_azimuth, and coordinate system.
print(f"Model dimensions (m): {model.dims}")
print("Model coordinate system:")
print(f"    Origin: {model.origin}")
print(f"    Y azimuth: {model.y_azimuth}")
print(f"    CRS: {model.crs}")

# Get the model metadata
info = model.get_info()

# Get and print the title, id, description, version, license, keywords, history, and comments.
print(f"Title: {info.title}")
print(f"Id: {info.id}")
print(f"Description: {info.description}")
print(f"Version: {info.version}")
print(f"License: {info.license}")
print(f"Keywords: {", ".join(info.keywords)}")
print(f"History: {info.history}")
print(f"Comments: {info.comment}")

# Get and print creator information
name = info.creator_name
institution = info.creator_institution
email = info.creator_email
print(f"Creator: {name}, {institution} ({email})")

# Get and print attribution information
print(f"Authors: {"; ".join(info.authors)}")
print(f"References:\n    {"\n    ".join(info.references)}")
print(f"Acknowledgements: {info.acknowledgement}")

# Get and print repository information
print(f"Repository: {info.repository_name}")
print(f"    URL: {info.repository_url}")
print(f"    DOI: {info.repository_doi}")

# Get, parse, and print auxiliary information (dict).
if info.auxiliary:
    import json
    auxiliary = json.loads(info.auxiliary)
    print(f"Auxiliary: {auxiliary}")
```
