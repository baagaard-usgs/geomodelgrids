import os
import subprocess

ev_env = {
    "PATH": "/bin:/usr/bin:/sbin:/usr/sbin:/home/tools/earthvision-10/ev10/bin64",
    "LD_LIBRARY_PATH": "/home/tools/earthvision-10/ev10/lib64",
    "LM_LICENSE_FILE": "/home/tools/earthvision-10/license/license.dat",
    }
model_dir = os.path.expanduser("~/scratch/cencalvm/geologic_model")


formula = "test_out.dat<elev> = bakint(dem_km_300m.2grd, test_in.dat<x>, test_in.dat<y>);"
subprocess.run(["ev_fp"], input=formula.encode("utf-8"), cwd=model_dir, env=ev_env)


cmd = "ev_label -m CenCal_104_125.seq -o test2_out.dat -suppress VolumeIndex test_in.dat"
subprocess.run(cmd.split(), pycwd=model_dir, env=ev_env)

cmd = "ev_facedump CenCal_104_125.faces"
result = subprocess.run(cmd.split(), input=formula.encode("utf-8"), cwd=model_dir, env=ev_env, stdout=subprocess.PIPE)
print(result.stdout.decode("utf-8"))

