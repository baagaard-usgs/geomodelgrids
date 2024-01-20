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

# Get the model metadata
info = model.get_info()

# Get and print the title, id, description, version, license, keywords, history, and comments.
print(f"Title: {info.get_title()}")
print(f"Id: {info.get_id()}")
print(f"Description: {info.get_description()}")
print(f"Version: {info.get_version()}")
print(f"License: {info.get_license()}")
print(f"Keywords: {info.get_keywords()}")
print(f"History: {info.get_history()}")
print(f"Comments: {info.get_comment()}")

# Get and print creator information
name = info.get_creator_name()
institution = info.get_creator_institution()
email = info.get_creator_email()
print(f"Creator: {name}, {institution} ({email})")

# Get and print attribution information
print(f"Authors: {info.get_authors()}")
print(f"References: {info.get_references()}")
print(f"Acknowledgements: {info.get_acknowledgement()}")

# Get and print repository information
print(f"Repository: {info.get_repository_name()}")
print(f"    URL: {info.get_repository_url()}")
print(f"    DOI: {info.get_repository_doi()}")

# Get, parse, and print auxiliary information (dict).
auxiliary_json = info.get_auxiliary()
if auxiliary_json:
    import json
    auxiliary = json.loads(auxiliary_json)
    print(f"Auxiliary: {auxiliary}")
```
