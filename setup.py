from setuptools import setup, find_packages

setup(
    name="hdf2hepnos",
    version="0.1",
    zip_safe=False,
    include_package_data=True,
    packages=find_packages(),
    install_requires=["jinja2"],
    scripts=['bin/hdf2hepnos'],
)
