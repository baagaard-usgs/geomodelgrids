#!/usr/bin/env python3
"""Application for building and pushing docker images.
"""


import os
import argparse
import subprocess

class DockerApp(object):
    """Application for building and pushing docker images.
    """

    def __init__(self):
        self.docker_filename = None
        self.tag = None

    def main(self, **kwargs):
        """Main entry point
        """
        args = argparse.Namespace(**kwargs) if kwargs else self._parse_command_line()
        self.initialize(args)

        if args.build:
            self.build(args.build_env)

        if args.push:
            self.push()

    def initialize(self, args):
        """Initialize builder.
        """
        self.docker_filename = args.dockerfile
        if args.prefix[-1] == "/":
            self.tag = args.prefix + os.path.split(args.dockerfile)[-1]
        else:
            self.tag = "-".join([args.prefix, os.path.split(args.dockerfile)[-1]])

    def build(self, build_env):
        """Build docker image.
        """
        if build_env:
            cmd = f"docker build -t {self.tag} -f {self.docker_filename} --build-arg BUILD_ENV={build_env} ."
        else:
            cmd = f"docker build -t {self.tag} -f {self.docker_filename} ."
        self._run_cmd(cmd)

    def push(self):
        """Push docker image to container registry.
        """
        cmd = "docker push {tag}".format(tag=self.tag)
        self._run_cmd(cmd)

    @staticmethod
    def _parse_command_line():
        """Parse command line arguments.
        """
        parser = argparse.ArgumentParser()
        parser.add_argument("--dockerfile", action="store", dest="dockerfile", required=True)
        parser.add_argument("--prefix", action="store", dest="prefix", default="registry.gitlab.com/baagaard-usgs/geomodelgrids/testenv")
        parser.add_argument("--build", action="store_true", dest="build")
        parser.add_argument("--push", action="store_true", dest="push")
        parser.add_argument("--build-env", action="store", dest="build_env", default=None)
        return parser.parse_args()

    @staticmethod
    def _run_cmd(cmd):
        """Run shell command.
        """
        print("Running '%s'..." % cmd)
        subprocess.check_call(cmd.split())


# ======================================================================
if __name__ == "__main__":
    DockerApp().main()


# End of file
