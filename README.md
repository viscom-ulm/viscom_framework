# VISCOM Framework

[![Build Status](https://travis-ci.com/viscom-ulm/viscom_framework.svg?branch=master)](https://travis-ci.com/viscom-ulm/viscom_framework)

This framework is used for graphics applications by the [Visual Computing Group](https://www.uni-ulm.de/en/in/mi/mi-forschung/viscom.html) at [Ulm University](https://www.uni-ulm.de/en/).

It is able to run on the groups powerwall and currently supports touch input from this wall.

## Build (Windows, *nix)
- Use [conan](https://conan.io/)
- Setup the remotes for openvr and glfw (need to do this only once):

  ```conan remote add arsen-studio https://api.bintray.com/conan/arsen-studio/arsen-deps```

  ```conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan```
  
  ```conan config set general.revisions_enabled=1```
- From the build folder run:

  ```conan install --build=missing --install-folder=./fwcore ../extern/fwcore/```

  ```conan install --build=missing --install-folder=./fwcore -s build_type=Debug ../extern/fwcore/```

  This does not generate debug symbols for Visual Studio and some warnings will be generated. To avoid use the `--build` parameter without `=missing`.

## Build (OSX)
- Use [conan](https://conan.io/)
- Setup the remotes for glfw (need to do this only once):

  ```conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan```
  
  ```conan config set general.revisions_enabled=1```
- From the project build folder run:

  ```conan install --build=missing --install-folder=./fwcore ../extern/fwcore/conanfile-osx.txt```

  ```conan install --build=missing --install-folder=./fwcore -s build_type=Debug ../extern/fwcore/conanfile-osx.txt```
