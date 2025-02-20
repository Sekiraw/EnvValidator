# SPDX-License-Identifier: Apache-2.0

from setuptools import setup, Extension

with open("README.md", "r") as fh:
    long_description = fh.read()

module = Extension(
    "{ModuleName}",
    sources=["main.c"],
)

setup(
    name="{ModuleName}",
    version="1.0.0",
    description="{ModuleName} description",
    long_description=long_description,
    long_description_content_type="text/markdown",
    author="{Author}",
    author_email="{AuthorEmail}",
    url = 'https://github.com/{Author}/{RepoName}',
    keywords=["USEFUL", "KEY", "WORDS"],
    license="Apache-2.0",
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Build Tools",
        "License :: OSI Approved :: Apache Software License",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
    ],
    ext_modules=[module],
)
