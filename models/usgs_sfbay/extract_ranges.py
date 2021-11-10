import numpy
import h5py
import json

h5 = h5py.File("data/geomodelgrids/USGS_SFCVM_detailed_v21-1.h5", "r")

auxiliary = json.loads(h5.attrs["auxiliary"])
fault_blocks = sorted(auxiliary["fault_block_ids"].items(), key=lambda x: x[1])
print("Fault Blocks")
for label, value in fault_blocks:
    lines = [
        "        <attrdomv>",
        "          <edom>",
        f"            <edomv>{value}</edomv>",
        f"            <edomvd>{label}</edomvd>",
         "            <edomvds>Producer-defined</edomvds>",
        "          </edom>",
        "        </attrdomv>",
        ]
    print("\n".join(lines))

zones = sorted(auxiliary["zone_ids"].items(), key=lambda x: x[1])
print("Zone Ids")
for label, value in zones:
    lines = [
        "        <attrdomv>",
        "          <edom>",
        f"            <edomv>{value}</edomv>",
        f"            <edomvd>{label}</edomvd>",
         "            <edomvds>Producer-defined</edomvds>",
        "          </edom>",
        "        </attrdomv>",
        ]
    print("\n".join(lines))

for surface in h5["surfaces"]:
    dset = h5["surfaces"][surface][:]
    value_min = numpy.min(dset)
    value_max = numpy.max(dset)
    print(f"{surface} min={value_min:.1f} max={value_max:.1f}")

value_ranges = {}
for block in h5["blocks"]:
    dset = h5["blocks"][block]
    for index in range(5):
        value_name = h5.attrs["data_values"][index]
        value = dset[:,:,:,index].ravel()
        mask = value > -1.0e+8
        value_min = numpy.min(value[mask])
        value_max = numpy.max(value[mask])
        value_nodata = True if numpy.sum(~mask) > 0 else False
        if not value_name in value_ranges:
            value_ranges[value_name] = (value_min, value_max, value_nodata)
        else:
            current_min, current_max, current_nodata = value_ranges[value_name]
            new_min = min(value_min, current_min)
            new_max = max(value_max, current_max)
            new_nodata = value_nodata or current_nodata
            value_ranges[value_name] = (new_min, new_max, new_nodata)
for value_name, (value_min, value_max, value_nodata) in value_ranges.items():
    print(f"{value_name} min={value_min:.1f} max={value_max:.1f}, nodata={value_nodata}")
