from setuptools import setup

name = "nsfd"

about = {}
with open("nsfd/__init__.py") as fp:
    exec(fp.read(), about)
release = about["__release__"]
version = about["__version__"]

dev_status = "Development Status :: 2 - Pre-Alpha"

setup_kwargs = {
    "name": name,
    "version": release,
    "packages": ["nsfd"],
    "license": "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
    "author": "Marian Domanski",
    "author_email": "shrieks.walnut.0s@icloud.com",
    "description": "Code for _Numerical Solutions in Fluid Dynamics_",
    "classifiers": [
        dev_status,
        "Intended Audience :: Science/Research",
        "Programming Language :: Python :: 3",
        "Topic :: Scientific/Engineering",
    ],
    "project_urls": {"Source": "https://github.com/mmdski/nsfd"},
    "install_requires": ["numpy"],
    "python_requires": "~=3.11",
}

setup(**setup_kwargs)
